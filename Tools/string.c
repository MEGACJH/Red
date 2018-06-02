

#include "string.h"
#include <stdlib.h>

int string_length(char *src) {
    int length = 0;
    while (*src != '\0') {
        length++;
        src++;
    }
    return length;
}

char string_char_at(char *src, int index) {
    if (index < 0 || index > string_length(src) - 1) {
        return -1;
    }
    return src[index];
}

char *string_append(char *src, char *arg) {
    char *new_string = (char *) malloc(sizeof(char) * (string_length(src) + string_length(arg) + 1));
    // Record the index of the array of the new_string
    int tmp_index = -1;
    int i = 0;
    for (i = 0; i < string_length(src); i++) {
        new_string[i] = src[i];
    }
    tmp_index = i;
    for (int j = 0; j < string_length(arg); j++) {
        new_string[tmp_index++] = arg[j];
    }
    new_string[tmp_index] = '\0';
    free(src);
    return new_string;
}


char *string_push_front(char *src, char *arg) {
    return string_append(arg, src);
}

char *string_append_char(char *src, char ch) {
    char *new_string = (char *) malloc(sizeof(char) * (string_length(src) + 2));
    int tmp_index = -1;
    int i = 0;
    for (i = 0; i < string_length(src); i++) {
        new_string[i] = src[i];
    }
    tmp_index = i;
    new_string[tmp_index++] = ch;
    new_string[tmp_index] = '\0';
    return new_string;
}

char *string_push_front_char(char *src, char ch) {
    char *new_string = (char *) malloc(sizeof(char) * (string_length(src) + 2));
    int i = 0;
    int tmp_index = -1;
    for (i = 1; i <= string_length(src); i++) {
        new_string[i] = src[i - 1];
    }
    tmp_index = i;
    new_string[tmp_index] = '\0';
    new_string[0] = ch;
    return new_string;
}
