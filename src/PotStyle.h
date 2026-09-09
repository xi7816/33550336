#pragma once

#include <string>

namespace PotStyle {
    constexpr int kMaxPotWidth = 20;
    constexpr int kMinPotWidth = 4;
    constexpr int kPotTemplateRows = 9;
    constexpr int kDefaultTermWidth = 80;

    constexpr const char* kOutlineTopLeft     = "\xE2\x94\x8C";
    constexpr const char* kOutlineTopRight    = "\xE2\x94\x90";
    constexpr const char* kOutlineBottomLeft  = "\xE2\x94\x94";
    constexpr const char* kOutlineBottomRight = "\xE2\x94\x98";
    constexpr const char* kOutlineHorizontal  = "\xE2\x94\x80";
    constexpr const char* kOutlineVertical    = "\xE2\x94\x82";

    constexpr const char* kActiveFill = "\xE2\x96\x88";
    constexpr const char* kIdleFill   = "\xE2\x96\x91";

    constexpr const char* kActiveBorderH = "\xE2\x95\x90";
    constexpr const char* kActiveBorderV = "\xE2\x95\x91";
    constexpr const char* kIdleBorderH   = "\xE2\x94\x80";
    constexpr const char* kIdleBorderV   = "\xE2\x94\x82";

    constexpr const char* kBlackLabel = "\xE9\xBB\x91";
    constexpr const char* kWhiteLabel = "\xE7\x99\xBD";
    constexpr const char* kActiveMark = "\xE2\x97\x8F";
    constexpr const char* kIdleMark   = "\xE2\x97\x8B";

    constexpr bool kUseAnsi = false;
    constexpr const char* kBlackActiveColor = "\033[38;5;130m";
    constexpr const char* kWhiteActiveColor = "\033[38;5;108m";
    constexpr const char* kIdleColor        = "\033[38;5;240m";
    constexpr const char* kColorReset       = "\033[0m";

    inline int displayWidth(const std::string& s) {
        int width = 0;
        for (size_t i = 0; i < s.size(); ) {
            unsigned char c = static_cast<unsigned char>(s[i]);
            int byteLen;
            if (c < 0x80) byteLen = 1;
            else if ((c >> 5) == 0x06) byteLen = 2;
            else if ((c >> 4) == 0x0E) byteLen = 3;
            else if ((c >> 3) == 0x1E) byteLen = 4;
            else byteLen = 1;
            if (i + byteLen > s.size()) byteLen = 1;
            width += (byteLen == 1) ? 1 : 2;
            i += byteLen;
        }
        return width;
    }

    inline std::string padToWidth(const std::string& s, int targetWidth, char pad = ' ') {
        int cur = displayWidth(s);
        if (cur >= targetWidth) return s;
        return s + std::string(targetWidth - cur, pad);
    }
}