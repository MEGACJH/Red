
#include "foundation.h"
#include "os_headers.h"

TypeObject g_stack_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "Stack",
    sizeof(StackObject),
    "$JAZI_HOME/Doc/stack_object.md",
    .base_class_ = &g_list_type_object,
};

SuperObject * stack_object_create(int cap) {
    StackObject *object = (StackObject *) malloc(sizeof(StackObject));
    INCREASE_REF_COUNT(object);
    INCREASE_REF_COUNT(&g_stack_type_object);
    object->obj_type_info_ = &g_stack_type_object;
    object->cap_ = cap;
    object->item_size_ = 0;
    object->items_ = (SuperObject **) malloc(sizeof(SuperObject *) * cap);
    return object;
}
