/**
 * @file debug.c
 * @author zhangmeng (847305142@qq.com)
 * @brief 用于实现日志打印功能
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "debug.h"

#include "stdarg.h"
#include "string.h"

void DebugShowPRINT(const char *str)
{
    printf(str);
}

void DebugShow(DEBUG_LEVEL_E level, const char *format, ...)
{
    if (level == DEBUG_LEVEL_NORMAL)
        return;

    char out[1024] = {0};
    char str[128] = {0};

    va_list arg;
    va_start(arg, format);

    /* 控制颜色 */
    if (level == DEBUG_LEVEL_ERR)
    {
        (void)vsprintf(str, "\033[1;31m", 0);
        strcat(out, str);
    }

    /* 打印前缀 */
    const char *strLevel[] = {"", "LOG:     ", "WARNING: ", "ERROR:   "};
    (void)vsprintf(str, strLevel[level], 0);
    strcat(out, str);

    /* 打印内容 */
    (void)vsprintf(str, format, arg);
    strcat(out, str);

    /* 恢复颜色 */
    (void)vsprintf(str, "\r\n\033[0m", 0);
    strcat(out, str);

    va_end(arg);

    DebugShowPRINT(out);
}