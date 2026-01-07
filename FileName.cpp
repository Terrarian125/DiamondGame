#include "DxLib.h"
#include "Library/Input.h"
#include "Board.h"
#include "GameManager.h"
#include "GUI.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetGraphMode(Config::SW, Config::SH, 32);
    ChangeWindowMode(TRUE);
    if (DxLib_Init() == -1) return -1;
    SetDrawScreen(DX_SCREEN_BACK);

    enum Scene { SELECT_PLAYER, MAIN_GAME };
    Scene currentScene = SELECT_PLAYER;
    bool showMenu = false;
    Board board;
    GameManager game;

    while (ProcessMessage() == 0) {
        ClearDrawScreen();
        Input::KeyStateUpdate();
        Input::MouseStateUpdate();
        int mx = Input::GetMouseX(), my = Input::GetMouseY();

        if (Input::IsKeyUP(KEY_INPUT_ESCAPE) && currentScene == MAIN_GAME) showMenu = !showMenu;

        if (currentScene == SELECT_PLAYER) {
            DrawFormatString(Config::OX - 100, Config::OY - 150, GetColor(255, 255, 255), "ダイヤモンドゲーム");
            if (GUI::Button(Config::OX - 150, Config::OY - 50, 120, 60, "2人対戦", mx, my)) {
                game.Reset(2); currentScene = MAIN_GAME;
            }
            if (GUI::Button(Config::OX + 30, Config::OY - 50, 120, 60, "3人対戦", mx, my)) {
                game.Reset(3); currentScene = MAIN_GAME;
            }
        }
        else if (currentScene == MAIN_GAME) {
            board.Draw(game.playerCount);
            if (!showMenu) game.HandleInput(mx, my);
            game.DrawPieces();

            if (GUI::Button(20, 20, 120, 40, "メニュー", mx, my)) showMenu = true;

            if (!showMenu) {
                if (Config::showUndoBtn) {
                    if (GUI::Button(20, Config::SH - 80, 140, 60, "一手戻る", mx, my)) game.Undo();
                }
                if (Config::showEndTurnBtn) {
                    if (GUI::Button(Config::SW - 160, Config::SH - 80, 140, 60, "ターン終了", mx, my)) game.EndTurn();
                }
                // 右クリック設定の適用
                if (Config::useRightClick && Input::IsMouseUP(MOUSE_INPUT_RIGHT)) game.EndTurn();
            }

            if (Config::showTurnMsg) {
                unsigned int tCol = (game.turn == 1) ? GetColor(255, 100, 100) : (game.turn == 2) ? GetColor(255, 255, 100) : GetColor(100, 255, 100);
                DrawFormatString(160, 30, tCol, "Player %d のターン", game.turn);
            }

            if (showMenu) {
                DrawBox(Config::OX - 300, Config::OY - 260, Config::OX + 300, Config::OY + 260, GetColor(30, 30, 30), TRUE);
                DrawBox(Config::OX - 300, Config::OY - 260, Config::OX + 300, Config::OY + 260, GetColor(255, 255, 255), FALSE);
                DrawFormatString(Config::OX - 40, Config::OY - 240, GetColor(255, 255, 255), "[ MENU ]");

                // --- 設定スイッチ ---
                if (GUI::Button(Config::OX - 250, Config::OY - 200, 500, 30, Config::showUndoBtn ? "一手戻るボタン: ON" : "一手戻るボタン: OFF", mx, my))
                    Config::showUndoBtn = !Config::showUndoBtn;
                if (GUI::Button(Config::OX - 250, Config::OY - 165, 500, 30, Config::showEndTurnBtn ? "ターン終了ボタン: ON" : "ターン終了ボタン: OFF", mx, my))
                    Config::showEndTurnBtn = !Config::showEndTurnBtn;
                if (GUI::Button(Config::OX - 250, Config::OY - 130, 500, 30, Config::useRightClick ? "右クリックで終了: ON" : "右クリックで終了: OFF", mx, my))
                    Config::useRightClick = !Config::useRightClick;
                if (GUI::Button(Config::OX - 250, Config::OY - 95, 500, 30, Config::showTurnMsg ? "ターン表示: ON" : "ターン表示: OFF", mx, my))
                    Config::showTurnMsg = !Config::showTurnMsg;

                // --- 説明ダイアログ系 ---
                if (GUI::Button(Config::OX - 250, Config::OY - 40, 240, 45, "基本ルール・操作", mx, my)) {
                    MessageBox(NULL,
                        "【基本ルール】\n自分の駒をすべて、対角線上にある反対側の陣地に運ぶのが目的です。\n\n"
                        "【操作方法】\n1. 自分の駒を左クリックして選択\n2. 移動したい空きマスを左クリックで移動\n"
                        "3. 移動が終わったら「ターン終了」ボタンか「右クリック」で次へ交代します。\n"
                        "※このゲームではどこへでも一気に動かせるツール形式を採用しています。",
                        "遊び方", MB_OK);
                }
                if (GUI::Button(Config::OX + 10, Config::OY - 40, 240, 45, "勝利への技・戦略", mx, my)) {
                    MessageBox(NULL,
                        "【1. センター制圧】\n中央付近のマスを確保すると、どの方向へも展開しやすくなります。\n\n"
                        "【2. 橋渡し】\n本来のルールでは「駒を飛び越える」動きが重要です。\nこのツールでは自由移動ですが、仲間を等間隔に並べることで、"
                        "本来のルールでの大ジャンプ（技）をシミュレーションして遊ぶことができます。\n\n"
                        "【3. ブロック】\n相手のゴール付近に自分の駒を居座らせることで、相手の完走を妨害するテクニックもあります。",
                        "戦略ガイド", MB_OK);
                }

                // --- システム操作 ---
                if (GUI::Button(Config::OX - 100, Config::OY + 40, 200, 45, "タイトルへ", mx, my)) {
                    showMenu = false; currentScene = SELECT_PLAYER;
                }
                if (GUI::Button(Config::OX - 100, Config::OY + 95, 200, 45, "ゲーム終了", mx, my)) break;
                if (GUI::Button(Config::OX - 100, Config::OY + 180, 200, 45, "閉じる", mx, my)) showMenu = false;
            }
        }
        ScreenFlip();
    }
    DxLib_End(); return 0;
}