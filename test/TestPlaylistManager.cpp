#include "TestFramework.h"
#include "BgmPlayerInternal.h"

#include <filesystem>
#include <fstream>
#include <set>

using namespace bgm_detail;

namespace {

std::filesystem::path makeTempDir(const std::string& name) {
    auto dir = std::filesystem::temp_directory_path() / ("bgm_test_" + name);
    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
    std::filesystem::create_directories(dir, ec);
    return dir;
}

void createFile(const std::filesystem::path& path, const std::string& content = "") {
    std::ofstream ofs(path, std::ios::binary);
    ofs << content;
    ofs.close();
}

void cleanupDir(const std::filesystem::path& dir) {
    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
}

} // namespace

static void testScanEmptyDir() {
    auto dir = makeTempDir("empty");
    PlaylistManager pm;
    ASSERT_FALSE(pm.scanAndLoad(dir));
    ASSERT_TRUE(pm.empty());
    cleanupDir(dir);
}

static void testScanNonExistentDir() {
    auto dir = std::filesystem::temp_directory_path() / "bgm_test_nonexistent_12345";
    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
    PlaylistManager pm;
    ASSERT_FALSE(pm.scanAndLoad(dir));
    ASSERT_TRUE(pm.empty());
}

static void testScanWithSubdirAndFiles() {
    auto dir = makeTempDir("mixed");
    createFile(dir / "track1.wav", "dummy");
    createFile(dir / "track2.mp3", "dummy");
    createFile(dir / "notes.txt", "dummy");
    std::filesystem::create_directories(dir / "subdir");
    createFile(dir / "subdir" / "inner.wav", "dummy");

    PlaylistManager pm;
    ASSERT_TRUE(pm.scanAndLoad(dir));
    ASSERT_EQ(pm.size(), static_cast<size_t>(3));
    cleanupDir(dir);
}

static void testNextTrackReturnsAll() {
    auto dir = makeTempDir("alltracks");
    createFile(dir / "a.wav", "x");
    createFile(dir / "b.wav", "x");
    createFile(dir / "c.wav", "x");

    PlaylistManager pm;
    ASSERT_TRUE(pm.scanAndLoad(dir));
    ASSERT_EQ(pm.size(), static_cast<size_t>(3));

    std::set<std::string> seen;
    for (int i = 0; i < 3; ++i) {
        auto t = pm.nextTrack();
        ASSERT_TRUE(t.has_value());
        seen.insert(t.value().filename().string());
    }
    ASSERT_EQ(seen.size(), static_cast<size_t>(3));
    cleanupDir(dir);
}

static void testNextTrackCycles() {
    auto dir = makeTempDir("cycle");
    createFile(dir / "a.wav", "x");
    createFile(dir / "b.wav", "x");

    PlaylistManager pm;
    ASSERT_TRUE(pm.scanAndLoad(dir));

    int count = 0;
    for (int i = 0; i < 6; ++i) {
        auto t = pm.nextTrack();
        if (t.has_value()) ++count;
    }
    ASSERT_EQ(count, 6);
    cleanupDir(dir);
}

void testPlaylistManager() {
    RUN_TEST(testScanEmptyDir);
    RUN_TEST(testScanNonExistentDir);
    RUN_TEST(testScanWithSubdirAndFiles);
    RUN_TEST(testNextTrackReturnsAll);
    RUN_TEST(testNextTrackCycles);
}