#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DEBUG_LEVEL_NORMAL = 0, /* 非错误：不会打印日志 */
    DEBUG_LEVEL_PRINT,      /* 普通，作为流程日志 */
    DEBUG_LEVEL_WARINIG,    /* 告警：作为告警日志 */
    DEBUG_LEVEL_ERR,        /* 错误：会显著打印日志*/
    DEBUG_LEVEL_END
} DEBUG_LEVEL_E;

void DebugShow(DEBUG_LEVEL_E level, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif