#include <gtest/gtest.h>

#include "base.h"

TEST(base, debug) {
    DebugShow(DEBUG_LEVEL_NORMAL, "%s ok %u", "123", 321);
    DebugShow(DEBUG_LEVEL_PRINT, "%s ok %u", "123", 321);
    DebugShow(DEBUG_LEVEL_WARINIG, "%s ok %u", "123", 321);
    DebugShow(DEBUG_LEVEL_ERR, "%s ok %u", "123", 321);
}

TEST(base, ini_cfg) {
    char *handleFile = NULL;
    char value[128] = {0};

    const char *destStr = "[subfile]\nmap = map.ini\nhigh = 7\n\n[map]\nwidth = 10\nhigh = 20\n";

    /* 异常用例 */
    EXPECT_NE(IniFileInit(NULL, &handleFile), SUCCESS);
    EXPECT_NE(IniFileInit("noexit.txt", &handleFile), SUCCESS);
    EXPECT_NE(IniFileInit("cfg.ini", NULL), SUCCESS);
    EXPECT_NE(IniFileRead(NULL, "map", "width", value, 128), SUCCESS);
    EXPECT_NE(IniFileRead((const char *)4, NULL, "width", value, 128), SUCCESS);
    EXPECT_NE(IniFileRead((const char *)4, "map", NULL, value, 128), SUCCESS);
    EXPECT_NE(IniFileRead((const char *)4, "map", "width", NULL, 128), SUCCESS);

    /* 正常用例 */
    EXPECT_EQ(IniFileInit("../test/base/cfg.ini", &handleFile), SUCCESS);
    EXPECT_EQ(strcmp(handleFile, destStr), 0);

    EXPECT_EQ(IniFileRead(handleFile, "subfile", "map", value, 128), SUCCESS);
    EXPECT_EQ(strcmp(value, "map.ini"), 0);
    EXPECT_EQ(IniFileRead(handleFile, "map", "width", value, 128), SUCCESS);
    EXPECT_EQ(strcmp(value, "10"), 0);
    EXPECT_EQ(IniFileRead(handleFile, "map", "high", value, 128), SUCCESS);
    EXPECT_EQ(strcmp(value, "20"), 0);
    EXPECT_EQ(IniFileRead(handleFile, "subfile", "high", value, 128), SUCCESS); /* 测试是否会找到其他content的同名key */
    EXPECT_EQ(strcmp(value, "7"), 0);

    { /* 找不到的用例 */
        EXPECT_NE(IniFileRead(handleFile, "subfile", "width", value, 128), SUCCESS); /* 测试是否会找到其他content的key */
        EXPECT_NE(IniFileRead(handleFile, "subfile", "hehe", value, 128), SUCCESS); /* 测试第一个content找不到报错 */
        EXPECT_NE(IniFileRead(handleFile, "map", "hehe", value, 128), SUCCESS); /* 测试最后一个content找不到报错 */
        EXPECT_NE(IniFileRead(handleFile, "hehe", "width", value, 128), SUCCESS); /* 测试content找不到报错 */
    }

    IniFileDestory(&handleFile);
    EXPECT_EQ(handleFile, (char *)NULL);
}