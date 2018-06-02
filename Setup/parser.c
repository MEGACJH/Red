

#include "parser.h"
#include "foundation.h"
#include "os_headers.h"
#include "scanner.h"


void tokens_parsing() {
    ListObject *tokens = environment->token_list_;
    DictObject *locals = environment->locals_;
    Token *token;
    FloatObject *const_float = NULL;
    /*  Store a statment token  */
    ListObject *tokens_in_a_statements = list_object_create(3);
    while (tokens->item_size_ > 0) {
        token = CALL_SQE(tokens)->sqe_pop_head_(tokens);
        switch (token->type_) {
            case TOKEN_TYPE_IDENTIFY:
            case TOKEN_TYPE_LE:
            case TOKEN_TYPE_EQ:
            case TOKEN_TYPE_GE:
            case TOKEN_TYPE_LT:
            case TOKEN_TYPE_GT:
            case TOKEN_TYPE_ASSIGIN:
            case TOKEN_TYPE_ADD:
            case TOKEN_TYPE_SUB:
            case TOKEN_TYPE_MULTI:
            case TOKEN_TYPE_DIV:
            case TOKEN_TYPE_NE:
            case TOKEN_TYPE_MOD:
            case TOKEN_TYPE_INT:
            case TOKEN_TYPE_FLOAT:
            case TOKEN_TYPE_STRING:
            case TOKEN_TYPE_ECHO:
            case TOKEN_TYPE_ECHON:
            case TOKEN_TYPE_INPUT:
            case TOKEN_TYPE_TOFLOAT:
            case TOKEN_TYPE_TOINT:
            case TOKEN_TYPE_IF:
            case TOKEN_TYPE_DO:
            case TOKEN_TYPE_DONE:
            case TOKEN_TYPE_ELSE:
            case TOKEN_TYPE_ELIF:
                CALL_SQE(tokens_in_a_statements)->sqe_append_obj_(tokens_in_a_statements, token);
                break;
            case TOKEN_TYPE_EOF:
                CALL_SQE(statments_stack)->sqe_append_obj_(statments_stack, tokens_in_a_statements);
                tokens_in_a_statements = list_object_create(3);
                break;
        }
    }
}
