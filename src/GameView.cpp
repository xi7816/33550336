#include "GameView.h"
#include "Renderer.h"
#include "PotRenderer.h"
#include "PotActivityResolver.h"
#include "LayoutComposer.h"
#include "PotStyle.h"
#include "Log.h"

#include <iostream>
#include <string>

void GameView::renderComposite(const Board& board, Player current, GameState state) {
    int n = board.size();
    int colWidth = (n >= 10) ? 3 : 2;
    int boardDisplayWidth = colWidth * (n + 1);
    int boardRows = n + 1;

    auto boardLines = Renderer::buildBoardLines(board);
    if (boardLines.empty()) {
        LOG("GameView: buildBoardLines返回空，中间区域以空行填充");
        boardLines.assign(boardRows, std::string(boardDisplayWidth, ' '));
    }

    PotSize potSize = LayoutComposer::fitSize(boardRows, boardDisplayWidth, PotStyle::kDefaultTermWidth);

    PotActivity blackAct = PotActivityResolver::resolve(Player::Black, current, state);
    PotActivity whiteAct = PotActivityResolver::resolve(Player::White, current, state);

    auto leftPot  = PotRenderer::buildPot(Player::Black, blackAct, potSize.height, potSize.width);
    auto rightPot = PotRenderer::buildPot(Player::White, whiteAct, potSize.height, potSize.width);

    auto composite = LayoutComposer::compose(boardLines, leftPot, rightPot);

    for (const auto& line : composite) {
        std::cout << line << '\n';
    }
    std::cout << "当前执子：" << PotActivityResolver::echoText(current) << "\n";
}