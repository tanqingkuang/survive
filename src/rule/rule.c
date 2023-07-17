/**
 * @file rule.c
 * @author zhangmeng (847305142@qq.com)
 * @brief 上帝规则实现
 * @version 0.1
 * @date 2023-07-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "rule.h"
#include "map_data.h"

typedef struct {
    float conflictMulti;
    float conflictConsume;
    float speedConsume;
    float sizeConsume;
    uint32 tick;
    float viewConsume;
    float reproductionConsume;
} RULE_INFO_S;

RULE_INFO_S gRuleInfo = {0};

INI_CFG_VALUE_S gRuleIniCfg[RULE_INI_INFO_END] = {
    {"conflictMulti", RULE_INI_INFO_CONFLICTMULTI},
    {"conflictConsume", RULE_INI_INFO_CONFLICCONSUME},
    {"speedConsume", RULE_INI_INFO_SPEEDCONSUME},
    {"sizeConsume", RULE_INI_INFO_SIZECONSUME},
    {"viewConsume", RULE_INI_INFO_VIEWCONSUME},
    {"tick", RULE_INI_INFO_TICK},
    {"reproductionConsume", RULE_INI_INFO_REPRODUCTIONCONSUME},
};

void RuleInitSet(uint32 type, uint32 value)
{
    switch (type) {
    case RULE_INI_INFO_CONFLICTMULTI:
        gRuleInfo.conflictMulti = value / 1000.0;
        break;
    case RULE_INI_INFO_CONFLICCONSUME:
        gRuleInfo.conflictConsume = value / 1000.0;
        break;
    case RULE_INI_INFO_SPEEDCONSUME:
        gRuleInfo.speedConsume = value / 1000.0;
        break;
    case RULE_INI_INFO_SIZECONSUME:
        gRuleInfo.sizeConsume = value / 1000.0;
        break;
    case RULE_INI_INFO_VIEWCONSUME:
        gRuleInfo.viewConsume = value / 1000.0;
        break;
    case RULE_INI_INFO_TICK:
        gRuleInfo.tick = value;
        break;
    case RULE_INI_INFO_REPRODUCTIONCONSUME:
        gRuleInfo.reproductionConsume = value / 1000.0;
        break;
    }
}

/*******Interface******/

uint32 RuleCreate(const char *filename)
{
    return IniFileValueGet(filename, "rule", gRuleIniCfg, ARRAYSIZE(gRuleIniCfg), RuleInitSet);
}

float RuleInfoGet(uint32 type)
{
    switch (type) {
    case RULE_INI_INFO_CONFLICTMULTI:
        return gRuleInfo.conflictMulti;
    case RULE_INI_INFO_CONFLICCONSUME:
        return gRuleInfo.conflictConsume;
    default:
        return 0;
    }
}

/* 资源分配功能
 * 资源分配的大体思路是先看一下各个占用者处于什么模式，一旦有一个要竞争那就开启竞争，否则是共享模式
 * 竞争模式下：提出竞争的己方有系数加成，但是会每个生物消耗相同的能量；共享模式则没有这些
 * 每个生物增加的能量 = 竞争系数(共享模式为1) * 自己的体型 * 总资源 / 总体型 - 资源消耗(共享模式为0)
 * 这意味这该功能除了从配置文件拿到的竞争系数和资源消耗外，还需要知道
 * * 每个生物的体型
 * * 每个生物的斗争倾向
 * * 总资源
 * 由于需要知道各个生物的体型和斗争倾向，这是个比较大的数据，因此该如何提供，目前还没有想好
 */

uint32 RuleResouceAllocate(uint32 resourceMap, MAP_RUN_INFO_TAKE_ANIMAL_S *head)
{
    MAP_RUN_INFO_TAKE_ANIMAL_S *p = head;
    MAP_RESOURCE_TAKE_E type = MAP_RESOURCE_TAKE_SHARE;
    float allSize = 0;
    uint32 animalNum = 0;
    while (p != NULL) {
        if (p->info.type == MAP_RESOURCE_TAKE_CONFLICT) {
            type = MAP_RESOURCE_TAKE_CONFLICT;
            allSize += *(p->info.size) * 1.5f;
        } else {
            allSize += *(p->info.size);
        }
        animalNum++;
        p = p->next;
    }
    CHECK_CONDITION_AUTORETURN(animalNum, CHECK_CONDITION_NE, 0);

    /* 进行资源分配 */
    p = head;
    while (p != NULL) {
        if (type == MAP_RESOURCE_TAKE_SHARE) { /* 资源进行平均分配，分配过程中没有资源消耗 */
            *(p->info.size) += 1.0f * resourceMap / animalNum;
        } else { /* 资源进行竞争分配，分配过程有资源消耗 */
            float rst = resourceMap / allSize * *(p->info.size);
            float conflictConsume = gRuleInfo.conflictConsume;
            if (p->info.type == MAP_RESOURCE_TAKE_SHARE) {
                *(p->info.size) += rst - conflictConsume;
            } else {
                float conflictMulti = gRuleInfo.conflictMulti;
                *(p->info.size) += conflictMulti * rst - conflictConsume;
            }
        }
        p = p->next;
    }
    return SUCCESS;
}

/** 寻找资源
 * 资源寻找需要知道
 * * 生物的移动策略
 * * 生物的当前坐标
 * * 生物的视野
 * * 生物的移动速度
 * * 地图周围坐标是否有食物
 * 出参
 * * 生物的后续坐标
 */
uint32 RuleFindResource(RULE_FIND_RESOURCE_S *info)
{
    CHECK_NULL_AUTORETURN(info);
    CHECK_NULL_AUTORETURN(info->point);
    CHECK_CONDITION_AUTORETURN(info->view, CHECK_CONDITION_GE, 1);
    CHECK_CONDITION_AUTORETURN(info->speed, CHECK_CONDITION_GE, 1);
    return SUCCESS;
}

/** 繁殖
 * 繁殖需要知道
 * * 繁殖策略
 * * 生物的繁殖水线
 * * 生物的create的回调函数
 * * 生物的能量
 * 出参
 * * 生物的能量
 */
uint32 RuleReproduction(RULE_EPRODUCTION_INFO_S *info)
{
    return SUCCESS;
}