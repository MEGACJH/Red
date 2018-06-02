

#include "str_object.h"
#include "string.h"


static SuperObject *str_object_show_description(SuperObject *self, int flag) {
    StrObject *str = TO_STR(self);
    if (flag) {
        printf("%s\n", str->str_value_);
    } else {
        printf("%s", str->str_value_);
    }
    return &g_none_object;
}

static long str_object_calc_hash(SuperObject *self) {
    StrObject *str_self = TO_STR(self);
    if (str_self->hash != -1)
        return str_self->hash;
    long hash;
    int h = 0;
    if (h == 0 && str_self->str_length_ > 0) {
        char val[str_self->str_length_];
        strcpy(val, str_self->str_value_);
        for (int i = 0; i < str_self->str_length_; i++) {
            h = 31 * h + val[i];
        }
        hash = h;
    }
    str_self->hash = h;
    return h;
}

static SuperObject *str_object_concat(SuperObject *left, SuperObject *right) {
    char *left_value = TO_STR(left)->str_value_;
    char *right_value = TO_STR(right)->str_value_;
    int str_value_length = strlen(left_value) + strlen(right_value) + 1;
    char *result_value = (char *) malloc(sizeof(char) * str_value_length);
    
    strcpy(result_value, left_value);
    strcat(result_value, right_value);
    
    StrObject *new_str_object = str_object_create(result_value);
    
    free(result_value);
    return new_str_object;
}

static SuperObject *str_object_get_length(SuperObject *self) {
    return int_object_create(TO_STR(self)->str_length_);
}

static SuperObject *str_object_get_cap(SuperObject *self) {
    return int_object_create(TO_STR(self)->str_length_);
}

static SuperObject *str_object_get_dup_count(SuperObject *self, SuperObject *target) {
    
    if (TO_STR(target)->str_length_ > 1) {
        printf("The length of the target string object should be 1");
        return &g_none_object;
    }
        
    StrObject *str_self = TO_STR(self);
    StrObject *str_target = TO_STR(target);
    char *str_self_chars = str_self->str_value_;
    char *str_target_chars = str_target->str_value_;
    
    int count = 0;
    
    for (int i = 0; i < str_self->str_length_; i++) {
        if (str_self_chars[i] == str_target_chars[0])
            count++;
    }
    
    return int_object_create(count);
}

static void str_object_dealloc(SuperObject *self) {
    free(TO_STR(self)->str_value_);
    free(TO_STR(self));
}

static void str_object_free(SuperObject *self) {
    DECREASE_REF_COUNT(self);
    if (!self->ref_count_)
        str_object_dealloc(self);
}

static SuperObject * str_object_delivery(SuperObject *self, int begin, int end) {
    if (!CHECK_TYPE(&g_str_type_object, self)) {
        printf(NEED_STR);
        return &g_none_object;
    }
    
    StrObject *str_self = TO_STR(self);
    if (begin > end || begin < 0 || end > str_self->str_length_ - 1) {
        printf(OUT_RANGE);
        return &g_none_object;
    }
    
    int size = sizeof(char) * (end - begin);
    char *new_value = (char *) malloc(sizeof(char) * size);
    memcpy(new_value, str_self->str_value_ + begin, size);
    StrObject *sub_str = str_object_create(new_value);
    free(new_value);
    return sub_str;
}

static SuperObject *str_object_append(SuperObject *self, SuperObject *to_append) {
    return str_object_concat(self, to_append);
}

static SuperObject *str_object_char_at(SuperObject *self, int index) {
    if (!CHECK_STR(self)) {
        printf(NEED_STR);
        return &g_none_object;
    }
    StrObject *str_self = TO_STR(self);
    if (index < 0 || index > str_self->str_length_ - 1) {
        printf(OUT_RANGE);
        return &g_none_object;
    }
    char value[] = {str_self->str_value_[index], '\0'};
    return str_object_create(value);
}


static const char *str_obj_protected_val(SuperObject *self) {
    if (!CHECK_STR(self)) {
        printf(NEED_STR);
        return &g_none_object;
    }
    
    StrObject *str_self = TO_STR(self);
    return str_self->str_value_;
}

static SuperObject *str_obj_eq(SuperObject *self, SuperObject *obj) {
    if (!CHECK_STR(self)) {
        printf(NEED_STR);
        return &g_none_object;
    }
    
    if (!IS_SAME_TYPE(self, obj)) {
        return int_object_create(0);
    }
    if (strcmp(str_obj_protected_val(self), str_obj_protected_val(obj)) == 0)
        return int_object_create(1);
    return int_object_create(0);
}

static SuperObject *str_obj_add(SuperObject *self, SuperObject *obj) {
    StrObject *str_self = TO_STR(self);
    StrObject *str_obj = TO_STR(obj);
    char *new = string_append(str_self->str_value_, str_obj->str_value_);
    return str_object_create(new);
}

NumbericalMethodsSet str_num_methods_set = {
    .num_eq_ = str_obj_eq,
    .num_add_ = str_obj_add,
};

SqeuenceMethodsSet str_sqeuence_methods_set = {
    .sqe_delivery_ = str_object_delivery,
    .sqe_concat_ = str_object_concat,
    .sqe_get_length_ = str_object_get_length,
    .sqe_get_by_index_ = str_object_char_at,
    .sqe_get_length_ = str_object_get_cap,
    .sqe_append_obj_ = str_object_append,
    .sqe_get_dup_count_ = str_object_get_dup_count,
};

TypeObject g_str_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "String",
    sizeof(StrObject),
    "$JAZI_PATH/Doc/int_object.md",
    .description_ = &str_object_show_description,
    .free_ = str_object_free,
    .dealloc_ = str_object_dealloc,
    .sqe_funcs_ = &str_sqeuence_methods_set,
    .calc_hash_ = str_object_calc_hash,
    .num_funcs_ = &str_num_methods_set,
};

SuperObject *str_object_create(char *str_value) {
    StrObject *str_object = (StrObject *) malloc(sizeof(StrObject));
    INCREASE_REF_COUNT(str_object);
    INCREASE_REF_COUNT(&g_str_type_object);
    str_object->obj_type_info_ = &g_str_type_object;
    str_object->hash = -1;
    int str_value_length = strlen(str_value) + 1;
    str_object->str_value_ = (char *) malloc(sizeof(char) * str_value_length);
    str_object->str_length_ = str_value_length - 1;
    if (str_value)
        strcpy(str_object->str_value_, str_value);
    else
        strcpy(str_object->str_value_, "");
    return str_object;
}
