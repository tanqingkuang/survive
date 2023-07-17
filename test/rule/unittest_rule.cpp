#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "rule.h"

#define SIZE 3

static uint32 gMapResource[SIZE][SIZE] = {{0}};

uint32 MapInfoGet(MAP_INI_INFO_E type)
{
    return SIZE;
}

/* 默认没有资源 */

uint32 MapResourceInfoGet(const MAP_POINT_S *point, uint32 *resourceSize)
{
    *resourceSize = gMapResource[point->y][point->x];
    return SUCCESS;
}

/* 测试资源分配功能 */
TEST(RULE, ResouceAllocate)
{
    EXPECT_EQ(RuleCreate("../test/rule/cfg.ini"), SUCCESS);
}

/* 测试资源查找功能0：只在view内查找，如果找不到则停止 */
TEST(RULE, FindResource0)
{
    EXPECT_EQ(RuleCreate("../test/rule/cfg.ini"), SUCCESS);

    MAP_POINT_S point = {1, 1};
    float size = 1;
    RULE_FIND_RESOURCE_S info = {0, &point, 1, 1, &size};

    /* 异常入参 */
    EXPECT_NE(RuleFindResource(NULL), SUCCESS);
    info.point = NULL;
    EXPECT_NE(RuleFindResource(&info), SUCCESS);
    info.point = &point;
    info.view = 0;
    EXPECT_NE(RuleFindResource(&info), SUCCESS);
    info.view = 1;
    info.speed = 0;
    EXPECT_NE(RuleFindResource(&info), SUCCESS);
    info.speed = 1;

    /* 模拟找不到食物的停止 */
    EXPECT_EQ(RuleFindResource(&info), SUCCESS);
    EXPECT_EQ(point.x, 1);
    EXPECT_EQ(point.y, 1);
    EXPECT_NEAR(size, 0.998, 0.0001); /* 虽然不动，但是看一眼也是要消耗能量的 */
    memset(gMapResource, 0, sizeof(gMapResource));
    point.x = 1;
    point.y = 1;
    size = 1;

    /* 只有一个食物能看到则取该食物点 */
    gMapResource[0][0] = 10;
    gMapResource[1][0] = 1;
    EXPECT_EQ(RuleFindResource(&info), SUCCESS);
    EXPECT_EQ(point.x, 0);
    EXPECT_EQ(point.y, 1);
    EXPECT_NEAR(size, 0.983, 0.0001);
    memset(gMapResource, 0, sizeof(gMapResource));
    point.x = 1;
    point.y = 1;
    size = 1;

    /* 有2个食物则取该食物点，不一样大则取大的那个 */
    gMapResource[1][0] = 1;
    gMapResource[1][2] = 2;
    EXPECT_EQ(RuleFindResource(&info), SUCCESS);
    EXPECT_EQ(point.x, 2);
    EXPECT_EQ(point.y, 1);
    EXPECT_NEAR(size, 0.983, 0.0001);
    memset(gMapResource, 0, sizeof(gMapResource));
    point.x = 1;
    point.y = 1;
    size = 1;

    /* 有2个食物则取该食物点，同样大小取近的哪个 */
    gMapResource[0][0] = 1;
    gMapResource[1][2] = 1;
    info.view = 10;
    EXPECT_EQ(RuleFindResource(&info), SUCCESS);
    EXPECT_EQ(point.x, 2);
    EXPECT_EQ(point.y, 1);
    EXPECT_NEAR(size, 0.965, 0.0001);
    memset(gMapResource, 0, sizeof(gMapResource));
    point.x = 1;
    point.y = 1;
    size = 1;
}

static uint32 AnimalCreatStub(uint32 id)
{
    return SUCCESS;
}

/* 测试繁殖功能 */
TEST(RULE, Reproduction)
{
    EXPECT_EQ(RuleCreate("../test/rule/cfg.ini"), SUCCESS);
    float size = 1;
    RULE_EPRODUCTION_INFO_S info = {0, 2, &size, 3, 0, AnimalCreatStub};

    /* 异常入参 */
    EXPECT_NE(RuleReproduction(NULL), SUCCESS);
    info.pfunc = NULL;
    EXPECT_NE(RuleReproduction(&info), SUCCESS);
    info.pfunc = AnimalCreatStub;
    info.size = NULL;
    EXPECT_NE(RuleReproduction(&info), SUCCESS);
    info.size = &size;

    /* 不满足繁殖要求 */
    MOCKER(AnimalCreatStub)
        .expects(never())
        .will(returnValue(SUCCESS));
    EXPECT_EQ(RuleReproduction(&info), SUCCESS);
    GlobalMockObject::verify();
    EXPECT_EQ(size, 1);

    /* 满足则繁殖 */
    size = 5;
    MOCKER(AnimalCreatStub)
        .expects(once())
        .will(returnValue(SUCCESS));
    EXPECT_EQ(RuleReproduction(&info), SUCCESS);
    GlobalMockObject::verify();
    EXPECT_NEAR(size, 4.9 - 2, 0.0001);
}