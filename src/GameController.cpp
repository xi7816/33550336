#include "GameController.h"

#include <iostream>
#include <sstream>
#include <string>
#include "Renderer.h"
#include "InputReader.h"
#include "Judge.h"
#include "Log.h"
#include "BgmPlayer.h"
#include "GameView.h"

GameController::GameController()
    : board_(15), currentPlayer_(Player::Black), state_(GameState::InProgress) {}

Player GameController::opponent(Player p) {
    return (p == Player::Black) ? Player::White : Player::Black;
}

void GameController::initGame() {
    Renderer::renderWelcome();

    int size = 0;
    while (true) {
        std::cout << "请输入棋盘尺寸（5-19 的奇数，默认 15，直接回车采用默认）：";
        std::cout.flush();
        std::string line = InputReader::readLine();

        if (std::cin.eof()) {
            size = 15;
            break;
        }
        if (line.empty()) {
            size = 15;
            break;
        }

        std::istringstream iss(line);
        int value;
        char leftover;
        if (!(iss >> value) || (iss >> leftover)) {
            Renderer::renderError("输入格式错误，请输入一个整数");
            continue;
        }
        if (value < 5 || value > 19 || value % 2 == 0) {
            Renderer::renderError("棋盘尺寸须为 5 至 19 之间的奇数，请重新输入");
            continue;
        }
        size = value;
        break;
    }

    board_ = Board(size);
    currentPlayer_ = Player::Black;
    state_ = GameState::InProgress;
    history_.clear();
    winningLine_.reset();

    LOG("初始化棋盘 " + std::to_string(size) + "x" + std::to_string(size) + "，黑方先手");
    GameView::renderComposite(board_, currentPlayer_, state_);
    bgmPlayer_.start();
}

void GameController::processMove(Position pos) {
    if (!board_.isValidPosition(pos)) {
        Renderer::renderError("坐标越界，有效范围为 1 至 " + std::to_string(board_.size()) + "，请重新输入");
        return;
    }
    if (!board_.isEmpty(pos)) {
        Renderer::renderError("该位置已有棋子，请选择其他位置");
        return;
    }

    board_.placeStone(pos, currentPlayer_);
    history_.push_back(MoveRecord{pos, currentPlayer_});

    LOG("落子 (" + std::to_string(pos.row) + "," + std::to_string(pos.col) + ") by " +
        (currentPlayer_ == Player::Black ? "Black" : "White"));

    auto win = Judge::checkWin(board_, pos, currentPlayer_);
    if (win.has_value()) {
        winningLine_ = win;
        state_ = (currentPlayer_ == Player::Black) ? GameState::BlackWin : GameState::WhiteWin;
        LOG("胜负判定：获胜");
        GameView::renderComposite(board_, currentPlayer_, state_);
        return;
    }
    if (Judge::checkDraw(board_)) {
        state_ = GameState::Draw;
        LOG("胜负判定：平局");
        GameView::renderComposite(board_, currentPlayer_, state_);
        return;
    }
    currentPlayer_ = opponent(currentPlayer_);
    GameView::renderComposite(board_, currentPlayer_, state_);
}

void GameController::processUndo() {
    if (state_ != GameState::InProgress) {
        Renderer::renderError("对局已结束，如需继续请开始新局");
        return;
    }
    if (history_.empty()) {
        Renderer::renderError("当前无步可悔");
        return;
    }

    MoveRecord last = history_.back();
    history_.pop_back();
    board_.clearStone(last.pos);
    currentPlayer_ = last.player;
    GameView::renderComposite(board_, currentPlayer_, state_);

    LOG("悔棋至 (" + std::to_string(last.pos.row) + "," + std::to_string(last.pos.col) + ")，回合恢复为 " +
        (last.player == Player::Black ? "Black" : "White"));
}

void GameController::endGame(GameState state) {
    state_ = state;
    Renderer::renderResult(state_, winningLine_);
}

bool GameController::askRestart() {
    while (true) {
        std::cout << "是否开始新局（Y/N）？";
        std::cout.flush();
        std::string line = InputReader::readLine();

        if (line.empty() && std::cin.eof()) return false;
        if (line == "Y" || line == "y") return true;
        if (line == "N" || line == "n") return false;
        Renderer::renderError("请输入 Y 开始新局或 N 退出");
    }
}

void GameController::run() {
    initGame();

    while (state_ == GameState::InProgress) {
        Renderer::renderPrompt(currentPlayer_);
        std::string line = InputReader::readLine();

        if (std::cin.eof()) break;
        if (line == "quit") break;

        if (line == "undo") {
            processUndo();
            bgmPlayer_.pollNextTrack();
            continue;
        }

        if (line == "vol+") {
            bgmPlayer_.handleVolumeCommand(VolumeCommand::Increase);
            bgmPlayer_.pollNextTrack();
            continue;
        }
        if (line == "vol-") {
            bgmPlayer_.handleVolumeCommand(VolumeCommand::Decrease);
            bgmPlayer_.pollNextTrack();
            continue;
        }
        if (line == "mute") {
            bgmPlayer_.handleVolumeCommand(VolumeCommand::ToggleMute);
            bgmPlayer_.pollNextTrack();
            continue;
        }

        std::istringstream iss(line);
        int row, col;
        char leftover;
        if (iss >> row >> col && !(iss >> leftover)) {
            processMove(Position{row, col});
            bgmPlayer_.pollNextTrack();
            if (state_ != GameState::InProgress) {
                endGame(state_);
                if (askRestart()) {
                    initGame();
                } else {
                    break;
                }
            }
            continue;
        }

        Renderer::renderError("输入格式错误，请以\"行 列\"形式输入两个整数，或输入 undo 悔棋、vol+ 增大音量、vol- 减小音量、mute 静音切换、quit 退出");
        bgmPlayer_.pollNextTrack();
    }

    bgmPlayer_.stop();
    std::cout << "感谢使用，再见！\n";
}