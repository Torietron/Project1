#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include "PlayerData.h"
#include "Projectiles.h"
#include "PhysicsData.h"
#include "KeyPoll.h"
#include "MousePoll.h"

static const double PI = M_PI;
static uint_fast8_t shiftFlag = 0;
static uint_fast32_t mTime, vTime, hTime, jTime, jDir;
static LastDelta_t Last[3];

extern uint_fast32_t sTime, dTime;
extern int_fast16_t Width, Height;
extern KeyPoll Key;
extern MousePoll Mouse;
extern PhysicsData Physics;
extern Delta_t Delta;

PlayerData::PlayerData()
{
    charge = 0, jFlag = 0, jGrace = 0;
    vel_x = 0.00, vel_y = 0.00, vMax = 2, vBase = 0, accel = 0.00;
    px = 0, py = 0, mx = px, my = py, lx = 0;
    pw = 0, ph = 0, sw = 0, sh = 0;
    rot = 0.00, shotSize = 0.20, playerSize = 0.25, shieldSize =0.25;
    shieldrot = rot;
    moved = 'N';
}

void PlayerData::Update(uint_fast8_t a) //int for movement type/style
{
    switch(a)
    {
        case 0:
            
            dTime = GetNowCount();
            if(shp >= shpmax)
            {
                shp = shpmax;
                hTime = GetNowCount();
            }
            if(dTime - hTime > 1500 && shp < shpmax)
            {
                shp++;
                if(shp >= 5) shp++;
                hTime = GetNowCount();
            }
            if(dTime - sTime > 50 && charge < 3 )
            {
                charge++;
                sTime = GetNowCount();
            }
            if(dTime - jTime > 20 && jFlag == 1)
            {
                if(jGrace >= 25) jFlag = 2;
                else jGrace++;
                if(jDir == 0) px -= 30/jGrace+1;
                else px += 30/jGrace+1;
                jTime = GetNowCount();
            }
            else jGrace++;
            if(dTime - jTime > 2570 && jFlag == 2)
            {
        
                jFlag = 0;
            }

            opx = px; 
            opy = py;

            if(Key.Poll[KEY_INPUT_A] >= 1 || Key.Poll[KEY_INPUT_LEFT] >= 1) px -= 4;
            if(Key.Poll[KEY_INPUT_D] >= 1 || Key.Poll[KEY_INPUT_RIGHT] >= 1) px += 4;
            if(Key.Poll[KEY_INPUT_W] >= 1 || Key.Poll[KEY_INPUT_UP] >= 1) py -= 5;
            if(Key.Poll[KEY_INPUT_S] >= 1 || Key.Poll[KEY_INPUT_DOWN] >= 1) py += 5;
            if((Key.Poll[KEY_INPUT_L] >= 1 || Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && (Key.Poll[KEY_INPUT_A] >= 1 || Key.Poll[KEY_INPUT_LEFT] >= 1) && jFlag == 0) 
            {
                px -= 6;
                jFlag = 1;
                jDir = 0;
                jGrace = 0;
            }
            if((Key.Poll[KEY_INPUT_L] >= 1 || Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && (Key.Poll[KEY_INPUT_D] >= 1 || Key.Poll[KEY_INPUT_RIGHT] >= 1) && jFlag == 0)
            {
                px += 6;
                jFlag = 1;
                jDir = 1;
                jGrace = 0;
            }
            if(py < Height - Height + Height *0.42) py = Height - Height + Height *0.42;
            if(py > Height - Height/6) py = Height - Height/6;
            if(px < Width - Width + 15) px = Width - Width + 15;
            if(px > Width - 15) px = Width - 15; //img edge 15px x, 20px y

            break;

        case 1:
            dTime = GetNowCount();
            if(shp >= shpmax)
            {
                shp = shpmax;
            }
            if(Delta.Time(Last[1].Time,1500) && shp < shpmax)
            {
                shp++;
                if(shp >= 5) shp++;
            }
            if(dTime - jTime > 20 && jFlag == 1)
            {
                if(jGrace >= 25) jFlag = 2;
                else jGrace++;
                switch(jDir)
                {
                    case 0:
                        px -=9/jGrace+1;
                        py -=9/jGrace+1;
                        break;
                    case 1:
                        px +=9/jGrace+1;
                        py -=9/jGrace+1;
                        break;
                    case 2:
                        px -=9/jGrace+1;
                        py +=9/jGrace+1;
                        break;
                    case 3:
                        px +=9/jGrace+1;
                        py +=9/jGrace+1;
                        break;
                    case 4:
                        py -= 18/jGrace+1;
                        break;
                    case 5:
                        py += 18/jGrace+1;
                        break;
                    case 6:
                        px -= 18/jGrace+1;
                        break;
                    case 7:
                        px += 18/jGrace+1;
                        break;
                }
                //else px += 30/jGrace+1;
                jTime = GetNowCount();
            }
            else jGrace++;
            if(dTime - jTime > 2570 && jFlag == 2)
            {
                jFlag = 0;
            }

            //rot update
            lx = mx;
            Key.Update();
            if(Delta.Time(Last[2].Time,35) && charge < 3)
            {
                charge++;
            }
            opx = px, opy = py;

            SetMousePoint(px,py);
            WaitTimer(5);
            GetMousePoint(&mx,&my);
            if(mx == lx) moved = 'N';
            else
            {
                SetMousePoint(px,py);
                WaitTimer(5);
                GetMousePoint(&mx,&my);
                if(mx==lx) moved = 'N';
                else
                {
                    if(mx / 2 > px / 2 && mx != px) moved = 'R';
                    if(mx / 2 < px / 2 && mx != px) moved = 'L';
                }   
            }
            if(moved == 'R') rot += PI/32;
            if(mx - px > 8) rot += PI/28;
            if(moved == 'L') rot -= PI/32;
            if(mx - px < -8) rot -= PI/28;
            if(Key.Poll[KEY_INPUT_K] >= 1 || Key.Poll[KEY_INPUT_C] >= 1) rot += PI/32;
            if(Key.Poll[KEY_INPUT_J] >= 1 || Key.Poll[KEY_INPUT_X] >= 1) rot -= PI/32;
            moved = 'N';

            if(Key.Poll[KEY_INPUT_A] >= 1 || Key.Poll[KEY_INPUT_LEFT] >= 1)
            {
                Physics.Accelerate(vel_x,vBase,vMax,accel);
                vel_x = vel_x * -1; //convert to left
                px += vel_x;
                accel = accel + 0.25;
            }

            if(Key.Poll[KEY_INPUT_D] >= 1 || Key.Poll[KEY_INPUT_RIGHT] >= 1) 
            {
                Physics.Accelerate(vel_x,vBase,vMax,accel);
                px += vel_x;
                accel = accel + 0.25;
            }

            if(Key.Poll[KEY_INPUT_W] >= 1 || Key.Poll[KEY_INPUT_UP] >= 1)
            {
                Physics.Accelerate(vel_y,vBase,vMax,accel);
                vel_y = vel_y * -1; //convert to up
                py += vel_y;
                accel = accel + 0.25;
            }

            if(Key.Poll[KEY_INPUT_S] >= 1 || Key.Poll[KEY_INPUT_DOWN] >= 1) 
            {
                Physics.Accelerate(vel_y,vBase,vMax,accel);
                py += vel_y;
                accel = accel + 0.25;
            }

            if(Key.Poll[KEY_INPUT_A] == 0 && Key.Poll[KEY_INPUT_D] == 0 
            && Key.Poll[KEY_INPUT_W] == 0 && Key.Poll[KEY_INPUT_S] == 0 
            && Key.Poll[KEY_INPUT_LEFT] == 0 && Key.Poll[KEY_INPUT_RIGHT] == 0 
            && Key.Poll[KEY_INPUT_UP] == 0 && Key.Poll[KEY_INPUT_DOWN] == 0)
            {
                accel = 0.00;           
            }

            //8 direction jump 
            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_W] >= 1 && Key.Poll[KEY_INPUT_A] >= 1 && jFlag == 0)
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_UP] >= 1 && Key.Poll[KEY_INPUT_LEFT] >= 1 && jFlag == 0))
            {
                px -=20;
                py -=20;
                jFlag = 1;
                jDir = 0;
                jGrace = 0;
            }
            
            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_W] >= 1 && Key.Poll[KEY_INPUT_D] >= 1 && jFlag == 0)
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_UP] >= 1 && Key.Poll[KEY_INPUT_RIGHT] >= 1 && jFlag == 0))
            {
                px +=20;
                py -=20;
                jFlag = 1;
                jDir = 1;
                jGrace = 0;
            }
            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_S] >= 1 && Key.Poll[KEY_INPUT_A] >= 1 && jFlag == 0)
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_DOWN] >= 1 && Key.Poll[KEY_INPUT_LEFT] >= 1 && jFlag == 0))
            {
                px -=20;
                py +=20;
                jFlag = 1;
                jDir = 2;
                jGrace = 0;
            }
            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_S] >= 1 && Key.Poll[KEY_INPUT_D] >= 1 && jFlag == 0)
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_DOWN] >= 1 && Key.Poll[KEY_INPUT_RIGHT] >= 1 && jFlag == 0))
            {
                px +=20;
                py +=20;
                jFlag = 1;
                jDir = 3;
                jGrace = 0;
            }

            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_W] >= 1 && jFlag == 0) 
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_UP] >= 1 && jFlag == 0))
            {
                py -= 60;
                jFlag = 1;
                jDir = 4;
                jGrace = 0;
            }
            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_S] >= 1 && jFlag == 0)
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_DOWN] >= 1 && jFlag == 0))
            {
                py += 60;
                jFlag = 1;
                jDir = 5;
                jGrace = 0;
            }
            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_A] >= 1 && jFlag == 0) 
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_LEFT] >= 1 && jFlag == 0))
            {
                px -= 60;
                jFlag = 1;
                jDir = 6;
                jGrace = 0;
            }
            if(((Key.Poll[KEY_INPUT_L] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_D] >= 1 && jFlag == 0)
            || ((Key.Poll[KEY_INPUT_V] >= 1 || (Mouse.Poll[MOUSE_INPUT_RIGHT] >= 1 || Mouse.Poll[MOUSE_INPUT_LEFT + MOUSE_INPUT_RIGHT] >= 1)) && Key.Poll[KEY_INPUT_RIGHT] >= 1 && jFlag == 0))
            {
                px += 60;
                jFlag = 1;
                jDir = 7;
                jGrace = 0;
            }
            
            Physics.Manipulate(px,py,vel_x,vel_y,Last[0].Time,520,1,1);
            
            if(py < Height - Height + 20) py = Height - Height + 20;
            if(py > Height - 20) py = Height - 20;
            if(px < Width - Width + 15) px = Width - Width + 15;
            if(px > Width - 15) px = Width - 15; //img edge 15px x, 20px y

            break;

        case 2:
            //ASK SANTA or Jesus or both
            break;
    }
}

