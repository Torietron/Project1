#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include "SceneControl.h"
#include "SceneVirt.h"
#include "Interface.h"
#include "Stage2.h"
#include "KeyPoll.h"
#include "MousePoll.h"
#include "PlayerData.h"
#include "PhysicsData.h"
#include "Projectiles.h"

#define SHOTMAX 50
#define INITIAL_ASTEROIDS 25
#define ASTEROIDS 200 //total tracking space, 25*2*4
#define ENEMYSHOTMAX 150
#define ENEMIES 2

static uint_fast8_t ShotDFlag[SHOTMAX] = {0};
static Shotobj_t sobj[SHOTMAX];

static uint_fast8_t aDFlag[ASTEROIDS] = {0};
static Enemy_t aobj[ASTEROIDS];
static Enemy_t aobjFULL;
static Enemy_t aobjHALF;
static Enemy_t aobjQUAD;

static uint_fast8_t eDFlag[ENEMIES] = {0};
static Enemy_t eobj[ENEMIES];

static uint_fast8_t eShotDFlag[ENEMYSHOTMAX] = {0};
static Shotobj_t esobj[ENEMYSHOTMAX];

static const double PI = M_PI;
static bool waveCheck, sComplete, aGrace;
static uint_fast8_t dice1, dice2, dice3, count, sAnim = 0, wv = 0;
static int_fast16_t anchor;
static int_fast32_t random1, random2;
static int_fast64_t bgH;
static Shotobj_t rTriObj;
static LastDelta_t Last[4];
static uint_fast8_t n = 0;
static Interface Ui_flytext;

extern PlayerData P;
extern EnemyData E;
extern KeyPoll Key;
extern MousePoll Mouse;
extern SceneControl Scene;
extern Interface Ui;
extern PhysicsData Physics;
extern Delta_t Delta;
extern int_fast16_t Width, Height;
extern bool debugflag;

