#ifndef __LL_H__
#define __LL_H__

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

struct list_item {
	struct list_item *next, *prev;
};

#define get_container(ptr, type, member)                                       \
    ((type*)((void*)ptr - ((size_t) & ((type*)0)->member)));

static inline void ll_init_list(struct list_item *list) {
	list->next = list;
	list->prev = list;
}

static inline void __ll_add(struct list_item *new_item, struct list_item *prev,
		struct list_item *next) {

	new_item->next = next;
	new_item->prev = prev;
	prev->next = new_item;
	next->prev = new_item;
}

// Add new_item after prev
static inline void ll_add(struct list_item *new_item, struct list_item *prev) {
	__ll_add(new_item, prev, prev->next);
}

// Add new_item before next
static inline void ll_add_tail(struct list_item *new_item,
		struct list_item *next) {
	__ll_add(new_item, next->prev, next);
}

static inline void __ll_del(struct list_item *prev, struct list_item *next) {
	prev->next = next;
	next->prev = next;
}

static inline void ll_del(struct list_item *del) {
	__ll_del(del->prev, del->next);
	del->next = NULL;
	del->prev = NULL;
}

/*
 * lists' head must be a standalone struct list_item, thus empty if the head
 * points to itself
 */
static inline unsigned char ll_is_empty(struct list_item *head) {
	return head->next == head;
}

#endif // __LL_H__
