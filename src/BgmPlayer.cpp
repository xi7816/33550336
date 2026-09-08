#include "BgmPlayer.h"
#include "BgmPlayerInternal.h"
#include "Log.h"

#include <atomic>
#include <filesystem>
#include <string>

#define MINIAUDIO_IMPLEMENTATION
#include "third_party/miniaudio.h"

namespace {

constexpr const char* kBgmDir = "C:\\Users\\16072\\Desktop\\game\\bgm";

} // namespace

namespace bgm_detail {

class AudioEngine {
public:
    AudioEngine() : needNextTrack_(false) {}
    ~AudioEngine() { shutdown(); }

    bool init() {
        if (engineInitialized_) return true;
        ma_result result = ma_engine_init(nullptr, &engine_);
        if (result != MA_SUCCESS) {
            LOG("音频子系统不可用");
            return false;
        }
        engineInitialized_ = true;
        return true;
    }

    bool isAvailable() const { return engineInitialized_; }

    bool loadAndPlay(const std::filesystem::path& filePath) {
        if (!engineInitialized_) return false;

        uninitSound();

        std::string pathStr = filePath.string();
        ma_result result = ma_sound_init_from_file(&engine_, pathStr.c_str(), 0, nullptr, nullptr, &sound_);
        if (result != MA_SUCCESS) {
            LOG("曲目 " + pathStr + " 加载失败，已跳过");
            return false;
        }
        soundInitialized_ = true;

        ma_sound_set_end_callback(&sound_, &AudioEngine::onEndCallback, &needNextTrack_);

        float vol = currentVolumeNormalized_;
        ma_sound_set_volume(&sound_, vol);

        result = ma_sound_start(&sound_);
        if (result != MA_SUCCESS) {
            LOG("曲目 " + pathStr + " 播放启动失败，已跳过");
            uninitSound();
            return false;
        }
        return true;
    }

    void setVolume(int logicalVolume) {
        currentVolumeNormalized_ = static_cast<float>(logicalVolume) / static_cast<float>(kVolumeMax);
        if (!soundInitialized_) return;
        ma_sound_set_volume(&sound_, currentVolumeNormalized_);
    }

    void stop() {
        if (soundInitialized_) {
            ma_sound_stop(&sound_);
            uninitSound();
        }
    }

    void shutdown() {
        stop();
        if (engineInitialized_) {
            ma_engine_uninit(&engine_);
            engineInitialized_ = false;
        }
    }

    bool consumeNextTrackFlag() {
        return needNextTrack_.exchange(false);
    }

private:
    static void onEndCallback(void* pUserData, ma_sound*) {
        auto* flag = static_cast<std::atomic<bool>*>(pUserData);
        if (flag) flag->store(true);
    }

    void uninitSound() {
        if (soundInitialized_) {
            ma_sound_uninit(&sound_);
            soundInitialized_ = false;
        }
    }

    ma_engine engine_;
    ma_sound sound_;
    bool engineInitialized_ = false;
    bool soundInitialized_ = false;
    std::atomic<bool> needNextTrack_;
    float currentVolumeNormalized_ = static_cast<float>(kVolumeInitial) / static_cast<float>(kVolumeMax);
};

} // namespace bgm_detail

class BgmPlayer::Impl {
public:
    Impl() : state_(bgm_detail::BgmState::Idle) {}
    ~Impl() {
        if (state_ == bgm_detail::BgmState::Playing) {
            stop();
        }
    }

    void start() {
        if (state_ == bgm_detail::BgmState::Playing || state_ == bgm_detail::BgmState::Loading) return;
        state_ = bgm_detail::BgmState::Loading;

        if (!playlist_.scanAndLoad(std::filesystem::path(kBgmDir))) {
            state_ = bgm_detail::BgmState::Disabled;
            return;
        }

        if (!audio_.init()) {
            state_ = bgm_detail::BgmState::Disabled;
            return;
        }

        audio_.setVolume(volumeCtrl_.effectiveVolume());

        if (!tryPlayNextAvailable()) {
            LOG("无任何可用曲目，背景音乐禁用");
            state_ = bgm_detail::BgmState::Disabled;
            audio_.shutdown();
            return;
        }

        state_ = bgm_detail::BgmState::Playing;
    }

    void stop() {
        if (state_ == bgm_detail::BgmState::Idle) return;
        state_ = bgm_detail::BgmState::Stopping;
        audio_.shutdown();
        state_ = bgm_detail::BgmState::Idle;
    }

    void handleVolumeCommand(VolumeCommand cmd) {
        volumeCtrl_.handle(cmd);
        if (state_ == bgm_detail::BgmState::Playing) {
            audio_.setVolume(volumeCtrl_.effectiveVolume());
            LOG("音量已调整为 " + std::to_string(volumeCtrl_.effectiveVolume()));
        } else {
            LOG("音量已调整为 " + std::to_string(volumeCtrl_.effectiveVolume()) + "，但当前无可用音频输出");
        }
    }

    void pollNextTrack() {
        if (state_ != bgm_detail::BgmState::Playing) return;
        if (!audio_.consumeNextTrackFlag()) return;
        tryPlayNextAvailable();
    }

    std::string currentTrackName() const { return currentTrackName_; }
    int currentVolume() const { return volumeCtrl_.logicalVolume(); }
    bool isMuted() const { return volumeCtrl_.isMuted(); }

private:
    bool tryPlayNextAvailable() {
        const size_t maxAttempts = playlist_.size();
        for (size_t i = 0; i < maxAttempts; ++i) {
            auto track = playlist_.nextTrack();
            if (!track.has_value()) break;
            if (audio_.loadAndPlay(track.value())) {
                currentTrackName_ = track.value().filename().string();
                LOG("开始播放曲目: " + currentTrackName_);
                return true;
            }
        }
        currentTrackName_.clear();
        return false;
    }

    bgm_detail::PlaylistManager playlist_;
    bgm_detail::AudioEngine audio_;
    bgm_detail::VolumeController volumeCtrl_;
    bgm_detail::BgmState state_;
    std::string currentTrackName_;
};

BgmPlayer::BgmPlayer() : impl_(new Impl()) {}

BgmPlayer::~BgmPlayer() {
    if (impl_) {
        impl_->stop();
        delete impl_;
        impl_ = nullptr;
    }
}

void BgmPlayer::start() { impl_->start(); }
void BgmPlayer::stop() { impl_->stop(); }
void BgmPlayer::handleVolumeCommand(VolumeCommand cmd) { impl_->handleVolumeCommand(cmd); }
void BgmPlayer::pollNextTrack() { impl_->pollNextTrack(); }
std::string BgmPlayer::currentTrackName() const { return impl_->currentTrackName(); }
int BgmPlayer::currentVolume() const { return impl_->currentVolume(); }
bool BgmPlayer::isMuted() const { return impl_->isMuted(); }
