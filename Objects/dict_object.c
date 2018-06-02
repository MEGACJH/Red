

#include "dict_object.h"

extern TypeObject g_entry_type_object;

static SuperObject *entry_obj_eq(SuperObject *self, SuperObject *obj) {
    if (!IS_SAME_TYPE(self, obj)) {
        return &g_none_object;
    }
    
    Entry *en_self = (Entry *) self;
    Entry *en_obj = (Entry *) obj;
    IntObject *rev = CALL_NUM(en_self->key_)->num_eq_(en_self->key_, en_obj->key_);
    IntObject *reval = int_object_create(rev->int_value_);
    return reval;
}

void list_object_show_description_selection(SuperObject *self, int flag, int flag2);

static SuperObject *entry_description(SuperObject *self, int flag) {
    if (self->obj_type_info_->type_name_ != g_entry_type_object.type_name_) {
        printf("Need an entry object\n");
        return &g_none_object;
    }
    
    Entry *en_self = (Entry *) self;
    printf("\n");
    printf("%s :", TO_STR(en_self->key_)->str_value_);
    api_print_object(en_self->value_, flag);
    printf("\n");
    return &g_none_object;
}

NumbericalMethodsSet entry_num_set = {
    .num_eq_ = entry_obj_eq,
};

TypeObject g_entry_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "DictEntry",
    sizeof(Entry),
    "none :)",
    .num_funcs_ = &entry_num_set,
    .description_ = entry_description,
};

static SuperObject *entry_create(SuperObject *key, SuperObject *value) {
    Entry *entry = (Entry *) malloc(sizeof(Entry));
    entry->obj_type_info_ = &g_entry_type_object;
    entry->key_ = key;
    entry->value_ = value;
    INCREASE_REF_COUNT(key);
    if (value)
        INCREASE_REF_COUNT(value);
    INCREASE_REF_COUNT(entry);
    return entry;
}

static SuperObject *dict_object_set_key_value(SuperObject *self, SuperObject *key, SuperObject *value) {
    if (!CHECK_DICT(self)) {
        printf(NEED_DICT);
        return &g_none_object;
    }
    if (!CHECK_STR(key)) {
        printf(NEED_STR);
        return &g_none_object;
    }
    
    DictObject *dict_self = TO_DICT(self);
    StrObject *str_obj = TO_STR(key);
    
    int index = str_obj->obj_type_info_->calc_hash_(str_obj) % DICT_RANGE;
    ListObject *entries_list_ = CALL_SQE(dict_self->entries)->sqe_get_by_index_(dict_self->entries, index);
    Entry *entry_obj = entry_create(key, value);
    if (!entries_list_->item_size_) {
        CALL_SQE(entries_list_)->sqe_append_obj_(entries_list_, entry_obj);
        CALL_SQE(dict_self->not_empty_indexes_)->sqe_append_obj_(dict_self->not_empty_indexes_, int_object_create(index));
    } else {
        SuperObject *reval = CALL_SQE(entries_list_)->sqe_return_index_(entries_list_, entry_obj);
        if (reval->obj_type_info_->type_name_ == g_none_type_object.type_name_) {
           CALL_SQE(entries_list_)->sqe_append_obj_(entries_list_, entry_obj);
        } else {
            CALL_SQE(entries_list_)->sqe_replace_by_index_(entries_list_, entry_obj, reval);
        }
    }
    dict_self->entry_size_++;
    return &g_none_object;
}

#define SELF_KEY_PREPARE_CHECK()   \
if (!CHECK_DICT(self)) {\
    printf(NEED_DICT);\
    return &g_none_object;\
}\
if (!CHECK_STR(key)) {\
    printf(NEED_STR);\
    return &g_none_object;\
}\
DictObject *dict_self = TO_DICT(self);\
StrObject *str_key = TO_STR(key);\

static SuperObject *dict_object_get_value(SuperObject *self, SuperObject *key) {
    SELF_KEY_PREPARE_CHECK();
    int index = str_key->obj_type_info_->calc_hash_(str_key) % DICT_RANGE;
    ListObject *entries_list_ = CALL_SQE(dict_self->entries)->sqe_get_by_index_(dict_self->entries, index);
    if (!entries_list_->item_size_) {
        return &g_none_object;
    }
    
    for (int i = 0; i < entries_list_->item_size_; i++) {
        Entry *tmp = entries_list_->items_[i];
        if (strcmp(TO_STR(tmp->key_)->str_value_, TO_STR(tmp->key_)->str_value_) == 0) {
            return tmp->value_;
        }
    }
    return &g_none_object;
}

