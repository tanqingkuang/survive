#include <gtest/gtest.h>

#include "base.h"

TEST(example, debug) {
    ASSERT_NEAR(3.0, 3.0, 1.0e-11);
    debug_show(DEBUG_LEVEL_ERR, "%s ok %u", "123", 321);
}
