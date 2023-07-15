/**
 * @file map.c
 * @author zhangmeng (847305142@qq.com)
 * @brief 完成地图相关功能实现
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "map.h"
#include "map_data.h"

INI_CFG_VALUE_S gMapIniCfg[MAP_INI_INFO_END] = {
    {"width", MAP_INI_INFO_WIDTH},
    {"high", MAP_INI_INFO_HIGHT},
    {"resourcenum", MAP_INI_INFO_RESOURCENUM},
    {"conflictMulti", MAP_INI_INFO_CONFLICTMULTI},
    {"conflictConsume", MAP_INI_INFO_CONFLICCONSUME},
};

void MapInitSet(uint32 type, uint32 value)
{
    MapInfoSet(type, value);
}

/* 构建地图 */
uint32 MapCreate(const char *filename)
{
    /* 如果已经初始化过了，则报错 */
    CHECK_CONDITION_AUTORETURN(MapInfoGet(MAP_INI_INFO_HIGHT), CHECK_CONDITION_EQ, 0);

    /* 读取ini配置文件中的信息更新 */
    uint32 ret = IniFileValueGet(filename, gMapIniCfg, ARRAYSIZE(gMapIniCfg), MapInitSet);
    CHECK_RET_AUTORETURN(ret);

    /* 初始化运行态数据 */
    ret = MapRunInfoInit();
    if (ret != SUCCESS) {
        MapDestory();
        return ret;
    }

    return SUCCESS;
}
