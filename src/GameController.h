#pragma once

#include <vector>
#include <optional>
#include "Types.h"
#include "Board.h"

class GameController {
public:
    GameController();
    void run();

private:
    void initGame();
    void processMove(Position pos);
    void processUndo();
    bool askRestart();
    void endGame(GameState state);
    static Player opponent(Player p);

    Board board_;
    Player currentPlayer_;
    GameState state_;
    std::vector<MoveRecord> history_;
    std::optional<WinningLine> winningLine_;
};