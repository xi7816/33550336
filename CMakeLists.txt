cmake_minimum_required(VERSION 3.10)
project(gomoku CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_executable(gomoku
    src/main.cpp
    src/Board.cpp
    src/Judge.cpp
    src/Renderer.cpp
    src/InputReader.cpp
    src/GameController.cpp
)

target_include_directories(gomoku PRIVATE src)

enable_testing()

set(TEST_SOURCES
    test/TestMain.cpp
    test/TestBoard.cpp
    test/TestJudge.cpp
    test/TestInputReader.cpp
    test/TestRenderer.cpp
    test/TestIntegration.cpp
)

add_executable(gomoku_test
    ${TEST_SOURCES}
    src/Board.cpp
    src/Judge.cpp
    src/Renderer.cpp
    src/InputReader.cpp
)

target_include_directories(gomoku_test PRIVATE src test)

add_test(NAME gomoku_test COMMAND gomoku_test)