//
// Created by userdepinfo on 13/11/26.
//

#include "matx_object.h"

#define INTEGER 0
#define DOUBLE 1

MatxObject* MatxObject_new() {
    MatxObject* res = calloc(1, sizeof(MatxObject));
    return res;
}

long MatxObjTypeCombinaison_hash(const MatxObjTypeCombinaison c) {
    long hash = c.type1;
    hash = hash + c.type2 * 31;
    hash= hash + c.operation*31;
    return hash;
}

bool MatxObjTypeCombinaison_equals(const MatxObjTypeCombinaison c1, const MatxObjTypeCombinaison c2) {
    return c1.type1 == c2.type1 && c1.type2 == c2.type2 && c1.operation==c2.operation;
}
