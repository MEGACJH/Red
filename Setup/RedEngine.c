

#include "RedEngine.h"
#include "foundation.h"
#include "scanner.h"

#define RUN_CODE_NUMBER_OPERATOR_CONTROLLER(num_add_)                                                                           \
do {                                                                                                                                                \
    token0 = CALL_SQE(statment)->sqe_pop_obj_(statment);                                                                    \
    obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);                                                     \
    if (token0->type_ == TOKEN_TYPE_IDENTIFY) {                                                                                 \
        str_obj = str_object_create(token0->name_);                                                                                 \
        if (find_object_in_locals_and_push_it_to_runtime_stack(token0, environment)) {                               \
            int_obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);                                             \
            int_obj = CALL_NUM(int_obj)->num_add_(int_obj, obj);                                                        \
            token0->name_ = "_";                                                                                                                \
            run_code_number_helper0(token0, _, int_obj, locals, statment);                                          \
        }                                                                                                                                                   \
    } else {                                                                                                                                                \
        if (token0->type_ == TOKEN_TYPE_INT) {                                                                                      \
            int_obj = int_object_create(atol(token0->name_));                                                                           \
            int_obj = CALL_NUM(int_obj)->num_add_(int_obj, obj);                                                                    \
            run_code_number_helper0(token0, _, int_obj, locals, statment);                                              \
        } else if (token0->type_ == TOKEN_TYPE_FLOAT){                                                                                      \
            float_obj = float_object_create(atof(token0->name_));                                                                   \
            float_obj = CALL_NUM(float_obj)->num_add_(float_obj, obj);                                                      \
            run_code_number_helper0(token0, _, float_obj, locals, statment);                                                    \
        } else if (token0->type_ == TOKEN_TYPE_STRING) {                                                                    \
            str_obj = str_object_create(token0->name_);                                                                                     \
            str_obj = CALL_NUM(str_obj)->num_add_(str_obj, obj);                                                                    \
            run_code_number_helper0(token0, _, str_obj, locals, statment);                                              \
        }                                                                                                                                                           \
    }                                                                                                                                       \
} while (0)


#define RUN_CODE_NUMBER_HELPER2(num_le_) do  {                                                  \
    token0 = CALL_SQE(statment)->sqe_pop_obj_(statment);                                         \
    obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);                                  \
    if (token0->type_ == TOKEN_TYPE_IDENTIFY) {                                                             \
        if (find_object_in_locals_and_push_it_to_runtime_stack(token0, environment)) {           \
            float_obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);                    \
            float_obj = CALL_NUM(float_obj)->num_le_(float_obj, obj);                                       \
            run_code_number_helper0(token0, _, float_obj, locals, statment);                            \
        }                                                                                                                                   \
    } else {                                                                                                                             \
        float_obj = float_object_create(atof(token0->name_));                                                    \
        float_obj = CALL_NUM(float_obj)->num_le_(float_obj, CALL_NUM(obj)->num_to_float_(obj)); \
        run_code_number_helper0(token0, _, float_obj, locals, statment);                                                \
    }                                                                                                                                           \
} while (0)


int if_condition_is_true = 0;

ListObject *subframes;

void init_subframes() {
    FrameObject *sub;
    subframes = list_object_create(10);
    for (int i = 0; i < 10; i++) {
        sub = frame_new();
        sub->f_back_ = NULL;
        sub->locals_ = TO_DICT(dict_object_create());
        sub->token_list_ = TO_LIST(list_object_create(10));
        CALL_SQE(subframes)->sqe_append_obj_(subframes, sub);
    }
}

void extend_subframes(ListObject *subframes) {
    FrameObject *sub = NULL;
    for (int i = 0; i < 5; i++) {
        sub = frame_new();
        sub->f_back_ = NULL;
        sub->locals_ = TO_DICT(dict_object_create());
        sub->token_list_ = TO_LIST(list_object_create(10));
        CALL_SQE(subframes)->sqe_append_obj_(subframes, sub);
    }
}


/*
 Return 1 if find
 Return 0 if not
 */
