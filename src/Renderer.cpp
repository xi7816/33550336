#include "Renderer.h"

#include <iostream>
#include <iomanip>
#include <sstream>

char Renderer::stoneToChar(CellState cell) {
    switch (cell) {
        case CellState::Black: return 'X';
        case CellState::White: return 'O';
        case CellState::Empty: return '+';
    }
    return '+';
}

const char* Renderer::playerName(Player player) {
    return (player == Player::Black) ? "黑方" : "白方";
}

const char* Renderer::directionName(Direction direction) {
    switch (direction) {
        case Direction::Horizontal:   return "横向";
        case Direction::Vertical:     return "纵向";
        case Direction::LeftDiagonal: return "左斜(左上至右下)";
        case Direction::RightDiagonal:return "右斜(右上至左下)";
    }
    return "未知";
}

void Renderer::renderWelcome() {
    std::cout << "==============================\n";
    std::cout << "       五 子 棋  游 戏\n";
    std::cout << "==============================\n";
    std::cout << "规则说明：\n";
    std::cout << "  - 双方轮流落子，黑方(X)先手，白方(O)后手\n";
    std::cout << "  - 率先在横、竖、斜任一方向形成五子连珠者获胜\n";
    std::cout << "  - 棋盘尺寸为 5 至 19 之间的奇数，默认 15\n";
    std::cout << "操作说明：\n";
    std::cout << "  - 落子：输入 \"行 列\" 两个整数（空格分隔），如 \"7 8\"\n";
    std::cout << "  - 悔棋：输入 \"undo\"\n";
    std::cout << "  - 退出：输入 \"quit\"\n";
    std::cout << "------------------------------\n";
}

std::vector<std::string> Renderer::buildBoardLines(const Board& board) {
    int n = board.size();
    int width = (n >= 10) ? 3 : 2;
    std::vector<std::string> lines;
    lines.reserve(n + 1);

    std::ostringstream header;
    header << std::string(width, ' ');
    for (int c = 1; c <= n; ++c) {
        header << std::setw(width) << c;
    }
    lines.push_back(header.str());

    for (int r = 1; r <= n; ++r) {
        std::ostringstream row;
        row << std::setw(width) << r;
        for (int c = 1; c <= n; ++c) {
            row << std::setw(width) << stoneToChar(board.getCell({r, c}));
        }
        lines.push_back(row.str());
    }
    return lines;
}

void Renderer::renderBoard(const Board& board) {
    auto lines = buildBoardLines(board);
    for (const auto& line : lines) {
        std::cout << line << '\n';
    }
}

void Renderer::renderPrompt(Player current) {
    std::cout << "请" << playerName(current) << "落子（输入 行 列）：";
    std::cout.flush();
}

void Renderer::renderResult(GameState state, const std::optional<WinningLine>& line) {
    std::cout << "------------------------------\n";
    switch (state) {
        case GameState::BlackWin:
            std::cout << "黑方获胜！\n";
            break;
        case GameState::WhiteWin:
            std::cout << "白方获胜！\n";
            break;
        case GameState::Draw:
            std::cout << "本局平局！\n";
            break;
        default:
            break;
    }
    if (line.has_value()) {
        const WinningLine& w = line.value();
        std::cout << "获胜连珠：" << directionName(w.direction)
                  << "，起点 (" << w.start.row << ", " << w.start.col
                  << ")，长度 " << w.length << "\n";
    }
    std::cout << "------------------------------\n";
}

void Renderer::renderError(const std::string& msg) {
    std::cout << "[错误] " << msg << "\n";
}