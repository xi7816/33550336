#include "TestFramework.h"
#include "Board.h"

#include <stdexcept>

void testBoard() {
    {
        bool threw = false;
        try { Board b(4); } catch (const std::invalid_argument&) { threw = true; }
        ASSERT_TRUE(threw);
    }
    {
        bool threw = false;
        try { Board b(20); } catch (const std::invalid_argument&) { threw = true; }
        ASSERT_TRUE(threw);
    }
    {
        bool threw = false;
        try { Board b(10); } catch (const std::invalid_argument&) { threw = true; }
        ASSERT_TRUE(threw);
    }

    Board b5(5);
    ASSERT_EQ(b5.size(), 5);
    Board b9(9);
    ASSERT_EQ(b9.size(), 9);
    Board b15(15);
    ASSERT_EQ(b15.size(), 15);
    Board b19(19);
    ASSERT_EQ(b19.size(), 19);

    ASSERT_TRUE(b15.isValidPosition({1, 1}));
    ASSERT_TRUE(b15.isValidPosition({15, 15}));
    ASSERT_FALSE(b15.isValidPosition({0, 1}));
    ASSERT_FALSE(b15.isValidPosition({1, 0}));
    ASSERT_FALSE(b15.isValidPosition({16, 1}));
    ASSERT_FALSE(b15.isValidPosition({1, 16}));

    ASSERT_TRUE(b15.isEmpty({8, 8}));
    b15.placeStone({8, 8}, Player::Black);
    ASSERT_FALSE(b15.isEmpty({8, 8}));
    ASSERT_EQ(b15.getCell({8, 8}), CellState::Black);
    b15.placeStone({1, 1}, Player::White);
    ASSERT_EQ(b15.getCell({1, 1}), CellState::White);

    b15.clearStone({8, 8});
    ASSERT_TRUE(b15.isEmpty({8, 8}));
    ASSERT_EQ(b15.getCell({8, 8}), CellState::Empty);

    ASSERT_FALSE(b15.isFull());
    b15.reset();
    ASSERT_FALSE(b15.isFull());
    ASSERT_TRUE(b15.isEmpty({1, 1}));

    Board b5full(5);
    for (int r = 1; r <= 5; ++r) {
        for (int c = 1; c <= 5; ++c) {
            b5full.placeStone({r, c}, Player::Black);
        }
    }
    ASSERT_TRUE(b5full.isFull());
    b5full.reset();
    ASSERT_FALSE(b5full.isFull());
}