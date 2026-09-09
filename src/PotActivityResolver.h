#pragma once

#include "Types.h"

class PotActivityResolver {
public:
    static PotActivity resolve(Player owner, Player current, GameState state);
    static const char* echoText(Player current);
};