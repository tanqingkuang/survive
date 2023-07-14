/**
 * @file ini_cfg.h
 * @author zhangmeng (847305142@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INI_CFG_H
#define INI_CFG_H

#include "type.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32 IniFileInit(const char *fileName, char **handleFile);
uint32 IniFileRead(const char *handleFile, const char *content, const char *key,
                   char *value, uint32 valueSize);
uint32 IniFileChange(char **handleFile, const char *content, const char *key,
                   const char *value, uint32 valueSize);
void IniFileDestory(char **handleFile);

typedef struct {
    const char *str;
    uint32 type;
} INI_CFG_VALUE_S;

typedef void (*INI_VALUE_SET_F)(uint32 type, uint32 value);

uint32 IniFileValueGet(const char *filename, INI_CFG_VALUE_S *cfg, uint32 cfgNum, INI_VALUE_SET_F func);

#ifdef __cplusplus
}
#endif

#endif