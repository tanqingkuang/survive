#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "rule.h"

/* 测试资源分配功能 */
TEST(RULE, ResouceAllocate)
{
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);
}

/* 测试资源查找功能 */
TEST(RULE, FindResource)
{
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);

    MAP_POINT_S point = {0, 0};
    RULE_FIND_RESOURCE_S info = {0, &point, 1, 1};

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

    /* */
}

/* 测试繁殖功能 */
TEST(RULE, Reproduction)
{
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);
}