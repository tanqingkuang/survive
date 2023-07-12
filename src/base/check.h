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

#if 0
/* 带默认返回值的检查宏 */
#define CHECK_NULL_AUTORETURN(p)                                 \
    do {                                                         \
        if (p == NULL) {                                         \
            DEBUG_SHOW("[%s] [%s] is null\n", __FUNCTION__, #p); \
            return ERR_POINTER_NULL;                             \
        }                                                        \
    } while (0)

/* 带默认返回值的检查宏 */
#define CHECK_NULL_PARAMRETURN(p, errcode)                                   \
    do {                                                                     \
        if (p == NULL) {                                                     \
            DEBUG_SHOW("[%s] [%s] is null, errcode[%u]\n", __FUNCTION__, #p, \
                       (uint32)errcode);                                     \
            return errcode;                                                  \
        }                                                                    \
    } while (0)

/* 不带返回值的宏检查 */
#define CHECK_NULL_VOID(p)                                       \
    do {                                                         \
        if (p == NULL) {                                         \
            DEBUG_SHOW("[%s] [%s] is null\n", __FUNCTION__, #p); \
            return;                                              \
        }                                                        \
    } while (0)
#endif

#endif