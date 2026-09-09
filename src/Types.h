#pragma once

enum class CellState {
    Empty,
    Black,
    White
};

enum class Player {
    Black,
    White
};

enum class GameState {
    InProgress,
    BlackWin,
    WhiteWin,
    Draw
};

enum class Direction {
    Horizontal,
    Vertical,
    LeftDiagonal,
    RightDiagonal
};

struct Position {
    int row;
    int col;
};

struct MoveRecord {
    Position pos;
    Player player;
};

struct WinningLine {
    Direction direction;
    Position start;
    int length;
    Player player;
};

enum class PotActivity {
    Active,
    Idle
};

enum class LayoutOrientation {
    LeftRight,
    TopBottom
};

struct PotSize {
    int height;
    int width;
};