#include "DxLib.h"
#include <cstdint>
#include "SceneControl.h"
#include "SceneVirt.h"
#include "Entry.h"
#include "KeyPoll.h"
#include "Interface.h"
#include "PlayerData.h"
#include "PhysicsData.h"

static bool intro;
static int_fast32_t bgH;
static LastDelta_t Last[2];

extern bool debugflag;
extern int_fast16_t Width, Height;
extern SceneControl Scene;
extern KeyPoll Key;
extern Delta_t Delta;
extern PlayerData P;
extern Interface Ui;

void EntryScene::Init()
{
    intro = true;
    P.rot = 0.00;
    P.px = Width/2, P.py = Height/2 + 300;
    LoadDivGraph(_T("core/phship3.png"),9,3,3,96,120,P.playerH);
    bgH = LoadGraph(_T("core/xbg.png"));
}
void EntryScene::End()
{
    //code
}
void EntryScene::Update()
{ 
    Delta.Update();
    if(Delta.Time(Last[0].Time, 5))
    {
        P.py -= 6;
        if(P.py <= Height - Height / 4) 
        {
            P.py = Height - Height / 4;
            intro = false;
            Scene.Change(S1_SCENE);
        }
        
    }

}
void EntryScene::Draw()
{
    DrawGraph(0,0,bgH,TRUE);
    DrawRotaGraph(P.px,P.py,P.playerSize,P.rot,P.playerH[5],TRUE);
    if(debugflag)
    {
        Ui.DrawValue(0,20,P.py);
    }
}