#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

void DebugShowPRINT(const char* str);

#ifdef __cplusplus
}
#endif

static const char *gDestChar = NULL;
static bool check_func(const char* str)
{
    EXPECT_STREQ(gDestChar, str);
    return 1;
}

TEST(base, debug) {
    /* 测试normal不会打日志 */
    MOCKER(DebugShowPRINT)
        .expects(never());
    DebugShow(DEBUG_LEVEL_NORMAL, "%s ok %u", "123", 321);
    GlobalMockObject::verify();

    /* 测试要打印的数据 */
    MOCKER(DebugShowPRINT)
        .stubs()
        .with(checkWith(check_func));

    gDestChar = "LOG:     123 ok 321\r\n\033[0m";
    DebugShow(DEBUG_LEVEL_PRINT, "%s ok %u", "123", 321);

    gDestChar = "WARNING: 123 ok 321\r\n\033[0m";
    DebugShow(DEBUG_LEVEL_WARINIG, "%s ok %u", "123", 321);

    gDestChar = "\033[1;31mERROR:   123 ok 321\r\n\033[0m";
    DebugShow(DEBUG_LEVEL_ERR, "%s ok %u", "123", 321);

    GlobalMockObject::verify();
}
