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

#define SUCCESS ((uint32)0)
#define ERR_POINTER_NULL ((uint32)1) /* 空指针 */
#define ERR_MALLOC ((uint32)2)       /* 内存申请失败 */
#define ERR_FILE_OPEN ((uint32)3)    /* 文件打开失败 */
#define ERR_NOT_FOUND ((uint32)4)    /* 未找到 */
#define ERR_CONDITION ((uint32)5)    /* 条件不对 */

#endif