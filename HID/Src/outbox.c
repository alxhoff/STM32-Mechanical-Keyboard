
#include <stdlib.h>

#include "outbox.h"
#include "descriptors.h"
#include "scanbuf.h"

struct outbox {
    void *handle;
    struct outbox_ops *ops;

};

outbox_handle_t outboxCreate(void *handle, struct outbox_ops *ops)
{
    struct outbox *ret = (struct outbox *) calloc(1, sizeof(struct outbox));
    if(!ret)
        return NULL;

    ret->handle = handle;
    ret->ops = ops;

    return (outbox_handle_t) ret;
}

void outboxConsumeScanBuf(void)
{

}

void outboxSendReport(struct outbox *ob, void *report, size_t len)
{
    (ob->ops->send_report)(ob->handle, report, len);
}
