#include "TestFramework.h"
#include "BgmPlayerInternal.h"

using namespace bgm_detail;

static void testVolumeIncreaseClamp() {
    VolumeController vc;
    ASSERT_EQ(vc.logicalVolume(), 50);
    ASSERT_EQ(vc.effectiveVolume(), 50);
    for (int i = 0; i < 10; ++i) {
        vc.handle(VolumeCommand::Increase);
    }
    ASSERT_EQ(vc.logicalVolume(), 100);
    ASSERT_EQ(vc.effectiveVolume(), 100);
    vc.handle(VolumeCommand::Increase);
    ASSERT_EQ(vc.logicalVolume(), 100);
}

static void testVolumeDecreaseClamp() {
    VolumeController vc;
    for (int i = 0; i < 10; ++i) {
        vc.handle(VolumeCommand::Decrease);
    }
    ASSERT_EQ(vc.logicalVolume(), 0);
    ASSERT_EQ(vc.effectiveVolume(), 0);
    vc.handle(VolumeCommand::Decrease);
    ASSERT_EQ(vc.logicalVolume(), 0);
}

static void testMuteToggleAndRestore() {
    VolumeController vc;
    ASSERT_FALSE(vc.isMuted());
    ASSERT_EQ(vc.effectiveVolume(), 50);

    vc.handle(VolumeCommand::ToggleMute);
    ASSERT_TRUE(vc.isMuted());
    ASSERT_EQ(vc.effectiveVolume(), 0);
    ASSERT_EQ(vc.logicalVolume(), 50);

    vc.handle(VolumeCommand::ToggleMute);
    ASSERT_FALSE(vc.isMuted());
    ASSERT_EQ(vc.effectiveVolume(), 50);
    ASSERT_EQ(vc.logicalVolume(), 50);
}

static void testVolumeAdjustWhileMuted() {
    VolumeController vc;
    vc.handle(VolumeCommand::ToggleMute);
    ASSERT_TRUE(vc.isMuted());
    ASSERT_EQ(vc.effectiveVolume(), 0);

    vc.handle(VolumeCommand::Increase);
    vc.handle(VolumeCommand::Increase);
    ASSERT_EQ(vc.logicalVolume(), 70);

    vc.handle(VolumeCommand::ToggleMute);
    ASSERT_FALSE(vc.isMuted());
    ASSERT_EQ(vc.effectiveVolume(), 70);
    ASSERT_EQ(vc.logicalVolume(), 70);
}

static void testMuteAtBoundary() {
    VolumeController vc;
    for (int i = 0; i < 5; ++i) {
        vc.handle(VolumeCommand::Decrease);
    }
    ASSERT_EQ(vc.logicalVolume(), 0);
    vc.handle(VolumeCommand::ToggleMute);
    ASSERT_TRUE(vc.isMuted());
    ASSERT_EQ(vc.effectiveVolume(), 0);
    vc.handle(VolumeCommand::ToggleMute);
    ASSERT_FALSE(vc.isMuted());
    ASSERT_EQ(vc.effectiveVolume(), 0);
}

void testVolumeController() {
    RUN_TEST(testVolumeIncreaseClamp);
    RUN_TEST(testVolumeDecreaseClamp);
    RUN_TEST(testMuteToggleAndRestore);
    RUN_TEST(testVolumeAdjustWhileMuted);
    RUN_TEST(testMuteAtBoundary);
}