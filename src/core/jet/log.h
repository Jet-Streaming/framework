/*
 * log.h
 *
 *  Created on: Jun 30, 2015
 *      Author: root
 */

#ifndef SRC_CORE_JET_LOG_H_
#define SRC_CORE_JET_LOG_H_

#include <jet/jet.h>
#include <jet/list.h>

//#include <stdarg.h>


struct jlog_category_s {
    int indent;
    int enable : 1;
    jlist_head_t handlers;
};

enum jlog_level_e {
    JLOG_FATAL,
    JLOG_ERROR,
    JLOG_WARNING,
    JLOG_INFO,
    JLOG_DEBUG,
    JLOG_VERBOSE,
};


int
jlog_printf(jlog_category_t *cg, jlog_level_t level,
            const char *file, int line,
            const char *fmt, ...);

#define jlog_error(cg, fmt, ...) \
    jlog_printf(cg, JLOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define jlog_info(cg, fmt, ...) \
    jlog_printf(cg, JLOG_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#endif /* SRC_CORE_JET_LOG_H_ */
