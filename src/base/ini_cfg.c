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
#include "base.h"

#if SEPARATOR("Private", 1)

uint32 IniFileFindKey(const char *handleFile, const char *content, const char *key, char **valueStr)
{
    CHECK_NULL_AUTORETURN(handleFile);
    CHECK_NULL_AUTORETURN(content);
    CHECK_NULL_AUTORETURN(key);
    CHECK_NULL_AUTORETURN(valueStr);

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

    *valueStr = dest + strlen(keyAll);
    return SUCCESS;
}

#endif

#if SEPARATOR("Public", 1)

uint32 IniFileInit(const char *fileName, char **handleFile) {
    CHECK_NULL_AUTORETURN(handleFile);

    FILE *fp = fopen(fileName, "r");
    if (fp == NULL) {
        DebugShow(DEBUG_LEVEL_ERR, "file[%s] open fail", fileName);
        return ERR_FILE_OPEN;
    }

    long len = filelength(fileno(fp));
    *handleFile = MALLOC_ZERO(len);
    if (*handleFile == NULL) {
        // fcolse(fp);
        DebugShow(DEBUG_LEVEL_ERR, "malloc[%u] fail", len);
        return ERR_MALLOC;
    }

    fread(*handleFile, len, sizeof(char), fp);
    // fclose(fp);
    return SUCCESS;
}

uint32 IniFileRead(const char *handleFile, const char *content, const char *key,
                   char *value, uint32 valueSize) {
    CHECK_NULL_AUTORETURN(value);

    char *valueStr = NULL;
    uint32 ret =  IniFileFindKey(handleFile, content, key, &valueStr);
    CHECK_RET_AUTORETURN(ret);

    /* 提取内容 */
    (void)sscanf(valueStr, "%s", value);
    return SUCCESS;
}

uint32 IniFileChange(char **handleFile, const char *content, const char *key,
                   const char *value, uint32 valueSize)
{
    CHECK_NULL_AUTORETURN(handleFile);
    CHECK_NULL_AUTORETURN(value);

    char *oldHandleFile = *handleFile;
    char *valueStr = NULL;
    uint32 ret =  IniFileFindKey(oldHandleFile, content, key, &valueStr);
    CHECK_RET_AUTORETURN(ret);

    char oldValue[128] = {0};
    (void)sscanf(valueStr, "%s", oldValue);

    int len = strlen(oldHandleFile) + 1 - strlen(oldValue) + strlen(value);
    char *newHandleFile = (char *)MALLOC_ZERO(len);
    memcpy(newHandleFile, oldHandleFile, valueStr - oldHandleFile);
    strcat(newHandleFile, value);
    strcat(newHandleFile, valueStr + strlen(oldValue));

    free(oldHandleFile);
    *handleFile = newHandleFile;
    
    return SUCCESS;
}

void IniFileDestory(char **handleFile) {
    CHECK_NULL_VOID(handleFile);
    CHECK_NULL_VOID(*handleFile);
    free(*handleFile);
    *handleFile = NULL;
}

uint32 IniFileValueGet(const char *filename, const char *content, INI_CFG_VALUE_S *cfg, uint32 cfgNum, INI_VALUE_SET_F func)
{
    CHECK_NULL_AUTORETURN(filename);
    CHECK_NULL_AUTORETURN(cfg);
    CHECK_NULL_AUTORETURN(func);

    /* 读取Ini文件 */
    char *handleFile = NULL;
    uint32 ret = IniFileInit(filename, &handleFile);
    CHECK_RET_AUTORETURN(ret);

    /* 获取数据 */
    for (uint32 i = 0; i < cfgNum; i++) {
        char valueStr[128] = {0};
        ret = IniFileRead(handleFile, content, cfg[i].str, valueStr, ARRAYSIZE(valueStr));
        CHECK_RET_AUTORETURN(ret);
        func(cfg[i].type, atoi(valueStr));
    }

    /* 销毁句柄 */
    IniFileDestory(&handleFile);
    return SUCCESS;
}

#endif