void Stage2::Init()
{
    //clearing old flags
    for(register uint_fast8_t i = 0; i < ENEMYSHOTMAX; i++)
    {
        if(eShotDFlag[i] != 0) eShotDFlag[i] = 0;
    }
    for (register uint_fast8_t i = 0; i < SHOTMAX; i++)
    {
        if(ShotDFlag[i] != 0) ShotDFlag[i] = 0;
    }

    //counters
    n = 0, wv = 0;

    Ui.brightness = 255;
    Ui_flytext.brightness = 255;
    Delta.Update();
    Last[1].Time = Delta.dTime;
    sComplete = false, waveCheck = false;
    P.px = Width/2, P.py = Height/2, P.mx = P.px, P.my = P.py;
    P.moved = 'N';
    P.rot = 0.00;
    P.hpmax = 60, P.shpmax = 7;
    P.hp = 60, P.shp = 7;
    rTriObj.size = 0.03;

    LoadDivGraph(_T("core/phship3.png"),9,3,3,96,120,P.playerH);
    LoadDivGraph(_T("core/pshield0.png"),2,2,1,150,145,P.shieldH);
    LoadDivGraph(_T("core/bt01.png"),3,2,2,58,90,P.playershotH);
    LoadDivGraph(_T("core/et01.png"),3,2,2,58,90,E.eShotH);
    P.playershot2H = LoadGraph(_T("core/bm01.png"));
    bgH = LoadGraph(_T("core/xbg.png"));
    P.TriH = LoadGraph(_T("core/rTri.png"));
    
    srand(static_cast<int_fast32_t>(time(0)));
    anchor = 240;

    //ast object templates 
    //whole state template
    LoadDivGraph(_T("core/ast0.png"),3,2,2,128,128,aobjFULL.enemyH);
    GetGraphSize(aobjFULL.enemyH[0],&aobjFULL.w,&aobjFULL.h); 
    aobjFULL.state = 3;
    aobjFULL.direction = 'R';
    aobjFULL.size = (E.size / E.sizeDiv) * 2.7; //2.7,1.5,0.8 sizes
    aobjFULL.rot += PI/16;
    aobjFULL.w = aobjFULL.size * 50; 
    aobjFULL.h = aobjFULL.size * 45;
    
    //half state template
    LoadDivGraph(_T("core/ast0.png"),3,2,2,128,128,aobjHALF.enemyH);
    GetGraphSize(aobjHALF.enemyH[0],&aobjHALF.w,&aobjHALF.h);
    aobjHALF.state = 2;
    aobjHALF.direction = 'L';
    aobjHALF.size = (E.size / E.sizeDiv) * 1.5;  // 0.8
    aobjHALF.rot -= PI/16;
    aobjHALF.w = aobjHALF.size * 50; 
    aobjHALF.h = aobjHALF.size * 45; 
     
    //quad state template
    LoadDivGraph(_T("core/ast0.png"),3,2,2,128,128,aobjQUAD.enemyH);
    GetGraphSize(aobjQUAD.enemyH[0],&aobjQUAD.w,&aobjQUAD.h);
    aobjQUAD.state = 1;
    aobjQUAD.direction = 'R';
    aobjQUAD.size = (E.size / E.sizeDiv) * 0.8;  // 0.8
    aobjQUAD.rot -= PI/16;
    aobjQUAD.w = aobjQUAD.size * 50; 
    aobjQUAD.h = aobjQUAD.size * 45;
      
    //asteroid init
    for(uint_fast8_t i = 0; i < ASTEROIDS; i++)
    {
        random1 = rand();
        random2 = rand();
        SRand(random1*random2);
        dice1 = GetRand(500) - 1;
        dice2 = GetRand(500) - 1;
        dice3 = GetRand(500) - 1;

        aobj[i].rot = 0; 
        aobj[i].state = 0;
        aobj[i].active = false;

        //set initial states 
        if(n < INITIAL_ASTEROIDS)
        {
            if(dice1 % 2 == 0)
            {
                //whole state
                aobj[i] = aobjFULL;
                aobj[i].pace = 32 - ((dice2 % 16) + 1);
                aDFlag[i] = 4;
                n++;
            }
            else
            {
                //half state
                aobj[i] = aobjHALF;
                aobj[i].pace = 32 - ((dice2 % 16) + 1);
                aDFlag[i] = 4;
                n++;
            }
            
            if(wv < 5)
            {
                aDFlag[i] = 3;
                aobj[i].active = true;
                wv++;
            }
            
        }
            
        //set location
        aobj[i].x = dice2 % Width;
        aobj[i].y = dice3 % Height;
        if(aobj[i].y >= Height/2) aobj[i].y = Height - Height + 40;
        if(aobj[i].y <= Height/2) aobj[i].y = Height +  40;

        //set float vel
        aobj[i].tx = (dice2 % 5) + 1; //7
        aobj[i].ty = (dice3 % 4) + 1; //6
        aobj[i].fx = aobj[i].tx;
        aobj[i].fy = aobj[i].ty;

        aobj[i].destroyed = false;
        aobj[i].degrade = false;
        if(aobj[i].state <= 0) aobj[i].state = 0;
    }

    //enemy init
    for(uint_fast8_t i = 0; i < ENEMIES ; i++)
    {
        //double pass gen
        random1 = rand();
        SRand(random1);
        dice1 - GetRand(2) - 1;
        if (dice1 % 2 == 0) LoadDivGraph(_T("core/eship0a.png"),6,3,2,128,126,eobj[i].enemyH);
        else LoadDivGraph(_T("core/eship0b.png"),6,3,2,128,126,eobj[i].enemyH);
        dice1 = GetRand(100);

        eobj[i].charge = 0;
        eobj[i].magnitude = dice1 + 100;
        eobj[i].hp = 8, eobj[i].hpmax = eobj[i].hp;
        eobj[i].tx = -400, eobj[i].ty = -400;
        eobj[i].ax = anchor, eobj[i].x = anchor;
        eobj[i].ay = 90;
        eobj[i].y = eobj[i].ay - 600;
        eobj[i].pace = 0;
        eobj[i].type = 0;
        eobj[i].cycletime = dice1;
        eobj[i].vel = E.vel - dice1 - 70;
        eobj[i].rot = 0;
        eobj[i].size = E.size / E.sizeDiv;
        eobj[i].frame = 0;

        eobj[i].active = false;
        eobj[i].warp = false;
        eobj[i].destroyed = false;
        eobj[i].chaser = false;
        eobj[i].edge = false;

        if(dice1 % 2 == 0) eobj[i].direction = 'R';
        else eobj[i].direction = 'L';

        anchor = GetRand(300) + 100;
    }


    //coll init
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

    E.sw = E.sw * (E.shotSize *0.3);
    E.sh = E.sh * (E.shotSize *0.3);

    P.pw = P.pw * (P.playerSize *0.35);
    P.ph = P.ph * (P.playerSize *0.30);

    //enemy ship time trigger
    Last[1].Flag = true; //will set last2 on last0 enemy ship obj destruction
    //aobj[0].active = true;
    //aobj[0].state = 3;

}

