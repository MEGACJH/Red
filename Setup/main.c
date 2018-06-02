#include "scanner.h"
#include "str_object.h"
#include "parser.h"
#include <string.h>
#include "RedEngine.h"

void RED_START_UP() {
    small_int_pool_initialize();
    init_inherit_func();
    build_environment();
    init_subframes();
    init_runtime_stack();
    init_statments_stack();
}

void RED_LANG_FLY(FILE *fp) {
    if (source_code_scanning(fp)) {
        tokens_parsing();
        run_code();
    }
}

int main(int argc, const char * argv[]) {
    RED_START_UP();
    FILE *fp;
    int i = 1;
    if (argc == 1) {
        printf("Sir, at least one argument\n");
        return 1;
    }
    
    for (; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (!fp) {
            return 1;
        }
        RED_LANG_FLY(fp);
    }
    return 0;
}

