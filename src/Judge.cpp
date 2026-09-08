#include "Judge.h"

CellState Judge::playerToCell(Player player) {
    return (player == Player::Black) ? CellState::Black : CellState::White;
}

int Judge::countDirection(const Board& board, Position pos, Player player, int dr, int dc) {
    CellState target = playerToCell(player);
    int count = 0;
    Position cur{pos.row + dr, pos.col + dc};
    while (board.isValidPosition(cur) && board.getCell(cur) == target) {
        ++count;
        cur.row += dr;
        cur.col += dc;
    }
    return count;
}

std::optional<WinningLine> Judge::checkWin(const Board& board, Position lastMove, Player player) {
    const int dirs[4][2] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1}
    };
    const Direction dirEnums[4] = {
        Direction::Horizontal,
        Direction::Vertical,
        Direction::LeftDiagonal,
        Direction::RightDiagonal
    };

    for (int i = 0; i < 4; ++i) {
        int dr = dirs[i][0];
        int dc = dirs[i][1];
        int forward = countDirection(board, lastMove, player, dr, dc);
        int backward = countDirection(board, lastMove, player, -dr, -dc);
        int total = 1 + forward + backward;

        if (total >= 5) {
            Position start{
                lastMove.row - dr * backward,
                lastMove.col - dc * backward
            };
            return WinningLine{dirEnums[i], start, total, player};
        }
    }
    return std::nullopt;
}

bool Judge::checkDraw(const Board& board) {
    return board.isFull();
}