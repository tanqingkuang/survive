#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "map.h"
#include "rule.h"

static void UnittestMapResourceNum(uint32 dest)
{
    uint32 sum = 0;
    for (uint32 y = 0; y < MapInfoGet(MAP_INI_INFO_HIGHT); y++) {
        for (uint32 x = 0; x < MapInfoGet(MAP_INI_INFO_WIDTH); x++) {
            MAP_POINT_S point = {x,y};
            uint32 resourceSize = 0;
            EXPECT_EQ(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
            sum += resourceSize;
        }
    }
    EXPECT_EQ(sum, dest);
}

TEST(MAP, CreateDestory) {
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
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);
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
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_WIDTH), 0); /* 创建后数据清零 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_HIGHT), 0); /* 创建后数据清零 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_RESOURCENUM), 0); /* 创建后数据清零 */

    /* 销毁后报错 */
    EXPECT_NE(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_NE(MapResourceReset(), SUCCESS);
}

static uint32 gResourceIdx = 0;
static float gResourceDest[10] = {0};

static uint32 UnitTestAnimalResourceAdd(float resource)
{
    EXPECT_NEAR(resource, gResourceDest[gResourceIdx], 0.001);
    gResourceIdx++;
    return SUCCESS;
}

/* 模拟一个生物的消耗过程 */
TEST(MAP, consume) {
    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);
    const uint32 resource = 3; /* 设定的map上的资源 */
    MAP_POINT_S point = {2, 8};
    MAP_RESCOURCE_TAKE_INFO_S info = {point, {0, 4, MAP_RESOURCE_TAKE_SHARE, UnitTestAnimalResourceAdd}};

    /* 错误入参 */
    EXPECT_NE(MapResourceTake(NULL), SUCCESS);
    info.animal.pfunc = NULL;
    EXPECT_NE(MapResourceTake(&info), SUCCESS);
    info.animal.pfunc = UnitTestAnimalResourceAdd;
    info.animal.size = 0;
    EXPECT_NE(MapResourceTake(&info), SUCCESS);
    info.animal.size = 4;

    /* 先预设一个资源 */
    EXPECT_EQ(MapResourceInfoSet(&point, resource), SUCCESS);

    /* 占用并更新数据 */
    EXPECT_EQ(MapResourceTake(&info), SUCCESS);
    EXPECT_NE(MapResourceTake(&info), SUCCESS); /* 同一个动物重复占用会报错 */
    MOCKER(UnitTestAnimalResourceAdd) /* 检测回调函数：入参为3，只会调用一次 */
        .expects(once())
        .with(eq(3.0f))
        .will(returnValue((uint32)0));
    EXPECT_EQ(MapRefresh(), SUCCESS);
    EXPECT_EQ(MapRefresh(), SUCCESS); /* 第二次刷新不会有回调 */
    GlobalMockObject::verify();

    /* 检测资源被消耗掉 */
    uint32 resourceSize = 0;
    EXPECT_EQ(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_EQ(resourceSize, 0);

    MapDestory();
}

/* 模拟竞争场景下的资源分配 */
TEST(MAP, conflictShare) {
    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);
    const uint32 resource = 3; /* 设定的map上的资源 */
    MAP_POINT_S point = {2, 8};
    MAP_RESCOURCE_TAKE_INFO_S info0 = {point, {0, 4, MAP_RESOURCE_TAKE_SHARE, UnitTestAnimalResourceAdd}};
    MAP_RESCOURCE_TAKE_INFO_S info1 = {point, {1, 5, MAP_RESOURCE_TAKE_CONFLICT, UnitTestAnimalResourceAdd}};

    /* 先预设一个资源 */
    EXPECT_EQ(MapResourceInfoSet(&point, resource), SUCCESS);

    /* 占用并更新数据 */
    EXPECT_EQ(MapResourceTake(&info0), SUCCESS);
    EXPECT_EQ(MapResourceTake(&info1), SUCCESS);
    gResourceDest[0] = 1.7565215826034546;
    gResourceDest[1] = 0.8434782624244689;
    gResourceIdx = 0;
    EXPECT_EQ(MapRefresh(), SUCCESS);
    GlobalMockObject::verify();

    /* 检测资源被消耗掉 */
    uint32 resourceSize = 0;
    EXPECT_EQ(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_EQ(resourceSize, 0);

    MapDestory();
}

/* 模拟竞争场景下的赢者通吃,这个对后面模拟意义不大，当前先保留 */
#if 0
TEST(MAP, conflictOne) {
    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
    EXPECT_EQ(RuleCreate("../test/map/cfg.ini"), SUCCESS);
    MapDestory();
}
#endif