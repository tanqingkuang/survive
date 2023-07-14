#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "map.h"

static void UnittestMapResourceNum(uint32 dest)
{
    uint32 sum = 0;
    for (uint32 y = 0; y < MapInfoGet(MAG_INI_INFO_HIGHT); y++) {
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
    EXPECT_NE(MapCreate("../test/map/cfg.ini"), SUCCESS); /* 第二次创建失败 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_WIDTH), 10);
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_HIGHT), 20);
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_RESOURCENUM), 50);
    UnittestMapResourceNum(MapInfoGet(MAG_INI_INFO_RESOURCENUM));

    /* 资源刷新 */
    EXPECT_EQ(MapResourceReset(), SUCCESS);
    UnittestMapResourceNum(MapInfoGet(MAG_INI_INFO_RESOURCENUM));

    /* 销毁用例 */
    MapDestory();
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_WIDTH), 0); /* 创建后数据清零 */
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_HIGHT), 0); /* 创建后数据清零 */
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_RESOURCENUM), 0); /* 创建后数据清零 */

    /* 销毁后报错 */
    EXPECT_NE(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_NE(MapResourceReset(), SUCCESS);
}

uint32 UnitTestAnimalResourceAdd(float resource)
{
    return SUCCESS;
}

static bool check_func(float resource)
{
    //EXPECT_NEAR(3.0, resource, 0.001);
    return 1;
}

/* 模拟一个生物的消耗过程 */
TEST(MAP, consume) {
    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
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
    EXPECT_EQ(MapRefresh(), 0);
    EXPECT_EQ(MapRefresh(), 0); /* 第二次刷新不会有回调 */
    GlobalMockObject::verify();

    /* 检测资源被消耗掉，生物能量增加 */
    uint32 resourceSize = 0;
    EXPECT_EQ(MapResourceInfoGet(&point, &resourceSize), SUCCESS);
    EXPECT_EQ(resourceSize, 0);

    MapDestory();
}

//TEST(MAP, conflict) {
//    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
//
//    /* 测试思路：2个生物同时抢占，看最后资源分配和消耗时候合理
//       这里要测试如下几种场景
//       1、分享 or 竞争
//       2、竞争时：赢者通吃 or 资源分配，由于这两个是通过配置文件修改的，所以需要有修改配置文件的能力
//     */
//
//    MapDestory();
//}