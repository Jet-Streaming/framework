/*
 * array.c
 *
 *  Created on: Sep 18, 2015
 *      Author: root
 */

#include "array.h"

int
jarray_init(jarray_t *array, uint32_t n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */

    array->nelts = 0;
    array->size = size;
    array->nalloc = n;

    if (n) {
        array->elts = malloc (n * size);
        if (!array->elts) {
            //return J_NOMEM;
            return -1;
        }
    } else {
        array->elts = 0;
    }

    return 0;
}

void *
jarray_push(jarray_t *a)
{
    void        *elt, *_new;
    size_t       size;

    if (a->nelts == a->nalloc) {

        /* the array is full */

        size = a->size * a->nalloc;

        /* allocate a new array */

        _new = realloc (a->elts, 2 * size);
        if (!_new) {
            return 0;
        }

        if (_new != a->elts) {
            //memcpy(_new, a->elts, size);
            a->elts = _new;
        }
        a->nalloc *= 2;
    }

    elt = (uint8_t *) a->elts + a->size * a->nelts;
    a->nelts++;

    return elt;
}

int
jarray_del(jarray_t *a, uint32_t n)
{
    if (n > a->nelts) {
        return -1;
    }

//    if (a->unordered) {
//        memcpy((uint8_t *)a->elts + (n) * a->size,
//               (uint8_t *)a->elts + (a->nelts - 1) * a->size,
//               a->size);
//    } else {
        memmove((uint8_t *)a->elts + (n - 1) * a->size, (uint8_t *)a->elts + n * a->size,
                (a->nelts - n) * a->size);
//    }
    a->nelts -= 1;
    return 0;
}

