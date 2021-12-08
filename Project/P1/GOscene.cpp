#include "DxLib.h"
#include <cstdint>
#include "SceneControl.h"
#include "SceneVirt.h"
#include "GOscene.h"
#include "KeyPoll.h"
#include "Interface.h"

static bool keytrigger = false;
static uint_fast8_t cyclefade = 0;
static Interface Ui2(0);

extern int_fast16_t Width, Height;
extern SceneControl Scene;
extern Interface Ui;
extern KeyPoll Key;

void GOscene::Init()
{
    Ui.brightness = 1;
    Ui2.brightness = 0;
}

void GOscene::End()
{
    //code
}

void GOscene::Update()
{
    Key.Update();
    if(Key.Poll[KEY_INPUT_RETURN] == 1) 
    {
        Scene.Change(MENU_SCENE);
    }
}

void GOscene::Draw()
{
    Ui.Fade(FADE_IN,SPEED1);
    DrawString(Width/3-30,Height/3,"M i s s i o n  F a i l e d !!!",Ui.Red);
    DrawBox(Width/3-30,Height/3+25,Width/3+270,Height/3+27,Ui.Red,TRUE);
    Ui.EndBlend();

    if(Ui.brightness = 255)
    {
        Ui2.Fade(cyclefade,SPEED2); 
        DrawString(Width/3+50,Height/3+120,"Press Enter",GetColor(150,0,0));
        if(Ui2.brightness == 255) cyclefade = 1;
        if(Ui2.brightness == 0) cyclefade = 0;
        Ui2.EndBlend();
    }
    
}