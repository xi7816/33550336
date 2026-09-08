#pragma once

#include <string>
#include <optional>
#include "Types.h"
#include "Board.h"

class Renderer {
public:
    static void renderWelcome();
    static void renderBoard(const Board& board);
    static void renderPrompt(Player current);
    static void renderResult(GameState state, const std::optional<WinningLine>& line);
    static void renderError(const std::string& msg);

private:
    static char stoneToChar(CellState cell);
    static const char* playerName(Player player);
    static const char* directionName(Direction direction);
};