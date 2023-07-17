/**
 * @file animal.h
 * @author zhangmeng (847305142@qq.com)
 * @brief
 * @version 0.1
 * @date 2023-07-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ANIMAL_ALL_H
#define ANIMAL_ALL_H

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ANIMAL_INI_INFO_NUM = 0,
    ANIMAL_INI_INFO_SIZE,
    ANIMAL_INI_INFO_VIEW,
    ANIMAL_INI_INFO_REPRODUCTIONTH,
    ANIMAL_INI_INFO_SPEED,
    ANIMAL_INI_INFO_FINDRESOURCEPLAN,
    ANIMAL_INI_INFO_REPRODUCTIONPLAN,
    ANIMAL_INI_INFO_END
} ANIMAL_INI_INFO_E;

typedef enum {
    ANIMAL_ALL_INI_INFO_NUM = 0,
    ANIMAL_ALL_INI_INFO_END
} ANIMAL_ALL_INI_INFO_E;

uint32 AnimalAllCreate(const char *filename);

uint32 AnimalAllInfoGet(ANIMAL_ALL_INI_INFO_E type);

void AnimalAllDestory(void);

/* end为1说明一天结束 */
uint32 AnimalAllRun(uint32 end);

#ifdef __cplusplus
}
#endif

#endif