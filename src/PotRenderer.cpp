#include "PotRenderer.h"
#include "PotStyle.h"
#include "Log.h"

#include <string>

std::vector<std::string> PotRenderer::buildPot(Player owner, PotActivity activity, int height, int width) {
    int w = width;
    if (w > PotStyle::kMaxPotWidth) w = PotStyle::kMaxPotWidth;
    if (w < 6) w = 6;
    int h = height;
    if (h < 3) h = 3;
    if (h > PotStyle::kPotTemplateRows) h = PotStyle::kPotTemplateRows;

    if (width > PotStyle::kMaxPotWidth || width < 6 || height <= 0 || height > PotStyle::kPotTemplateRows) {
        LOG("PotRenderer buildPot: 尺寸越界已钳制 (input width=" + std::to_string(width) + " height=" + std::to_string(height) + ")");
    }

    bool active = (activity == PotActivity::Active);
    const char* fill    = active ? PotStyle::kActiveFill    : PotStyle::kIdleFill;
    const char* borderH = active ? PotStyle::kActiveBorderH : PotStyle::kIdleBorderH;
    const char* borderV = active ? PotStyle::kActiveBorderV : PotStyle::kIdleBorderV;
    std::string label   = (owner == Player::Black) ? PotStyle::kBlackLabel : PotStyle::kWhiteLabel;
    std::string mark    = active ? PotStyle::kActiveMark : PotStyle::kIdleMark;

    int innerW = w - 4;
    int fillCount = innerW / 2;
    bool hasAsciiPad = (innerW % 2) != 0;

    auto makeBorderRow = [&](const char* left, const char* right) -> std::string {
        std::string row = left;
        for (int i = 0; i < fillCount; ++i) row += borderH;
        if (hasAsciiPad) row += ' ';
        row += right;
        return row;
    };

    int labelRowIdx = (h - 1) / 2;

    std::vector<std::string> lines;
    lines.reserve(h);
    lines.push_back(makeBorderRow(PotStyle::kOutlineTopLeft, PotStyle::kOutlineTopRight));

    for (int r = 1; r < h - 1; ++r) {
        std::string row = borderV;
        if (r == labelRowIdx) {
            int labelW = PotStyle::displayWidth(label);
            int markW  = PotStyle::displayWidth(mark);
            int coreW  = labelW + markW;
            int remainCols = innerW - coreW;
            if (remainCols < 0) {
                remainCols = innerW - labelW;
                if (remainCols < 0) remainCols = 0;
                int fillChars = remainCols / 2;
                int left = fillChars / 2;
                int right = fillChars - left;
                for (int i = 0; i < left; ++i) row += fill;
                row += label;
                for (int i = 0; i < right; ++i) row += fill;
            } else {
                int fillChars = remainCols / 2;
                int left = fillChars / 2;
                int right = fillChars - left;
                for (int i = 0; i < left; ++i) row += fill;
                row += label;
                row += mark;
                for (int i = 0; i < right; ++i) row += fill;
            }
        } else {
            for (int i = 0; i < fillCount; ++i) row += fill;
        }
        if (hasAsciiPad) row += ' ';
        row += borderV;
        lines.push_back(row);
    }

    lines.push_back(makeBorderRow(PotStyle::kOutlineBottomLeft, PotStyle::kOutlineBottomRight));
    return lines;
}