#pragma once
#include <vector>

namespace Config {
    const int SW = 1280;
    const int SH = 720;
    const int CHIP = 26;
    const int OX = SW / 2;
    const int OY = (SH / 2) + 40;

    // 表示・機能フラグ
    extern bool showUndoBtn;
    extern bool showEndTurnBtn;
    extern bool showTurnMsg;
    extern bool useRightClick; // 右クリック機能のオンオフ
}

struct Point {
    int q, r;
    bool operator==(const Point& other) const { return q == other.q && r == other.r; }
    void ToPixel(int* x, int* y) const;
};

struct Piece {
    Point pos;
    unsigned int color;
    int team;
};

struct GameState {
    std::vector<Piece> pieces;
    int turn;
};