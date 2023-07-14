#include <gtest/gtest.h>

#include "base.h"

TEST(base, ini_cfg) {
    char *handleFile = NULL;
    char value[128] = {0};

    /* 异常入参用例 */
    EXPECT_NE(IniFileInit(NULL, &handleFile), SUCCESS);
    EXPECT_NE(IniFileInit("noexit.txt", &handleFile), SUCCESS);
    EXPECT_NE(IniFileInit("cfg.ini", NULL), SUCCESS);
    EXPECT_NE(IniFileRead(NULL, "map", "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileRead((const char *)4, NULL, "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileRead((const char *)4, "map", NULL, value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileRead((const char *)4, "map", "width", NULL, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileChange(NULL, "map", "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileChange(&handleFile, "map", "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileChange((char **)&value, NULL, "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileChange((char **)&value, "map", NULL, value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_NE(IniFileChange((char **)&value, "map", "width", NULL, ARRAYSIZE(value)), SUCCESS);

    /* 初始化句柄 */
    EXPECT_EQ(IniFileInit("../test/base/cfg.ini", &handleFile), SUCCESS);

    /* 读信息 */
    EXPECT_EQ(IniFileRead(handleFile, "subfile", "map", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, "map.ini");
    EXPECT_EQ(IniFileRead(handleFile, "map", "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, "10");
    EXPECT_EQ(IniFileRead(handleFile, "map", "high", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, "20");
    EXPECT_EQ(IniFileRead(handleFile, "subfile", "high", value, ARRAYSIZE(value)), SUCCESS); /* 测试是否会找到其他content的同名key */
    EXPECT_STREQ(value, "7");

    /* 修改成更长数据 */
    const char *longValue = "newmap.ini";
    EXPECT_EQ(IniFileChange(&handleFile, "subfile", "map", longValue, strlen(longValue) + 1), SUCCESS);
    EXPECT_EQ(IniFileRead(handleFile, "subfile", "map", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, longValue);
    EXPECT_EQ(IniFileRead(handleFile, "map", "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, "10");
    EXPECT_EQ(IniFileRead(handleFile, "map", "high", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, "20");
    EXPECT_EQ(IniFileRead(handleFile, "subfile", "high", value, ARRAYSIZE(value)), SUCCESS); /* 测试是否会找到其他content的同名key */
    EXPECT_STREQ(value, "7");

    /* 修改成更短数据 */
    const char *shortValue = "map.ini";
    EXPECT_EQ(IniFileChange(&handleFile, "subfile", "map", shortValue, strlen(longValue) + 1), SUCCESS);
    EXPECT_EQ(IniFileRead(handleFile, "subfile", "map", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, shortValue);
    EXPECT_EQ(IniFileRead(handleFile, "map", "width", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, "10");
    EXPECT_EQ(IniFileRead(handleFile, "map", "high", value, ARRAYSIZE(value)), SUCCESS);
    EXPECT_STREQ(value, "20");
    EXPECT_EQ(IniFileRead(handleFile, "subfile", "high", value, ARRAYSIZE(value)), SUCCESS); /* 测试是否会找到其他content的同名key */
    EXPECT_STREQ(value, "7");

    /* 找不到的用例 */
    EXPECT_NE(IniFileRead(handleFile, "subfile", "width", value, ARRAYSIZE(value)), SUCCESS); /* 测试是否会找到其他content的key */
    EXPECT_NE(IniFileRead(handleFile, "subfile", "hehe", value, ARRAYSIZE(value)), SUCCESS); /* 测试第一个content找不到报错 */
    EXPECT_NE(IniFileRead(handleFile, "map", "hehe", value, ARRAYSIZE(value)), SUCCESS); /* 测试最后一个content找不到报错 */
    EXPECT_NE(IniFileRead(handleFile, "hehe", "width", value, ARRAYSIZE(value)), SUCCESS); /* 测试content找不到报错 */
    EXPECT_NE(IniFileChange(&handleFile, "subfile", "width", value, ARRAYSIZE(value)), SUCCESS); /* 测试是否会找到其他content的key */
    EXPECT_NE(IniFileChange(&handleFile, "subfile", "hehe", value, ARRAYSIZE(value)), SUCCESS); /* 测试第一个content找不到报错 */
    EXPECT_NE(IniFileChange(&handleFile, "map", "hehe", value, ARRAYSIZE(value)), SUCCESS); /* 测试最后一个content找不到报错 */
    EXPECT_NE(IniFileChange(&handleFile, "hehe", "width", value, ARRAYSIZE(value)), SUCCESS); /* 测试content找不到报错 */

    /* 句柄销毁 */
    IniFileDestory(&handleFile);
    EXPECT_EQ(handleFile, (char *)NULL);
}