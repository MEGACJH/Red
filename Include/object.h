

#ifndef OBJECT_H
#define OBJECT_H

#include "common_macro.h"

typedef unsigned int jazi_uint;
typedef unsigned long jazi_ulong;


/*
 *
 *Make this SuperObject small enough so that it's easier to convert type
 *Question:
 *   Which fields will be put into the struct _super_object but not into the obj_type_info?
 *Answer:
 *       If the field is unique, then put it into the obj_type_info, or put it into the struct _super_object
 */
typedef struct _super_object {
    SUPER_OBJECT_HEAD;
} SuperObject;

typedef SuperObject *(*obj_one_arg) (SuperObject *);
typedef void (*void_one_arg) (SuperObject *);
typedef void (*one_arg_void_ptr_returned) (SuperObject *);
typedef long (*long_one_arg) (SuperObject *);
typedef SuperObject *(*obj_db_args) (SuperObject *, SuperObject *);
typedef SuperObject *(*obj_three_args_objobjint) (SuperObject *, SuperObject *, int);
typedef SuperObject *(*obj_three_args_objintint) (SuperObject *, int, int);
typedef SuperObject *(*obj_db_args_objint) (SuperObject *, int);

typedef SuperObject *(*obj_three_args_objobjobj) (SuperObject *, SuperObject *, SuperObject *);

typedef struct _numberical_methods_set {
    obj_db_args num_add_;
    obj_db_args num_sub_;
    obj_db_args num_multi_;
    obj_db_args num_divide_;
    obj_db_args num_mod_;
    obj_one_arg num_abs_;
    obj_db_args num_pow_;
    obj_one_arg num_to_float_;
    obj_one_arg num_to_int;
    obj_db_args num_eq_;
    obj_db_args num_lt_;
    obj_db_args num_gt_;
    obj_db_args num_le_;
    obj_db_args num_ge_;
    obj_db_args num_ne_;
} NumbericalMethodsSet;

typedef struct _sqeuence_methods_set {
    obj_db_args sqe_concat_; /* Ling two sequences */
    obj_one_arg sqe_get_length_; /* The count of the elements */
    obj_one_arg sqe_get_cap_; /* The allocated size */
    obj_db_args sqe_get_dup_count_; /* Get the count of the duplicated elements */
    obj_db_args sqe_remove_obj_;
    obj_db_args sqe_append_obj_;
    obj_three_args_objobjint sqe_insert_obj_;
    obj_db_args sqe_remove_by_index_;
    obj_three_args_objintint sqe_delivery_;
    obj_db_args_objint sqe_get_by_index_;
    obj_db_args sqe_return_index_;
    obj_three_args_objobjobj sqe_replace_by_index_;
    obj_db_args sqe_push_obj_;
    obj_one_arg sqe_pop_obj_;
    obj_one_arg sqe_top_obj_;
    obj_one_arg sqe_pop_head_;
    obj_one_arg sqe_return_head_;
} SqeuenceMethodsSet;

typedef struct _dict_methods_set {
    obj_one_arg dict_get_entrys_; /* Return a sequence contains key & values */
    obj_one_arg dict_get_keys_; /* Return keys */
    obj_one_arg dict_get_values_; /* Return values */
    obj_db_args dict_get_value_; /* Return the value according to the specified key */
    obj_three_args_objobjobj dict_set_key_value_;
    obj_db_args dict_remove_obj_entry_by_key_;
} DictMethodsSet;

/*
 *brief: TypeObject is an object to save the detail infomation of the specified object
 *description:
 *   Define this TypeObject likes defining an class in Java
 *details:
 *   Different from the SuperObject, the TypeObject aims to contain all infomation of the types in Jazi,
 *   so the other type such as IntTypeObject, FloatTypeObject, StringTypeObject and so on are subsets of
 *   this TypeObject
 */
typedef struct _type_object {
    SUPER_OBJECT_HEAD;
    char *type_name_; /* The type name of the object */
    jazi_uint object_size_; /* The allocated size of the object in the memory */
    char *doc_path_;
    
    obj_db_args_objint description_; /* Print object itself */
    long_one_arg calc_hash_; /* Digest hash value of object */
    void_one_arg free_; /* Decrease reference count, if the count is zero, call the dealloc func to free the object */
    void_one_arg dealloc_; /* release the memory of the object */
    
    NumbericalMethodsSet *num_funcs_;
    SqeuenceMethodsSet *sqe_funcs_;
    DictMethodsSet *dic_funcs_;
    
    struct _type_object *base_class_; /* Point to the base class */
    SuperObject *methods_dict_;
} TypeObject;

extern TypeObject g_root_type_object;

REDAPI_FUNC(void) api_print_object(SuperObject *, int);

#endif /* object_h */
