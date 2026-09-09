#pragma once

#include <vector>
#include <string>
#include "Types.h"

class LayoutComposer {
public:
    static PotSize fitSize(int boardRows, int boardWidth, int termWidth);
    static std::vector<std::string> compose(const std::vector<std::string>& boardLines,
                                            const std::vector<std::string>& leftPot,
                                            const std::vector<std::string>& rightPot);
};