void Stage2::End()
{
    DeleteGraph((int)P.playerH);
    DeleteGraph((int)P.playershotH);
}

void Stage2::Update()
{
    Key.Update();
    Mouse.Update();
    Delta.Update();
    P.Update(1);
    E.Update(0,eobj,ENEMIES,eDFlag);
    /*
    if(eobj[0].active) 
    {
        eobj[0].rotx = eobj[0].x;
        Physics.ApproxAngle(eobj[0].rot, eobj[0].rotx, eobj[0].y, P.px, P.py, 0.50, 1, 1420);
    }
    if(eobj[1].active) 
    {
        eobj[1].rotx = eobj[1].x;
        Physics.ApproxAngle(eobj[1].rot, eobj[1].rotx, eobj[1].y, P.px, P.py);
    }
    */
    E.Update(1,aobj,ASTEROIDS,aDFlag);
    E.Fire(eobj,esobj,P,ENEMIES,ENEMYSHOTMAX,eShotDFlag);

    if(Delta.Time(Last[1].Time, 20000) && Last[1].Flag || Last[2].Flag) 
    {
        if(Last[1].Flag)
        {
            eobj[0].warp = true;
            eDFlag[0] = 1;
            Last[1].Flag = false;
        }
        else
        {
            eobj[1].warp = true;
            eDFlag[1] = 1;
            Last[2].Flag = false;
        }
       
    }

    if((Mouse.Poll[MOUSE_INPUT_LEFT] >= 1 || Key.Poll[KEY_INPUT_SPACE] >= 1 || Key.Poll[KEY_INPUT_Z] >= 1) && P.charge >= 3)
    {
        P.CreateFire(0,sobj,SHOTMAX,ShotDFlag);
    }

    //coll det - ast
    for(register uint_fast8_t i = 0; i < SHOTMAX; i++)
    {
        if(ShotDFlag[i] == 1 || ShotDFlag[i] == 2)
        {
            for(register uint_fast8_t j = 0; j < ASTEROIDS; j++)
            {
                if(aobj[j].active == true && Physics.BoxCollision(sobj[i].x,sobj[i].y,P.sw,P.sh,aobj[j].x,aobj[j].y,aobj[j].w,aobj[j].h))
                {
                    aobj[j].destroyed = true;
                    aobj[j].degrade = true;
                    ShotDFlag[i] = 0;
                }
            }
        }
    }

    //coll det - enemy
    for(register uint_fast8_t i = 0; i < SHOTMAX; i++)
    {
        if(ShotDFlag[i] == 1)
        {
            for(register uint_fast8_t j = 0; j < ENEMIES; j++)
            {
                if(eobj[j].warp == false && eobj[j].active == true && eobj[j].chaser == false)
                {
                        if(Physics.BoxCollision(sobj[i].x, sobj[i].y, P.sw, P.sh, eobj[j].x, eobj[j].y, E.w, E.h)) 
                        {
                            ShotDFlag[i] = 0;
                            eobj[j].hp--;
                            if(eobj[j].hp <= 0)
                            {
                                eobj[j].destroyed = true;
                                eobj[j].active = false;
                                waveCheck = true;
                            }
                            
                        }            
                }
            }
        }   
    }

    //coll det - player
    if(Delta.Time(Last[0].Time,500)) aGrace = false;
    for(register uint_fast8_t i = 0; i < ASTEROIDS; i++)
    {
        register uint_fast8_t d = 6, d2 = 0;
        if(aobj[i].active && aDFlag[i] == 3)
        {
            if(aGrace == false && P.jFlag !=1 && Physics.BoxCollision(aobj[i].x,aobj[i].y,aobj[i].w,aobj[i].h,P.px,P.py,P.pw,P.ph))
            {
                d2 = P.shp - d;
                if(P.shp > 0) P.shp -= d;
                else P.hp -= d;
                if(P.shp < 0)
                {
                    P.hp += d2;
                    P.shp = 0;
                }
                aGrace = true;
                if(P.hp <= 0)
                if(debugflag) DrawString(0,350,"HIT",Ui.White);
            }
        }   
    }
    for(register uint_fast8_t i = 0; i < ENEMYSHOTMAX; i++)
    {
        register uint_fast8_t d = 1;
        if(eShotDFlag[i] != 0)
        {
            if(Physics.BoxCollision(esobj[i].x,esobj[i].y,E.sw,E.sh,P.px,P.py,P.pw,P.ph) && P.jFlag != 1)
            {
                if(eShotDFlag[i] == 2) d = d + 2;
                eShotDFlag[i] = 0;
                if(P.shp > 0) P.shp -= d;
                else P.hp -= d;
                if(P.shp < 0) P.shp = 0;
                if(P.hp <= 0) P.hp = 0;
            }
        }
    }
    
    //check for end conditions
    //all objects destroyed
    count = 0;
    if(eobj[0].active || eobj[1].active) count++;
    for(register uint_fast8_t i = 0; i < ASTEROIDS; i++)
    {
        if(aobj[i].active) count++;
    }
    if(count == 0) Scene.Change(W_SCENE);
    
    //player is out of hp
    if(P.hp <= 0) Scene.Change(GO_SCENE);
}

