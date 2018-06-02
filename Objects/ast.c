
#include "foundation.h"
#include "os_headers.h"

void token_description(SuperObject *self) {
    Token *token = (Token *) self;
    printf("<%s", token->name_);
    switch (token->type_) {
        case TOKEN_TYPE_INT:
            printf(" int>\n");
            break;
        case TOKEN_TYPE_DICT:
            printf(" dict>\n");
            break;
        case TOKEN_TYPE_KEYW:
            printf(" keyword>\n");
            break;
        case TOKEN_TYPE_LIST:
            printf(" list>\n");
            break;
        case TOKEN_TYPE_FLOAT:
            printf(" float>\n");
            break;
        case TOKEN_TYPE_STRING:
            printf(" string>\n");
            break;
        case TOKEN_TYPE_IDENTIFY:
            printf(" id>\n");
            break;
        case TOKEN_TYPE_OPERATER:
            printf(" operator>\n");
            break;
        case TOKEN_TYPE_ADD:
            printf(" add>\n");
            break;
        case  TOKEN_TYPE_SUB:
            printf(" sub>\n");
            break;
        case TOKEN_TYPE_EQ:
            printf(" eq>\n");
            break;
        case TOKEN_TYPE_GT:
            printf(" gt>\n");
            break;
        case TOKEN_TYPE_LT:
            printf(" lt>\n");
            break;
        case TOKEN_TYPE_LE:
            printf(" le>\n");
            break;
        case TOKEN_TYPE_GE:
            printf(" ge>\n");
            break;
        case TOKEN_TYPE_MULTI:
            printf(" multi>\n");
            break;
        case TOKEN_TYPE_MOD:
            printf(" mod>\n");
            break;
        case TOKEN_TYPE_DIV:
            printf(" div>\n");
            break;
        case TOKEN_TYPE_LEFT_BREKET:
            printf(" leftbracket>\n");
            break;
        case TOKEN_TYPE_LEFT_MID_BREKET:
            printf(" leftmidbracket>\n");
            break;
        case TOKEN_TYPE_RIGHT_BREKET:
            printf(" rightbracket>\n");
            break;
        case TOKEN_TYPE_RIGHT_MID_BREKET:
            printf(" rightmidbracket>\n");
            break;
        case TOKEN_TYPE_EOF:
            printf(" eof>\n");
            break;
        case TOKEN_TYPE_IF:
            printf(" if>\n");
            break;
        case TOKEN_TYPE_ELSE:
            printf(" else>\n");
            break;
        case TOKEN_TYPE_ELIF:
            printf(" elif>\n");
            break;
        case TOKEN_TYPE_DO:
            printf(" do>\n");
            break;
        case TOKEN_TYPE_DONE:
            printf(" done>\n");
            break;
        case TOKEN_TYPE_WHILE:
            printf(" while>\n");
            break;
        case TOKEN_TYPE_ASSIGIN:
            printf(" assign>\n");
            break;
        case TOKEN_TYPE_AT:
            printf(" call>\n");
            break;
        case TOKEN_TYPE_ECHO:
            printf(" echo>\n");
            break;
        case TOKEN_TYPE_ECHON:
            printf(" echon>\n");
            break;
        case TOKEN_TYPE_COMMENTS:
            printf(" comments>\n");
            break;
        default:
            printf(" wrong>\n");
            break;
    }
}

void token_dealloc(Token *self) {
    free(self);
}

void token_free(Token *self) {
    DECREASE_REF_COUNT(self);
    if (!self->ref_count_)
        token_dealloc(self);
}

TypeObject g_token_type_obj = {
    SUPER_OBJECT_HEAD_INIT(&g_root_type_object),
    "Token",
    sizeof(TypeObject),
    "None :)",
    .dealloc_ = token_dealloc,
    .free_ = token_free,
    .description_ = token_description,
};

Token * token_create(char *name, TokenType type, int lineno) {
    Token *token = (Token *) malloc(sizeof(Token));
    token->name_ = name;
    token->obj_type_info_ = &g_token_type_obj;
    token->type_ = type;
    token->numline_ = lineno;
    INCREASE_REF_COUNT(token);
    return token;
}

