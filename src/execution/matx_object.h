//
// Created by userdepinfo on 13/11/26.
//

#ifndef MATX_OBJECT_H
#define MATX_OBJECT_H

#include <stddef.h>
#include "spds/hash_map.h"

typedef struct MatematixObject {
    size_t type;
    size_t ref_count;

    union {
        int int_value;
        double double_value;
        void* attributes;
    };
} MatxObject;

typedef struct BinaryMatxObjectTypeCombinaison {
    size_t type1;
    size_t type2;
} MatxObjTypeCombinaison;

typedef MatxObject* (*BinMatxObjOperation)(MatxObject*, MatxObject*);

HASH_MAP(MatxObjTypeCombinaison, BinMatxObjOperation, BinMatxObjOperationHashMap, MatxObjectBinOperationEntry,
         BinMatxObjOperationHashMap)

long MatxObjTypeCombinaison_hash(const MatxObjTypeCombinaison c);
bool MatxObjTypeCombinaison_equals(const MatxObjTypeCombinaison c1, const MatxObjTypeCombinaison c2);
MatxObject* MatxObject_new();

#endif //MATX_OBJECT_H
