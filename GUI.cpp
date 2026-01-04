#include "DxLib.h"
#include "GUI.h"
#include "Library/Input.h"
#include <string.h>

bool GUI::Button(int x, int y, int w, int h, const char* text, int mx, int my) {
    bool hover = (mx >= x && mx <= x + w && my >= y && my <= y + h);
    unsigned int col = hover ? GetColor(180, 180, 180) : GetColor(100, 100, 100);
    DrawBox(x, y, x + w, y + h, col, TRUE);
    DrawBox(x, y, x + w, y + h, GetColor(255, 255, 255), FALSE);
    DrawFormatString(x + (w / 2) - (int)strlen(text) * 4, y + (h / 2) - 8, GetColor(255, 255, 255), text);
    return hover && Input::IsMouseUP(MOUSE_INPUT_LEFT);
}