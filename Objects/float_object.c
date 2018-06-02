

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "float_object.h"

static SuperObject *float_object_show_description(SuperObject *self, int flag) {
    if (flag == 1) {
        printf("%lf\n", TO_FLOAT(self)->float_value_);
    } else {
        printf("%lf", TO_FLOAT(self)->float_value_);
    }
    
    return &g_none_object;
}

static long float_object_calc_hash(SuperObject *self) {
    return TO_FLOAT(self)->float_value_ * 2;
}

static SuperObject *float_object_add(SuperObject *self, SuperObject *right) {
    if (right->obj_type_info_->type_name_ == g_int_type_object.type_name_) {
        return float_object_create(TO_FLOAT(self)->float_value_ + TO_INT(right)->int_value_);
    }
    
    return float_object_create(TO_FLOAT(self)->float_value_ + TO_FLOAT(right)->float_value_);
}

static SuperObject *float_object_sub(SuperObject *self, SuperObject *right) {
    if (right->obj_type_info_->type_name_ == g_int_type_object.type_name_) {
        return float_object_create(TO_FLOAT(self)->float_value_ - TO_INT(right)->int_value_);
    }
    return TO_SUPER(float_object_create(TO_FLOAT(self)->float_value_ - TO_FLOAT(right)->float_value_));
}

static SuperObject *float_object_multi(SuperObject *self, SuperObject *right) {
    if (right->obj_type_info_->type_name_ == g_int_type_object.type_name_) {
        return float_object_create(TO_FLOAT(self)->float_value_ * TO_INT(right)->int_value_);
    }
    return TO_SUPER(float_object_create(TO_FLOAT(self)->float_value_ * TO_FLOAT(right)->float_value_));
}

static SuperObject *float_object_divide(SuperObject *self, SuperObject *right) {
    if (right->obj_type_info_->type_name_ == g_int_type_object.type_name_) {
        return float_object_create(TO_FLOAT(self)->float_value_ / TO_INT(right)->int_value_);
    }
    return TO_SUPER(float_object_create(TO_FLOAT(self)->float_value_ / TO_FLOAT(right)->float_value_));
}

static SuperObject *float_object_mod(SuperObject *self, SuperObject *right) {
    return TO_SUPER(float_object_create((long)TO_FLOAT(self)->float_value_ % (long)TO_FLOAT(right)->float_value_));
}

static SuperObject *float_object_abs(SuperObject *self) {
    double float_value = TYPE_CONVERT(FloatObject, self)->float_value_;
    return TO_SUPER(float_object_create(float_value < 0 ? (-float_value) : float_value));
}

static void float_object_dealloc(SuperObject *self) {
    free(TO_FLOAT(self));
}

static void float_object_free(SuperObject *self) {
    self->ref_count_--;
    if (!self->ref_count_)
        float_object_dealloc(self);
}

static SuperObject *float_object_pow(SuperObject *self, SuperObject *an_int_object) {
    if (!CHECK_INT_OR_FLOAT(self) || !CHECK_INT(an_int_object)) {
        printf(NEED_INT);
        return &g_none_object;
    }
    
    FloatObject *float_self = TO_FLOAT(self);
    IntObject *an_int = TO_INT(an_int_object);
    
    double float_self_value = float_self->float_value_;
    long an_int_value = an_int->int_value_;
    
    double result = float_self_value;
    
    for (int i = 0; i < an_int_value - 1; i++) {
        result *= float_self_value;
    }
    
    return float_object_create(result);
}

static SuperObject *float_object_to_int(SuperObject *self) {
    return int_object_create((long) (TO_FLOAT(self)->float_value_));
}


#define BEFORE_FLOAT_CMP()        \
if (!CHECK_FLOAT(self) && !CHECK_FLOAT(self)) { \
printf(NEED_FLOAT);\
return &g_none_object;\
}\
FloatObject *float_self = TO_FLOAT(self);\
if (!IS_SAME_TYPE(float_self, obj)) {\
return int_object_create(0);\
}\

#define FLOAT_CMP(symbol) \
BEFORE_FLOAT_CMP();\
FloatObject *float_obj = TO_FLOAT(obj);\
IntObject *reval = int_object_create(float_self->float_value_ symbol float_obj->float_value_);\

static SuperObject *float_obj_eq(SuperObject *self, SuperObject *obj) {
    
    FLOAT_CMP(==);
    return reval;
}

static SuperObject *float_obj_lt(SuperObject *self, SuperObject *obj) {
    FLOAT_CMP(<);
    return reval;
}

static SuperObject *float_obj_le(SuperObject *self, SuperObject *obj) {
    FLOAT_CMP(<=);
    return reval;
}

static SuperObject *float_obj_gt(SuperObject *self, SuperObject *obj) {
    FLOAT_CMP(>);
    return reval;
}

static SuperObject *float_obj_ge(SuperObject *self, SuperObject *obj) {
    FLOAT_CMP(>=);
    return reval;
}

static SuperObject *float_obj_ne(SuperObject *self, SuperObject *obj) {
    FLOAT_CMP(!=);
    return reval;
}

NumbericalMethodsSet float_num_funs = {
    float_object_add,
    float_object_sub,
    float_object_multi,
    float_object_divide,
    float_object_mod,
    float_object_abs,
    .num_pow_ = float_object_pow,
    .num_to_int = float_object_to_int,
    .num_eq_ = float_obj_eq,
    .num_lt_ = float_obj_lt,
    .num_gt_ = float_obj_gt,
    .num_le_ = float_obj_le,
    .num_ge_ = float_obj_ge,
    .num_ne_ = float_obj_ne,
};

TypeObject g_float_type_object = {
    SUPER_OBJECT_HEAD_INIT((SuperObject *) (&g_root_type_object)),
    "Float",
    sizeof(FloatObject),
    "$JAZI_PATH/Doc/float_object.md",
    float_object_show_description,
    float_object_calc_hash,
    float_object_free,
    float_object_dealloc,
    &float_num_funs,
    0,
    0
};

FloatObject *float_object_create(float float_value) {
    FloatObject *float_object = (FloatObject *) malloc(sizeof(FloatObject));
    INCREASE_REF_COUNT(float_object);
    float_object->float_value_ = float_value;
    float_object->obj_type_info_ = CONVERT_TYPE_OBJECT_TO_SUPER_OBJECT(&g_float_type_object);
    INCREASE_REF_COUNT(&g_float_type_object);
    return float_object;
}

