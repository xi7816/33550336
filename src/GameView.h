#pragma once

#include "Types.h"
#include "Board.h"

class GameView {
public:
    static void renderComposite(const Board& board, Player current, GameState state);
};