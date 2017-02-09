/*
 * time.c
 *
 *  Created on: Jul 31, 2015
 *      Author: root
 */

#include <time.h>
#include <assert.h>
#include "types.h"
#include "time.h"

static jtime_t cached_time;

jtime_t jtime_get()
{
#ifdef _WIN32
    assert(0);
    return 0;
#else
    struct timespec ts;
    int rc;
    jtime_t us;

    rc = clock_gettime(CLOCK_REALTIME, &ts);
    if (0 != rc) {
        return 0;
    }

    us = ts.tv_sec * 1000000LL;
    us += ts.tv_nsec / 1000;
    return us;
#endif
}

jtime_t jtime_get_cache()
{
    return cached_time;
}

jtime_t jtime_update_cache()
{
    jtime_t old;

    old = cached_time;
    cached_time = jtime_get();
    return cached_time - old;
}
