#pragma once

#include <vector>
#include <string>
#include "Types.h"

class PotRenderer {
public:
    static std::vector<std::string> buildPot(Player owner, PotActivity activity, int height, int width);
};