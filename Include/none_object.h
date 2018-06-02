

#ifndef none_object_h
#define none_object_h

#include "foundation.h"
#include "os_headers.h"

typedef struct _none_object {
    SUPER_OBJECT_HEAD;
} NoneObject;

extern TypeObject g_none_type_object;

extern NoneObject g_none_object;

#endif /* none_object_h */
