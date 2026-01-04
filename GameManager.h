#pragma once
#include <vector>
#include <stack>
#include "Point.h"

class GameManager {
public:
    std::vector<Piece> pieces;
    std::stack<GameState> history;
    int turn = 1;
    int selectedIdx = -1;
    int playerCount = 2;

    void Reset(int players);
    void Undo();
    void EndTurn();
    void HandleInput(int mx, int my);
    void DrawPieces();
};