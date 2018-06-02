
#ifndef stack_object_h
#define stack_object_h

#include "foundation.h"

typedef struct _stack_object {
    SUPER_OBJECT_HEAD;
    int item_size_;
    int cap_; /* Allocated size */
    SuperObject **items_; /* An array that store the SuperObjects */
} StackObject;

REDAPI_FUNC(SuperObject *) stack_object_create(int);

extern TypeObject g_stack_type_object;

#endif /* stack_object_h */
