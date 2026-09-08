#pragma once

#include <iostream>

#ifdef GOMOKU_DEBUG
#define LOG(msg) do { std::cerr << "[LOG] " << (msg) << std::endl; } while (0)
#else
#define LOG(msg) do {} while (0)
#endif