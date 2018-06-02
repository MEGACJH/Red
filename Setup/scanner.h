
#ifndef scanner_h
#define scanner_h

#include "foundation.h"
#include "os_headers.h"
#include "string.h"

#define KEYWORDS_SIZE 15

#define IS_NEWLINE(ascii) ((ascii) == '\n')

#define ADD_TOKEN(tokens, name,type, line) CALL_SQE(tokens)->sqe_append_obj_(tokens, token_create(name, type, line))
#define ADD_OPERATOR_TOKEN(tokens, name, line) ADD_TOKEN(tokens, name,TOKEN_TYPE_OPERATER, line)
#define ADD_KEYWORD(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_KEYW, line)
#define ADD_IDENT(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_IDENTIFY, line)
#define ADD_INT(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_INT, line)
#define ADD_FLOAT(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_FLOAT, line)
#define ADD_WRONG(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_WRONG, line)
#define ADD_ADD(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_ADD, line)
#define ADD_SUB(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_SUB, line)
#define ADD_MULTI(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_MULTI, line)
#define ADD_DIV(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_DIV, line)
#define ADD_MOD(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_MOD, line)
#define ADD_EQ(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_EQ, line)
#define ADD_GT(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_GT, line)
#define ADD_LT(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_LT, line)
#define ADD_LQ(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_LQ, line)
#define ADD_GQ(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_GQ, line)
#define ADD_NE(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_NE, line)
#define ADD_LE(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_LE, line)
#define ADD_GE(tokens, name, line) ADD_TOKEN(tokens, name, TOKEN_TYPE_GE, line)


#define IS_SEPERATOR_CONDITION(ascii) \
((ascii) == '\t' || (ascii) == ' ' || (ascii) == '\n' || (ascii) == '<' || (ascii) == '>' || (ascii) == '=' || (ascii) == '!' || (ascii) == '+' || (ascii) == '-' || (ascii) == '*' || (ascii) == '/' || (ascii) == '%' || (ascii) == '(' || (ascii) == ')' || (ascii) == '[' || (ascii) == ']' || (ascii) == '"' || (ascii) == ';')

#define IS_ALL_SEPERATOR_CONDITION \
((ascii) == '\t' || (ascii) == ' ' || (ascii) == '\n' || (ascii) == '<' || (ascii) == '>' || (ascii) == '=' || (ascii) == '!' || (ascii) == '+' || (ascii) == '-' || (ascii) == '*' || (ascii) == '/' || (ascii) == '%' || (ascii) == '@'  || (ascii) == '(' || (ascii) == ')' || (ascii) == '[' || (ascii) == ']' || (ascii) == '"' || (ascii) == ';')

#define IS_SEPERATOR(ascii)  IS_SEPERATOR_CONDITION(ascii)
#define IS_ALL_SEPERATOR(ascii) IS_ALL_SEPERATOR_CONDITION(ascii)

extern char *g_keywords[KEYWORDS_SIZE];

extern FrameObject *environment;

void build_environment();

int source_code_scanning(FILE *file);

#endif /* scanner_h */


/*
 #define WHILE_SIZE 5
 #define DONE_SIZE 4
 #define IF_SIZE 2
 #define ELIF_SIZE 4
 #define ENF_OF_STR '\0'
 #define DO_SIZE 2
 
 #define LIST_START '['
 #define LIST_END ']'
 #define DICT_START '{'
 #define DICT_END '}'
 */
