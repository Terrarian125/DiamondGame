#include "DxLib.h"
#include "Board.h"
#include "Point.h"
#include <cmath>

bool Board::IsValid(int q, int r) {
    int s = -q - r;
    const int N = 4;
    return (q <= N && r <= N && s <= N) || (q >= -N && r >= -N && s >= -N);
}

int Board::GetAreaTeam(int q, int r) {
    int s = -q - r;
    if (r >= 5) return 1;
    if (q >= 5) return 2;
    if (s >= 5) return 3;
    return 0;
}

void Board::Draw(int playerCount) {
    for (int r = -8; r <= 8; r++) {
        for (int q = -8; q <= 8; q++) {
            if (!IsValid(q, r)) continue;
            int px, py;
            Point{ q, r }.ToPixel(&px, &py);
            int area = GetAreaTeam(q, r);
            if (area == 1) DrawCircle(px, py, Config::CHIP / 2, GetColor(60, 20, 20), TRUE);
            if (area == 2) DrawCircle(px, py, Config::CHIP / 2, GetColor(60, 60, 20), TRUE);
            if (area == 3 && playerCount == 3) DrawCircle(px, py, Config::CHIP / 2, GetColor(20, 60, 20), TRUE);
            DrawCircle(px, py, Config::CHIP / 2, GetColor(120, 120, 120), FALSE);
        }
    }
}