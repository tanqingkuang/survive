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
}

/* 测试繁殖功能 */
TEST(RULE, Reproduction)
{
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);
}