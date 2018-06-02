
#ifndef frame_object_h
#define frame_object_h

#include "foundation.h"
#include "os_headers.h"

typedef struct _frame_object {
    SUPER_OBJECT_HEAD;
    struct _frame_object *f_back_;
    struct _dict_object *locals_;
    struct _list_object *token_list_;
} FrameObject;

extern TypeObject g_frame_type_object;

REDAPI_FUNC(FrameObject *) frame_new();
REDAPI_FUNC(FrameObject *) frame_create(SuperObject *f_back, struct _dict_object *locals, struct _dict_object *globals);

REDAPI_FUNC(void) frame_back_to_pool(FrameObject *);

#endif /* frame_object_h */
