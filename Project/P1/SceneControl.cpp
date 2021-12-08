#include "DxLib.h"
#include "SceneControl.h"
#include "SceneVirt.h"
#include "NoScene.h"
#include "DebugScene.h"
#include "Menu.h"
#include "Entry.h"
#include "Stage1.h"
#include "Stage2.h"
#include "GOscene.h"
#include "Wscene.h"
#include "Inst.h"

NoScene noscene;
DebugScene debugscene;
Menu menuscene;
Inst instscene;
EntryScene entryscene;
GOscene goscene;
Wscene wscene;
Stage1 stage1scene;
Stage2 stage2scene;
SceneVirt *sPtr;

SceneControl::SceneControl(uint_fast8_t startup)
:nScene(startup)
{
    lScene = NO_SCENE;
    sPtr = &noscene;
}

void SceneControl::Change(uint_fast8_t nextscene)
{
    nScene = nextscene;
}

void SceneControl::Init()
{
    //code
}

void SceneControl::End()
{
    //code
}

int SceneControl::Update()
{
    if(this->lScene != this->nScene)
    {
        sPtr->End();
        switch(this->nScene)
        {
            case DEBUG_SCENE:
                sPtr = &debugscene;
                break;
            case MENU_SCENE:
                sPtr = &menuscene;
                break;
            case INST_SCENE:
                sPtr = &instscene;
                break;
            case ENTRY_SCENE:
                sPtr = &entryscene;
                break;
            case S1_SCENE:
                sPtr = &stage1scene;
                break;
            case S2_SCENE:
                sPtr = &stage2scene;
                break;
            case GO_SCENE:
                sPtr = &goscene;
                break;
            case W_SCENE:
                sPtr = &wscene;
                break;
        }
        this->lScene = this->nScene;
        sPtr->Init();
    }
    sPtr->Update();
    return 0;
}

int SceneControl::Draw()
{
    sPtr->Draw();
    return 0;
}