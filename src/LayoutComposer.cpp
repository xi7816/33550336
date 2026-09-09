#include "LayoutComposer.h"
#include "PotStyle.h"
#include "Log.h"

#include <algorithm>
#include <string>

PotSize LayoutComposer::fitSize(int boardRows, int boardWidth, int termWidth) {
    int h = boardRows;
    if (h > PotStyle::kPotTemplateRows) h = PotStyle::kPotTemplateRows;
    if (h < 3) h = 3;

    int avail = termWidth - boardWidth - 2;
    int w = (avail > 0) ? (avail / 2) : 6;
    if (w > PotStyle::kMaxPotWidth) w = PotStyle::kMaxPotWidth;
    if (w < 6) w = 6;

    return PotSize{h, w};
}

std::vector<std::string> LayoutComposer::compose(const std::vector<std::string>& boardLines,
                                                 const std::vector<std::string>& leftPot,
                                                 const std::vector<std::string>& rightPot) {
    int boardH = static_cast<int>(boardLines.size());
    int leftH  = static_cast<int>(leftPot.size());
    int rightH = static_cast<int>(rightPot.size());
    int maxH = std::max({boardH, leftH, rightH});
    if (maxH == 0) return {};

    auto emptyRowOf = [](const std::vector<std::string>& v) -> std::string {
        if (v.empty()) return std::string();
        return std::string(PotStyle::displayWidth(v[0]), ' ');
    };
    std::string emptyBoard = emptyRowOf(boardLines);
    std::string emptyLeft  = emptyRowOf(leftPot);
    std::string emptyRight = emptyRowOf(rightPot);

    std::vector<std::string> result;
    result.reserve(maxH);
    for (int i = 0; i < maxH; ++i) {
        std::string left = emptyLeft;
        std::string mid  = emptyBoard;
        std::string right = emptyRight;

        if (!leftPot.empty()) {
            int offset = (maxH - leftH) / 2;
            int idx = i - offset;
            if (idx >= 0 && idx < leftH) left = leftPot[idx];
        }
        if (!boardLines.empty()) {
            int offset = (maxH - boardH) / 2;
            int idx = i - offset;
            if (idx >= 0 && idx < boardH) mid = boardLines[idx];
        }
        if (!rightPot.empty()) {
            int offset = (maxH - rightH) / 2;
            int idx = i - offset;
            if (idx >= 0 && idx < rightH) right = rightPot[idx];
        }

        result.push_back(left + " " + mid + " " + right);
    }
    return result;
}