#include "Input.h"
#include "DxLib.h"
#include <string.h>

namespace Input {
    const int KEY_MAX = 256;
    char keyBuff[KEY_MAX] = { 0 };
    char keyBuffOld[KEY_MAX] = { 0 };
    char key_down[KEY_MAX] = { 0 };
    char key_up[KEY_MAX] = { 0 };
    int Key_Keep[KEY_MAX] = { 0 };

    const int MOUSE_MAX = 3;
    int mouseBuff = 0;
    int mouseBuffOld = 0;
    int mouse_down[MOUSE_MAX] = { 0 };
    int mouse_up[MOUSE_MAX] = { 0 };
    int Mouse_Keep[MOUSE_MAX] = { 0 };

    int mouseX = 0, mouseY = 0, mouseWheel = 0;

    void KeyStateUpdate() {
        memcpy(keyBuffOld, keyBuff, sizeof(keyBuff));
        GetHitKeyStateAll(keyBuff);
        for (int i = 0; i < KEY_MAX; i++) {
            if (keyBuff[i] && keyBuffOld[i]) Key_Keep[i]++;
            int key_xor = keyBuff[i] ^ keyBuffOld[i];
            if (key_xor) Key_Keep[i] = 0;
            key_down[i] = key_xor & keyBuff[i];
            key_up[i] = key_xor & keyBuffOld[i];
        }
    }

    void MouseStateUpdate() {
        mouseBuffOld = mouseBuff;
        mouseBuff = GetMouseInput();
        GetMousePoint(&mouseX, &mouseY);
        mouseWheel = GetMouseWheelRotVol();

        for (int i = 0; i < MOUSE_MAX; i++) {
            int mouseCode = 1 << i;
            bool current_is_on = (mouseBuff & mouseCode) != 0;
            bool old_is_on = (mouseBuffOld & mouseCode) != 0;

            if (current_is_on && old_is_on) Mouse_Keep[i]++;
            if (current_is_on ^ old_is_on) Mouse_Keep[i] = 0;

            mouse_down[i] = (current_is_on && !old_is_on) ? 1 : 0;
            mouse_up[i] = (!current_is_on && old_is_on) ? 1 : 0;
        }
    }

    bool IsKeyUP(int keyCode) { return (keyCode >= 0 && keyCode < KEY_MAX) ? key_up[keyCode] != 0 : false; }
    bool IsKeyDown(int keyCode) { return (keyCode >= 0 && keyCode < KEY_MAX) ? key_down[keyCode] != 0 : false; }
    int IsKeepKeyDown(int keyCode) { return (keyCode >= 0 && keyCode < KEY_MAX) ? Key_Keep[keyCode] : 0; }
    bool IsMouseUP(int mouseCode) {
        int idx = (mouseCode == MOUSE_INPUT_LEFT) ? 0 : (mouseCode == MOUSE_INPUT_RIGHT) ? 1 : 2;
        return mouse_up[idx] != 0;
    }
    bool IsMouseDown(int mouseCode) {
        int idx = (mouseCode == MOUSE_INPUT_LEFT) ? 0 : (mouseCode == MOUSE_INPUT_RIGHT) ? 1 : 2;
        return mouse_down[idx] != 0;
    }
    int IsKeepMouseDown(int mouseCode) {
        int idx = (mouseCode == MOUSE_INPUT_LEFT) ? 0 : (mouseCode == MOUSE_INPUT_RIGHT) ? 1 : 2;
        return Mouse_Keep[idx];
    }
    int GetMouseX() { return mouseX; }
    int GetMouseY() { return mouseY; }
    int GetMouseWheel() { return mouseWheel; }
}