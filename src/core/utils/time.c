/*
 * time.c
 *
 *  Created on: Jul 31, 2015
 *      Author: root
 */

#include <time.h>

uint64_t jtime_get()
{
    struct timespec ts;
    int rc;
    uint64_t us;

    rc = clock_gettime(CLOCK_REALTIME, &ts);
    if (0 != rc) {
        return 0;
    }

    us = ts.tv_sec * 1000000LL;
    us += ts.tv_nsec / 1000;
    return us;
}
