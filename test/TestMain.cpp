#include "TestFramework.h"

void testBoard();
void testJudge();
void testInputReader();
void testRenderer();
void testIntegration();
void testVolumeController();
void testPlaylistManager();
void testBgmPlayer();

int main() {
    RUN_TEST(testBoard);
    RUN_TEST(testJudge);
    RUN_TEST(testInputReader);
    RUN_TEST(testRenderer);
    RUN_TEST(testIntegration);
    RUN_TEST(testVolumeController);
    RUN_TEST(testPlaylistManager);
    RUN_TEST(testBgmPlayer);
    TEST_REPORT();
}