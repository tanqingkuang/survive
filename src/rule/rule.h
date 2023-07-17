/**
 * @file rule.h
 * @author zhangmeng (847305142@qq.com)
 * @brief 上帝功能对外接口汇总
 * @version 0.1
 * @date 2023-07-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef RULE_H
#define RULE_H

#include "base.h"
#include "map_data.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 为了方便测试，这里将信息作为dfx开放出去 */
typedef enum {
    RULE_INI_INFO_CONFLICTMULTI = 0,
    RULE_INI_INFO_CONFLICCONSUME,
    RULE_INI_INFO_SPEEDCONSUME,
    RULE_INI_INFO_SIZECONSUME,
    RULE_INI_INFO_VIEWCONSUME,
    RULE_INI_INFO_TICK,
    RULE_INI_INFO_REPRODUCTIONCONSUME,
    RULE_INI_INFO_END
} RULE_INI_INFO_E;

typedef struct {
    uint32 type; /* 算法类型 */
    MAP_POINT_S *point;
    float view;
    float speed;
} RULE_FIND_RESOURCE_S;

typedef uint32 (*ANIMAL_CREATE_F)(uint32 id);

typedef struct {
    uint32 type;
    float *resource;
    float reproductionTh;
    uint32 idx;
    ANIMAL_CREATE_F pfunc;
} RULE_EPRODUCTION_INFO_S;

uint32 RuleCreate(const char *filename);
float RuleInfoGet(uint32 type);
uint32 RuleResouceAllocate(uint32 resourceMap, MAP_RUN_INFO_TAKE_ANIMAL_S *head);
uint32 RuleFindResource(RULE_FIND_RESOURCE_S *info);
uint32 RuleReproduction(RULE_EPRODUCTION_INFO_S *info);

#ifdef __cplusplus
}
#endif

#endif