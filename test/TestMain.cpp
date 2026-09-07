#include "TestFramework.h"

void testBoard();
void testJudge();
void testInputReader();
void testRenderer();
void testIntegration();

int main() {
    RUN_TEST(testBoard);
    RUN_TEST(testJudge);
    RUN_TEST(testInputReader);
    RUN_TEST(testRenderer);
    RUN_TEST(testIntegration);
    TEST_REPORT();
}