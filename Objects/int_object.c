
#ifndef _int_object_h
#define _int_object_h

#include "int_object.h"

#define IS_FLOAT(obj) ((obj)->obj_type_info_->type_name_ == g_float_type_object.type_name_)
#define INT_OPERATOR_TO_FLOAT(obj) (IS_FLOAT(obj) ? TO_FLOAT((obj))->float_value_ : TO_INT((obj))->int_value_)
#define CONDITION_NEW_INT_FLOAT(self, obj, operator) (IS_FLOAT(obj) ? float_object_create(TO_INT(self)->int_value_ operator  INT_OPERATOR_TO_FLOAT(obj)) : int_object_create(TO_INT(obj)->int_value_ operator  INT_OPERATOR_TO_FLOAT(obj)))

static SuperObject *int_object_show_description(SuperObject *self, int flag) {
    if (flag == 1) {
        printf("%ld\n", ((IntObject *) self)->int_value_);
    } else {
        printf("%ld", ((IntObject *) self)->int_value_);
    }
    
    return &g_none_object;
}

static long int_object_calc_hash(SuperObject *self) {
    return TO_INT(self)->int_value_ * 2;
}

static SuperObject *int_object_add(SuperObject *self, SuperObject *obj) {
    if (IS_FLOAT(obj)) {
        return float_object_create(TO_INT(self)->int_value_ + TO_FLOAT(obj)->float_value_);
    }
    
    return int_object_create(TO_INT(self)->int_value_ + TO_INT(obj)->int_value_);
}

static SuperObject *int_object_sub(SuperObject *self, SuperObject *right) {
    if (IS_FLOAT(right)) {
        return float_object_create(TO_INT(self)->int_value_ - TO_FLOAT(right)->float_value_);
    }
    
    return int_object_create(TO_INT(self)->int_value_ - TO_INT(right)->int_value_);
}

static SuperObject *int_object_multi(SuperObject *self, SuperObject *right) {
    if (IS_FLOAT(right)) {
        return float_object_create(TO_INT(self)->int_value_ * TO_FLOAT(right)->float_value_);
    }
    
    return int_object_create(TO_INT(self)->int_value_ * TO_INT(right)->int_value_);
}

static SuperObject *int_object_divide(SuperObject *self, SuperObject *right) {
    if (IS_FLOAT(right)) {
        return float_object_create(TO_INT(self)->int_value_ / TO_FLOAT(right)->float_value_);
    }
    
    return int_object_create(TO_INT(self)->int_value_ / TO_INT(right)->int_value_);
}

static SuperObject *int_object_mod(SuperObject *self, SuperObject *right) {
    IntObject *tmp = NULL;
    if (!CHECK_INT(right)) {
        tmp = CALL_NUM(right)->num_to_int(right);
        return TO_SUPER(int_object_create(TO_INT(self)->int_value_ % tmp->int_value_));
    }
    
    return TO_SUPER(int_object_create(TO_INT(self)->int_value_ % TO_INT(right)->int_value_));
}

static SuperObject *int_object_abs(SuperObject *self) {
    long int_value = TYPE_CONVERT(IntObject, self)->int_value_;
    return int_object_create(int_value < 0 ? (-int_value) : int_value);
}

static SuperObject *int_object_pow(SuperObject *self, SuperObject *power) {
    
    if (!CHECK_INT(power)) {
        printf(NEED_INT);
        return &g_none_object;
    }
    
    IntObject *int_self = TO_INT(self);
    IntObject *int_power = TO_INT(power);
    
    long int_self_value = int_self->int_value_;
    long int_power_value = int_power->int_value_;
    
    long result = int_self_value;
    
    for (int i  = 0; i < int_power_value - 1; i++) {
        result *= int_self_value;
    }
    
    return int_object_create(result);
}

static SuperObject *int_object_to_float(SuperObject *self) {
    if (!CHECK_INT(self)) {
        printf(NEED_INT);
        return &g_none_object;
    }
    
    IntObject *int_self = TO_INT(self);
    
    return float_object_create((double) (int_self->int_value_));
}

