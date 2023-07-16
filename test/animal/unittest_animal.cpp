#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "animal_all.h"
#include "map.h"

uint32 MapInfoGet(MAP_INI_INFO_E type)
{
    switch (type)
    {
    case MAP_INI_INFO_WIDTH:
        return 6;
    case MAP_INI_INFO_HIGHT:
        return 7;
    default:
        return 0;
    }
}

uint32 MapResourceTake(const MAP_RESCOURCE_TAKE_INFO_S *info)
{
    return SUCCESS;
}

TEST(Animal, CreateDestory)
{
    uint32 animalNum = 0;
    EXPECT_EQ(AnimalAllCreate("../test/animal/cfg.ini"), SUCCESS);
    EXPECT_EQ(AnimalAllInfoGet(ANIMAL_ALL_INI_INFO_NUM), 2);

    AnimalAllDestory();
    EXPECT_EQ(AnimalAllInfoGet(ANIMAL_ALL_INI_INFO_NUM), 0);
}

TEST(Animal, Run)
{
    uint32 animalNum = 0;
    EXPECT_EQ(AnimalAllCreate("../test/animal/cfg.ini"), SUCCESS);

    AnimalAllRun(0);

    AnimalAllDestory();
}