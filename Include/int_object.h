
#ifndef int_object_h
#define int_object_h

#include "foundation.h"
#include "os_headers.h"
extern TypeObject g_int_type_object;

typedef struct _int_object {
    SUPER_OBJECT_HEAD;
    long int_value_;
} IntObject;

REDAPI_FUNC(IntObject *)int_object_create(long);
REDAPI_FUNC(void) api_print_bool(IntObject *);

IntObject *int_object_new(long int_value);

extern IntObject **g_small_int_obj_pool;

#endif /* int_object_h */
