/**
 * @file animal.c
 * @author zhangmeng (847305142@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "animal.h"
#include "map.h"
#include "rule.h"

typedef struct {
    uint32 num;
    float size;
    float view;
    float reproductionTh;
    float speed;
    uint32 findResourcePlan;
    uint32 reproductionPlan;
} ANIMAL_INI_INFO_S;

typedef struct ANIMAL_RUN_INFO_S {
    uint32 animalId;
    MAP_POINT_S point;
    float size;
    struct ANIMAL_RUN_INFO_S *next;
    struct ANIMAL_RUN_INFO_S *prev;
} ANIMAL_RUN_INFO_S;

typedef struct
{
    ANIMAL_INI_INFO_S iniInfo;
    uint32 num;
    ANIMAL_RUN_INFO_S *next;
} ANIMAL_INFO_S;

INI_CFG_VALUE_S gAnimalIniCfg[ANIMAL_INI_INFO_END] = {
    {"num", ANIMAL_INI_INFO_NUM},
    {"size", ANIMAL_INI_INFO_SIZE},
    {"view", ANIMAL_INI_INFO_VIEW},
    {"reproductionTh", ANIMAL_INI_INFO_REPRODUCTIONTH},
    {"speed", ANIMAL_INI_INFO_SPEED},
    {"findResourcePlan", ANIMAL_INI_INFO_FINDRESOURCEPLAN},
    {"reproductionPlan", ANIMAL_INI_INFO_REPRODUCTIONPLAN},
};

static uint32 gAnimalId = 0;
ANIMAL_INFO_S *gAnimalInfo = NULL;

void AnimalInitSet(uint32 type, uint32 value)
{
    CHECK_NULL_VOID(gAnimalInfo);
    switch (type) {
    case ANIMAL_INI_INFO_NUM:
        gAnimalInfo[gAnimalId].iniInfo.num = value;
        break;
    case ANIMAL_INI_INFO_SIZE:
        gAnimalInfo[gAnimalId].iniInfo.size = value / 100.0f;
        break;
    case ANIMAL_INI_INFO_VIEW:
        gAnimalInfo[gAnimalId].iniInfo.view = value / 100.0f;
        break;
    case ANIMAL_INI_INFO_REPRODUCTIONTH:
        gAnimalInfo[gAnimalId].iniInfo.reproductionTh = value / 100.0f;
        break;
    case ANIMAL_INI_INFO_SPEED:
        gAnimalInfo[gAnimalId].iniInfo.speed = value / 100.0f;
        break;
    case ANIMAL_INI_INFO_FINDRESOURCEPLAN:
        gAnimalInfo[gAnimalId].iniInfo.findResourcePlan = value;
        break;
    case ANIMAL_INI_INFO_REPRODUCTIONPLAN:
        gAnimalInfo[gAnimalId].iniInfo.reproductionPlan = value;
        break;
    }
}

uint32 AnimalCreate(uint32 idx)
{
    ANIMAL_INFO_S *animals = gAnimalInfo + idx;
    ANIMAL_RUN_INFO_S *animal = (ANIMAL_RUN_INFO_S *)MALLOC_ZERO(sizeof(ANIMAL_RUN_INFO_S));
    CHECK_NULL_AUTORETURN(animal);

    /* 赋予初值 */
    animal->animalId = gAnimalId;
    gAnimalId++;
    animal->point.x = rand() % MapInfoGet(MAP_INI_INFO_WIDTH);
    animal->point.y = rand() % MapInfoGet(MAP_INI_INFO_HIGHT);
    animal->size = animals->iniInfo.size;

    /* 节点挂接 */
    animal->next = animals->next;
    animal->prev = NULL;
    if (animals->next != NULL) {
        animals->next->prev = animal;
    }
    animals->next = animal;
    animals->num += 1;
    return SUCCESS;
}

uint32 AnimalInit(const char *filename, uint32 num)
{
    gAnimalInfo = (ANIMAL_INFO_S *)MALLOC_ZERO(sizeof(ANIMAL_INFO_S) * num);
    CHECK_NULL_AUTORETURN(gAnimalInfo);

    for (uint32 idx = 0; idx < num; idx++) {
        char str[128] = {0};
        sprintf(str, "animal%u", idx);
        gAnimalId = idx;
        CHECK_RET_AUTORETURN(IniFileValueGet(filename, str, gAnimalIniCfg, ARRAYSIZE(gAnimalIniCfg), AnimalInitSet));
    }

    gAnimalId = 0;
    for (uint32 i = 0; i < num; i++) {
        for (uint32 j = 0; j < gAnimalInfo[i].iniInfo.num; j++) {
            CHECK_RET_AUTORETURN(AnimalCreate(i));
        }
    }
    return SUCCESS;
}

void AnimalDestory(uint32 num)
{
    if (gAnimalInfo == NULL)
        return;
    for (uint32 i = 0; i < num; i++) {
        ANIMAL_RUN_INFO_S *p = gAnimalInfo[i].next;
        while (p != NULL) {
            gAnimalInfo[i].next = p->next;
            free(p);
            p = gAnimalInfo[i].next;
        }
    }
    free(gAnimalInfo);
}

/* 每个生物的运行 */
uint32 AnimalRun(uint32 idx, ANIMAL_INI_INFO_S *iniInfo, ANIMAL_RUN_INFO_S *animal, uint32 end)
{
    CHECK_NULL_AUTORETURN(iniInfo);
    CHECK_NULL_AUTORETURN(animal);

    // 寻找食物并移动
    RULE_FIND_RESOURCE_S info = {iniInfo->findResourcePlan, &animal->point, iniInfo->view, iniInfo->speed, &animal->size};
    CHECK_RET_AUTORETURN(RuleFindResource(&info));

    uint32 resourceSize = 0;
    CHECK_RET_AUTORETURN(MapResourceInfoGet(&animal->point, &resourceSize));
    if (resourceSize != 0) { // 如果该节点有食物则占用
        MAP_RESCOURCE_TAKE_INFO_S info = {
            {animal->point.x, animal->point.y},
            {animal->animalId, &(animal->size), 0}};
        CHECK_RET_AUTORETURN(MapResourceTake(&info));
    }

    if (end == 1) { // 一天结束开始繁殖
        RULE_EPRODUCTION_INFO_S info = {iniInfo->reproductionPlan, iniInfo->size, &animal->size, iniInfo->reproductionTh, idx, AnimalCreate};
        CHECK_RET_AUTORETURN(RuleReproduction(&info));
    }
    return SUCCESS;
}

/* 每个物种的运行 */
uint32 AnimalsRun(uint32 idx, uint32 end)
{
    ANIMAL_RUN_INFO_S *p = gAnimalInfo[idx].next;
    while (p != NULL) {
        CHECK_RET_AUTORETURN(AnimalRun(idx, &gAnimalInfo[idx].iniInfo, p, end));
        if (p->size <= 0) { /* 说明死亡 */
            if (p->prev != NULL)
                p->prev->next = p->next;
            else
                gAnimalInfo[idx].next = p->next;
            if (p->next != NULL)
                p->next->prev = p->prev;
            gAnimalInfo[idx].num -= 1;
            p = p->next;
            free(p);
            continue;
        }
        p = p->next;
    }
    return SUCCESS;
}

uint32 AnimalsNumGet(uint32 idx)
{
    return gAnimalInfo[idx].num;
}