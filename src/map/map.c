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
#include "map_ini_cfg.h"

uint32 MapCreate(const char *filename)
{
    CHECK_CONDITION_AUTORETURN(MapInfoGet(MAG_INI_INFO_HIGHT), CHECK_CONDITION_EQ, 0);

    /* 读取ini配置文件中的信息更新 */
    uint32 ret = MapIniCfg(filename);
    CHECK_RET_AUTORETURN(ret);

    /* 初始化运行态数据 */
    ret = MapRunInfoInit();
    CHECK_RET_AUTORETURN(ret);

    /* 分配资源 */
    MapResourceReset();
    return SUCCESS;
}
