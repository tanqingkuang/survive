/**
 * @file map_data.h
 * @author zhangmeng (847305142@qq.com)
 * @brief 做map数据管理
 * @version 0.1
 * @date 2023-07-14
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "base.h"
#include "map.h"

#ifdef __cplusplus
extern "C" {
#endif

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

void MapInfoSet(MAP_INI_INFO_E type, uint32 data);
uint32 MapRunInfoInit(void);

#ifdef __cplusplus
}
#endif

#endif