
#include "frame_object.h"
#include "RedEngine.h"

static void frame_dealloc(SuperObject *self) {
    FrameObject *frame = (FrameObject *) self;
    free(frame->f_back_);
    free(frame->locals_);
    free(frame);
}

static void frame_free(SuperObject *self) {
    DECREASE_REF_COUNT(self);
    if (!self->ref_count_)
        frame_dealloc(self);
}

TypeObject g_frame_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "Frame",
    sizeof(FrameObject),
    "&JAZI_HOME/Doc/frame_object.md",
};

FrameObject * frame_new() {
    FrameObject *frame = (FrameObject *) malloc(sizeof(FrameObject));
    INCREASE_REF_COUNT(frame);
    INCREASE_REF_COUNT(&g_frame_type_object);
    return frame;
}

FrameObject *frame_create(SuperObject *f_back, struct _dict_object *locals, struct _dict_object *globals) {
    FrameObject *frame = (FrameObject *) malloc(sizeof(FrameObject));
    INCREASE_REF_COUNT(frame);
    INCREASE_REF_COUNT(&g_frame_type_object);
    frame->f_back_ = f_back;
    frame->locals_ = locals;
    return frame;
}

REDAPI_FUNC(void) frame_back_to_pool(FrameObject *sub) {
    sub->f_back_ = NULL;
    sub->locals_ = TO_DICT(dict_object_create());
    sub->token_list_ = TO_LIST(list_object_create(10));
    CALL_SQE(subframes)->sqe_append_obj_(subframes, sub);
}
