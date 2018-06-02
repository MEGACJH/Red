
#ifndef foundation_h
#define foundation_h

#define debug   1
#if debug == 1
#define log(fmt, ...) do {printf(fmt, ##__VA_ARGS__); printf("\n");} while (0)
#else
#define log(fmt, ...)
#endif


#include "object.h"
#include "int_object.h"
#include "float_object.h"
#include "str_object.h"
#include "dict_object.h"
#include "list_object.h"
#include "none_object.h"
#include "error_macro.h"
#include "arguments.h"
#include "common_macro.h"
#include "obj_pool_init.h"
#include "buitintype_init.h"
#include "builtin_method.h"
#include "stack_object.h"
#include "frame_object.h"
#include "ast.h"

#endif /* foundation_h */
