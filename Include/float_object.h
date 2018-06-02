
#ifndef float_object_h
#define float_object_h

#include "foundation.h"
#include "os_headers.h"

extern TypeObject g_float_type_object;

typedef struct _float_object {
    SUPER_OBJECT_HEAD;
    double float_value_;
} FloatObject;

REDAPI_FUNC(FloatObject *) float_object_create(float);

#endif /* float_object_h */
