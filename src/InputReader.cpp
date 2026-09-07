#include "InputReader.h"

#include <iostream>
#include <limits>
#include <sstream>

std::optional<int> InputReader::readInt() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return std::nullopt;
    }
    std::istringstream iss(line);
    int value;
    if (iss >> value) {
        char leftover;
        if (iss >> leftover) {
            return std::nullopt;
        }
        return value;
    }
    return std::nullopt;
}

std::optional<Position> InputReader::readPosition() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return std::nullopt;
    }
    std::istringstream iss(line);
    int row, col;
    if (iss >> row >> col) {
        char leftover;
        if (iss >> leftover) {
            return std::nullopt;
        }
        return Position{row, col};
    }
    return std::nullopt;
}

std::string InputReader::readLine() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return "";
    }
    return line;
}