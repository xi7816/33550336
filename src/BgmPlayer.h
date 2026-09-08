#pragma once

#include <string>

enum class VolumeCommand {
    Increase,
    Decrease,
    ToggleMute
};

class BgmPlayer {
public:
    BgmPlayer();
    ~BgmPlayer();

    BgmPlayer(const BgmPlayer&) = delete;
    BgmPlayer& operator=(const BgmPlayer&) = delete;
    BgmPlayer(BgmPlayer&&) = delete;
    BgmPlayer& operator=(BgmPlayer&&) = delete;

    void start();
    void stop();
    void handleVolumeCommand(VolumeCommand cmd);
    void pollNextTrack();

    std::string currentTrackName() const;
    int currentVolume() const;
    bool isMuted() const;

private:
    class Impl;
    Impl* impl_;
};