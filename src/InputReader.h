#pragma once

#include <string>
#include <optional>
#include "Types.h"

class InputReader {
public:
    static std::optional<int> readInt();
    static std::optional<Position> readPosition();
    static std::string readLine();
};