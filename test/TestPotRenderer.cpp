#include "TestFramework.h"
#include "PotRenderer.h"
#include "PotStyle.h"

#include <string>

void testPotRenderer() {
    {
        auto pot = PotRenderer::buildPot(Player::Black, PotActivity::Active, 7, 12);
        ASSERT_EQ(pot.size(), static_cast<size_t>(7));
        for (const auto& line : pot) {
            ASSERT_EQ(PotStyle::displayWidth(line), 12);
        }
    }

    {
        auto pot = PotRenderer::buildPot(Player::Black, PotActivity::Active, 7, 12);
        bool hasLabel = false;
        for (const auto& line : pot) {
            if (line.find(std::string(PotStyle::kBlackLabel)) != std::string::npos) hasLabel = true;
        }
        ASSERT_TRUE(hasLabel);
    }

    {
        auto pot = PotRenderer::buildPot(Player::White, PotActivity::Active, 7, 12);
        bool hasLabel = false;
        for (const auto& line : pot) {
            if (line.find(std::string(PotStyle::kWhiteLabel)) != std::string::npos) hasLabel = true;
        }
        ASSERT_TRUE(hasLabel);
    }

    {
        auto active = PotRenderer::buildPot(Player::Black, PotActivity::Active, 5, 10);
        auto idle   = PotRenderer::buildPot(Player::Black, PotActivity::Idle, 5, 10);
        bool differ = false;
        size_t common = std::min(active.size(), idle.size());
        for (size_t i = 0; i < common; ++i) {
            if (active[i] != idle[i]) differ = true;
        }
        ASSERT_TRUE(differ);
    }

    {
        auto clamped = PotRenderer::buildPot(Player::Black, PotActivity::Active, 0, 100);
        ASSERT_TRUE(clamped.size() >= 3);
        for (const auto& line : clamped) {
            ASSERT_TRUE(PotStyle::displayWidth(line) <= PotStyle::kMaxPotWidth);
        }
    }

    {
        auto pot = PotRenderer::buildPot(Player::Black, PotActivity::Active, 9, 20);
        ASSERT_EQ(pot.size(), static_cast<size_t>(9));
        for (const auto& line : pot) {
            ASSERT_EQ(PotStyle::displayWidth(line), 20);
        }
    }
}