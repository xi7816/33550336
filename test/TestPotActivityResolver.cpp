#include "TestFramework.h"
#include "PotActivityResolver.h"

#include <string>

void testPotActivityResolver() {
    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::Black, GameState::InProgress), PotActivity::Active);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::Black, GameState::InProgress), PotActivity::Idle);
    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::White, GameState::InProgress), PotActivity::Idle);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::White, GameState::InProgress), PotActivity::Active);

    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::Black, GameState::BlackWin), PotActivity::Active);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::Black, GameState::BlackWin), PotActivity::Idle);
    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::White, GameState::BlackWin), PotActivity::Active);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::White, GameState::BlackWin), PotActivity::Idle);

    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::Black, GameState::WhiteWin), PotActivity::Idle);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::Black, GameState::WhiteWin), PotActivity::Active);
    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::White, GameState::WhiteWin), PotActivity::Idle);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::White, GameState::WhiteWin), PotActivity::Active);

    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::Black, GameState::Draw), PotActivity::Active);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::Black, GameState::Draw), PotActivity::Idle);
    ASSERT_EQ(PotActivityResolver::resolve(Player::Black, Player::White, GameState::Draw), PotActivity::Idle);
    ASSERT_EQ(PotActivityResolver::resolve(Player::White, Player::White, GameState::Draw), PotActivity::Active);

    ASSERT_TRUE(std::string(PotActivityResolver::echoText(Player::Black)) == "黑方");
    ASSERT_TRUE(std::string(PotActivityResolver::echoText(Player::White)) == "白方");
}