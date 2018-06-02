
#include "foundation.h"
#include "os_headers.h"

#define macro_func_helper_move_elements_from_right_to_left(object, index) do{    \
for (int j = index; j < object->item_size_; j++) {                                  \
object->items_[j] = object->items_[j + 1];                                          \
}                                                                                                       \
}while (0)


#define macro_func_ajust_after_remove(list_self, index) do{    \
list_self->item_size_--;                                                   \
DECREASE_REF_COUNT(list_self->items_[index]);            \
macro_func_helper_move_elements_from_right_to_left(list_self, index); \
} while (0)

void list_object_show_description_selection(SuperObject *self, int flag, int flag2) {
    ListObject *list_self = TO_LIST(self);
    int size = list_self->item_size_, i = 0;
    if (flag == 1)
        printf("<%s size = %d, cap = %d> {\n", strcmp(list_self->obj_type_info_->type_name_, "List") == 0 ? "List" : "Stack", size, list_self->cap_);
    
    SuperObject *tmp_object;
    for (; i < size; i++) {
        tmp_object = list_self->items_[i];
        tmp_object->obj_type_info_->description_(tmp_object, flag2);
    }
    
    if (flag == 1)
        printf("\n}\n");
}

static SuperObject *list_object_show_description(SuperObject *self, int flag) {
    list_object_show_description_selection(self, 1, flag);
    return &g_none_object;
}

static SuperObject *list_object_concat(SuperObject *self, SuperObject *other) {
    ListObject *list_self = TO_LIST(self);
    
    if (!CHECK_TYPE(&g_list_type_object, other) || !CHECK_TYPE(&g_list_type_object, self)) {
        printf(NEED_LIST);
        return NULL;
    }
    ListObject *list_other = TO_LIST(other);
    
    int all_items_size = list_self->item_size_ + list_other->item_size_ ;
    
    if (all_items_size > list_self->cap_) {
        list_self->items_ = (SuperObject **) realloc(list_self->items_, sizeof(SuperObject *) * all_items_size + 10);
        list_self->cap_ = all_items_size + 10;
    }
    
    
    for (int i = list_self->item_size_, j = 0; i < all_items_size; i++, j++) {
        list_self->items_[i] = list_other->items_[j];
        INCREASE_REF_COUNT(list_self->items_[i]);
        list_self->item_size_++;
    }
    
    return &g_none_object;
}

static SuperObject * list_object_get_length(SuperObject *self) {
    return int_object_create(TO_LIST(self)->item_size_);
}

static SuperObject *list_object_get_cap(SuperObject *self) {
    return int_object_create(TO_LIST(self)->cap_);
}

static SuperObject *list_object_get_dup_count(SuperObject *self, SuperObject *target) {
    ListObject *list_self = TO_LIST(self);
    
    int count = 0;
    
    for (int i = 0; i < list_self->item_size_; i++) {
        if (list_self->items_[i] == target)
            count++;
    }
    
    return int_object_create(count);
}

static SuperObject *list_object_insert_obj(SuperObject *self, SuperObject *to_insert, int index) {
    if (!CHECK_TYPE(&g_list_type_object, self)) {
        printf(NEED_LIST);
        return &g_none_object;
    }
    
    ListObject *list_self = TO_LIST(self);
    
    if (index < 0 || index > list_self->item_size_) {
        printf(OUT_RANGE);
        return &g_none_object;
    }
    
    
    if (list_self->item_size_ == list_self->cap_) {
        int new_cap = list_self->cap_  * 2;
        list_self->items_ = (SuperObject **) realloc(list_self->items_, sizeof(SuperObject *) * (new_cap));
        list_self->cap_ = new_cap;
    }
    
    if (index == list_self->item_size_) {
        list_self->items_[list_self->item_size_] = to_insert;
        list_self->item_size_++;
        INCREASE_REF_COUNT(to_insert);
        return &g_none_object;
    }

    for (int i = list_self->item_size_; i > index + 1; i--) {
        list_self->items_[i] = list_self->items_[i - 1];
    }
    list_self->items_[index] = to_insert;
    list_self->item_size_++;
    INCREASE_REF_COUNT(to_insert);
    return &g_none_object;
}

static SuperObject *list_object_append_obj(SuperObject *self, SuperObject *to_append) {
    return list_object_insert_obj(self, to_append, TO_LIST(self)->item_size_);
}

