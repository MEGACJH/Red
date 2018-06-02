
#include <limits.h>

#ifndef common_macro_h
#define common_macro_h


#define REDAPI_FUNC(return_type) return_type
#define REDFORBIDEN_FUNC(return_type) return_type

#define SUPER_OBJECT_HEAD    \
int ref_count_;                           \
struct _type_object *obj_type_info_ \

#define SUPER_OBJECT_HEAD_INIT(obj_type_info)  0, obj_type_info

#define INCREASE_REF_COUNT(object)    ((object)->ref_count_++)
#define DECREASE_REF_COUNT(object)   ((object)->ref_count_--)

/* Aim to simplify assignment of type object */
#define CONVERT_TYPE_OBJECT_TO_SUPER_OBJECT(type_object) ((SuperObject *) (type_object))

/* Aim to simplify assignment of every object */
#define TYPE_CONVERT(SuperObject, object) ((SuperObject *) (object))

/* Fast paths to convert type */
#define TO_SUPER(object) ((SuperObject *) (object))
#define TO_INT(object) ((IntObject *) (object))
#define TO_FLOAT(object) ((FloatObject *) (object))
#define TO_STR(object) ((StrObject *) (object))
#define TO_LIST(object) ((ListObject *) (object))
#define TO_DICT(object) ((DictObject *) (object))
#define TO_BUILTINMO(object) ((BuiltinMO *) (object))
#define TO_TOKEN(object) (Token *) (object)

/* Check type */
#define CHECK_TYPE(type, object) (object->obj_type_info_->type_name_ == (type)->type_name_ || object->obj_type_info_->base_class_->type_name_ == ((type)->type_name_))
#define CHECK_INT(object) CHECK_TYPE(&g_int_type_object, object)
#define CHECK_FLOAT(object) CHECK_TYPE(&g_float_type_object, object)
#define CHECK_INT_OR_FLOAT(object) (CHECK_INT(object) || CHECK_FLOAT(object))
#define CHECK_LIST(object) CHECK_TYPE(&g_list_type_object, object)
#define CHECK_STR(object) CHECK_TYPE(&g_str_type_object, object)
#define CHECK_LINKED_LIST(object) CHECK_TYPE(&g_linkedlist_type_object, object)
#define CHECK_DICT(object) CHECK_TYPE(&g_dict_type_object, object)
#define CHECK_NONE(object) ((object)->obj_type_info_->type_name_ == (g_none_type_object).type_name_)
#define CHECK_BUILTINMO(object) CHECK_TYPE(&g_builtin_method_type_object, object)

#define IS_SAME_TYPE(obj0, obj1) (obj0->obj_type_info_->type_name_ == obj1->obj_type_info_->type_name_)

#define HASH    INT_MAX

#define CALL_SQE(object) (object->obj_type_info_->sqe_funcs_)
#define CALL_NUM(object) (object->obj_type_info_->num_funcs_)
#define CALL_DIC(object) (object->obj_type_info_->dic_funcs_)

#define BUILTINMO_GETCFUN(object) (TO_BUILTINMO(object)->cfun)

#define ADD_LINE(ascii, numilne) do {               \
    if ((ascii) == '\n')                                            \
    (numline)++;                                                \
} while (0)

#endif /* common_macro_h */
