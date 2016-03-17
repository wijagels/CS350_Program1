#ifndef _DBG_H
#define _DBG_H

#include <stdio.h>

#ifdef NDEBUG
#define logd(message, ...)
#else
#define logd(message, ...) fprintf(stderr, "DEBUG[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#endif

#define loge(message, ...) fprintf(stderr, "ERROR[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define logi(message, ...) fprintf(stderr, "INFO[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#endif
