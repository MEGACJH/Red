
#ifndef builtin_method_h
#define builtin_method_h

#include "foundation.h"
#include "os_headers.h"

/* One BuiltMO object packages one C function pointer [Today I'm tired :(] */
typedef union cfun_ptr_union {
    obj_one_arg obj_one_arg_c_;
    void_one_arg void_one_arg_c_;
    one_arg_void_ptr_returned one_arg_void_ptr_returned_c_;
    long_one_arg long_one_arg_c_;
    obj_db_args obj_db_args_c_;
    obj_three_args_objobjint obj_three_args_objobjint_c_;
    obj_three_args_objintint obj_three_args_objintint_c_;
    obj_db_args_objint obj_db_args_objint_c_;
} cfun_ptr_union;

typedef struct _builtin_method_object {
    SUPER_OBJECT_HEAD;
    cfun_ptr_union cfun_ptr_;
} BuiltinMO;

extern TypeObject g_builtin_method_type_object;

REDAPI_FUNC(SuperObject *) builtinmo_obj_create(cfun_ptr_union cfun);

#endif /* builtin_method_h */
