#pragma once

class Board {
public:
    static bool IsValid(int q, int r);
    static int GetAreaTeam(int q, int r);
    void Draw(int playerCount);
};