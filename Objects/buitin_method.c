
#include "builtin_method.h"

static void builtinmo_dealloc(SuperObject *self) {
    BuiltinMO *mo = TO_BUILTINMO(self);
    free(mo);
}

static void builtinmo_free(SuperObject *self) {
    if (!CHECK_BUILTINMO(self)) {
        printf("Silly! I need a builtin object\n");
        return;
    }
    
    DECREASE_REF_COUNT(self);
    if (!self->ref_count_)
        builtinmo_dealloc(self);
}

TypeObject g_builtin_method_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "BuiltinMethod",
    sizeof(BuiltinMO),
    "$JAZI_HOME/Doc/built_method.md",
    .free_ = builtinmo_free,
    .dealloc_ = builtinmo_dealloc,
};


SuperObject * builtinmo_obj_create(cfun_ptr_union cfun) {
    BuiltinMO *self = (BuiltinMO *) malloc(sizeof(BuiltinMO));
    INCREASE_REF_COUNT(self);
    INCREASE_REF_COUNT(&g_builtin_method_type_object);
    self->cfun_ptr_ = cfun;
    self->obj_type_info_ = &g_builtin_method_type_object;
    return self;
}

