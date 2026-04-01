// SPDX-License-Identifier: LGPL-3.0
#ifndef ROBIF2B_EXAMPLE_TIME_UTIL_H
#define ROBIF2B_EXAMPLE_TIME_UTIL_H

#include <time.h>

#define USEC_IN_MSEC 1000
#define NSEC_IN_USEC 1000
#define USEC_IN_SEC  1000000

inline static long timespec_to_usec(const struct timespec *t) {
    return t->tv_sec * USEC_IN_SEC + t->tv_nsec / NSEC_IN_USEC;
}

#endif
