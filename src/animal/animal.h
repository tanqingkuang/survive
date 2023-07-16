/**
 * @file animal.h
 * @author zhangmeng (847305142@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-07-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ANIMAL_H
#define ANIMAL_H

#include "base.h"
#include "animal_all.h"

#ifdef __cplusplus
extern "C"
{
#endif

    uint32 AnimalInit(const char *filename, uint32 num);
    void AnimalDestory(uint32 num);
    uint32 AnimalsRun(uint32 idx, uint32 end);

#ifdef __cplusplus
}
#endif

#endif