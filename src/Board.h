#pragma once

#include <vector>
#include "Types.h"

class Board {
public:
    explicit Board(int size);

    int size() const;
    CellState getCell(Position pos) const;
    void placeStone(Position pos, Player player);
    void clearStone(Position pos);
    bool isEmpty(Position pos) const;
    bool isFull() const;
    void reset();
    bool isValidPosition(Position pos) const;

private:
    int size_;
    std::vector<std::vector<CellState>> grid_;

    static void validateSize(int size);
    CellState& cellRef(Position pos);
    const CellState& cellRef(Position pos) const;
};