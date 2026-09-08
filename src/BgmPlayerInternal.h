#pragma once

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "BgmPlayer.h"

namespace bgm_detail {

constexpr int kVolumeStep = 10;
constexpr int kVolumeMax = 100;
constexpr int kVolumeMin = 0;
constexpr int kVolumeInitial = 50;

inline int clampVolume(int v) {
    if (v < kVolumeMin) return kVolumeMin;
    if (v > kVolumeMax) return kVolumeMax;
    return v;
}

enum class BgmState {
    Idle,
    Loading,
    Playing,
    Disabled,
    Stopping
};

class VolumeController {
public:
    VolumeController() : volume_(kVolumeInitial), muted_(false), restoreVolume_(kVolumeInitial) {}

    void handle(VolumeCommand cmd) {
        switch (cmd) {
            case VolumeCommand::Increase:
                if (muted_) {
                    restoreVolume_ = clampVolume(restoreVolume_ + kVolumeStep);
                } else {
                    volume_ = clampVolume(volume_ + kVolumeStep);
                }
                break;
            case VolumeCommand::Decrease:
                if (muted_) {
                    restoreVolume_ = clampVolume(restoreVolume_ - kVolumeStep);
                } else {
                    volume_ = clampVolume(volume_ - kVolumeStep);
                }
                break;
            case VolumeCommand::ToggleMute:
                if (muted_) {
                    muted_ = false;
                    volume_ = restoreVolume_;
                } else {
                    restoreVolume_ = volume_;
                    muted_ = true;
                }
                break;
        }
    }

    int logicalVolume() const { return muted_ ? restoreVolume_ : volume_; }
    int effectiveVolume() const { return muted_ ? 0 : volume_; }
    bool isMuted() const { return muted_; }

private:
    int volume_;
    bool muted_;
    int restoreVolume_;
};

class PlaylistManager {
public:
    bool scanAndLoad(const std::filesystem::path& dirPath) {
        tracks_.clear();
        queue_.clear();
        queuePos_ = 0;

        std::error_code ec;
        if (!std::filesystem::exists(dirPath, ec)) {
            return false;
        }
        if (!std::filesystem::is_directory(dirPath, ec)) {
            return false;
        }

        std::vector<std::filesystem::path> collected;
        for (auto it = std::filesystem::directory_iterator(dirPath, ec);
             it != std::filesystem::directory_iterator(); ++it) {
            if (ec) {
                return false;
            }
            const auto& entry = *it;
            if (!entry.is_regular_file(ec)) continue;
            collected.push_back(entry.path());
        }

        if (collected.empty()) {
            return false;
        }

        tracks_ = std::move(collected);
        reshuffle();
        return true;
    }

    std::optional<std::filesystem::path> nextTrack() {
        if (tracks_.empty()) return std::nullopt;
        if (queuePos_ >= queue_.size()) {
            reshuffle();
        }
        if (queue_.empty()) return std::nullopt;
        size_t idx = queue_[queuePos_++];
        return tracks_[idx];
    }

    bool empty() const { return tracks_.empty(); }
    size_t size() const { return tracks_.size(); }

    const std::vector<std::filesystem::path>& tracks() const { return tracks_; }

private:
    void reshuffle() {
        queue_.resize(tracks_.size());
        for (size_t i = 0; i < tracks_.size(); ++i) queue_[i] = i;
        try {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::shuffle(queue_.begin(), queue_.end(), gen);
        } catch (...) {
            auto seed = static_cast<unsigned>(
                std::chrono::steady_clock::now().time_since_epoch().count());
            std::mt19937 gen(seed);
            std::shuffle(queue_.begin(), queue_.end(), gen);
        }
        queuePos_ = 0;
    }

    std::vector<std::filesystem::path> tracks_;
    std::vector<size_t> queue_;
    size_t queuePos_ = 0;
};

} // namespace bgm_detail