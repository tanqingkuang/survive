/**
 * @file errcode.h
 * @author zhangmeng (847305142@qq.com)
 * @brief 用于承载错误码，利于后续定位
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef ERRCODE_H
#define ERRCODE_H

#include "type.h"

#define SUCCESS 0
#define ERR_POINTER_NULL 1 /* 空指针 */
#define ERR_MALLOC 2       /* 内存申请失败 */
#define ERR_FILE_OPEN 2    /* 内存申请失败 */
#define ERR_NOT_FOUND 3    /* 未找到 */

#endif