void PlayerData::CreateFire(uint_fast8_t type, Shotobj_t *sobj, const uint_fast8_t MAX, uint_fast8_t *sFlag)
{
    switch(type)
    {
        case 0:
            for(uint_fast8_t i = 0; i < MAX; i++)
            {
                if(sFlag[i] == 0)
                {
                    sobj[i].x = this->px;
                    sobj[i].y = this->py;
                    sobj[i].angle = this->rot;
                    sFlag[i] = 1;
                    this->charge = 0;
                    break;
                }
            }
            break;
        case 1:
            for(uint_fast8_t i = 0; i < MAX; i++)
            {
                if(sFlag[i] == 0)
                {
                    sobj[i].x = this->px;
                    sobj[i].y = this->py;
                    sFlag[i] = 2;
                    this->charge = 0;
                    break;
                }
            }
            break;
    }
}

void PlayerData::DrawFire(Shotobj_t *sobj, const uint_fast8_t MAX, uint_fast8_t *sFlag)
{
    for(uint_fast8_t i = 0; i < MAX; i++)
    {
        if(sFlag[i] == 1)
        {
            Physics.Propel(sobj[i].x,sobj[i].y,sobj[i].angle,8);
            {
                if(sobj[i].y < - (Height*1.02) || sobj[i].y > (Height*1.02)) sFlag[i] = 0;
                if(sobj[i].x < - (Width*1.02) || sobj[i].x > (Width*1.2)) sFlag[i] = 0;
            }
            DrawRotaGraphF(sobj[i].x,sobj[i].y,shotSize,sobj[i].angle,this->playershotH[2],TRUE);
        }
        if(sFlag[i] == 2)
        {
            sobj[i].y += 6;
            {
                if(sobj[i].y > (Height*1.02)) sFlag[i] == 0;
            }
            DrawRotaGraphF(sobj[i].x,sobj[i].y,shotSize/4,sobj[i].angle,this->playershot2H,TRUE);
        }
    }
}

void PlayerData::DrawAim(Shotobj_t &aimObj, int_fast32_t imgH, uint_fast16_t distance)
{
    Physics.AnchoredAngle(px,py,rot,aimObj.x,aimObj.y,aimObj.rot,distance);
    DrawRotaGraph(aimObj.x,aimObj.y,aimObj.size,aimObj.rot,imgH,TRUE);
}