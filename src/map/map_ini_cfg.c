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

INI_CFG_VALUE_S gMapIniCfg[MAG_INI_INFO_END] = {
    {"width", MAP_INI_INFO_WIDTH},
    {"high", MAG_INI_INFO_HIGHT},
    {"resourcenum", MAG_INI_INFO_RESOURCENUM},
    {"conflictMulti", MAG_INI_INFO_CONFLICTMULTI},
    {"conflictconsume", MAG_INI_INFO_CONFLICCONSUME},
};

void MapInitSet(uint32 type, uint32 value)
{
    MapInfoSet(type, value);
}

uint32 MapIniCfg(const char *filename)
{
    return IniFileValueGet(filename, gMapIniCfg, ARRAYSIZE(gMapIniCfg), MapInitSet);
}