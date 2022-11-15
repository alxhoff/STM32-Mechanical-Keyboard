#ifndef __SM_H__
#define __SM_H__

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#define STATE_MACHINE_STATE_QUEUE_LENGTH 10
#define STATE_MACHINE_INTERVAL 10

extern QueueHandle_t state_queue;

unsigned int smAddState(signed int ID, char *name, void* (*init)(void*),
		void (*enter)(void*), void (*run)(void*), void (*exit)(void*),
		void *data);
void smSetStates(unsigned int next_state_ID);
void smHandlerTask(void *argument);
unsigned char smInit(void);

#endif
