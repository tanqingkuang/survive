#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "map.h"

TEST(MAP, CreateDestory) {
    /* 异常用例 */
    EXPECT_NE(MapCreate(NULL), SUCCESS);
    EXPECT_NE(MapCreate("noexit.ini"), SUCCESS);

    EXPECT_EQ(MapCreate("../test/map/cfg.ini"), SUCCESS);
    EXPECT_NE(MapCreate("../test/map/cfg.ini"), SUCCESS); /* 第二次创建失败 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_WIDTH), 10);
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_HIGHT), 20);
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_RESOURCENUM), 50);
    MapDestory(); /* 创建后数据清零 */
    EXPECT_EQ(MapInfoGet(MAP_INI_INFO_WIDTH), 0);
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_HIGHT), 0);
    EXPECT_EQ(MapInfoGet(MAG_INI_INFO_RESOURCENUM), 0);
}