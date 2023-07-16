/**
 * @file animal.c
 * @author zhangmeng (847305142@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-07-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "animal_all.h"
#include "animal.h"

static uint32 gAnimalAllNum = 0;

INI_CFG_VALUE_S gAnimalAllIniCfg[] = {
    {"num", 0},
};

/* 范围值为信息，非错误码，如果未初始化则返回0 */
uint32 AnimalAllInfoGet(ANIMAL_ALL_INI_INFO_E type)
{
    return gAnimalAllNum;
}

void AnimalAllInitSet(uint32 type, uint32 value)
{
    gAnimalAllNum = value;
}

uint32 AnimalAllCreate(const char *filename)
{
    CHECK_RET_AUTORETURN(IniFileValueGet(filename, "animalall", gAnimalAllIniCfg, ARRAYSIZE(gAnimalAllIniCfg), AnimalAllInitSet));
    CHECK_RET_AUTORETURN(AnimalInit(filename, gAnimalAllNum));
}

void AnimalAllDestory(void)
{
    AnimalDestory(gAnimalAllNum);
    gAnimalAllNum = 0;
}

uint32 AnimalAllRun(uint32 end)
{
    for (uint32 idx = 0; idx < gAnimalAllNum; idx++)
    {
        AnimalsRun(idx, end);
    }
    return SUCCESS;
}