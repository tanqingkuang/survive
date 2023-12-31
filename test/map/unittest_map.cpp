#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "map.h"
#include "rule.h"

static void UnittestMapResourceNum(uint32 dest)
{
    uint32 sum = 0;
    for (uint32 y = 0; y < MapInfoGet(MAP_INI_INFO_HIGHT); y++) {
        for (uint32 x = 0; x < MapInfoGet(MAP_INI_INFO_WIDTH); x++) {
            MAP_POINT_S point = {x, y};
            uint32 resourceSize = 0;
            EXPECT_EQ(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
            sum += resourceSize;
        }
    }
    EXPECT_EQ(sum, dest);
}

uint32 RuleResouceAllocate(uint32 resourceMap, MAP_RUN_INFO_TAKE_ANIMAL_S *head)
{
    return SUCCESS;
}

TEST(MAP, CreateDestory)
{
    MAP_POINT_S point = {0};
    uint32 resourceSize = 0;

    /* 异常用例 */
    EXPECT_NE(MapCreate(NULL), SUCCESS);
    EXPECT_NE(MapCreate("noexit.ini"), SUCCESS);
    EXPECT_NE(MapResourceInfoGet(NULL, &resourceSize), SUCCESS);
    EXPECT_NE(MapResourceInfoGet(&point, NULL), SUCCESS);

    /* 未创建时报错 */
    EXPECT_NE(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_NE(MapResourceReset(), SUCCESS);

    /* 正常创建用例 */
    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
    EXPECT_NE(MapCreate("../test/map/cfg.ini"), SUCCESS); /* 第二次创建失败 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_WIDTH), 10);
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_HIGHT), 20);
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_RESOURCENUM), 50);
    UnittestMapResourceNum(MapInfoGet(MAP_INI_INFO_RESOURCENUM));

    /* 资源刷新 */
    EXPECT_EQ(MapResourceReset(), SUCCESS);
    UnittestMapResourceNum(MapInfoGet(MAP_INI_INFO_RESOURCENUM));

    /* 销毁用例 */
    MapDestory();
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_WIDTH), 0);       /* 创建后数据清零 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_HIGHT), 0);       /* 创建后数据清零 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_RESOURCENUM), 0); /* 创建后数据清零 */

    /* 销毁后报错 */
    EXPECT_NE(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_NE(MapResourceReset(), SUCCESS);
}

/* 模拟一个生物的消耗过程 */
TEST(MAP, consume)
{
    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
    const uint32 resource = 3; /* 设定的map上的资源 */
    MAP_POINT_S point = {2, 8};
    float size = 4;
    MAP_RESCOURCE_TAKE_INFO_S info = {point, {0, &size, MAP_RESOURCE_TAKE_SHARE}};

    /* 错误入参 */
    EXPECT_NE(MapResourceTake(NULL), SUCCESS);
    size = 0;
    EXPECT_NE(MapResourceTake(&info), SUCCESS);
    size = 4;

    /* 先预设一个资源 */
    EXPECT_EQ(MapResourceInfoSet(&point, resource), SUCCESS);

    /* 占用并更新数据 */
    EXPECT_EQ(MapResourceTake(&info), SUCCESS);
    EXPECT_NE(MapResourceTake(&info), SUCCESS); /* 同一个动物重复占用会报错 */

    EXPECT_EQ(MapRefresh(), SUCCESS);
    EXPECT_EQ(MapRefresh(), SUCCESS); /* 第二次刷新不会有回调 */

    /* 检测资源被消耗掉 */
    uint32 resourceSize = 0;
    EXPECT_EQ(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_EQ(resourceSize, 0);

    MapDestory();
}

/* 模拟竞争场景下的资源分配 */
TEST(MAP, conflictShare)
{
    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);

    const uint32 resource = 3; /* 设定的map上的资源 */
    MAP_POINT_S point = {2, 8};
    float size0 = 4, size1 = 5;
    MAP_RESCOURCE_TAKE_INFO_S info0 = {point, {0, &size0, MAP_RESOURCE_TAKE_SHARE}};
    MAP_RESCOURCE_TAKE_INFO_S info1 = {point, {1, &size1, MAP_RESOURCE_TAKE_CONFLICT}};

    /* 先预设一个资源 */
    EXPECT_EQ(MapResourceInfoSet(&point, resource), SUCCESS);

    /* 占用并更新数据 */
    EXPECT_EQ(MapResourceTake(&info0), SUCCESS);
    EXPECT_EQ(MapResourceTake(&info1), SUCCESS);
    EXPECT_EQ(MapRefresh(), SUCCESS);

    /* 检测资源被消耗掉 */
    uint32 resourceSize = 0;
    EXPECT_EQ(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_EQ(resourceSize, 0);

    MapDestory();
}
