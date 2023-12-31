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
#include "rule.h"

#if SEPARATOR("private", 1)

typedef struct
{
    uint32 width;
    uint32 high;
    uint32 resourceNum;
    float conflictConsume;
} MAP_INI_INFO_S;

typedef struct
{
    uint8 using;
    MAP_RUN_INFO_TAKE_ANIMAL_S *next;
} MAP_RUN_INFO_TAKE_S;

typedef struct
{
    uint32 *resourceMap;
    MAP_RUN_INFO_TAKE_S *resourceTake;
} MAP_RUN_INFO_S;

typedef struct
{
    MAP_INI_INFO_S iniInfo;
    MAP_RUN_INFO_S runInfo;
} MAP_INFO_S;

MAP_INFO_S gMapInfo = {{0}};

void MapInfoSet(MAP_INI_INFO_E type, uint32 data)
{
    switch (type) {
    case MAP_INI_INFO_WIDTH:
        gMapInfo.iniInfo.width = data;
        break;
    case MAP_INI_INFO_HIGHT:
        gMapInfo.iniInfo.high = data;
        break;
    case MAP_INI_INFO_RESOURCENUM:
        gMapInfo.iniInfo.resourceNum = data;
        break;
    }
}

uint32 MapRunInfoInit(void)
{
    /* 申请空间 */
    uint32 pointNum = gMapInfo.iniInfo.width * gMapInfo.iniInfo.high;
    gMapInfo.runInfo.resourceMap = MALLOC_ZERO(pointNum * sizeof(uint32));
    gMapInfo.runInfo.resourceTake = MALLOC_ZERO(pointNum * sizeof(MAP_RUN_INFO_TAKE_S));
    CHECK_NULL_AUTORETURN(gMapInfo.runInfo.resourceMap);
    CHECK_NULL_AUTORETURN(gMapInfo.runInfo.resourceTake);
    /* 分配资源 */
    MapResourceReset();
    return SUCCESS;
}

/* 为了方便，RunInfo使用的一维数组进行的申请，后面为了使用方便，提供二维转一维的接口 */
uint32 MapDimen2to1(int x, int y)
{
    return y * gMapInfo.iniInfo.width + x;
}

/* 更新某个节点 */
uint32 MapRefreshNode(uint32 *resourceMap, MAP_RUN_INFO_TAKE_S *resourceTake)
{
    CHECK_NULL_AUTORETURN(resourceMap);
    CHECK_NULL_AUTORETURN(resourceTake);

    if (resourceTake->using == 0) { /* 说明该节点无人占用，则直接返回 */
        return SUCCESS;
    }

    /* 资源分配 */
    CHECK_RET_AUTORETURN(RuleResouceAllocate(*resourceMap, resourceTake->next)); // TODO 从这里看出来，map对rule有依赖

    /* 资源清零 */
    *resourceMap = 0;
    resourceTake->using = 0;
    return SUCCESS;
}

#endif

#if SEPARATOR("Interface", 1)

uint32 MapInfoGet(MAP_INI_INFO_E type)
{
    switch (type) {
    case MAP_INI_INFO_WIDTH:
        return gMapInfo.iniInfo.width;
    case MAP_INI_INFO_HIGHT:
        return gMapInfo.iniInfo.high;
    case MAP_INI_INFO_RESOURCENUM:
        return gMapInfo.iniInfo.resourceNum;
    default:
        return 0;
    }
}

void MapDestory(void)
{
    if (gMapInfo.runInfo.resourceMap != NULL)
        free(gMapInfo.runInfo.resourceMap);
    if (gMapInfo.runInfo.resourceTake != NULL)
        free(gMapInfo.runInfo.resourceTake);
    (void)memset(&gMapInfo, 0, sizeof(gMapInfo));
}

uint32 MapResourceInfoGet(const MAP_POINT_S *point, uint32 *resourceSize)
{
    CHECK_NULL_AUTORETURN(point);
    CHECK_NULL_AUTORETURN(resourceSize);
    CHECK_NULL_AUTORETURN(gMapInfo.runInfo.resourceMap);
    CHECK_CONDITION_AUTORETURN(point->x, CHECK_CONDITION_LT, gMapInfo.iniInfo.width);
    CHECK_CONDITION_AUTORETURN(point->y, CHECK_CONDITION_LT, gMapInfo.iniInfo.high);
    *resourceSize = gMapInfo.runInfo.resourceMap[MapDimen2to1(point->x, point->y)];
    return SUCCESS;
}

uint32 MapResourceInfoSet(const MAP_POINT_S *point, uint32 resourceSize)
{
    CHECK_NULL_AUTORETURN(point);
    CHECK_NULL_AUTORETURN(gMapInfo.runInfo.resourceMap);
    CHECK_CONDITION_AUTORETURN(point->x, CHECK_CONDITION_LT, gMapInfo.iniInfo.width);
    CHECK_CONDITION_AUTORETURN(point->y, CHECK_CONDITION_LT, gMapInfo.iniInfo.high);
    gMapInfo.runInfo.resourceMap[MapDimen2to1(point->x, point->y)] = resourceSize;
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

/* 资源抢占 */
uint32 MapResourceTake(const MAP_RESCOURCE_TAKE_INFO_S *info)
{
    CHECK_NULL_AUTORETURN(info);
    CHECK_NULL_AUTORETURN(info->animal.size);
    CHECK_CONDITION_AUTORETURN(*(info->animal.size), CHECK_CONDITION_NE, 0);

    /* 检查是否会重复注册 */
    uint32 idx = MapDimen2to1(info->point.x, info->point.y);
    MAP_RUN_INFO_TAKE_ANIMAL_S *p = gMapInfo.runInfo.resourceTake[idx].next;
    while (p != NULL) {
        CHECK_CONDITION_AUTORETURN(p->info.animalId, CHECK_CONDITION_NE, info->animal.animalId);
        p = p->next;
    }

    /* 申请节点并初始化数据 */
    MAP_RUN_INFO_TAKE_ANIMAL_S *node = (MAP_RUN_INFO_TAKE_ANIMAL_S *)MALLOC_ZERO(sizeof(MAP_RUN_INFO_TAKE_ANIMAL_S));
    CHECK_NULL_AUTORETURN(node);
    node->info.animalId = info->animal.animalId;
    node->info.size = info->animal.size;
    node->info.type = info->animal.type;

    /* 挂接链表 */
    node->next = gMapInfo.runInfo.resourceTake[idx].next;
    gMapInfo.runInfo.resourceTake[idx].next = node;

    /* 表示占用 */
    gMapInfo.runInfo.resourceTake[idx].using = 1;
    return SUCCESS;
}

/* 地图更新 */
uint32 MapRefresh(void)
{
    for (uint32 y = 0; y < MapInfoGet(MAP_INI_INFO_HIGHT); y++) {
        for (uint32 x = 0; x < MapInfoGet(MAP_INI_INFO_WIDTH); x++) {
            uint32 idx = MapDimen2to1(x, y);
            CHECK_RET_AUTORETURN(MapRefreshNode(gMapInfo.runInfo.resourceMap + idx, gMapInfo.runInfo.resourceTake + idx));
        }
    }
    return SUCCESS;
}

#endif