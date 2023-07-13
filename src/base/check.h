/**
 * @file check.h
 * @author zhangmeng (847305142@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CHECK_H
#define CHECK_H

#include <stdio.h>

#include "debug.h"
#include "errcode.h"

/* 带默认返回值的检查宏 */
#define CHECK_NULL_AUTORETURN(p)                                               \
    do {                                                                       \
        if (p == NULL) {                                                       \
            DebugShow(DEBUG_LEVEL_ERR, "[%s] [%s] is null", __FUNCTION__, #p); \
            return ERR_POINTER_NULL;                                           \
        }                                                                      \
    } while (0)

#define CHECK_RET_AUTORETURN(ret)                                        \
    do {                                                                 \
        if (ret != SUCCESS) {                                            \
            return ret;                                                  \
        }                                                                \
    } while (0)

typedef enum {
    CHECK_CONDITION_EQ = 0, /* == */
    CHECK_CONDITION_NE, /* != */
    CHECK_CONDITION_LE, /* < */
    CHECK_CONDITION_LT, /* <= */
    CHECK_CONDITION_GE, /* > */
    CHECK_CONDITION_GT, /* >= */
    CHECK_CONDITION_END
} CHECK_CONDITION_E;

inline uint32 static check_condition(uint32 a, CHECK_CONDITION_E type, uint32 b)
{
    switch(type) {
        case CHECK_CONDITION_EQ: return a == b;
        case CHECK_CONDITION_NE: return a != b;
        case CHECK_CONDITION_LE: return a < b;
        case CHECK_CONDITION_LT: return a <= b;
        case CHECK_CONDITION_GE: return a > b;
        case CHECK_CONDITION_GT: return a >= b;
    }
    return 0;
}

#define CHECK_CONDITION_AUTORETURN(a, type, b)                                 \
    do {                                                                       \
        if (check_condition(a, type, b) == 0) {                                \
            const char *str[] = {"==", "!=", "<", "<=", ">", ">="};            \
            DebugShow(DEBUG_LEVEL_ERR, "%s[%u] %s %s[%u] not match", #a, a, str[type], #b, b); \
            return ERR_CONDITION;                                           \
        }                                                                      \
    } while (0)


/* 带默认返回值的检查宏 */
#define CHECK_NULL_PARAMRETURN(p, errcode)                               \
    do {                                                                 \
        if (p == NULL) {                                                 \
            DebugShow(DEBUG_LEVEL_ERR, "[%s] [%s] is null, errcode[%u]", \
                      __FUNCTION__, #p, (uint32)errcode);                \
            return errcode;                                              \
        }                                                                \
    } while (0)


/* 不带返回值的宏检查 */
#define CHECK_NULL_VOID(p)                                                     \
    do {                                                                       \
        if (p == NULL) {                                                       \
            DebugShow(DEBUG_LEVEL_ERR, "[%s] [%s] is null", __FUNCTION__, #p); \
            return;                                                            \
        }                                                                      \
    } while (0)

#endif