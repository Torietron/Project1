#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include <ctime>
#include "SceneControl.h"
#include "SceneVirt.h"
#include "Stage1.h"
#include "KeyPoll.h"
#include "MousePoll.h"
#include "PlayerData.h"
#include "EnemyData.h"
#include "PhysicsData.h"
#include "Projectiles.h"
#include "Interface.h"

#define SHOTMAX 150
#define ENEMYSHOTMAX 150
#define ENEMIES 14

static uint_fast8_t ShotDFlag[SHOTMAX] = {0};
static Shotobj_t sobj[SHOTMAX];

static uint_fast8_t EnemyDFlag[ENEMIES] = {0};
static Enemy_t eobj[ENEMIES];

static uint_fast8_t eShotDFlag[ENEMYSHOTMAX] = {0};
static Shotobj_t esobj[ENEMYSHOTMAX];

static bool w1, w2, w3, w4, w5, waveCheck, sComplete, bGrace;
static const double PI = M_PI;
static uint_fast8_t dice, tDestroyed, sAnim = 0;
static int_fast16_t anchor;
static int_fast32_t random;
static uint_fast32_t waveTime;
static int_fast64_t bgH, bgHc;
static LastDelta_t Last[2];
static Interface Ui_flytext;

extern bool debugflag;
extern Delta_t Delta;
extern PlayerData P;
extern EnemyData E;
extern KeyPoll Key;
extern MousePoll Mouse;
extern Interface Ui;
extern SceneControl Scene;
extern PhysicsData Physics;
extern int_fast16_t Width, Height;

void Stage1::Init()
{
    tDestroyed = 0;
    Ui.brightness = 255;
    Ui_flytext.brightness = 255;
    sComplete = false, waveCheck = false;
    w1 = false, w2 = false, w3 = false, w4 = false, w5 = false;
    P.px = Width/2, P.py = Height - Height / 4, P.mx = P.px, P.my = P.py;
    P.moved = 'N';
    P.rot = 0.00;
    P.hpmax = 40, P.shpmax = 7;
    P.hp = 40, P.shp = 7;

    LoadDivGraph(_T("core/phship3.png"),9,3,3,96,120,P.playerH);
    LoadDivGraph(_T("core/pshield0.png"),2,2,1,150,145,P.shieldH);
    LoadDivGraph(_T("core/bt01.png"),3,2,2,58,90,P.playershotH);
    LoadDivGraph(_T("core/et01.png"),3,2,2,58,90,E.eShotH);
    P.playershot2H = LoadGraph(_T("core/bm01.png"));
    bgH = LoadGraph(_T("core/xbg.png"));
    bgHc = bgH;

    srand(static_cast<int_fast32_t>(time(0)));
    anchor = 240;

    //enemy init
    for(uint_fast8_t i = 0; i < ENEMIES ; i++)
    {
        //double pass gen
        random = rand();
        SRand(random);
        dice - GetRand(2) - 1;
        if (dice % 2 == 0) LoadDivGraph(_T("core/eship0a.png"),6,3,2,128,126,eobj[i].enemyH);
        else LoadDivGraph(_T("core/eship0b.png"),6,3,2,128,126,eobj[i].enemyH);
        dice = GetRand(100);

        eobj[i].charge = 0;
        eobj[i].magnitude = dice + 100;
        eobj[i].hp = 3, eobj[i].hpmax = eobj[i].hp;
        eobj[i].tx = -400, eobj[i].ty = -400;
        eobj[i].ax = anchor, eobj[i].x = anchor;
        eobj[i].ay = 90;
        eobj[i].y = eobj[i].ay - 600;
        eobj[i].pace = 0;
        eobj[i].type = 0;
        eobj[i].cycletime = dice;
        eobj[i].vel = E.vel - dice - 70;
        eobj[i].rot = PI*2;
        eobj[i].size = E.size / E.sizeDiv;
        eobj[i].frame = 0;

        eobj[i].active = false;
        eobj[i].warp = false;
        eobj[i].destroyed = false;
        eobj[i].chaser = false;
        eobj[i].edge = false;

        if(dice % 2 == 0) eobj[i].direction = 'R';
        else eobj[i].direction = 'L';

        anchor = GetRand(300) + 100;
    }

    //chaser init-adjust
    for(uint_fast8_t i = 9; i < ENEMIES; i++)
    {
        eobj[i].ay = 430;
        eobj[i].y = eobj[9].ay + 1200;
        eobj[i].rot = E.rot;
    }

    //boss eobj[8]; init
    eobj[8].size = eobj[8].size * 1.8;
    eobj[8].hp = 25, eobj[8].hpmax = eobj[8].hp;
    eobj[8].ay = 115;
    eobj[8].type = 1;
    eobj[8].rot = PI*2;
    LoadDivGraph(_T("core/eship0c.png"),6,3,2,128,126,eobj[8].enemyH);
    
    //collision init
    GetGraphSize(P.playerH[0],&P.pw,&P.ph);
    GetGraphSize(P.playershotH[0],&P.sw,&P.sh);
    GetGraphSize(eobj[0].enemyH[0],&E.w,&E.h);
    E.w = (E.w *0.50) / E.sizeDiv;
    E.h = (E.h *0.40) / E.sizeDiv;
    E.bw = E.w * 1.8;
    E.bh = E.h * 1.8;
    E.sw = P.sw;
    E.sh = P.sh;

    P.sw = P.sw * (P.shotSize *0.8);
    P.sh = P.sh * (P.shotSize *0.8);

    E.sw = E.sw * (E.shotSize *0.5);
    E.sh = E.sh * (E.shotSize *0.5);

    P.pw = P.pw * (P.playerSize *0.35);
    P.ph = P.ph * (P.playerSize *0.30);

    //first trigger
    w1 = true;
    eobj[0].warp = true;
    eobj[0].rot = E.rot;
    EnemyDFlag[0] = 1;
    eobj[1].rot = E.rot;

    //double check that array init was 0
    for(uint_fast8_t i = 0; i < ENEMYSHOTMAX; i++)
    {
        eShotDFlag[i] = 0;
    }
    
}

