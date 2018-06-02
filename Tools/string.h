
#ifndef string_h
#define string_h

#include <stdio.h>
#include <string.h>

char string_char_at(char *src, int index);

/**
    Return a new string which is combined
    with the src and the arg
    for example:
         char *src = "src";
         char *arg = "arg";
         after the function, the return value "is srcarg"
 
    Attention:
         the new string is created in the heap
 */
char *string_append(char *src, char *arg);

/**
    Similar to the function named string_append, return a new string which combined with the arg and the string
    for example:
         char *src = "src";
         char *arg = "arg";
         after the function, the return value is "argarc"
    Attention:
    the new string is created in the heap
 */
char *string_push_front(char *src, char *arg);

int string_length(char *src);

char *string_append_char(char *src, char ch);

char *string_push_front_char(char *src, char ch);

#endif /* string_h */
