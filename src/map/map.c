/**
 * @file map.c
 * @author your name (you@domain.com)
 * @brief 完成地图相关功能实现
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "map.h"

typedef struct {
    const char *str;
    uint32 *value;
} MAP_INI_CFG_S;

MAP_INI_CFG_S gMapIniCfg[MAG_INI_INFO_END] = {
    {"width"},
    {"high"},
    {"resourcenum"},
};

typedef struct {
    uint32 width;
    uint32 high;
    uint32 resourceNum;
} MAP_INI_INFO_S;

typedef struct {
    uint32 *resourceMap;
    uint8 *resourceTake;
} MAP_RUN_INFO_S;

typedef struct {
    MAP_INI_INFO_S iniInfo;
    MAP_RUN_INFO_S runInfo;
} MAP_INFO_S;

MAP_INFO_S gMapInfo = {{0}};

uint32 MapCreate(const char *filename)
{
    CHECK_NULL_AUTORETURN(filename);
    CHECK_CONDITION_AUTORETURN(gMapInfo.iniInfo.high, CHECK_CONDITION_EQ, 0);

    /* 读取Ini文件 */
    char *handleFile = NULL;
    uint32 ret = IniFileInit(filename, &handleFile);
    CHECK_RET_AUTORETURN(ret);

    /* 关联数据 */
    gMapIniCfg[MAP_INI_INFO_WIDTH].value = &gMapInfo.iniInfo.width;
    gMapIniCfg[MAG_INI_INFO_HIGHT].value = &gMapInfo.iniInfo.high;
    gMapIniCfg[MAG_INI_INFO_RESOURCENUM].value = &gMapInfo.iniInfo.resourceNum;

    /* 获取数据 */
    for (uint32 i = 0; i < ARRAYSIZE(gMapIniCfg); i++) {
        char value[128] = {0};
        ret = IniFileRead(handleFile, "map", gMapIniCfg[i].str, value, ARRAYSIZE(value));
        CHECK_RET_AUTORETURN(ret);
        sscanf(value, "%u", gMapIniCfg[i].value);
    }

    /* 创建空间 */
    uint32 pointNum = gMapInfo.iniInfo.width * gMapInfo.iniInfo.high;
    gMapInfo.runInfo.resourceMap = MALLOC_ZERO(pointNum * sizeof(uint32));
    gMapInfo.runInfo.resourceTake = MALLOC_ZERO(pointNum * sizeof(uint8));

    /* 分配资源 */
    for (uint32 i = 0; i < gMapInfo.iniInfo.resourceNum; i++) {
        srand((uint32)time(NULL));
        uint32 x = rand() % gMapInfo.iniInfo.width;
        uint32 y = rand() % gMapInfo.iniInfo.high;
        gMapInfo.runInfo.resourceMap[x * gMapInfo.iniInfo.width + y] += 1;
    }
    return SUCCESS;
}

uint32 MapInfoGet(MAP_INI_INFO_E type)
{
    switch (type) {
        case MAP_INI_INFO_WIDTH: return gMapInfo.iniInfo.width;
        case MAG_INI_INFO_HIGHT: return gMapInfo.iniInfo.high;
        case MAG_INI_INFO_RESOURCENUM: return gMapInfo.iniInfo.resourceNum;
        default: return 0; 
    }
}

void MapDestory(void)
{
    if (gMapInfo.runInfo.resourceMap != NULL) free(gMapInfo.runInfo.resourceMap);
    if (gMapInfo.runInfo.resourceTake != NULL) free(gMapInfo.runInfo.resourceTake);
    (void)memset(&gMapInfo, 0, sizeof(gMapInfo));
}