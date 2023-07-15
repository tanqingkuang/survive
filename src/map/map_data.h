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

typedef struct MAP_RUN_INFO_TAKE_ANIMAL_S {
    MAP_ANIMAL_S info;
    struct MAP_RUN_INFO_TAKE_ANIMAL_S *next;
} MAP_RUN_INFO_TAKE_ANIMAL_S;

void MapInfoSet(MAP_INI_INFO_E type, uint32 data);
uint32 MapRunInfoInit(void);

#ifdef __cplusplus
}
#endif

#endif