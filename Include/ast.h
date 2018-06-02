
#ifndef ast_h
#define ast_h

#include "foundation.h"
#include "os_headers.h"

extern TypeObject g_token_type_obj;
extern TypeObject g_astnode_obj;

typedef enum _token_type{
    TOKEN_TYPE_WRONG = 0,
    TOKEN_TYPE_IDENTIFY = 1,
    TOKEN_TYPE_STRING = 2,
    TOKEN_TYPE_INT = 3,
    TOKEN_TYPE_FLOAT = 4,
    TOKEN_TYPE_KEYW = 5,
    TOKEN_TYPE_DICT = 6,
    TOKEN_TYPE_LIST = 7,
    TOKEN_TYPE_OPERATER = 8,
    TOKEN_TYPE_ADD = 9,
    TOKEN_TYPE_SUB = 10,
    TOKEN_TYPE_MULTI = 11,
    TOKEN_TYPE_DIV = 12,
    TOKEN_TYPE_MOD = 13,
    TOKEN_TYPE_EQ = 14,
    TOKEN_TYPE_GT = 15,
    TOKEN_TYPE_LT = 16,
    TOKEN_TYPE_LQ = 17,
    TOKEN_TYPE_GQ = 18,
    TOKEN_TYPE_NE = 19,
    TOKEN_TYPE_LE = 20,
    TOKEN_TYPE_GE = 21,
    TOKEN_TYPE_LEFT_BREKET = 22,
    TOKEN_TYPE_RIGHT_BREKET = 23,
    TOKEN_TYPE_LEFT_MID_BREKET = 24,
    TOKEN_TYPE_RIGHT_MID_BREKET = 25,
    TOKEN_TYPE_EOF = 26,
    TOKEN_TYPE_IF = 27,
    TOKEN_TYPE_ELIF = 28,
    TOKEN_TYPE_ELSE = 29,
    TOKEN_TYPE_WHILE = 30,
    TOKEN_TYPE_DO = 31,
    TOKEN_TYPE_DONE = 32,
    TOKEN_TYPE_ASSIGIN = 33,
    TOKEN_TYPE_AT = 34, /* The symbol to call a method */
    TOKEN_TYPE_ECHO = 35,
    TOKEN_TYPE_ECHON = 36,
    TOKEN_TYPE_COMMENTS = 37,
    TOKEN_TYPE_START = 38,
    TOKNE_TYPE_END = 39,
    TOKEN_TYPE_LOCAL = 40,
    TOKEN_TYPE_INPUT = 41,
    TOKEN_TYPE_TOINT = 42,
    TOKEN_TYPE_TOFLOAT = 43,
} TokenType;

typedef struct _token {
    SUPER_OBJECT_HEAD;
    char *name_;
    TokenType type_;
    int numline_;
} Token;

REDAPI_FUNC(Token *) token_create(char *name, TokenType type, int numline);

#endif /* ast_h */

