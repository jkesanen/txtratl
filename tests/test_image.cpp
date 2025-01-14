#include <gtest/gtest.h>

#include "txtratl/image.hpp"

using namespace txtratl;

TEST(Image, outOfBounds)
{
    auto image = Image(1, 1, 4);
    EXPECT_NO_THROW(image.data(0, 0, 2));
    EXPECT_THROW(image.data(2, 1, 1), std::out_of_range);
}
