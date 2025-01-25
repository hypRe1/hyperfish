#pragma once
#include <sys/time.h>

// https://stackoverflow.com/a/44896326
long long timeInMilliseconds() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}
