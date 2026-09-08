cmake_minimum_required(VERSION 3.10)
project(gomoku CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(GOMOKU_DEBUG "Enable debug logging" OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(GOMOKU_DEBUG)
    add_compile_definitions(GOMOKU_DEBUG)
endif()

add_executable(gomoku
    src/main.cpp
    src/Board.cpp
    src/Judge.cpp
    src/Renderer.cpp
    src/InputReader.cpp
    src/GameController.cpp
    src/BgmPlayer.cpp
)

target_include_directories(gomoku PRIVATE src)

if(NOT MSVC)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(gomoku PRIVATE stdc++fs)
    endif()
endif()

enable_testing()

set(TEST_SOURCES
    test/TestMain.cpp
    test/TestBoard.cpp
    test/TestJudge.cpp
    test/TestInputReader.cpp
    test/TestRenderer.cpp
    test/TestIntegration.cpp
    test/TestVolumeController.cpp
    test/TestPlaylistManager.cpp
    test/TestBgmPlayer.cpp
)

add_executable(gomoku_test
    ${TEST_SOURCES}
    src/Board.cpp
    src/Judge.cpp
    src/Renderer.cpp
    src/InputReader.cpp
    src/BgmPlayer.cpp
)

target_include_directories(gomoku_test PRIVATE src test)

if(NOT MSVC)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
        target_link_libraries(gomoku_test PRIVATE stdc++fs)
    endif()
endif()

add_test(NAME gomoku_test COMMAND gomoku_test)
