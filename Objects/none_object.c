
#include "none_object.h"

TypeObject g_none_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "None",
    sizeof(NoneObject),
    "$JAZI_HOME/Doc/none_object.md",
};

NoneObject g_none_object = {
    SUPER_OBJECT_HEAD_INIT(&g_none_type_object)
};


