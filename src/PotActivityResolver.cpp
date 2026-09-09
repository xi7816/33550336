#include "PotActivityResolver.h"
#include "Log.h"

#include <string>

PotActivity PotActivityResolver::resolve(Player owner, Player current, GameState state) {
    switch (state) {
        case GameState::InProgress:
            return (owner == current) ? PotActivity::Active : PotActivity::Idle;
        case GameState::BlackWin:
            return (owner == Player::Black) ? PotActivity::Active : PotActivity::Idle;
        case GameState::WhiteWin:
            return (owner == Player::White) ? PotActivity::Active : PotActivity::Idle;
        case GameState::Draw:
            return (owner == current) ? PotActivity::Active : PotActivity::Idle;
        default:
            LOG("PotActivityResolver: 非法对局状态，棋罐置沉寂");
            return PotActivity::Idle;
    }
}

const char* PotActivityResolver::echoText(Player current) {
    switch (current) {
        case Player::Black: return "黑方";
        case Player::White: return "白方";
        default:           return "未知";
    }
}