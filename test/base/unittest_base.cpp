#include <gtest/gtest.h>

#include "base.h"

TEST(example, base) {
    ASSERT_NEAR(3.0, 3.0, 1.0e-11);
    DebugShow(DEBUG_LEVEL_NORMAL, "%s ok %u", "123", 321);
    DebugShow(DEBUG_LEVEL_PRINT, "%s ok %u", "123", 321);
    DebugShow(DEBUG_LEVEL_WARINIG, "%s ok %u", "123", 321);
    DebugShow(DEBUG_LEVEL_ERR, "%s ok %u", "123", 321);
}
