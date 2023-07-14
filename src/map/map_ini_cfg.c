/**
 * @file map_ini_cfg.c
 * @author zhangmeng (847305142@qq.com)
 * @brief 读取ini配置文件
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "map_ini_cfg.h"
#include "map_data.h"

typedef struct {
    const char *str;
    MAP_INI_INFO_E typeDest;
} MAP_INI_CFG_S;

MAP_INI_CFG_S gMapIniCfg[MAG_INI_INFO_END] = {
    {"width", MAP_INI_INFO_WIDTH},
    {"high", MAG_INI_INFO_HIGHT},
    {"resourcenum", MAG_INI_INFO_RESOURCENUM},
    {"conflictMulti", MAG_INI_INFO_CONFLICTMULTI},
    {"conflictconsume", MAG_INI_INFO_CONFLICCONSUME},
};

uint32 MapIniCfg(const char *filename)
{
    CHECK_NULL_AUTORETURN(filename);

    /* 读取Ini文件 */
    char *handleFile = NULL;
    uint32 ret = IniFileInit(filename, &handleFile);
    CHECK_RET_AUTORETURN(ret);

    /* 获取数据 */
    for (uint32 i = 0; i < ARRAYSIZE(gMapIniCfg); i++) {
        char valueStr[128] = {0};
        ret = IniFileRead(handleFile, "map", gMapIniCfg[i].str, valueStr, ARRAYSIZE(valueStr));
        CHECK_RET_AUTORETURN(ret);
        MapInfoSet(gMapIniCfg[i].typeDest, atoi(valueStr));
    }

    /* 销毁句柄 */
    IniFileDestory(&handleFile);
    return SUCCESS;
}