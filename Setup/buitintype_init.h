
#ifndef buitintype_init_h
#define buitintype_init_h

#include "foundation.h"
#include "os_headers.h"

extern struct _stack_object *runtime_stack;
extern struct _stack_object *statments_stack;

REDFORBIDEN_FUNC(void) init_inherit_func();
REDAPI_FUNC(void) init_runtime_stack();
REDAPI_FUNC(void) init_statments_stack();

#endif /* buitintype_init_h */
