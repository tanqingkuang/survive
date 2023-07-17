#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "animal_all.h"
#include "map.h"
#include "rule.h"

#if SEPARATOR("打桩", 1)

/* 这种情况下模拟map只有1*1，且有一个资源 */
uint32 MapInfoGet(MAP_INI_INFO_E type)
{
    return 1;
}

uint32 RuleFindResource(RULE_FIND_RESOURCE_S *info)
{
    return SUCCESS;
}

uint32 MapResourceTake(const MAP_RESCOURCE_TAKE_INFO_S *info)
{
    return SUCCESS;
}

uint32 RuleReproduction(RULE_EPRODUCTION_INFO_S *info)
{
    *(info->resource) = 0;
    return SUCCESS;
}

/* 说明该节点只有一个资源了 */
uint32 MapResourceInfoGet(const MAP_POINT_S *point, uint32 *resourceSize)
{
    *resourceSize = 1;
    return SUCCESS;
}

uint32 MapResourceInfoGetZero(const MAP_POINT_S *point, uint32 *resourceSize)
{
    *resourceSize = 0;
    return SUCCESS;
}
#endif

TEST(Animal, CreateDestory)
{
    EXPECT_EQ(AnimalAllCreate("../test/animal/cfg.ini"), SUCCESS);
    EXPECT_EQ(AnimalAllInfoGet(ANIMAL_ALL_INI_INFO_NUM), 2);

    AnimalAllDestory();
    EXPECT_EQ(AnimalAllInfoGet(ANIMAL_ALL_INI_INFO_NUM), 0);
}

/* 用于测试接口调用测试是否正确 */
TEST(Animal, RunTimes)
{
#if SEPARATOR("有食物占用", 1)
    EXPECT_EQ(AnimalAllCreate("../test/animal/cfg_only.ini"), SUCCESS);

    MOCKER(RuleFindResource)
        .expects(exactly(2))
        .will(returnValue(SUCCESS));
    MOCKER(MapResourceTake)
        .expects(exactly(2))
        .will(returnValue(SUCCESS));
    MOCKER(RuleReproduction)
        .expects(exactly(1))
        .will(returnValue(SUCCESS));
    AnimalAllRun(0);
    AnimalAllRun(1); /* 一天结束 */

    GlobalMockObject::verify();

    AnimalAllDestory();
#endif

#if SEPARATOR("无食物占用", 1)

    EXPECT_EQ(AnimalAllCreate("../test/animal/cfg_only.ini"), SUCCESS);

    MOCKER(MapResourceInfoGet)
        .stubs()
        .will(invoke(MapResourceInfoGetZero));
    MOCKER(RuleFindResource)
        .expects(exactly(2))
        .will(returnValue(SUCCESS));
    MOCKER(MapResourceTake)
        .expects(exactly(0))
        .will(returnValue(SUCCESS));
    MOCKER(RuleReproduction)
        .expects(exactly(1))
        .will(returnValue(SUCCESS));
    AnimalAllRun(0);
    AnimalAllRun(1); /* 一天结束 */

    GlobalMockObject::verify();

    AnimalAllDestory();
#endif
}

TEST(Animal, Die)
{
    EXPECT_EQ(AnimalAllCreate("../test/animal/cfg_only.ini"), SUCCESS);

    EXPECT_EQ(AnimalNumGet(ANIMALS_NUM_E, 0), 1);
    EXPECT_EQ(AnimalNumGet(ANIMAL_NUM_E, 0), 1);

    AnimalAllRun(0);
    EXPECT_EQ(AnimalNumGet(ANIMALS_NUM_E, 0), 1);
    EXPECT_EQ(AnimalNumGet(ANIMAL_NUM_E, 0), 1); /* 由于这里是通过繁殖策略来将资源降到0，所以这里迭代不会死亡 */

    AnimalAllRun(1); /* 一天结束 */
    EXPECT_EQ(AnimalNumGet(ANIMALS_NUM_E, 0), 1);
    EXPECT_EQ(AnimalNumGet(ANIMAL_NUM_E, 0), 0);

    AnimalAllDestory();
}