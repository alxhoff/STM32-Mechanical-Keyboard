#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sm.h"

typedef struct system_state {
	unsigned int ID;
	char *name;

	void *data;

	void* (*init)(void*); // Init should return 1 on success
	void (*enter)(void*);
	void (*run)(void*);
	void (*exit)(void*);

	struct system_state *next;

	unsigned char _initialized :1;
} system_state_t;

struct state_machine {
	QueueHandle_t queue;

	system_state_t *head;

	unsigned int _state_count;

	system_state_t *current;
	system_state_t *next;

	unsigned char _initialized :1;
} sm = { 0 };

unsigned int smAddState(signed int ID, char *name, void* (*init)(void*),
		void (*enter)(void*), void (*run)(void*), void (*exit)(void*),
		void *data) {

	// system_state_t structure
	system_state_t *new_state = (system_state_t*) calloc(1,
			sizeof(system_state_t));

	new_state->name = strdup(name);
	new_state->init = init;
	new_state->enter = enter;
	new_state->run = run;
	new_state->exit = exit;
	new_state->data = data;
	new_state->ID = (ID != -1) ? ID : ++sm._state_count;

	if (!sm.head)
		sm.head = new_state;
	else {
		system_state_t *iterator;

		// Get end of state linked list
		for (iterator = sm.head; iterator->next; iterator = iterator->next)
			;

		iterator->next = new_state;
	}

	return new_state->ID;
}

system_state_t* smFindState(unsigned int ID) {
	system_state_t *iterator;

	for (iterator = sm.head; iterator->next && (iterator->next->ID != ID);
			iterator = iterator->next)
		;

	return iterator->next;
}

void smDeleteState(unsigned int ID) {
	system_state_t *iterator, *delete;

	for (iterator = sm.head; iterator->next && (iterator->next->ID != ID);
			iterator = iterator->next)
		;

	if (iterator->next)
		if (iterator->next->ID) {
			delete = iterator->next;

			if (!iterator->next->next)
				iterator->next = NULL;
			else
				iterator->next = delete->next;

			free(delete);
		}
}

unsigned char smInit(void) {
	system_state_t *iterator;

	sm.queue = xQueueCreate(STATE_MACHINE_STATE_QUEUE_LENGTH,
			sizeof(unsigned int));

	if (!sm.queue) {
		fprintf(stderr, "State queue creation failed\n");
		exit(EXIT_FAILURE);
	}

	// Call each state's init function
	for (iterator = sm.head; iterator; iterator = iterator->next) {
		if (iterator->init)
			iterator->data = (iterator->init)(iterator->data);
		iterator->_initialized = 1;
	}
	sm._initialized = 1;
	return 0;
}

void smSwapStates(unsigned int next_state_ID) {
	system_state_t *next_state = smFindState(next_state_ID);

	// Swap states and run appropriate functions
	if (next_state) {
		sm.next = next_state;

		if (sm.current != sm.next) {
			if (sm.current->exit)
				(sm.current->exit)(sm.current->data);

			if (sm.next->enter)
				(sm.next->enter)(sm.next->data);

			sm.current = sm.next;
		}
	}
}

void smSetStates(unsigned int next_state_ID) {
	xQueueSend(sm.queue, &next_state_ID, 0);
}

void smRunStates(void) {
	if (sm.current->run)
		(sm.current->run)(sm.current->data);
}

void smHandlerTask(void *argument) {
	TickType_t prev_wake_time;
	unsigned char state_in;

	if (!sm._initialized) {
		if (smInit()) {
			fprintf(stderr, "No states\n");
			exit(EXIT_FAILURE);
		}
		sm.current = sm.next = sm.head;
	}

	while (1) {
		if (xQueueReceive(sm.queue, &state_in, 0) == pdTRUE)
			smSwapStates(state_in);
		smRunStates();
		vTaskDelayUntil(&prev_wake_time, STATE_MACHINE_INTERVAL);
	}
}
