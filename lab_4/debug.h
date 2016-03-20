#ifndef _DBG_H
#define _DBG_H

#include <stdio.h>
#include <assert.h>

#ifdef NDEBUG
#define logd(message, ...)
#define assert_debug(predicate, message, ...)
#else
#define logd(message, ...) fprintf(stderr, "DEBUG[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define assert_debug(predicate, message, ...) if (!(predicate)) { logd(message, ##__VA_ARGS__); } assert(predicate)
#endif

#define loge(message, ...) fprintf(stderr, "ERROR[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define logw(message, ...) fprintf(stderr, "WARN[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define logi(message, ...) fprintf(stderr, "INFO[%s:%s:%d]: " message "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)


#endif
