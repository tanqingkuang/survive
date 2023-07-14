/**
 * @file map.h
 * @author zhangmeng (847305142@qq.com)
 * @brief 地图功能对外接口汇总
 * @version 0.1
 * @date 2023-07-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAP_H
#define MAP_H

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 地图初始化：为了测试方便，将filename作为入参传入 */
uint32 MapCreate(const char *filename);

/* 地图销毁 */
void MapDestory(void);

/* TODO:当前没有想好要注册那些信息 */
typedef struct {
    uint32 size;
} MAP_RESCOURCE_TAKE_INFO_S;

/** 坐标轴如下
  *——width——> x
  |
 high
  |
  V y 
 */
typedef struct {
    uint32 x;
    uint32 y;
} MAP_POINT_S;

uint32 MapResourceInfoGet(const MAP_POINT_S *point, uint32 *resourceSize);

/* 资源抢占 */
uint32 MapResourceTake(const MAP_RESCOURCE_TAKE_INFO_S *info);

/* 地图更新 */
uint32 MapRefresh(void);

/* 资源初始化 */
uint32 MapResourceReset(void);

/* 为了方便测试，这里将信息作为dfx开放出去 */
typedef enum {
    MAP_INI_INFO_WIDTH = 0,
    MAG_INI_INFO_HIGHT,
    MAG_INI_INFO_RESOURCENUM,
    MAG_INI_INFO_END
} MAP_INI_INFO_E;

/* 范围值为信息，非错误码，如果未初始化则返回0 */
uint32 MapInfoGet(MAP_INI_INFO_E type);

#ifdef __cplusplus
}
#endif

#endif