int find_object_in_locals_and_push_it_to_runtime_stack(Token *token0, FrameObject *file_frame) {
    DictObject *locals = file_frame->locals_;
    /* First of all, find object in current locals, if not find, go to last locals */
    SuperObject *obj = CALL_DIC(locals)->dict_get_value_(locals, str_object_create(token0->name_));
    
    if (obj != &g_none_object) {
        /* Put the object into runtime stack */
        CALL_SQE(runtime_stack)->sqe_append_obj_(runtime_stack, obj);
        return 1;
    }
    
    /* Now the Environment is the highest namespace */
    if (!file_frame->f_back_)
        return 0;
    else
        return find_object_in_locals_and_push_it_to_runtime_stack(token0, file_frame->f_back_);
}

static void run_code_number_helper0(Token *token0,
                                                        SuperObject *_,
                                                        SuperObject *obj,
                                                        DictObject *locals,
                                                        StackObject *statment)
{
    token0->name_ = "_";
    if (find_object_in_locals_and_push_it_to_runtime_stack(token0, environment))
        DECREASE_REF_COUNT(CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack));
    CALL_DIC(locals)->dict_set_key_value_(locals, _, obj);
    token0->type_ = TOKEN_TYPE_IDENTIFY;
    CALL_SQE(statment)->sqe_append_obj_(statment, token0);
}

static void _run_code_pop_statments(StackObject *statements_stack, ListObject *statment, int flag) {
     Token *token0 = CALL_SQE(statment)->sqe_return_head_(statment);
     if (flag && (token0->type_ == TOKEN_TYPE_ELIF ||
                       token0->type_ == TOKEN_TYPE_ELSE ||
                       token0->type_ == TOKEN_TYPE_DONE))
            return;
     else if (token0->type_ == TOKEN_TYPE_DONE)
             return;
    statment = CALL_SQE(statments_stack)->sqe_pop_head_(statments_stack);
    while (statment->item_size_ > 0) CALL_SQE(statment)->sqe_pop_obj_(statment);
    
    statment =CALL_SQE(statments_stack)->sqe_return_head_(statments_stack);
    _run_code_pop_statments(statements_stack, statment, flag);
}

static void run_code_pop_statments(StackObject *statments_stack, ListObject *statment) {
    _run_code_pop_statments(statments_stack, statment, 1);
}

void run_code_switch_to_next_frame() {
    if (subframes->item_size_  < 5) extend_subframes(subframes);
    
    SuperObject *sub = CALL_SQE(subframes)->sqe_pop_head_(subframes);
    SuperObject *obj = environment;
    environment = sub;
    sub = obj;
    environment->f_back_ = sub;
}

void run_code_switch_to_last_frame() {
    SuperObject *obj = environment->f_back_;
    frame_back_to_pool(environment);
    environment = obj;
}

#define BUFFER 50

