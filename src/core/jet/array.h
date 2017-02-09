/*
 * array.h
 *
 *  Created on: Sep 18, 2015
 *      Author: root
 */

#ifndef SRC_CORE_UTILS_ARRAY_H_
#define SRC_CORE_UTILS_ARRAY_H_

//#include <jet/jet.h>
#include <stdint.h>
#include <stdlib.h>
#include <uv.h>

typedef struct {
    void        *elts;
    size_t       size;
    uint32_t   nelts;
    uint32_t   nalloc;
//    uint32_t   unordered : 1;
} jarray_t;

int
jarray_init(jarray_t *array, uint32_t n, size_t size);
void *
jarray_push(jarray_t *a);
int
jarray_del(jarray_t *a, uint32_t n);


#endif /* SRC_CORE_UTILS_ARRAY_H_ */
