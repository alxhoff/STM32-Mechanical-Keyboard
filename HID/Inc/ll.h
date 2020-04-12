#ifndef __LL_H__
#define __LL_H__ 

#include <stdlib.h>

typedef void* ll_handle_t;

typedef struct ll_item {
    void *data;
    struct ll_item *next;
};

ll_handle_t llCreateList(size_t item_size);
int llAddItem(ll_handle_t *list, void *data);
int llDeleteItem(ll_handle_t *list, struct ll_item *item);
int llDeleteItemData(ll_handle_t *list, void *data);
struct ll_item *llGetHead(ll_handle_t list);
size_t llGetItemSize(ll_handle_t list);
size_t llGetItemCount(ll_handle_t list);

#endif 
