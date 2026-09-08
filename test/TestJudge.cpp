#include "TestFramework.h"
#include "Judge.h"
#include "Board.h"

static void placeLine(Board& board, Player player, int row, int col, int dr, int dc, int count) {
    for (int i = 0; i < count; ++i) {
        board.placeStone({row + dr * i, col + dc * i}, player);
    }
}

void testJudge() {
    {
        Board b(15);
        placeLine(b, Player::Black, 7, 7, 0, 1, 5);
        auto result = Judge::checkWin(b, {7, 9}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->direction, Direction::Horizontal);
        ASSERT_EQ(result->length, 5);
        ASSERT_EQ(result->player, Player::Black);
        ASSERT_EQ(result->start.row, 7);
        ASSERT_EQ(result->start.col, 7);
    }

    {
        Board b(15);
        placeLine(b, Player::White, 5, 5, 1, 0, 5);
        auto result = Judge::checkWin(b, {7, 5}, Player::White);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->direction, Direction::Vertical);
        ASSERT_EQ(result->length, 5);
        ASSERT_EQ(result->player, Player::White);
    }

    {
        Board b(15);
        placeLine(b, Player::Black, 5, 5, 1, 1, 5);
        auto result = Judge::checkWin(b, {7, 7}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->direction, Direction::LeftDiagonal);
        ASSERT_EQ(result->length, 5);
    }

    {
        Board b(15);
        placeLine(b, Player::Black, 5, 9, 1, -1, 5);
        auto result = Judge::checkWin(b, {7, 7}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->direction, Direction::RightDiagonal);
        ASSERT_EQ(result->length, 5);
    }

    {
        Board b(15);
        placeLine(b, Player::Black, 7, 5, 0, 1, 6);
        auto result = Judge::checkWin(b, {7, 8}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->length, 6);
    }

    {
        Board b(15);
        placeLine(b, Player::Black, 7, 7, 0, 1, 4);
        auto result = Judge::checkWin(b, {7, 8}, Player::Black);
        ASSERT_FALSE(result.has_value());
    }

    {
        Board b(15);
        b.placeStone({7, 7}, Player::Black);
        auto result = Judge::checkWin(b, {7, 7}, Player::Black);
        ASSERT_FALSE(result.has_value());
    }

    {
        Board b(15);
        ASSERT_FALSE(Judge::checkDraw(b));
    }

    {
        Board b(5);
        for (int r = 1; r <= 5; ++r) {
            for (int c = 1; c <= 5; ++c) {
                b.placeStone({r, c}, Player::Black);
            }
        }
        ASSERT_TRUE(Judge::checkDraw(b));
    }

    {
        Board b(15);
        b.placeStone({7, 7}, Player::Black);
        b.placeStone({7, 8}, Player::White);
        ASSERT_FALSE(Judge::checkDraw(b));
    }

    {
        Board b(15);
        placeLine(b, Player::Black, 1, 1, 0, 1, 5);
        auto result = Judge::checkWin(b, {1, 3}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->start.row, 1);
        ASSERT_EQ(result->start.col, 1);
    }
}