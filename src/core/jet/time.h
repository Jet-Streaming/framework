/*
 * time.h
 *
 *  Created on: Jul 31, 2015
 *      Author: root
 */

#ifndef SRC_CORE_UTILS_TIME_H_
#define SRC_CORE_UTILS_TIME_H_

#include "types.h"


static jtime_t JTIME_INFINITE = (jtime_t)-1;

jtime_t jtime_get();
jtime_t jtime_get_cache();
jtime_t jtime_update_cache();


#endif /* SRC_CORE_UTILS_TIME_H_ */
