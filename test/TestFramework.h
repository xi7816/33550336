#pragma once

#include <iostream>
#include <string>
#include <functional>

namespace testframework {
    inline int g_passCount = 0;
    inline int g_failCount = 0;
    inline std::string g_currentTest;

    inline void check(bool cond, const std::string& expr, const char* file, int line) {
        if (cond) {
            ++g_passCount;
        } else {
            ++g_failCount;
            std::cout << "[FAIL] " << g_currentTest << ": " << expr
                      << " (" << file << ":" << line << ")\n";
        }
    }

    inline void runTest(const std::string& name, std::function<void()> func) {
        g_currentTest = name;
        std::cout << "[RUN]  " << name << "\n";
        func();
        std::cout << "[DONE] " << name << "\n";
    }

    inline int report() {
        std::cout << "==============================\n";
        std::cout << "通过: " << g_passCount << "，失败: " << g_failCount << "\n";
        std::cout << "==============================\n";
        return (g_failCount == 0) ? 0 : 1;
    }
}

#define ASSERT_TRUE(cond) testframework::check((cond), #cond, __FILE__, __LINE__)
#define ASSERT_FALSE(cond) testframework::check(!(cond), #cond, __FILE__, __LINE__)
#define ASSERT_EQ(a, b) testframework::check((a) == (b), #a " == " #b, __FILE__, __LINE__)
#define RUN_TEST(name) testframework::runTest(#name, name)
#define TEST_REPORT() return testframework::report()