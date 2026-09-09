#include "TestFramework.h"
#include "GameView.h"
#include "Board.h"

#include <sstream>
#include <streambuf>
#include <string>

void testGameView() {
    {
        Board b(9);
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        GameView::renderComposite(b, Player::Black, GameState::InProgress);
        std::cout.rdbuf(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find(std::string("\xE9\xBB\x91")) != std::string::npos);
        ASSERT_TRUE(out.find(std::string("\xE7\x99\xBD")) != std::string::npos);
        ASSERT_TRUE(out.find("当前执子") != std::string::npos);
        ASSERT_TRUE(out.find("黑方") != std::string::npos);
    }

    {
        Board b(9);
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        GameView::renderComposite(b, Player::White, GameState::InProgress);
        std::cout.rdbuf(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("白方") != std::string::npos);
    }

    {
        Board b(9);
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        GameView::renderComposite(b, Player::Black, GameState::BlackWin);
        std::cout.rdbuf(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("黑方") != std::string::npos);
    }

    {
        Board b(19);
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        GameView::renderComposite(b, Player::Black, GameState::InProgress);
        std::cout.rdbuf(old);
        std::string out = oss.str();
        ASSERT_TRUE(!out.empty());
        ASSERT_TRUE(out.find("当前执子") != std::string::npos);
    }

    {
        Board b(5);
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        GameView::renderComposite(b, Player::Black, GameState::InProgress);
        std::cout.rdbuf(old);
        ASSERT_TRUE(!oss.str().empty());
    }

    {
        Board b(15);
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        GameView::renderComposite(b, Player::White, GameState::InProgress);
        std::cout.rdbuf(old);
        ASSERT_TRUE(!oss.str().empty());
    }
}