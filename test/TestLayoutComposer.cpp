#include "TestFramework.h"
#include "LayoutComposer.h"
#include "PotStyle.h"

#include <string>

void testLayoutComposer() {
    {
        auto sz = LayoutComposer::fitSize(16, 48, 80);
        ASSERT_TRUE(sz.height <= PotStyle::kPotTemplateRows);
        ASSERT_TRUE(sz.width <= PotStyle::kMaxPotWidth);
        ASSERT_TRUE(sz.width >= 6);
    }

    {
        auto sz = LayoutComposer::fitSize(16, 48, 40);
        ASSERT_TRUE(sz.width >= 6);
    }

    {
        std::vector<std::string> board = {"ABC", "DEF", "GHI"};
        std::vector<std::string> left  = {"L1", "L2", "L3"};
        std::vector<std::string> right = {"R1", "R2", "R3"};
        auto result = LayoutComposer::compose(board, left, right);
        ASSERT_EQ(result.size(), static_cast<size_t>(3));
        ASSERT_TRUE(result[0].find("L1") != std::string::npos);
        ASSERT_TRUE(result[0].find("ABC") != std::string::npos);
        ASSERT_TRUE(result[0].find("R1") != std::string::npos);
    }

    {
        std::vector<std::string> board = {"ABC", "DEF", "GHI"};
        std::vector<std::string> small = {"X"};
        auto result = LayoutComposer::compose(board, small, small);
        ASSERT_EQ(result.size(), static_cast<size_t>(3));
    }

    {
        std::vector<std::string> emptyBoard;
        std::vector<std::string> left  = {"L1", "L2", "L3"};
        std::vector<std::string> right = {"R1", "R2", "R3"};
        auto result = LayoutComposer::compose(emptyBoard, left, right);
        ASSERT_EQ(result.size(), static_cast<size_t>(3));
        ASSERT_TRUE(result[0].find("L1") != std::string::npos);
        ASSERT_TRUE(result[0].find("R1") != std::string::npos);
    }

    {
        std::vector<std::string> board = {"ABC", "DEF", "GHI"};
        std::vector<std::string> left  = {"L1", "L2", "L3"};
        std::vector<std::string> right = {"R1", "R2", "R3"};
        auto result = LayoutComposer::compose(board, left, right);
        ASSERT_TRUE(result[0].find("L1") < result[0].find("ABC"));
        ASSERT_TRUE(result[0].find("ABC") < result[0].find("R1"));
    }
}