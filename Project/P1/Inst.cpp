#include "DxLib.h"
#include <cstdint>
#include "SceneControl.h"
#include "SceneVirt.h"
#include "Inst.h"
#include "KeyPoll.h"
#include "MousePoll.h"
#include "Interface.h"

extern int_fast16_t Width, Height;
extern SceneControl Scene;
extern KeyPoll Key;
extern MousePoll Mouse;
extern Interface Ui;

void Inst::Init()
{
    Ui.brightness = 0;
}

void Inst::End()
{
    //code
}

void Inst::Update()
{
    Key.Update();
    Mouse.Update();
    if(Key.Poll[KEY_INPUT_L] == 1 || Key.Poll[KEY_INPUT_V] == 1 || Mouse.Poll[MOUSE_INPUT_RIGHT] == 1) 
    {
        Scene.Change(MENU_SCENE);
    }
}

void Inst::Draw()
{
    Ui.Fade(FADE_IN,SPEED2);
    SetFontSize(12);
    DrawString(Width/2-200,Height/3-120,"* Flight Mode automatically changes based on level.",GetColor(255,255,255));
    DrawString(Width/2-200,Height/3-100,"The ship is immune to damage during a hyperspace JUMP.",GetColor(255,255,255));
    DrawString(Width/2-200,Height/3-80,"Time until next JUMP is displayed as a white bar.",GetColor(255,255,255));
    DrawString(Width/2-200,Height/3-60,"Your ship has SHIELDS that recharge over time,",GetColor(255,255,255));
    DrawString(Width/2-200,Height/3-40,"HULL and SHIELD status are shown as RED/BLUE bars.",GetColor(255,255,255));
    DrawString(Width/2-210,Height/3,"Controls in Sub-Light Travel mode:",GetColor(255,255,255));
    DrawBox(Width/2-210,Height/3+18,Width/3+250,Height/3+19,Ui.White,TRUE);
    DrawString(Width/2-210,Height/3+30,"- Control Style 1: W,A,S,D keys to move, SPACE to Fire, L to Jump",GetColor(255,255,255));
    DrawString(Width/2-210,Height/3+50,"- Control Style 2: ARROW keys to move, Z/SPACE to Fire, V to Jump",GetColor(255,255,255));
    DrawString(Width/2-210,Height/3+70,"* Optionally, you may LEFT CLICK to Fire, RIGHT CLICK to Jump",GetColor(255,255,255));
    DrawString(Width/2-210,Height/3+90,"* To JUMP, press JUMP along with any directional key.",GetColor(255,255,255));
    DrawString(Width/2-210,Height/3+125,"Extra Controls in Omni-Thrust mode: ",GetColor(255,255,255));
    DrawBox(Width/2-210,Height/3+143,Width/3+250,Height/3+144,Ui.White,TRUE);
    DrawString(Width/2-210,Height/3+155,"- Control Style 1: Rotate with J and K",GetColor(255,255,255));
    DrawString(Width/2-210,Height/3+175,"- Control Style 2: Rotate with X and C",GetColor(255,255,255));
    DrawString(Width/2-210,Height/3+195,"* Optionally, you may rotate in this mode by moving the MOUSE",GetColor(255,255,255));
    DrawString(Width/2-150,Height/3+234,"Press JUMP to return to the Menu",GetColor(255,255,255));
    SetFontSize(Ui.FontSize);
    Ui.EndBlend();
    
}