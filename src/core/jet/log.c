/*
 * log.c
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */

#include "log.h"
#include <stdio.h>

int
jlog_printf(jlog_category_t *cg, jlog_level_t level,
            const char *file, int line,
            const char *fmt, ...)
{
    printf(fmt);
    return 0;
}