static void int_object_dealloc(SuperObject *self) {
    free(TO_INT(self));
}


static void int_object_free(SuperObject *self) {
    self->ref_count_--;
    if (!self->ref_count_)
        int_object_dealloc(self);
}

#define BEFORE_INT_CMP()        \
    if (!CHECK_INT(self)) { \
        printf(NEED_INT);\
        return &g_none_object;\
    }\
    IntObject *int_self = TO_INT(self);\
    if (obj->obj_type_info_->type_name_ == g_float_type_object.type_name_) {\
        int_self = int_object_to_float(int_self);\
    }\
    if (!IS_SAME_TYPE(int_self, obj) && (obj->obj_type_info_->type_name_ == g_float_type_object.type_name_)) {\
        return int_object_create(0);\
    }\

#define INT_CMP(symbol) \
BEFORE_INT_CMP();\
IntObject *int_obj = TO_INT(obj);\
IntObject *reval = int_object_create(int_self->int_value_ symbol int_obj->int_value_);\

static SuperObject *int_obj_eq(SuperObject *self, SuperObject *obj) {
    INT_CMP(==);
    return reval;
}

static SuperObject *int_obj_lt(SuperObject *self, SuperObject *obj) {
    INT_CMP(<);
    return reval;
}

static SuperObject *int_obj_le(SuperObject *self, SuperObject *obj) {
    INT_CMP(<=);
    return reval;
}

static SuperObject *int_obj_gt(SuperObject *self, SuperObject *obj) {
    INT_CMP(>);
    return reval;
}

static SuperObject *int_obj_ge(SuperObject *self, SuperObject *obj) {
    INT_CMP(>=);
    return reval;
}

static SuperObject *int_obj_ne(SuperObject *self, SuperObject *obj) {
    INT_CMP(!=);
    return reval;
}

REDAPI_FUNC(void) api_print_bool(IntObject *self) {
    
    if (!CHECK_INT(self)) {
        printf(NEED_INT);
        return;
    }
    
    if (!self->int_value_)
        printf("NO\n");
    else
        printf("YES\n");
}

NumbericalMethodsSet int_num_funs = {
    int_object_add,
    int_object_sub,
    int_object_multi,
    int_object_divide,
    int_object_mod,
    int_object_abs,
    .num_pow_ = int_object_pow,
    .num_to_float_ = int_object_to_float,
    .num_eq_ = int_obj_eq,
    .num_lt_ = int_obj_lt,
    .num_gt_ = int_obj_gt,
    .num_le_ = int_obj_le,
    .num_ge_ = int_obj_ge,
    .num_ne_ = int_obj_ne,
};

TypeObject g_int_type_object = {
    SUPER_OBJECT_HEAD_INIT((SuperObject *) (&g_root_type_object)),
    "Int",
    sizeof(IntObject),
    "$JAZI_PATH/Doc/int_object.md",
    int_object_show_description,
    int_object_calc_hash,
    int_object_free,
    int_object_dealloc,
    &int_num_funs,
    0,
    0
};

IntObject **g_small_int_obj_pool;

IntObject *int_object_new(long int_value) {
    IntObject *int_object = (IntObject *) malloc(sizeof(IntObject));
    INCREASE_REF_COUNT(int_object);
    int_object->int_value_ = int_value;
    int_object->obj_type_info_ = CONVERT_TYPE_OBJECT_TO_SUPER_OBJECT(&g_int_type_object);
    INCREASE_REF_COUNT(&g_int_type_object);
    return int_object;
}

IntObject *int_object_create(long int_value) {
    if (int_value >= SMALL_INT_POOL_RANGE_FROM && int_value <= SMALL_INT_POOL_RANGE_TO) {
        IntObject *int_obj = g_small_int_obj_pool[int_value - SMALL_INT_POOL_RANGE_FROM];
        INCREASE_REF_COUNT(int_obj);
        return int_obj;
    } else {
        return int_object_new(int_value);
    }
    return &g_none_object;
}


#endif
