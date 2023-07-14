/**
 * @file map_data.c
 * @author zhangmeng (847305142@qq.com)
 * @brief 做map数据管理
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "map_data.h"
#include "map.h"

#if SEPARATOR("private", 1)

MAP_INFO_S gMapInfo = {{0}};

void MapInfoSet(MAP_INI_INFO_E type, uint32 data)
{
    switch (type) {
        case MAP_INI_INFO_WIDTH: gMapInfo.iniInfo.width = data;
        case MAG_INI_INFO_HIGHT: gMapInfo.iniInfo.high = data;
        case MAG_INI_INFO_RESOURCENUM: gMapInfo.iniInfo.resourceNum = data;
    }
}

uint32 MapRunInfoInit(void)
{
    uint32 pointNum = gMapInfo.iniInfo.width * gMapInfo.iniInfo.high;
    gMapInfo.runInfo.resourceMap = MALLOC_ZERO(pointNum * sizeof(uint32));
    gMapInfo.runInfo.resourceTake = MALLOC_ZERO(pointNum * sizeof(uint8));
    return SUCCESS;
}

uint32 MapDimen2to1(int x, int y)
{
    return y * gMapInfo.iniInfo.width + x;
}

#endif

#if SEPARATOR("Interface", 1)

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

uint32 MapResourceInfoGet(const MAP_POINT_S *point, uint32 *resourceSize)
{
    CHECK_NULL_AUTORETURN(point);
    CHECK_NULL_AUTORETURN(resourceSize);
    CHECK_NULL_AUTORETURN(gMapInfo.runInfo.resourceMap);
    CHECK_CONDITION_AUTORETURN(point->x, CHECK_CONDITION_LE, gMapInfo.iniInfo.width);
    CHECK_CONDITION_AUTORETURN(point->y, CHECK_CONDITION_LE, gMapInfo.iniInfo.high);
    *resourceSize = gMapInfo.runInfo.resourceMap[MapDimen2to1(point->x, point->y)];
    return SUCCESS;
}

uint32 MapResourceReset(void)
{
    CHECK_NULL_AUTORETURN(gMapInfo.runInfo.resourceMap);

    memset(gMapInfo.runInfo.resourceMap, 0, gMapInfo.iniInfo.width * gMapInfo.iniInfo.high * sizeof(uint32));
    srand((uint32)time(NULL)); /* 由于下面流程运行的太快，所以不能再for外初始化随机种子 */
    for (uint32 i = 0; i < gMapInfo.iniInfo.resourceNum; i++) {
        uint32 x = rand() % gMapInfo.iniInfo.width;
        uint32 y = rand() % gMapInfo.iniInfo.high;
        gMapInfo.runInfo.resourceMap[MapDimen2to1(x, y)] += 1;
        gMapInfo.runInfo.resourceMap[MapDimen2to1(x, y)] = gMapInfo.runInfo.resourceMap[MapDimen2to1(x, y)];
    }

    return SUCCESS;
}

#endif