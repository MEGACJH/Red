
#include "scanner.h"
#include "ast.h"

char *g_keywords[KEYWORDS_SIZE] = {
    "if",
    "elif",
    "while",
    "else",
    "done",
    "@",
    "do",
    "echo",
    "echon",
    "start",
    "end",
    "local",
    "input",
    "toint",
    "tofloat",
};

FrameObject *environment;

void build_environment() {
    environment = frame_new();
    environment->f_back_ = 0;
    environment->locals_ = TO_DICT(dict_object_create());
    environment->token_list_ = TO_LIST(list_object_create(10));
}

static void source_code_scanning_kw (Token *tokens, FILE *file, char head, int *numline) {
    char ascii = 0, *identifier = "";
    identifier = string_append_char(identifier, head);
    ascii = fgetc(file);
    while (!IS_SEPERATOR(ascii)) {
        identifier = string_append_char(identifier, ascii);
        ascii = fgetc(file);
    }
    ADD_LINE(ascii, *numline);
    for (int i = 0; i < KEYWORDS_SIZE; i++)
        if (!strcmp(identifier, g_keywords[i])) {
            switch (i) {
                case 0:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_IF, *numline);
                    break;
                case 1:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_ELIF, *numline);
                    break;
                case 2:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_WHILE, *numline);
                    break;
                case 3:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_ELSE, *numline);
                    break;
                case 4:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_DONE, *numline);
                    break;
                case 5:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_AT, *numline);
                    break;
                case 6:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_DO, *numline);
                    break;
                case 7:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_ECHO, *numline);
                    break;
                case 8:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_ECHON, *numline);
                    break;
                case 9:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_START, *numline);
                    break;
                case 10:
                    ADD_TOKEN(tokens, identifier, TOKNE_TYPE_END, *numline);
                    break;
                case 11:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_LOCAL, *numline);
                    break;
                case 12:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_INPUT, *numline);
                    break;
                case 13:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_TOINT, *numline);
                    break;
                case 14:
                    ADD_TOKEN(tokens, identifier, TOKEN_TYPE_TOFLOAT, *numline);
                    break;
            }
            return;
        }
    ADD_IDENT(tokens, identifier, *numline);
}

static int source_code_scanning_check_valid(ListObject *tokens, int *numline) {
    Token *tmp = CALL_SQE(tokens)->sqe_top_obj_(tokens);
    if (tmp->name_ == "=" || tmp->name_ == "!" || tmp->name_ == "=" || tmp->name_ == "<" || tmp->name_ == ">" || tmp->name_ == "==") {
        ADD_WRONG(tokens, "=", *numline);
        printf("Invalid operator: at line %d\n", *numline);
        return 1;
    }
    return 0;
}

static int source_code_scanning_check_valid2(ListObject *tokens, int *numline) {
    Token *tmp = CALL_SQE(tokens)->sqe_top_obj_(tokens);
    if (tmp->name_ == "=" || tmp->name_ == "!" || tmp->name_ == "=" || tmp->name_ == "<" || tmp->name_ == ">" || tmp->name_ == "==" || tmp->name_ == "+" || tmp->name_ == "-" || tmp->name_ == "*" || tmp->name_ == "/" || tmp->name_ == "%") {
        ADD_WRONG(tokens, "=", *numline);
        printf("Invalid operator: at line %d\n", *numline);
        return 1;
    }
    return 0;
}

#define OPERATOR_CHECK(tokens, numline)                                         \
if (source_code_scanning_check_valid(tokens, (&numline))) {                 \
    is_all_right = 0;                                                                                       \
    break;                                                                                                      \
}



#define OPERATOR_CHECK_STRONG(tokens, numline) \
if (source_code_scanning_check_valid2(tokens, (&numline))) {\
is_all_right = 0;\
break;\
}


#define macro_func_add_operator(id, type)  \
OPERATOR_CHECK_STRONG(tokens, numline) \
ascii = fgetc(file);                                         \
if (IS_SEPERATOR(ascii)) {                              \
    ADD_TOKEN(tokens, id,type, numline);      \
} else {                                                         \
    ADD_WRONG(tokens, id, numline);             \
    printf("Wrong at line %d\n", numline);          \
    is_all_right = 0;                                           \
}                                                                    \
break;

