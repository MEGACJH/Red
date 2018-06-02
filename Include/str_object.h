
#ifndef str_object_h
#define str_object_h

#include "foundation.h"
#include "os_headers.h"

typedef struct _str_object {
    SUPER_OBJECT_HEAD;
    int str_length_;
    char *str_value_;
    long hash; /* Store the hash value to speed up the routine */
} StrObject;

REDAPI_FUNC(SuperObject *) str_object_create(char *);

extern TypeObject g_str_type_object;

#endif /* str_object_h */
