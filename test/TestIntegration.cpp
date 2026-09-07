#include "TestFramework.h"
#include "Board.h"
#include "Judge.h"
#include "Renderer.h"
#include "InputReader.h"

#include <sstream>
#include <streambuf>

void testIntegration() {
    {
        Board b(9);
        ASSERT_EQ(b.size(), 9);
        for (int r = 1; r <= 9; ++r) {
            for (int c = 1; c <= 9; ++c) {
                ASSERT_TRUE(b.isEmpty({r, c}));
            }
        }
    }

    {
        Board b(15);
        b.placeStone({8, 8}, Player::Black);
        b.placeStone({1, 1}, Player::White);
        b.placeStone({8, 9}, Player::Black);
        b.placeStone({2, 2}, Player::White);
        b.placeStone({8, 10}, Player::Black);
        b.placeStone({3, 3}, Player::White);
        b.placeStone({8, 11}, Player::Black);
        b.placeStone({4, 4}, Player::White);
        b.placeStone({8, 12}, Player::Black);

        auto result = Judge::checkWin(b, {8, 12}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->direction, Direction::Horizontal);
        ASSERT_EQ(result->length, 5);
        ASSERT_EQ(result->player, Player::Black);
    }

    {
        Board b(15);
        b.placeStone({7, 7}, Player::Black);
        ASSERT_FALSE(b.isValidPosition({0, 0}));
        ASSERT_FALSE(b.isEmpty({7, 7}));

        b.placeStone({7, 8}, Player::White);
        b.clearStone({7, 8});
        ASSERT_TRUE(b.isEmpty({7, 8}));
        b.clearStone({7, 7});
        ASSERT_TRUE(b.isEmpty({7, 7}));
    }

    {
        Board b(5);
        for (int r = 1; r <= 5; ++r) {
            for (int c = 1; c <= 5; ++c) {
                Player p = ((r + c) % 2 == 0) ? Player::Black : Player::White;
                b.placeStone({r, c}, p);
            }
        }
        ASSERT_TRUE(b.isFull());

        bool anyWin = false;
        for (int r = 1; r <= 5 && !anyWin; ++r) {
            for (int c = 1; c <= 5 && !anyWin; ++c) {
                Player p = ((r + c) % 2 == 0) ? Player::Black : Player::White;
                auto res = Judge::checkWin(b, {r, c}, p);
                if (res.has_value()) anyWin = true;
            }
        }
        ASSERT_TRUE(Judge::checkDraw(b));
    }

    {
        Board b(15);
        std::vector<MoveRecord> history;

        b.placeStone({7, 7}, Player::Black);
        history.push_back({{7, 7}, Player::Black});
        b.placeStone({8, 8}, Player::White);
        history.push_back({{8, 8}, Player::White});
        b.placeStone({7, 8}, Player::Black);
        history.push_back({{7, 8}, Player::Black});

        ASSERT_EQ(history.size(), 3u);

        MoveRecord last = history.back();
        history.pop_back();
        b.clearStone(last.pos);
        ASSERT_TRUE(b.isEmpty({7, 8}));
        ASSERT_EQ(history.size(), 2u);

        last = history.back();
        history.pop_back();
        b.clearStone(last.pos);
        ASSERT_TRUE(b.isEmpty({8, 8}));
        ASSERT_EQ(history.size(), 1u);

        last = history.back();
        history.pop_back();
        b.clearStone(last.pos);
        ASSERT_TRUE(b.isEmpty({7, 7}));
        ASSERT_EQ(history.size(), 0u);
    }

    {
        Board b(15);
        b.placeStone({10, 10}, Player::Black);
        b.placeStone({10, 11}, Player::Black);
        b.placeStone({10, 12}, Player::Black);
        b.placeStone({10, 13}, Player::Black);
        b.placeStone({10, 14}, Player::Black);

        auto result = Judge::checkWin(b, {10, 12}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->start.row, 10);
        ASSERT_EQ(result->start.col, 10);
        ASSERT_EQ(result->length, 5);
    }

    {
        Board b(15);
        b.placeStone({3, 3}, Player::Black);
        b.placeStone({4, 4}, Player::Black);
        b.placeStone({5, 5}, Player::Black);
        b.placeStone({6, 6}, Player::Black);
        b.placeStone({7, 7}, Player::Black);

        auto result = Judge::checkWin(b, {5, 5}, Player::Black);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->direction, Direction::LeftDiagonal);
        ASSERT_EQ(result->start.row, 3);
        ASSERT_EQ(result->start.col, 3);
    }
}