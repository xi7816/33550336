#include "Board.h"

#include <stdexcept>

void Board::validateSize(int size) {
    if (size < 5 || size > 19) {
        throw std::invalid_argument("棋盘尺寸须为 5 至 19 之间的奇数");
    }
    if (size % 2 == 0) {
        throw std::invalid_argument("棋盘尺寸须为奇数");
    }
}

Board::Board(int size) : size_(size) {
    validateSize(size);
    grid_.assign(size_, std::vector<CellState>(size_, CellState::Empty));
}

int Board::size() const {
    return size_;
}

CellState& Board::cellRef(Position pos) {
    return grid_[pos.row - 1][pos.col - 1];
}

const CellState& Board::cellRef(Position pos) const {
    return grid_[pos.row - 1][pos.col - 1];
}

CellState Board::getCell(Position pos) const {
    return cellRef(pos);
}

void Board::placeStone(Position pos, Player player) {
    cellRef(pos) = (player == Player::Black) ? CellState::Black : CellState::White;
}

void Board::clearStone(Position pos) {
    cellRef(pos) = CellState::Empty;
}

bool Board::isEmpty(Position pos) const {
    return cellRef(pos) == CellState::Empty;
}

bool Board::isFull() const {
    for (int r = 0; r < size_; ++r) {
        for (int c = 0; c < size_; ++c) {
            if (grid_[r][c] == CellState::Empty) {
                return false;
            }
        }
    }
    return true;
}

void Board::reset() {
    for (int r = 0; r < size_; ++r) {
        for (int c = 0; c < size_; ++c) {
            grid_[r][c] = CellState::Empty;
        }
    }
}

bool Board::isValidPosition(Position pos) const {
    return pos.row >= 1 && pos.row <= size_ && pos.col >= 1 && pos.col <= size_;
}