#include "DxLib.h"
#include <cstdint>
#include "SceneControl.h"
#include "SceneVirt.h"
#include "Wscene.h"
#include "KeyPoll.h"
#include "Interface.h"

static Interface Ui2(0);
static uint_fast8_t cyclefade = 0;

extern int_fast16_t Width, Height;
extern SceneControl Scene;
extern KeyPoll Key;
extern Interface Ui;

void Wscene::Init()
{
    Ui.brightness = 1;
    Ui2.brightness = 0;
}

void Wscene::End()
{
    //code
}

void Wscene::Update()
{
    Key.Update();
    if(Key.Poll[KEY_INPUT_RETURN] == 1) 
    {
        Scene.Change(MENU_SCENE);
    }
}

void Wscene::Draw()
{
    Ui.Fade(FADE_IN,SPEED1);
    DrawString(Width/3-50,Height/3,"M i s s i o n  C o m p l e t e !!!",GetColor(0,150,125));
    DrawBox(Width/3-50,Height/3+25,Width/3+300,Height/3+27,GetColor(0,150,125),TRUE);
    Ui.EndBlend();

    if(Ui.brightness = 255)
    {
        Ui2.Fade(cyclefade,SPEED2); 
        DrawString(Width/3+50,Height/3+120,"Press Enter",GetColor(0,150,125));
        if(Ui2.brightness == 255) cyclefade = 1;
        if(Ui2.brightness == 0) cyclefade = 0;
        Ui2.EndBlend();
    }
    
}