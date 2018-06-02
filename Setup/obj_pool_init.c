
#include "foundation.h"
#include "obj_pool_init.h"

/* Small int pool */
REDFORBIDEN_FUNC(void) small_int_pool_initialize() {
    g_small_int_obj_pool = (IntObject **) malloc(sizeof(IntObject *)
                                                 * (SMALL_INT_POOL_RANGE_TO - SMALL_INT_POOL_RANGE_FROM
                                                 + 1));
    
    for (int i = SMALL_INT_POOL_RANGE_FROM, j = 0; i < SMALL_INT_POOL_RANGE_TO; i++, j++)
        g_small_int_obj_pool[j] = int_object_new(i);
    
}