static SuperObject *list_object_remove_obj(SuperObject *self, SuperObject *to_remove) {
    if (!CHECK_TYPE(&g_list_type_object, self)) {
        printf(NEED_LIST);
        return &g_none_object;
    }
    
    ListObject *list_self = TO_LIST(self);
    for (int i = 0; i < list_self->item_size_; i++)
        if (CALL_NUM(list_self->items_[i])->num_eq_(list_self->items_[i], to_remove)) {
            macro_func_ajust_after_remove(list_self, i);
            break;
        }
    return &g_none_object;
}

static SuperObject *list_object_remove_by_index(SuperObject *self, int index) {
    if (!CHECK_TYPE(&g_list_type_object, self)) {
        printf(NEED_LIST);
        return &g_none_object;
    }
    
    ListObject *list_self = TO_LIST(self);
    
    if (index < 0 || index > list_self->item_size_ - 1) {
        printf(OUT_RANGE);
        return &g_none_object;
    }
    
    macro_func_ajust_after_remove(list_self, index);
    return &g_none_object;
}

static SuperObject *list_object_delivery(SuperObject *self, int begin, int end) {
    if (!CHECK_TYPE(&g_list_type_object, self)) {
        printf(NEED_LIST);
        return &g_none_object;
    }
    
    ListObject *list_self = TO_LIST(self);
    if (begin > end || begin < 0 || end > list_self->item_size_) {
        printf(OUT_RANGE);
        return &g_none_object;
    }
    
    ListObject *sub_list_object = list_object_create(end - begin);
    
    for (int i = begin; i < end; i++) {
        CALL_SQE(sub_list_object)->sqe_append_obj_(sub_list_object, list_self->items_[i]);
        INCREASE_REF_COUNT(list_self->items_[i]);
    }
    
    return sub_list_object;
}

static void list_object_dealloc(SuperObject *self) {
    ListObject *list_self = TO_LIST(self);
    
    SuperObject *tmp;
    for (int i = 0; i < list_self->item_size_; i++) {
        tmp = list_self->items_[i];
        tmp->obj_type_info_->free_(tmp);
    }
    
    free(TO_LIST(self));
}

static void list_object_free(SuperObject *self) {
    DECREASE_REF_COUNT(self);
    if (!self->ref_count_)
        list_object_dealloc(self);
}

static SuperObject *list_object_get_by_index(SuperObject *self, int index) {
    if (!CHECK_LIST(self)) {
        printf(NEED_LIST);
        return &g_none_object;
    }
    
    ListObject *list_self = TO_LIST(self);
    if (index < 0 || index > list_self->item_size_ - 1) {
        printf(OUT_RANGE);
        return &g_none_object;
    }
    ListObject *reval = TO_LIST(list_object_delivery(self, index, index + 1));
    DECREASE_REF_COUNT(reval);
    return reval->items_[0];
}

static SuperObject *list_object_return_index_(SuperObject *self, SuperObject *obj) {
    if (!CHECK_LIST(self)) {
        printf(NEED_LIST);
        return &g_none_object;
    }
    
    ListObject *list_self = TO_LIST(self);
    for (int i = 0; i < list_self->item_size_; i++) {
        if (TO_INT(CALL_NUM(list_self->items_[i])->num_eq_(list_self->items_[i], obj))->int_value_ == 1) {
            return int_object_create(i);
        }
    }
    
    return &g_none_object;
}

static SuperObject *list_obj_replace_by_index(SuperObject *self, SuperObject *obj, SuperObject *index) {
    SuperObject *reval = list_object_return_index_(self, obj);
    if (!reval)
        return &g_none_object;
    
    if (reval == &g_none_object)
        return &g_none_object;
    
    ListObject *list_self = TO_LIST(self);
    
    if (!CHECK_INT(index)) {
        printf(NEED_INT);
        return &g_none_object;
    }
    
    int index0 = TO_INT(index)->int_value_;
    
    if (index0 > list_self->item_size_ - 1 || index0 < 0) {
        printf(OUT_RANGE);
        return &g_none_object;
    }
    
    SuperObject *old = list_self->items_[index0];
    list_self->items_[index0] = obj;
    DECREASE_REF_COUNT(old);
    INCREASE_REF_COUNT(obj);
    return &g_none_object;
}

