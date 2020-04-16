#ifndef __OUTBOX_H__
#define __OUTBOX_H__ 

typedef void *outbox_handle_t;

struct outbox_ops {
    void (*send_report)(void *handle, void *report, size_t len);
};

#endif // __OUTBOX_H__