void Stage1::End()
{
    DeleteGraph((int)P.playerH);
    DeleteGraph((int)P.playershotH);
    DeleteGraph((int)P.shieldH);
    DeleteGraph((int)P.playershot2H);

    for(uint_fast8_t i = 0; i < ENEMIES; i++)
    {
        EnemyDFlag[i] = 0;
        DeleteGraph((int)eobj[i].enemyH);
    }
    for(uint_fast8_t i = 0; i < SHOTMAX; i++)
    {
        ShotDFlag[i] = 0;
    }
}

void Stage1::Update()
{
    Mouse.Update();
    Key.Update();
    Delta.Update();
    P.Update(0);
    E.Update(0,eobj,ENEMIES,EnemyDFlag);
    E.Fire(eobj,esobj,P,ENEMIES,ENEMYSHOTMAX,eShotDFlag);

    if((Mouse.Poll[MOUSE_INPUT_LEFT] >= 1 || Key.Poll[KEY_INPUT_SPACE] >= 1 || Key.Poll[KEY_INPUT_Z] >= 1) && P.charge >= 3)
    {
        P.CreateFire(0,sobj,SHOTMAX,ShotDFlag);
        if(tDestroyed >= 2) P.CreateFire(1,sobj,SHOTMAX,ShotDFlag);
    }

    if(Key.Poll[KEY_INPUT_ESCAPE] == 1) 
    {
        Scene.Change(MENU_SCENE);
    }

    //enemy wave triggers
    if(waveCheck)
    {
        if(eobj[0].destroyed && w1)
        {
            EnemyDFlag[1] = 1;
            eobj[1].warp = true;
            w1 = false, w2 = true;
        }
        if(eobj[1].destroyed && w2) 
        {
            EnemyDFlag[2] = 1, EnemyDFlag[3] = 1, EnemyDFlag[4] = 1;
            eobj[2].warp = true, eobj[3].warp = true, eobj[4].warp = true;
            w2 = false, w3 = true;
            EnemyDFlag[9] = 1, EnemyDFlag[10] = 1;
            eobj[9].chaser = true, eobj[10].chaser = true;
        }
        if(eobj[2].destroyed && eobj[3].destroyed && eobj[4].destroyed && w3)
        {
            EnemyDFlag[5] = 1, EnemyDFlag[6] = 1, EnemyDFlag[7] = 1; 
            eobj[5].warp = true, eobj[6].warp = true, eobj[7].warp = true;
            w3 = false, w4 = true;
            EnemyDFlag[11] = 1, EnemyDFlag[12] = 1, EnemyDFlag[13] = 1;
            eobj[11].chaser = true, eobj[12].chaser = true, eobj[13].chaser = true;
        }
        if(eobj[5].destroyed && eobj[6].destroyed && eobj[7].destroyed && w4)
        {
            EnemyDFlag[8] = 1;
            eobj[8].warp = true;
            w4 = false; w5 = true;
        }
        waveCheck = false;
    }
    //stage complete trigger
    if(tDestroyed == 14)
    {
        sComplete = true;
    }

    //collision det - enemy
    for(register uint_fast8_t i = 0; i < SHOTMAX; i++)
    {
        if(ShotDFlag[i] == 1 || ShotDFlag[i] == 2)
        {
            for(register uint_fast8_t j = 0; j < ENEMIES; j++)
            {
                if(eobj[j].warp == false && eobj[j].active == true && eobj[j].chaser == false)
                {
                    if(eobj[j].type == 1)
                    {
                        if(Physics.BoxCollision(sobj[i].x, sobj[i].y, P.sw, P.sh, eobj[j].x, eobj[j].y, E.bw, E.bh)) 
                        {
                            ShotDFlag[i] = 0;
                            eobj[j].hp--;
                            if(eobj[j].hp <= 0)
                            {
                                eobj[j].destroyed = true;
                                eobj[j].active = false;
                                tDestroyed++;
                                waveCheck = true;
                            }
                            
                        }
                    }
                    else if(Physics.BoxCollision(sobj[i].x, sobj[i].y, P.sw, P.sh, eobj[j].x, eobj[j].y, E.w, E.h)) 
                    {
                        ShotDFlag[i] = 0;
                        eobj[j].hp--;
                        if(eobj[j].hp <= 0)
                        {
                            eobj[j].destroyed = true;
                            eobj[j].active = false;
                            tDestroyed++;
                            waveCheck = true;
                        }
                    }
                    
                }
            }
        }   
    }
    //coll det - player
    if(Delta.Time(Last[0].Time,50)) bGrace = false;
    for(register uint_fast8_t i = 0; i < ENEMYSHOTMAX; i++)
    {
        register uint_fast8_t d = 2;
        if(eShotDFlag[i] != 0 && bGrace == false && P.jFlag != 1)
        {
            if(Physics.BoxCollision(esobj[i].x,esobj[i].y,E.sw,E.sh,P.px,P.py,P.pw,P.ph) && P.jFlag != 1)
            {
                if(eShotDFlag[i] == 2) d = d + 1;
                eShotDFlag[i] = 0;
                if(P.shp > 0) P.shp -= d;
                else P.hp -= d;
                if(P.shp < 0) P.shp = 0;
                if(P.hp <= 0) Scene.Change(GO_SCENE);
                bGrace = true;
            }
        }
    }
}

