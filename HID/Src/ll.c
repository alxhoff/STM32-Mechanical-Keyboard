
#include <ll.h>

struct ll_list {
    struct ll_item head;
    size_t item_size;
    unsigned item_count;
};

ll_handle_t llCreateList(size_t item_size)
{
    struct ll_list *ret = (struct ll_list *)calloc(1, sizeof(struct ll_list));
    if(!ret)
        return NULL;

    ret->item_size = item_size;

    return (ll_handle_t) ret;
}

int llAddItem(ll_handle_t *list, void *data)
{
    struct ll_list *l = (struct ll_list *)list;
    struct ll_item *iterator = &l->head;

    for(; iterator->next; iterator = iterator->next)
        ;

    iterator->next = (struct ll_item *) calloc(1, sizeof(struct ll_item));
    if(!iterator->next)
        return -1;

    iterator->next->data = data;
    l->item_count++;
    
    return 0;
}


int llDeleteItem(ll_handle_t *list, struct ll_item *item)
{
    struct ll_list *l = (struct ll_list *)list;
    struct ll_item *iterator = &l->head;
    struct ll_item *del;

    for(; iterator->next && iterator->next != item; iterator=iterator->next)
        ;

    if(iterator->next){
        if(iterator->next == item){
            del = iterator->next;
            if(!iterator->next->next)
                iterator->next = NULL;
            else 
                iterator->next = del->next;

            free(del);
            l->item_count--;
            return 0;
        }
    }

    return -1;
}

int llDeleteItemData(ll_handle_t *list, void *data)
{
    struct ll_list *l = (struct ll_list *)list;
    struct ll_item *iterator = &l->head;
    struct ll_item *del;

    for(; iterator->next; iterator=iterator->next)
        if(iterator->next->data == date)
            break;

    if(iterator->next){
        if(iterator->next->data == data){
            del = iterator->next;
            if(!iterator->next->next)
                iterator->next = NULL;
            else 
                iterator->next = del->next;

            free(del);
            l->item_count--;
            return 0;
        }
    }

    return -1;
}

struct ll_item *llGetHead(ll_handle_t list)
{
    return ((struct ll_list *) list)->head;
}

size_t llGetItemSize(ll_handle_t list)
{
    return ((struct ll_list *) list)->item_size;
}

size_t llGetItemCount(ll_handle_t list)
{
    return ((struct ll_list *) list)->item_count;
}
