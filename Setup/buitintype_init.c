
#include "buitintype_init.h"

#define macro_func_int_add_funcptr(methodname)         \
CALL_DIC(int_type_dict)->dict_set_key_value_(int_type_dict, str_object_create("abs"), builtinmo_obj_create(ptr));

void init_inherit_func() {
    g_stack_type_object.description_ = g_list_type_object.description_;
    g_stack_type_object.sqe_funcs_ = g_list_type_object.sqe_funcs_;
    g_stack_type_object.free_ = g_list_type_object.free_;
    g_stack_type_object.dealloc_ = g_list_type_object.dealloc_;
}

struct _stack_object *runtime_stack;

REDAPI_FUNC(void) init_runtime_stack() {
    runtime_stack = stack_object_create(10);
}

struct _stack_object *statments_stack;

void init_statments_stack() {
    statments_stack = stack_object_create(10);
}