void run_code() {
    SuperObject *obj;
    
    IntObject *int_obj;
    FloatObject *float_obj;
    
    StrObject *str_obj;
    StrObject *_ = str_object_create("_");
    
    ListObject *list_obj;
    DictObject *dict_obj;
    
    StackObject *statment;
    
    Token *token0;
    DictObject *locals = environment->locals_;
    char input[BUFFER];
    
    while (statments_stack->item_size_ > 0) {
        /* Get a statment */
        statment =  CALL_SQE(statments_stack)->sqe_pop_head_(statments_stack);
        while (statment->item_size_ > 0) {
            /* Get a token from the tail of this statment */
            token0 = CALL_SQE(statment)->sqe_pop_obj_(statment);
            switch (token0->type_) {
                case TOKEN_TYPE_IDENTIFY:
                    if (!find_object_in_locals_and_push_it_to_runtime_stack(token0, environment)) {
                        printf("At line %d: %s object is not exist in the routine\n", token0->numline_, token0->name_);
                        return;
                    }
                    break;
                case TOKEN_TYPE_LE:
                    RUN_CODE_NUMBER_HELPER2(num_le_);
                    break;
                case TOKEN_TYPE_EQ:
                    RUN_CODE_NUMBER_HELPER2(num_eq_);
                    break;
                case TOKEN_TYPE_GE:
                    RUN_CODE_NUMBER_HELPER2(num_ge_);
                    break;
                case TOKEN_TYPE_LT:
                    RUN_CODE_NUMBER_HELPER2(num_lt_);
                    break;
                case TOKEN_TYPE_GT:
                    RUN_CODE_NUMBER_HELPER2(num_gt_);
                    break;
                case TOKEN_TYPE_NE:
                    RUN_CODE_NUMBER_HELPER2(num_ne_);
                    break;
                case TOKEN_TYPE_ASSIGIN:
                    token0 = CALL_SQE(statment)->sqe_pop_obj_(statment);
                    if (find_object_in_locals_and_push_it_to_runtime_stack(token0, environment))
                        DECREASE_REF_COUNT(CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack));
                    CALL_DIC(locals)->dict_set_key_value_(locals, str_object_create(token0->name_), CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack));
                    break;
                case TOKEN_TYPE_ADD:
                    RUN_CODE_NUMBER_OPERATOR_CONTROLLER(num_add_);
                    break;
                case TOKEN_TYPE_SUB:
                    RUN_CODE_NUMBER_OPERATOR_CONTROLLER(num_sub_);
                    break;
                case TOKEN_TYPE_MULTI:
                    RUN_CODE_NUMBER_OPERATOR_CONTROLLER(num_multi_);
                    break;
                case TOKEN_TYPE_DIV:
                    RUN_CODE_NUMBER_OPERATOR_CONTROLLER(num_divide_);
                    break;
                case TOKEN_TYPE_MOD:
                    RUN_CODE_NUMBER_OPERATOR_CONTROLLER(num_mod_);
                    break;
                case TOKEN_TYPE_INT:
                    CALL_SQE(runtime_stack)->sqe_append_obj_(runtime_stack, int_object_create(atol(token0->name_)));
                    break;
                case TOKEN_TYPE_FLOAT:
                    CALL_SQE(runtime_stack)->sqe_append_obj_(runtime_stack, float_object_create(atof(token0->name_)));
                    break;
                case TOKEN_TYPE_STRING:
                    CALL_SQE(runtime_stack)->sqe_append_obj_(runtime_stack, str_object_create(token0->name_));
                    break;
                case TOKEN_TYPE_ECHO:
                    while (runtime_stack->item_size_ > 0) {
                        obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);
                        api_print_object(obj, 1);
                    }
                    break;
                case TOKEN_TYPE_ECHON:
                    while (runtime_stack->item_size_ > 0) {
                        obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);
                        api_print_object(obj, 0);
                    }
                    break;
                case TOKEN_TYPE_IF:
                    if (if_condition_is_true) {
                        statment =CALL_SQE(statments_stack)->sqe_return_head_(statments_stack);
                        _run_code_pop_statments(statments_stack, statment, 0);
                        break;
                    }
                    run_code_switch_to_next_frame();
                    break;
                
                case TOKEN_TYPE_ELIF:
                    if (if_condition_is_true) {
                        statment =CALL_SQE(statments_stack)->sqe_return_head_(statments_stack);
                        _run_code_pop_statments(statments_stack, statment, 0);
                        break;
                    }
                    run_code_switch_to_next_frame();
                    break;
                
                case TOKEN_TYPE_ELSE:
                    if (if_condition_is_true) {
                        statment =CALL_SQE(statments_stack)->sqe_return_head_(statments_stack);
                        _run_code_pop_statments(statments_stack, statment, 0);
                        break;
                    }
                    run_code_switch_to_next_frame();
                    break;
                case TOKEN_TYPE_DO:
                    if (!TO_INT(CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack))->int_value_) {
                        if_condition_is_true = 0;
                        statment =CALL_SQE(statments_stack)->sqe_return_head_(statments_stack);
                        run_code_pop_statments(statments_stack, statment);
                    } else
                        if_condition_is_true = 1;
                    break;
                case TOKEN_TYPE_DONE:
                    run_code_switch_to_last_frame();
                    break;
                case TOKEN_TYPE_INPUT:
                    scanf("%s", input);
                    run_code_number_helper0(token0, _, str_object_create(input), locals, statment);
                    break;
                case TOKEN_TYPE_TOFLOAT:
                    str_obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);
                    run_code_number_helper0(token0, _, float_object_create(atof(str_obj->str_value_)), locals, statment);
                    break;
                case TOKEN_TYPE_TOINT:
                    str_obj = CALL_SQE(runtime_stack)->sqe_pop_obj_(runtime_stack);
                    run_code_number_helper0(token0, _, int_object_create(atol(str_obj->str_value_)), locals, statment);
                    break;
            }
        }
    }
}

