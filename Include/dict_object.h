

#ifndef dict_object_h
#define dict_object_h

#include "foundation.h"
#include "os_headers.h"
#include "list_object.h"
    /*                   This area is a ListObject           */
    // =======================================              =======================================
    // ||                  ||                  ||                   ||             ||                 ||                    ||                   ||
    // ||                  ||                  ||                   ||             ||                 ||                    ||                   ||
    //  ======================================               =======================================                    
    // =======================================              =======================================
    // ||                  ||                  ||                   ||             ||                 ||                    ||                   ||
    // ||                  ||                  ||                   ||             ||                 ||                    ||                   ||
    //  ======================================               =======================================
    //                                 ||
    //                                 ||
    //                                  .
    //                                  .
    //                                  .
/*
 This is a fake Object :)
 GC will not care about it
 */
typedef struct _entry {
    SUPER_OBJECT_HEAD;
    SuperObject *key_;
    SuperObject *value_;
} Entry;

/* Above is an example of the policy of the DictObject to store `key&value`s */
typedef struct _dict_object {
    SUPER_OBJECT_HEAD;
    ListObject *entries; /* It's a ListObject */
    int entry_size_; /* Contains the entry size the elements in lists */
    ListObject *not_empty_indexes_; /* Aim to make print dict object faster */
} DictObject;

extern TypeObject g_dict_type_object;

REDAPI_FUNC(SuperObject *) dict_object_create();

#endif /* dict_object_h */
