#include "TestFramework.h"
#include "Renderer.h"
#include "Board.h"

#include <sstream>
#include <streambuf>

static std::streambuf* redirectCout(std::ostringstream& oss) {
    return std::cout.rdbuf(oss.rdbuf());
}

static void restoreCout(std::streambuf* old) {
    std::cout.rdbuf(old);
}

void testRenderer() {
    {
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderError("测试错误");
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("[错误]") != std::string::npos);
        ASSERT_TRUE(out.find("测试错误") != std::string::npos);
    }

    {
        Board b(9);
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderBoard(b);
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find('1') != std::string::npos);
        ASSERT_TRUE(out.find('+') != std::string::npos);
    }

    {
        Board b(9);
        b.placeStone({5, 5}, Player::Black);
        b.placeStone({3, 7}, Player::White);
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderBoard(b);
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find('X') != std::string::npos);
        ASSERT_TRUE(out.find('O') != std::string::npos);
    }

    {
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderPrompt(Player::Black);
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("黑方") != std::string::npos);
    }

    {
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderPrompt(Player::White);
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("白方") != std::string::npos);
    }

    {
        WinningLine line{Direction::Horizontal, {7, 7}, 5, Player::Black};
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderResult(GameState::BlackWin, line);
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("黑方获胜") != std::string::npos);
        ASSERT_TRUE(out.find("横向") != std::string::npos);
    }

    {
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderResult(GameState::Draw, std::nullopt);
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("平局") != std::string::npos);
    }

    {
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderWelcome();
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("五") != std::string::npos);
    }

    {
        Board b(19);
        std::ostringstream oss;
        std::streambuf* old = redirectCout(oss);
        Renderer::renderBoard(b);
        restoreCout(old);
        std::string out = oss.str();
        ASSERT_TRUE(out.find("19") != std::string::npos);
    }
}