int source_code_scanning(FILE *file) {
    char ascii = 0;
    char *identifier = "";
    int flag = 0, dot_count = 0, numline = 1, is_all_right = 1;
    Token *tmp = NULL;
    ListObject *tokens = environment->token_list_;
    while ((ascii = fgetc(file)) != EOF) {
        switch (ascii) {
            case '=':
                OPERATOR_CHECK(tokens, numline);
                ascii = fgetc(file);
                if (ascii == '=') { /* is `==` */
                    ADD_EQ(tokens, "==", numline);
                } else { /*  is `=`  */
                    ADD_LINE(ascii, numline);
                    ADD_TOKEN(tokens, "=", TOKEN_TYPE_ASSIGIN, numline);
                }
                break;
            case '<':
                OPERATOR_CHECK(tokens, numline);
                ascii = fgetc(file);
                if (ascii == '<') {
                    break;
                }
                if (ascii == '=') { /*  is `<=`  */
                    ADD_LE(tokens, "<=", numline);
                } else { /*  is `<`  */
                    ADD_LINE(ascii, numline);
                    ADD_LT(tokens, "<", numline);
                }
                break;
            case '>':
                OPERATOR_CHECK(tokens, numline);
                ascii = fgetc(file);
                if (ascii == '>') {
                    break;
                }
                if (ascii == '=') { /*  is `>=`  */
                    ADD_GE(tokens, ">=", numline);
                } else { /*  is `>`  */
                    ADD_LINE(ascii, numline);
                    ADD_GT(tokens, ">", numline);
                }
                break;
            case '!':
                OPERATOR_CHECK(tokens, numline);
                ascii = fgetc(file);
                if (ascii == '=') { /*  is `!=`  */
                    ADD_NE(tokens, "!=", numline);
                } else { /*  is `<`  */
                    ADD_LINE(ascii, numline);
                    printf(" \'=\' must follows \'!\' at line: %d\n", numline);
                    is_all_right = 0;
                }
                break;
            case 'i':
                source_code_scanning_kw(tokens, file, 'i', &numline);
                break;
            case 'e':
                source_code_scanning_kw(tokens, file, 'e', &numline);
                break;
            case 'w':
                source_code_scanning_kw(tokens, file, 'w', &numline);
                break;
            case 'd':
                source_code_scanning_kw(tokens, file, 'd', &numline);
                break;
            case '@':
                source_code_scanning_kw(tokens, file, '@', &numline);
                break;
            case 'p':
                source_code_scanning_kw(tokens, file, 'p', &numline);
                break;
            case 't':
                source_code_scanning_kw(tokens, file, 't', &numline);
                break;
            case '+':
                ADD_LINE(ascii, numline);
                OPERATOR_CHECK_STRONG(tokens, numline);
                macro_func_add_operator("+", TOKEN_TYPE_ADD);
            case '-':
                ascii = fgetc(file);
                ADD_LINE(ascii, numline);
                while (ascii >= '0' && ascii <= '9') {
                    identifier = string_append_char(identifier, ascii);
                    ascii = fgetc(file);
                    flag = 1;
                }
                if (flag == 1) {
                    identifier = string_push_front_char(identifier, '-');
                    if (ascii == '.') {
                        flag = 2;
                        dot_count++;
                        identifier = string_append_char(identifier, '.');
                        ascii == fgetc(file);
                        
                        while (ascii >= '0' && ascii <= '9') {
                            identifier = string_append_char(identifier, ascii);
                            ascii = fgetc(file);
                        }
                        
                        if (ascii == '.') {
                            printf("Wrong at line %d\n", numline);
                            return 0;
                        }
                        
                    }
                }
                
                if (flag == 1) {
                    ADD_INT(tokens, identifier, numline);
                    identifier = "";
                    break;
                } else if (flag == 2) {
                    ADD_FLOAT(tokens, identifier, numline);
                    identifier = "";
                    break;
                }
                ADD_LINE(ascii, numline);
                OPERATOR_CHECK_STRONG(tokens, numline);
                macro_func_add_operator("-", TOKEN_TYPE_SUB);
            case '*':
                ADD_LINE(ascii, numline);
                OPERATOR_CHECK_STRONG(tokens, numline);
                macro_func_add_operator("*", TOKEN_TYPE_MULTI);
            case '/':
                ADD_LINE(ascii, numline);
                OPERATOR_CHECK_STRONG(tokens, numline);
                macro_func_add_operator("/", TOKEN_TYPE_DIV);
            case '%':
                ADD_LINE(ascii, numline);
                OPERATOR_CHECK_STRONG(tokens, numline);
                macro_func_add_operator("%", TOKEN_TYPE_MOD);
            case '\n':
                ADD_LINE(ascii, numline);
                CALL_SQE(tokens)->sqe_append_obj_(tokens, token_create("\n", TOKEN_TYPE_EOF, numline));
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
                if (ascii == '0')
                    flag = 2;
                
                ADD_LINE(ascii, numline);
                identifier = string_append_char(identifier, ascii);
                ascii = fgetc(file);
                while ((!IS_SEPERATOR(ascii)) && ascii != EOF) {
                    if (ascii >= 65 && ascii <= 122)
                        /* Wrong grammar: number with letters */
                        flag = 2;
                    else if (ascii == '.' && !dot_count) {
                        flag = 1;
                        dot_count++;
                    } else if (ascii == '.' && dot_count == 1)
                        /* Wrong grammar: two dots in number  */
                        flag = 2;
                    
                    identifier = string_append_char(identifier, ascii);
                    ascii = fgetc(file);
                }
                if (flag == 1)
                    ADD_FLOAT(tokens, identifier, numline);
                else if (!flag)
                    ADD_INT(tokens, identifier, numline);
                else if (flag == 2) {
                    ADD_WRONG(tokens, identifier, numline);
                    printf("Bad grammar at line: %d\n", numline);
                    is_all_right = 0;
                }
                identifier = "";
                flag = 0, dot_count = 0;
                break;
            case '"':
                while ((ascii = fgetc(file)) != '"') identifier = string_append_char(identifier, ascii);
                
                CALL_SQE(tokens)->sqe_append_obj_(tokens, token_create(identifier, TOKEN_TYPE_STRING, numline));
                identifier = "";
                break;
            case '#':
                while ((ascii = fgetc(file)) != '\n');
                ADD_LINE(ascii, numline);
                break;
            case ',':
                ADD_LINE(ascii, numline);
                break;
            default:
                /* Other identifiers  */
                ADD_LINE(ascii, &numline);
                if (IS_SEPERATOR(ascii))
                    break;
                identifier = string_append_char(identifier, ascii);
                ascii = fgetc(file);
                while ((!IS_SEPERATOR(ascii)) && ascii != EOF) {
                    identifier = string_append_char(identifier, ascii);
                    ascii = fgetc(file);
                }
                ADD_IDENT(tokens, identifier, numline);
                identifier = "";
                break;
        }
    }
    return is_all_right;
}



/*
 case '(':
 ADD_LINE(ascii, numline);
 ADD_TOKEN(tokens, "(", TOKEN_TYPE_LEFT_BREKET, numline);
 break;
 case ')':
 ADD_LINE(ascii, numline);
 ADD_TOKEN(tokens, ")", TOKEN_TYPE_RIGHT_BREKET, numline);
 break;
 case '[':
 ascii = fgetc(file);
 ADD_LINE(ascii, numline);
 while (ascii != ']' && ascii != '\n') {
 identifier = string_append_char(identifier, ascii);
 ascii = fgetc(file);
 }
 CALL_SQE(tokens)->sqe_append_obj_(tokens, token_create(identifier, TOKEN_TYPE_LIST, numline));
 identifier = "";
 break;
 */

/*
 #define OPERATOR_CHECK2(tokens, numline) \
 if (source_code_scanning_check_valid(tokens, (numline))){                   \
    is_all_right = 0;                                                                                   \
    break;                                                                                                  \
 }
 
 #define OPERATOR_CHECK_STRONG2(tokens, numline) \
 if (source_code_scanning_check_valid2(tokens, (numline))){\
    is_all_right = 0;\
    break;\
 }

 */