static SuperObject *dict_object_remove_entry(SuperObject *self, SuperObject *key) {
    SELF_KEY_PREPARE_CHECK();
    int index = str_key->obj_type_info_->calc_hash_(str_key) % DICT_RANGE;
    ListObject *entries_list_ = CALL_SQE(dict_self->entries)->sqe_get_by_index_(dict_self->entries, index);
    
    if (!entries_list_->item_size_) {
        return &g_none_object;
    }
    
    for (int i = 0; i < entries_list_->item_size_; i++) {
        Entry *tmp = entries_list_->items_[i];
        if (strcmp(TO_STR(tmp->key_)->str_value_, TO_STR(tmp->key_)->str_value_) == 0) {
            DECREASE_REF_COUNT(tmp->key_);
            DECREASE_REF_COUNT(tmp->value_);
            CALL_SQE(entries_list_)->sqe_remove_by_index_(entries_list_, i);
            free(tmp);
            break;
        }
    }
    
    dict_self->entry_size_--;
    if (entries_list_->item_size_ == 0) {
        CALL_SQE(dict_self->not_empty_indexes_)->sqe_remove_obj_(dict_self->not_empty_indexes_, int_object_create(index));
    }
    return &g_none_object;
}

static SuperObject * dict_description(SuperObject *self, int flag) {
    if (!CHECK_DICT(self)) {
        printf(NEED_DICT);
        return &g_none_object;
    }
    
    DictObject *dict_self = TO_DICT(self);
    printf("<<Dict item-size: %d>> {\n", dict_self->entry_size_);
    ListObject *tmp;
    int i = 0;
    for (; i < dict_self->not_empty_indexes_->item_size_ - 1; i++) {
        tmp = (dict_self->entries->items_[TO_INT(dict_self->not_empty_indexes_->items_[i])->int_value_]);
        list_object_show_description_selection(tmp, 0, flag);
    }
    tmp = (dict_self->entries->items_[TO_INT(dict_self->not_empty_indexes_->items_[i])->int_value_]);
    list_object_show_description_selection(tmp, 0, flag);
    printf("}\n");
    return &g_none_object;
}

DictMethodsSet dict_funcs = {
    .dict_set_key_value_ = dict_object_set_key_value,
    .dict_get_value_ = dict_object_get_value,
    .dict_remove_obj_entry_by_key_ = dict_object_remove_entry,
};

static void dict_dealloc(SuperObject *self) {
    DictObject *dict = TO_DICT(self);
    
    for (int i = 0; i < dict->not_empty_indexes_->item_size_; i++) {
        IntObject *tmp = CALL_SQE(dict->not_empty_indexes_)->sqe_get_by_index_(dict->not_empty_indexes_, i);
        dict->entries->items_[tmp->int_value_]->obj_type_info_->free_(dict->entries->items_[tmp->int_value_]);
    }
    
}

static void dict_free(SuperObject *self) {
    if (!CHECK_DICT(self)) {
        printf(NEED_DICT);
        return;
    }
    
    DECREASE_REF_COUNT(self);
    if (!self->ref_count_)
        dict_dealloc(self);
}

TypeObject g_dict_type_object = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "Dict",
    sizeof(DictObject),
    "$JAZI_HOME/Doc/dict_object.md",
    .dic_funcs_ = &dict_funcs,
    .description_ = dict_description,
    .free_ = dict_free,
    .dealloc_ = dict_dealloc,
};

SuperObject * dict_object_create() {
    DictObject *dict_object = (DictObject *) malloc(sizeof(DictObject));
    INCREASE_REF_COUNT(dict_object);
    INCREASE_REF_COUNT(&g_dict_type_object);
    dict_object->obj_type_info_ = &g_dict_type_object;
    dict_object->entry_size_ = 0;
    dict_object->entries = list_object_create(DICT_RANGE);
    dict_object->not_empty_indexes_ = list_object_create(1);
    for (int i = 0; i < DICT_RANGE; i++) {
       CALL_SQE(dict_object->entries)->sqe_append_obj_(dict_object->entries, list_object_create(1));
    }
    return dict_object;
}

