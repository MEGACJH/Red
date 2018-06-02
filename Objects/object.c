
#include <stdio.h>

#ifndef _OBJECT_H
#define _OBJECT_H
#include "object.h"

TypeObject g_root_type_object = {
    SUPER_OBJECT_HEAD_INIT((SuperObject *) (&g_root_type_object)),
    "Type",
    sizeof(TypeObject),
    "$JAZI_PAHT/Doc/type_object.md",
};

REDAPI_FUNC(void) api_print_object(SuperObject *object, int flag) {
    if (!object)
        return;
    object->obj_type_info_->description_(object, flag);
}

#endif


