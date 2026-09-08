#include "TestFramework.h"
#include "BgmPlayer.h"

static void testLifeCycleNoCrash() {
    {
        BgmPlayer player;
        player.start();
        player.stop();
    }
    {
        BgmPlayer player;
        player.start();
        player.stop();
        player.stop();
    }
    ASSERT_TRUE(true);
}

static void testVolumeCommandInDisabledNoCrash() {
    BgmPlayer player;
    player.start();
    player.handleVolumeCommand(VolumeCommand::Increase);
    player.handleVolumeCommand(VolumeCommand::Decrease);
    player.handleVolumeCommand(VolumeCommand::ToggleMute);
    player.handleVolumeCommand(VolumeCommand::ToggleMute);
    ASSERT_TRUE(true);
}

static void testVolumeQueryAfterAdjust() {
    BgmPlayer player;
    player.start();
    int v0 = player.currentVolume();
    player.handleVolumeCommand(VolumeCommand::Increase);
    int v1 = player.currentVolume();
    ASSERT_EQ(v1, v0 + 10);
    player.stop();
}

static void testMuteQuery() {
    BgmPlayer player;
    player.start();
    ASSERT_FALSE(player.isMuted());
    player.handleVolumeCommand(VolumeCommand::ToggleMute);
    ASSERT_TRUE(player.isMuted());
    player.handleVolumeCommand(VolumeCommand::ToggleMute);
    ASSERT_FALSE(player.isMuted());
    player.stop();
}

static void testPollNextTrackNoCrash() {
    BgmPlayer player;
    player.start();
    for (int i = 0; i < 10; ++i) {
        player.pollNextTrack();
    }
    player.stop();
    ASSERT_TRUE(true);
}

static void testStartIdempotent() {
    BgmPlayer player;
    player.start();
    player.start();
    player.start();
    player.stop();
    ASSERT_TRUE(true);
}

void testBgmPlayer() {
    RUN_TEST(testLifeCycleNoCrash);
    RUN_TEST(testVolumeCommandInDisabledNoCrash);
    RUN_TEST(testVolumeQueryAfterAdjust);
    RUN_TEST(testMuteQuery);
    RUN_TEST(testPollNextTrackNoCrash);
    RUN_TEST(testStartIdempotent);
}