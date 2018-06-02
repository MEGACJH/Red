

#ifndef list_object_h
#define list_object_h

#include "foundation.h"
#include "os_headers.h"

typedef struct _list_object {
    SUPER_OBJECT_HEAD;
    int item_size_;
    int cap_; /* Allocated size */
    SuperObject **items_; /* An array that store the SuperObjects */
} ListObject;

REDAPI_FUNC(SuperObject *) list_object_create(int);

extern TypeObject g_list_type_object;

#endif /* list_object_h */
