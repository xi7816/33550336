#include "TestFramework.h"

void testBoard();
void testJudge();
void testInputReader();
void testRenderer();
void testIntegration();
void testVolumeController();
void testPlaylistManager();
void testBgmPlayer();
void testPotActivityResolver();
void testPotRenderer();
void testLayoutComposer();
void testGameView();

int main() {
    RUN_TEST(testBoard);
    RUN_TEST(testJudge);
    RUN_TEST(testInputReader);
    RUN_TEST(testRenderer);
    RUN_TEST(testIntegration);
    RUN_TEST(testVolumeController);
    RUN_TEST(testPlaylistManager);
    RUN_TEST(testBgmPlayer);
    RUN_TEST(testPotActivityResolver);
    RUN_TEST(testPotRenderer);
    RUN_TEST(testLayoutComposer);
    RUN_TEST(testGameView);
    TEST_REPORT();
}