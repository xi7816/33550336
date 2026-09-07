#include "TestFramework.h"
#include "InputReader.h"

#include <sstream>
#include <streambuf>

static std::streambuf* saveCin(const std::string& input) {
    static std::istringstream iss;
    iss.clear();
    iss.str(input);
    std::streambuf* old = std::cin.rdbuf(iss.rdbuf());
    return old;
}

static void restoreCin(std::streambuf* old) {
    std::cin.rdbuf(old);
}

void testInputReader() {
    {
        std::streambuf* old = saveCin("15\n");
        auto result = InputReader::readInt();
        restoreCin(old);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result.value(), 15);
    }

    {
        std::streambuf* old = saveCin("abc\n");
        auto result = InputReader::readInt();
        restoreCin(old);
        ASSERT_FALSE(result.has_value());
    }

    {
        std::streambuf* old = saveCin("15 abc\n");
        auto result = InputReader::readInt();
        restoreCin(old);
        ASSERT_FALSE(result.has_value());
    }

    {
        std::streambuf* old = saveCin("7 8\n");
        auto result = InputReader::readPosition();
        restoreCin(old);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->row, 7);
        ASSERT_EQ(result->col, 8);
    }

    {
        std::streambuf* old = saveCin("abc\n");
        auto result = InputReader::readPosition();
        restoreCin(old);
        ASSERT_FALSE(result.has_value());
    }

    {
        std::streambuf* old = saveCin("7\n");
        auto result = InputReader::readPosition();
        restoreCin(old);
        ASSERT_FALSE(result.has_value());
    }

    {
        std::streambuf* old = saveCin("7,8\n");
        auto result = InputReader::readPosition();
        restoreCin(old);
        ASSERT_FALSE(result.has_value());
    }

    {
        std::streambuf* old = saveCin("undo\n");
        std::string line = InputReader::readLine();
        restoreCin(old);
        ASSERT_EQ(line, "undo");
    }

    {
        std::streambuf* old = saveCin("\n");
        std::string line = InputReader::readLine();
        restoreCin(old);
        ASSERT_EQ(line, "");
    }

    {
        std::streambuf* old = saveCin("Y\n");
        std::string line = InputReader::readLine();
        restoreCin(old);
        ASSERT_EQ(line, "Y");
    }
}