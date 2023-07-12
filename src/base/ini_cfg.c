/**
 * @file ini_cfg.c
 * @author zhangmeng (847305142@qq.com)
 * @brief 实现cfg.ini文件的信息获取功能
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ini_cfg.h"

#include <stdio.h>
#include <string.h>

#include "base.h"
#include "malloc.h"

uint32 IniFileInit(const char *fileName, char **handleFile) {
    CHECK_NULL_AUTORETURN(handleFile);

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        DebugShow(DEBUG_LEVEL_ERR, "file[%s] open fail", fileName);
        return ERR_FILE_OPEN;
    }

    long len = filelength(fileno(fp));
    *handleFile = malloc(len);
    if (*handleFile == NULL) {
        // fcolse(fp);
        DebugShow(DEBUG_LEVEL_ERR, "malloc[%u] fail", len);
        return ERR_MALLOC;
    }
    memset(*handleFile, 0, len);

    fread(*handleFile, len, sizeof(char), fp);
    // fclose(fp);
    return SUCCESS;
}

uint32 IniFileRead(const char *handleFile, const char *content, const char *key,
                   char *value, uint32 valueSize) {
    CHECK_NULL_AUTORETURN(handleFile);
    CHECK_NULL_AUTORETURN(content);
    CHECK_NULL_AUTORETURN(key);
    CHECK_NULL_AUTORETURN(value);

    /* 通过content和下一个[确定范围 */
    char contentAll[128] = {0};
    (void)sprintf(contentAll, "[%s]", content);
    char *start = strstr(handleFile, contentAll);
    if (start == NULL) { /* 说明content没有找到 */
        DebugShow(DEBUG_LEVEL_ERR, "content[%s] not found", content);
        return ERR_NOT_FOUND;
    }
    char *end = strstr(start + 1, "[");
    if (end == NULL) {
        end = start + strlen(start);
    }

    /* 在范围内找到key */
    char keyAll[128] = {0};
    (void)sprintf(keyAll, "%s = ", key);
    char *dest = strstr(start + strlen(contentAll), keyAll);
    if (dest >= end || dest == NULL) {
        DebugShow(DEBUG_LEVEL_ERR, "key[%s] not found in content[%s]", key, content);
        return ERR_NOT_FOUND;
    }

    /* 获取key信息 */
    sscanf(dest + strlen(keyAll), "%s", value);
    return SUCCESS;
}

void IniFileDestory(char **handleFile) {
    CHECK_NULL_VOID(handleFile);
    CHECK_NULL_VOID(*handleFile);
    free(*handleFile);
    *handleFile = NULL;
}