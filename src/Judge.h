#pragma once

#include <optional>
#include "Types.h"
#include "Board.h"

class Judge {
public:
    static std::optional<WinningLine> checkWin(const Board& board, Position lastMove, Player player);
    static bool checkDraw(const Board& board);

private:
    static int countDirection(const Board& board, Position pos, Player player, int dr, int dc);
    static CellState playerToCell(Player player);
};