void Stage1::Draw()
{
    if(bgH != bgHc) bgH = LoadGraph(_T("core/xbg.png")); 
    if(sComplete) Ui.Fade(FADE_OUT,SPEED1);
    if(Ui_flytext.brightness != 0)
    {
        Ui_flytext.Fade(FADE_OUT,SPEED1);
        DrawString(Width/2-180,Height - Height/2 + 40,"HIGH VELOCITY SUB-LIGHT ENGINE ACTIVE.",GetColor(150,150,150));
        Ui_flytext.EndBlend();
    }

    //bg
    DrawGraph(0,0,bgH,TRUE);
    
    //player draw
    P.DrawFire(sobj,SHOTMAX,ShotDFlag);
    DrawRotaGraph(P.px,P.py,P.playerSize,P.rot,P.playerH[5],TRUE);
    if(P.shp > 3) DrawRotaGraph(P.px,P.py,P.shieldSize,P.shieldrot,P.shieldH[0],TRUE);
    if(Delta.Time(Last[1].Time,5))
    {
        sAnim++;
        if(sAnim > 1) sAnim = 0;
        P.shieldrot += PI/3;
    }
    Ui.DrawBar(P.px,P.py+50,P.hp,P.hpmax,P.playerSize);
    Ui.DrawBar(P.px,P.py+45,P.shp,P.shpmax,P.playerSize,60,80,Ui.White,GetColor(0,120,190));
    if(P.jFlag != 0) DrawBox(P.px-10,P.py-30,P.px+12-P.jGrace/8,P.py-29,Ui.White,TRUE);

    //enemy draw
    E.Draw(eobj,esobj,ENEMIES,ENEMYSHOTMAX,EnemyDFlag,eShotDFlag);

    if(debugflag)
    {
        Ui.DrawValue(0,20,(int)bgH);
        Physics.DrawHitBox(P.px,P.py,P.pw,P.ph);
        for(register uint_fast8_t i = 0; i < ENEMIES; i++)
        {
            if(eobj[i].active) Physics.DrawHitBox(eobj[i].x,eobj[i].y,E.w,E.h);
        }
        for(register uint_fast8_t i = 0; i < ENEMYSHOTMAX; i++)
        {
            if(eShotDFlag[i] == 1 || eShotDFlag[i] == 2) Physics.DrawHitBox(esobj[i].x,esobj[i].y,E.sw,E.sh);
        }
        for(register uint_fast8_t i = 0; i < ENEMYSHOTMAX; i++)
        { 
            if(ShotDFlag[i] == 1) Physics.DrawHitBox(sobj[i].x,sobj[i].y,P.sw,P.sh);
        }
    }
    
    if(sComplete) Ui.EndBlend();
    if(Ui.brightness <= 5) 
    {
        Ui.brightness = 255;
        Scene.Change(S2_SCENE);
    }


}
