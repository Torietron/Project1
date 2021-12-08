#include "DxLib.h"
#include <cstdint>
#include "SceneControl.h"
#include "SceneVirt.h"
#include "Menu.h"
#include "KeyPoll.h"
#include "MousePoll.h"
#include "Interface.h"
#include "PhysicsData.h"

extern bool debugflag;
extern SceneControl Scene;
extern int_fast16_t Width, Height;
extern KeyPoll Key;
extern MousePoll Mouse;
extern Interface Ui;
extern PhysicsData Physics;

int flag = 0;
static const uint_fast8_t TOTAL_OPTIONS = 4;
static int_fast16_t pad[TOTAL_OPTIONS] = {0};
static int_fast16_t menux = Width/2-85, menuy = Height/2-15;

Options_t Options[4] = {
        {menux, 900, "Play", Ui.Yellow},
        {menux, 920, "Skip to Level 2", Ui.Yellow},
        {menux, 940, "Instructions", Ui.Yellow},
        {menux, 960, "Quit", Ui.Yellow},
};

void Menu::Init()
{
    Ui.selected = 0;
    Ui.brightness = 10;
    uint_fast8_t spacing = 0;
    for(uint_fast8_t i = 0; i < TOTAL_OPTIONS; i++)
    {
       pad[i] = 0 + spacing;
       spacing = spacing + 20;
    }

}

void Menu::End()
{
    //code
}

void Menu::Update()
{
    Key.Update();
    Mouse.Update();
    Ui.UpdateMenu(TOTAL_OPTIONS);
    Ui.UpdateMenu(TOTAL_OPTIONS,KEY_INPUT_W,KEY_INPUT_S);
    
    if(Key.Poll[KEY_INPUT_RETURN] == 1 || Key.Poll[KEY_INPUT_SPACE] == 1 || Key.Poll[KEY_INPUT_Z] == 1 || Mouse.Poll[MOUSE_INPUT_LEFT] == 1)
    {
        switch(Ui.selected)
        {
            case 0:
                Scene.Change(ENTRY_SCENE);
                break;
            case 1:
                Scene.Change(S2_SCENE);
                break;
            case 2:
                Scene.Change(INST_SCENE);
                break;
            case 3:
                DxLib_End();
                break;
            default:
                break;
        }
    } 
}

void Menu::Draw()
{
    Ui.Fade(FADE_IN,SPEED2);
    for(uint_fast8_t i = 0; i < TOTAL_OPTIONS; i++)
    {
        if(Physics.Fling(Options[i].y,menuy+pad[i],FLING_UP,1)) flag = 1;
        else flag = 0;
    }
    Ui.DrawMenu(Options,TOTAL_OPTIONS);
    DrawString(Width/3,Height/3,"S T A R  G R A V E !",Ui.White);
    DrawBox(Width/3,Height/3+25,Width/3+200,Height/3+26,Ui.White,TRUE);
    Ui.EndBlend();

    if(debugflag) Ui.DrawValue(0,20,Ui.selected);

}