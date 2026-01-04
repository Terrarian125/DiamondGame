#include "DxLib.h"
#include "GameManager.h"
#include "Board.h"
#include "Library/Input.h"
#include <cmath>

// Config変数の実体定義
bool Config::showUndoBtn = true;
bool Config::showEndTurnBtn = true;
bool Config::showTurnMsg = true;
bool Config::useRightClick = true; // デフォルトはON

void Point::ToPixel(int* x, int* y) const {
    *x = Config::OX + (int)(Config::CHIP * sqrt(3.0) * (q + r / 2.0));
    *y = Config::OY + (int)(Config::CHIP * 1.5 * r);
}

void GameManager::Reset(int players) {
    playerCount = players;
    pieces.clear();
    while (!history.empty()) history.pop();
    turn = 1;
    selectedIdx = -1;
    for (int r = -8; r <= 8; r++) {
        for (int q = -8; q <= 8; q++) {
            if (!Board::IsValid(q, r)) continue;
            int s = -q - r;
            if (r <= -5) pieces.push_back({ {q, r}, (unsigned int)GetColor(255, 60, 60), 1 });
            else if (q <= -5) pieces.push_back({ {q, r}, (unsigned int)GetColor(255, 255, 60), 2 });
            else if (s <= -5 && playerCount == 3) pieces.push_back({ {q, r}, (unsigned int)GetColor(60, 255, 60), 3 });
        }
    }
}

void GameManager::Undo() {
    if (!history.empty()) {
        GameState last = history.top(); history.pop();
        pieces = last.pieces;
        turn = last.turn;
        selectedIdx = -1;
    }
}

void GameManager::EndTurn() {
    history.push({ pieces, turn });
    turn = (turn % playerCount) + 1;
    selectedIdx = -1;
}

void GameManager::HandleInput(int mx, int my) {
    if (Input::IsMouseDown(MOUSE_INPUT_LEFT)) {
        int clickedIdx = -1;
        float hitRange = Config::CHIP * 0.7f;
        for (int i = 0; i < (int)pieces.size(); i++) {
            int px, py; pieces[i].pos.ToPixel(&px, &py);
            if (hypot(px - mx, py - my) < hitRange) { clickedIdx = i; break; }
        }
        if (clickedIdx != -1) {
            if (pieces[clickedIdx].team == turn) selectedIdx = clickedIdx;
        }
        else if (selectedIdx != -1) {
            for (int r = -8; r <= 8; r++) {
                for (int q = -8; q <= 8; q++) {
                    if (!Board::IsValid(q, r)) continue;
                    int px, py; Point{ q, r }.ToPixel(&px, &py);
                    if (hypot(px - mx, py - my) < hitRange) {
                        bool occupied = false;
                        for (auto& p : pieces) if (p.pos.q == q && p.pos.r == r) occupied = true;
                        if (!occupied) {
                            history.push({ pieces, turn });
                            pieces[selectedIdx].pos = { q, r };
                        }
                    }
                }
            }
        }
    }
}

void GameManager::DrawPieces() {
    for (int i = 0; i < (int)pieces.size(); i++) {
        int px, py; pieces[i].pos.ToPixel(&px, &py);
        DrawCircle(px, py, Config::CHIP / 2 - 2, pieces[i].color, TRUE);
        if (selectedIdx == i) DrawCircle(px, py, Config::CHIP / 2 + 5, GetColor(255, 255, 255), FALSE);
    }
}