void Stage2::Draw()
{
    if(Ui_flytext.brightness != 0)
    {
        Ui_flytext.Fade(FADE_OUT,SPEED1);
        DrawString(Width/2-180,Height - Height/2 + 40,"OMNI-DIRECTIONAL THRUST MODE ACTIVE.",GetColor(150,150,150));
        Ui_flytext.EndBlend();
    }

    //bg
    DrawGraph(0,0,bgH,TRUE);

    //player draw
    P.DrawFire(sobj,SHOTMAX,ShotDFlag);
    P.DrawAim(rTriObj,P.TriH,31);
    DrawRotaGraph(P.px,P.py,P.playerSize,P.rot,P.playerH[5],TRUE);
    if(P.shp > 3) DrawRotaGraph(P.px,P.py,P.shieldSize,P.shieldrot,P.shieldH[sAnim],TRUE);
    if(Delta.Time(Last[3].Time,5))
    {
        sAnim++;
        if(sAnim > 1) sAnim = 0;
        P.shieldrot += PI/3;
    }
    Ui.DrawBar(P.px,P.py+50,P.hp,P.hpmax,P.playerSize);
    Ui.DrawBar(P.px,P.py+45,P.shp,P.shpmax,P.playerSize,60,80,Ui.White,GetColor(0,120,190));
    if(P.jFlag != 0) DrawBox(P.px-10,P.py-30,P.px+12-P.jGrace/8,P.py-29,Ui.White,TRUE);

    //target draw
    E.Draw(aobj,ASTEROIDS,aDFlag);
    E.Draw(eobj,esobj,ENEMIES,ENEMYSHOTMAX,eDFlag,eShotDFlag);

    //debug
    if(debugflag)
    {
        Ui.DrawValue(0,40,aobj[3].state);
        Ui.DrawValue(0,80,aDFlag[3]);
        int xx = 0;
        if(aobj[3].active == true) xx = 1;
        Ui.DrawValue(0,120,xx);
        Ui.DrawValue(0,160,aobj[3].x);
        Ui.DrawValue(0,180,aobj[3].y);
        int yy = 0;
        if(aobj[3].destroyed == true) yy = 1;
        Ui.DrawValue(0,200,yy);
        Physics.DrawHitBox(P.px,P.py,P.pw,P.ph);
        for(register uint_fast8_t i = 0; i < ASTEROIDS; i++)
        {
            if(aobj[i].active) Physics.DrawHitBox(aobj[i].x,aobj[i].y,aobj[i].w,aobj[i].h);
        }
    }
}