static SuperObject *list_obj_eq(SuperObject *self, SuperObject *obj) {
    if (!CHECK_LIST(self)) {
        printf(NEED_LIST);
        return &g_none_object;
    }
    
    if (!IS_SAME_TYPE(self, obj)) {
        return int_object_create(0);
    }
    
    ListObject *list_self = TO_DICT(self);
    ListObject *obj_self = TO_DICT(obj);
    
    if (list_self->item_size_ != obj_self->item_size_)
        return int_object_create(0);
    
    int flag = 1, cycle = list_self->item_size_;
    SuperObject *tmp_obj0, *tmp_obj1;
    for (int i = 0; i < cycle; i++) {
        tmp_obj0 = list_self->items_[i];
        tmp_obj1 = obj_self->items_[i];
        if (!CALL_NUM(tmp_obj0)->num_eq_(tmp_obj0, tmp_obj1)) {
            flag = 0;
            break;
        }
    }
    
    return int_object_create(flag);
}

static SuperObject *list_obj_push(SuperObject *self, SuperObject *obj) {
    return list_object_append_obj(self, obj);
}

static SuperObject *list_obj_pop(SuperObject *self) {
    ListObject *list_self = TO_LIST(self);
    SuperObject *reval = list_object_get_by_index(list_self, list_self->item_size_ - 1);
    list_self->obj_type_info_->sqe_funcs_->sqe_remove_by_index_(list_self, list_self->item_size_ - 1);
    return reval;
}

static SuperObject *list_obj_top(SuperObject *self) {
    ListObject *list_self = TO_LIST(self);
    return list_object_get_by_index(list_self, list_self->item_size_ - 1);
}

static SuperObject *list_obj_head(SuperObject *self) {
    ListObject *list_self = TO_LIST(self);
    return list_object_get_by_index(list_self, 0);
}

/*
 obj_db_args sqe_push_obj_;
 obj_one_arg sqe_pop_obj_;
 obj_one_arg sqe_top_obj_;
 */

static SuperObject *list_obj_pop_head(SuperObject *self) {
    ListObject *list_self = TO_LIST(self);
    SuperObject *reval = list_object_get_by_index(list_self, 0);
    list_self->obj_type_info_->sqe_funcs_->sqe_remove_by_index_(list_self, 0);
    return reval;
}

SqeuenceMethodsSet list_seq_methods_set = {
    .sqe_concat_ = list_object_concat,
    .sqe_get_length_ = list_object_get_length,
    .sqe_get_cap_ = list_object_get_cap,
    .sqe_get_dup_count_ = list_object_get_dup_count,
    .sqe_insert_obj_ = list_object_insert_obj,
    .sqe_append_obj_ = list_object_append_obj,
    .sqe_delivery_ = list_object_delivery,
    .sqe_remove_obj_ = list_object_remove_obj,
    .sqe_remove_by_index_ = list_object_remove_by_index,
    .sqe_get_by_index_ = list_object_get_by_index,
    .sqe_replace_by_index_ = list_obj_replace_by_index,
    .sqe_return_index_ = list_object_return_index_,
    .sqe_push_obj_ = list_obj_push,
    .sqe_pop_obj_ = list_obj_pop,
    .sqe_top_obj_ = list_obj_top,
    .sqe_pop_head_ = list_obj_pop_head,
    .sqe_return_head_ = list_obj_head,
};

static void list_dealloc(SuperObject *self) {
    ListObject *list = TO_LIST(self);
    
    for (int i = 0; i < list->item_size_; i++)
        list->items_[i]->obj_type_info_->free_(list->items_[i]);
    
    free(list->items_);
    free(list);
}

static void list_free(SuperObject *self) {
    if (!CHECK_LIST(self)) {
        printf(NEED_LIST);
        return;
    }
    
    ListObject *list = TO_LIST(self);
    DECREASE_REF_COUNT(list);
    if (!self->ref_count_)
        list_dealloc(self);
}

TypeObject g_list_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "List",
    sizeof(ListObject),
    "$JAZI_HOME/Doc/list_object.md",
    .description_ = list_object_show_description,
    .sqe_funcs_ = &list_seq_methods_set,
    .free_ = list_free,
    .dealloc_ = list_dealloc
};

SuperObject * list_object_create(int cap) {
    ListObject *object = (ListObject *) malloc(sizeof(ListObject));
    INCREASE_REF_COUNT(object);
    INCREASE_REF_COUNT(&g_list_type_object);
    object->obj_type_info_ = &g_list_type_object;
    object->cap_ = cap;
    object->item_size_ = 0;
    object->items_ = (SuperObject **) malloc(sizeof(SuperObject *) * cap);
    return object;
}
