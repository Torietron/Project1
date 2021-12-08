#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "EnemyData.h"
#include "PlayerData.h"
#include "Interface.h"
#include "PhysicsData.h"

static const double PI = M_PI;
static uint_fast8_t dice1, dice2, dice3;
static uint_fast8_t counter = 0;
static int_fast32_t random;
static uint_fast8_t xFlag[15] = {0}; 
static LastDelta_t Last[200];  
static LastDelta_t eLast[15];   

extern int_fast16_t Width, Height;
extern Interface Ui;
extern PhysicsData Physics;
extern Delta_t Delta;

EnemyData::EnemyData()
{
    x = 0, y = 0, vel = 350;
    rot = PI, size = 0.85; sizeDiv = 3;
    shotSize = 0.12;
}

void EnemyData::Update(uint_fast8_t type, Enemy_t *eobj, const uint_fast8_t MAX, uint_fast8_t *eFlag)
{
    srand(static_cast<int_fast32_t>(time(0)));
    switch(type) //param for logic type
    {
        case 0:
            for(uint_fast8_t i = 0; i < MAX; i++)
            {
                //entry state
                {
                    if(eobj[i].warp == true)
                    {
                        if(eobj[i].ay - eobj[i].y > 70) eobj[i].y += 12;
                        if(eobj[i].ay - eobj[i].y > 50) eobj[i].y += 4;
                        if(eobj[i].ay - eobj[i].y < 50) eobj[i].y += 2;
                        if(eobj[i].ay - eobj[i].y < 15) eobj[i].y += 1;
                        if(eobj[8].warp == true) eobj[8].y -= 1;
                    }
                    if(eobj[i].chaser == true)
                    {
                        if(eobj[i].ay - eobj[i].y < 70) eobj[i].y -= 12;
                        if(eobj[i].ay - eobj[i].y < 50) eobj[i].y -= 4;
                        if(eobj[i].ay - eobj[i].y > 50) eobj[i].y -= 2;
                        if(eobj[i].ay - eobj[i].y > 15) eobj[i].y -= 1;
                    }
                }

                //transitional state, entry exit
                {
                    if(eobj[i].y >= eobj[i].ay && eobj[i].warp == true) 
                    {
                        eobj[i].y = eobj[i].ay;
                        eobj[i].warp = false;
                        eobj[i].active = true;
                    }
                    if(eobj[i].y <= eobj[i].ay && eobj[i].chaser == true) 
                    {
                        eobj[i].y = eobj[i].ay;
                        eobj[i].chaser = false;
                        eobj[i].active = true;
                    }
                }

                //active state
                if(eobj[i].active == true)
                {
                    eobj[i].cycletime++;
                    eobj[i].pace++;
                    if(eobj[i].charge < 10)
                    {
                        eobj[i].charge++;
                        eobj[i].charge2++;
                    }

                    if(eobj[i].direction == 'R') eobj[i].x = eobj[i].ax + sin(PI*2/eobj[i].vel*eobj[i].pace)*eobj[i].magnitude;
                    if(eobj[i].direction == 'L') eobj[i].x = eobj[i].ax - sin(PI*2/eobj[i].vel*eobj[i].pace)*eobj[i].magnitude;
                    if(eobj[i].x < 0+30) 
                    {
                        eobj[i].direction = 'R';
                        eobj[i].tx = eobj[i].ax;
                        eobj[i].ax = eobj[i].x;
                        eobj[i].pace = 0;
                        eobj[i].magnitude = 500;
    
                    }
                    if(eobj[i].x > Width-30)
                    {
                        eobj[i].direction = 'L';
                        eobj[i].tx = eobj[i].ax;
                        eobj[i].ax = eobj[i].x;
                        eobj[i].pace = 0;
                        eobj[i].magnitude = 500;
                    }
                    if(eobj[i].cycletime > 150)
                    {
                        eobj[i].cycletime = 0;
                        if(eobj[i].direction == 'L') eobj[i].direction = 'R';
                        else if(eobj[i].direction == 'R') eobj[i].direction = 'L';
                        eobj[i].ax = eobj[i].x;
                        eobj[i].magnitude = 5000;
                        eobj[i].pace = 0;
                    }
                    if(eobj[i].cycletime > 40 && eobj[i].rot != PI*2 && eobj[i].edge == false)
                    {
                        random = rand();
                        SRand(random);
                        dice1 = GetRand(3) + 1;
                        dice2 = GetRand(2) - 1;
                        if(dice2 % 2 == 0) eobj[i].y = eobj[i].y-1 + dice1;
                        else eobj[i].y = eobj[i].y+2 - dice1;
                        if(eobj[i].y > eobj[i].ay + 20) eobj[i].y = eobj[i].ay + 20;
                        if(eobj[i].y < Height - Height + 33 && eobj[i].edge == false) 
                        {
                            eobj[i].y = Height - Height + 33;
                            eobj[i].edge = true;
                        }
                    }
                    if(eobj[i].cycletime > 90 && eobj[i].rot != 0 && eobj[i].edge == false)
                    {
                        random = rand();
                        SRand(random);
                        dice1 = GetRand(0) + 1;
                        dice2 = GetRand(2) - 1;
                        if(dice2 % 2 == 0) eobj[i].y = eobj[i].y + dice1;
                        else eobj[i].y = eobj[i].y - dice1;
                        if(eobj[i].y > eobj[i].ay + 20) eobj[i].y = eobj[i].ay + 20;
                        if(eobj[i].y < Height - Height + 45 && eobj[i].edge == false) 
                        {
                            eobj[i].y = Height - Height + 45;
                            eobj[i].edge = true;
                        }
                    }
                    if(eobj[i].edge)
                    {
                        eobj[i].y = eobj[i].y + 1;
                        if(eobj[i].y > Height - Height + 103) eobj[i].edge = false;
                    }
                }

                //destroyed state
                if(eobj[i].destroyed == true)
                {
                    if(eobj[i].frame < 6 && eobj[i].type != 1)
                    {
                        if(Delta.Time(eLast[i].Time, 60)) eobj[i].frame++;
                        if(eobj[i].frame >= 6) eFlag[i] = 0;
                    }
                    else if(eobj[i].frame < 6 && eobj[i].type == 1)
                    {
                        if(Delta.Time(eLast[i].Time, 170)) eobj[i].frame++;
                        if(eobj[i].frame >= 6) eFlag[i] = 0;
                    }
                }
                
            }
            break;
        case 1:
            //ast state, integral downgrade
            for(uint_fast8_t i = 0; i < MAX; i++)
            {
                //state machine
                if(eobj[i].state == 3 && eobj[i].destroyed && eobj[i].degrade == true)
                {
                    //degrade
                    eobj[i].state--;
                    eobj[i].direction = 'L';
                    eobj[i].size = (size / sizeDiv) * 1.5;
                    eobj[i].rot -= PI/16;
                    eobj[i].w = eobj[i].size * 50; 
                    eobj[i].h = eobj[i].size * 45; 

                    //for adjustments
                    int_fast8_t offsetX = 18;
                    int_fast8_t offsetY = 2;
                    double tempRot = eobj[i].rot;
                    random = rand();
                    SRand(random);
                    dice1 = GetRand(500) - 1;
                    dice2 = GetRand(500) - 1;
                    dice3 = GetRand(500) - 1;

                    //create and adjust twins
                    for(uint_fast8_t j = 0; j < MAX; j++)
                    {
                        if(eFlag[j] == 0)
                        {
                            eobj[j] = eobj[i];
                            eobj[j].x = eobj[i].x - offsetX;
                            eobj[j].rot -= PI/16;
                            eobj[j].tx = (dice1 % 5) + 1; //7
                            eobj[j].ty = (dice2 % 4) + 1; //6
                            eFlag[j] = 3;
                            eobj[j].destroyed = false;
                            eobj[j].degrade = false;
                            eobj[j].active = true;
                            break;
                        }
                    }
                    eobj[i].tx = (dice3 % 5) + 1; //7
                    eobj[i].ty = (dice2 % 4) + 1; //6
                    eobj[i].rot += PI/16;
                    eobj[i].x = eobj[i].x + offsetX;
                    eobj[i].destroyed = false; 
                    eobj[i].degrade = false;
                }

                if(eobj[i].state == 2 && eobj[i].destroyed && eobj[i].degrade == true)
                {
                    //degrade
                    eobj[i].state--;
                    eobj[i].direction = 'R';
                    eobj[i].size = (size / sizeDiv) * 0.8;
                    eobj[i].rot -= PI/16;
                    eobj[i].w = eobj[i].size * 50; 
                    eobj[i].h = eobj[i].size * 45; 

                    //for adjustments
                    int_fast16_t CornerX[4] = {eobj[i].x - eobj[i].w, eobj[i].x + eobj[i].w, eobj[i].x - eobj[i].w, eobj[i].x + eobj[i].w}; //TopLeft to TopRight, to BotLeft, BotRight
                    int_fast16_t CornerY[4] = {eobj[i].y - eobj[i].h, eobj[i].y - eobj[i].h, eobj[i].y + eobj[i].h, eobj[i].y + eobj[i].h}; 
                    int_fast8_t num = 1;
                    double tempRot = eobj[i].rot;
                    random = rand();
                    SRand(random);
                    dice3 = GetRand(500) - 1;

                    //create and adjust quadruplets
                    for(uint_fast8_t j = 0; j < MAX; j++)
                    {
                        if(eFlag[j] == 0)
                        {
                            dice1 = GetRand(500) - 1;
                            dice2 = GetRand(500) - 1;
                            tempRot -= PI/16;
                            eobj[j] = eobj[i];
                            eobj[j].x = CornerX[num];
                            eobj[j].y = CornerY[num];
                            eFlag[j] = 3;
                            eobj[j].destroyed = false;
                            eobj[j].degrade = false;
                            eobj[j].active = true;
                            eobj[j].rot -= tempRot;
                            eobj[j].tx = (dice1 % 5) + 1;
                            eobj[j].ty = (dice2 % 4) + 1;
                            num++;
                            if(num >= 4) break;
                        }
                    }
                    eobj[i].tx = (dice2 % 7) + 1;
                    eobj[i].ty = (dice3 % 6) + 1;
                    eobj[i].x = CornerX[0];
                    eobj[i].y = CornerY[0];
                    eobj[i].destroyed = false; 
                    eobj[i].degrade = false;
                }
                //remove a state 1 obj, add to counter for spawning next (after 4 state 1s)
                if(eobj[i].state <= 1 && eobj[i].destroyed && eobj[i].active)
                {
                    eFlag[i] = 0;
                    eobj[i].active = false;
                    counter++;
                    bool instantbreak = false;
                    if(counter >= 4)
                    {
                        counter = 0;
                        for(uint_fast8_t j = 0; j < MAX; j++)
                        {
                            if(instantbreak == true) break;
                            if(eFlag[j] == 4 && instantbreak == false )
                            {
                                eFlag[j] = 3;
                                eobj[j].active = true;
                                eobj[j].y = -90;
                                instantbreak = true;
                            }
                        }
                    }
                }

                //screen bounds
                if(eobj[i].x > Width + 50) eobj[i].x = -50;
                if(eobj[i].x < Width - Width - 50) eobj[i].x = 690;
                if(eobj[i].y > Height + 100) eobj[i].y = -50;
                if(eobj[i].y < Height - Height - 100) eobj[i].y = -100;

                //update active
                if(eobj[i].active) 
                {
                    Physics.Spin(eobj[i].rot,eobj[i].direction,eobj[i].pace);
                    Physics.Manipulate(eobj[i].x,eobj[i].y,eobj[i].fx,eobj[i].fy,Last[i].Time);
                    //fx/ty for vel/velchanges, reset&check for velocity changes
                    eobj[i].fx = eobj[i].tx;
                    eobj[i].fy = eobj[i].ty;
                }
               
            }

            break;
    }
    
}

void EnemyData::Fire(Enemy_t *eobj, Shotobj_t *sobj, PlayerData &P, const uint_fast8_t eMAX, const uint_fast8_t sMAX, uint_fast8_t *sFlag)
{
    srand(static_cast<int_fast32_t>(time(0)));  
    random = rand();
    SRand(random);
    dice1 = GetRand(50);
    for(uint_fast8_t i = 0; i < eMAX; i++)
    {
        dice1 = GetRand(50);
        if(eobj[i].active && dice1 > 20)
        { 
            xFlag[i] = 1;
        }
    }

    for(uint_fast8_t i = 0; i < eMAX; i++)
    {
        if(eobj[i].active && xFlag[i] == 1 && eobj[i].charge >= 7)
        {
            for(uint_fast8_t j = 0; j < sMAX; j++)
            {
                if(sFlag[j] == 0 && eobj[i].hpmax < 10)
                {
                    sobj[j].x = eobj[i].x, sobj[j].y = eobj[i].y;
                    sFlag[j] = 1;
                    xFlag[i] = 0;
                    sobj[j].sx = P.px - eobj[i].x, sobj[j].sy = P.py - eobj[i].y;
                    sobj[j].divisor = 60;
                    sobj[j].size = shotSize;
                    if(eobj[i].rot != PI*2) sobj[j].angle = eobj[i].rot;
                    else sobj[j].angle = PI;
                    sobj[j].rotx = eobj[i].x;

                    if(sobj[j].sx < 0) Physics.Standardize(sobj[j].sx, -700, -10);
                    if(sobj[j].sx > 0) Physics.Standardize(sobj[j].sx, 10, 700);

                    if(sobj[j].sy < 0) Physics.Standardize(sobj[j].sy, -350, -200);
                    if(sobj[j].sy > 0) Physics.Standardize(sobj[j].sy, 200, 350);
                    eobj[i].charge = 0;

                    sobj[j].angle = Physics.ApproxAngle(sobj[j].angle, sobj[j].rotx, sobj[j].y, P.px, P.py);
                    break;         
                }

                if(sFlag[j] == 0 && eobj[i].hpmax > 10)
                {
                dice2 = GetRand(50);
                    if(dice2 < 25)
                    {
                        sobj[j].x = eobj[i].x-10, sobj[j].y = eobj[i].y;
                        sobj[j+1].x = eobj[i].x+10, sobj[j+1].y = eobj[i].y;
                        sFlag[j] = 1, sFlag[j+1] = 1;
                        xFlag[i] = 0;
                        sobj[j].sx = P.px - eobj[i].x, sobj[j].sy = P.py - eobj[i].y;
                        sobj[j+1].sx = P.px - eobj[i].x, sobj[j+1].sy = P.py - eobj[i].y;
                        sobj[j].divisor = 50, sobj[j+1].divisor = 50;
                        sobj[j].size = shotSize*1.4, sobj[j+1].size = shotSize*1.4;
                        sobj[j].angle = PI;
                        sobj[j+1].angle = PI;
                        sobj[j].rotx = eobj[i].x-10;
                        sobj[j+1].rotx = eobj[i].x+10;

                        if(sobj[j].sy < 0) Physics.Standardize(sobj[j].sy, -350, -200);
                        if(sobj[j].sy > 0) Physics.Standardize(sobj[j].sy, 200, 350);
                        if(sobj[j+1].sy < 0) Physics.Standardize(sobj[j].sy, -350, -200);
                        if(sobj[j+1].sy > 0) Physics.Standardize(sobj[j].sy, 200, 350);
                        eobj[i].charge = 0;

                        sobj[j].angle = Physics.ApproxAngle(sobj[j].angle, sobj[j].rotx, sobj[j].y, P.px, P.py);
                        sobj[j+1].angle = Physics.ApproxAngle(sobj[j+1].angle, sobj[j+1].rotx, sobj[j+1].y, P.px, P.py);    
                              
                    }
                    if(dice2 > 25)
                    {
                        sobj[j].x = eobj[i].x, sobj[j].y = eobj[i].y;
                        sobj[j+1].x = eobj[i].x+30, sobj[j+1].y = eobj[i].y-2;
                        sobj[j+2].x = eobj[i].x-30, sobj[j+2].y = eobj[i].y-2;
                        sFlag[j] = 2, sFlag[j+1] = 2, sFlag[j+2] = 2;
                        xFlag[i] = 0;
                        sobj[j].divisor = 80, sobj[j+1].divisor = 80, sobj[j+2].divisor = 80;
                        sobj[j].size = shotSize*1.4, sobj[j+1].size = shotSize*1.4, sobj[j+2].size = shotSize*1.4;
                        sobj[j].angle = PI;
                        sobj[j+1].angle = PI+PI/32;
                        sobj[j+2].angle = PI-PI/32;
                        eobj[i].charge = 0;
                    }
                    if(eobj[i].charge2 >= 12)
                    {
                        sobj[j].x = eobj[i].x, sobj[j].y = eobj[i].y;
                        sobj[j+1].x = eobj[i].x+30, sobj[j+1].y = eobj[i].y-2;
                        sobj[j+2].x = eobj[i].x-30, sobj[j+2].y = eobj[i].y-2;
                        sFlag[j] = 2, sFlag[j+1] = 2, sFlag[j+2] = 2;
                        xFlag[i] = 0;
                        sobj[j].divisor = 80, sobj[j+1].divisor = 80, sobj[j+2].divisor = 80;
                        sobj[j].size = shotSize*1.4, sobj[j+1].size = shotSize*1.4, sobj[j+2].size = shotSize*1.4;
                        sobj[j].angle = PI;
                        sobj[j+1].angle = PI-PI/4;
                        sobj[j+2].angle = PI+PI/4;
                        eobj[i].charge2 = 0;
                        break;
                    }
                }
            } 
        }
    }
   
          
    for(uint_fast8_t i = 0; i < sMAX; i++)
    {
        if(sFlag[i] == 1)
        {
            sobj[i].x = (sobj[i].sx / sobj[i].divisor) + sobj[i].x;
            sobj[i].y = (sobj[i].sy / sobj[i].divisor) + sobj[i].y;
            if(sobj[i].y > Height + 150 || sobj[i].y < Height - Height - 150) sFlag[i] = 0;
            if(sobj[i].x > Width + 150 || sobj[i].x < Width - Width - 150) sFlag[i] = 0;
        }
        if(sFlag[i] == 2)
        {
            sobj[i].x += sin(sobj[i].angle)*8;
            sobj[i].y -= cos(sobj[i].angle)*8;
            if(sobj[i].y > Height + 150 || sobj[i].y < Height - Height - 150) sFlag[i] = 0;
            if(sobj[i].x > Width + 150 || sobj[i].x < Width - Width - 150) sFlag[i] = 0;
        }
    } 
}

void EnemyData::Draw(Enemy_t *eobj, Shotobj_t *sobj, const uint_fast8_t eMAX, const uint_fast8_t sMAX, uint_fast8_t *eFlag, uint_fast8_t *sFlag)
{
    for(uint_fast8_t i = 0; i < eMAX; i++)
    {
        if(eFlag[i] == 1)
        {
            DrawRotaGraph(eobj[i].x,eobj[i].y,eobj[i].size,eobj[i].rot,eobj[i].enemyH[eobj[i].frame],TRUE);
        }
        if(eobj[i].active)
        {
            Ui.DrawBar(eobj[i].x,eobj[i].y,eobj[i].hp,eobj[i].hpmax,eobj[i].size);
        }
    }
    for(uint_fast8_t i = 0; i < sMAX; i++)
    {
        if(sFlag[i] == 1)
        {
            DrawRotaGraph(sobj[i].x,sobj[i].y,sobj[i].size,sobj[i].angle,eShotH[1],TRUE);
        }
        if(sFlag[i] == 2)
        {
            DrawRotaGraph(sobj[i].x,sobj[i].y,sobj[i].size,sobj[i].angle,eShotH[1],TRUE);
        }
    }
}

void EnemyData::Draw(Enemy_t *eobj, const uint_fast8_t eMAX, uint_fast8_t *eFlag)
{
    for(uint_fast8_t i = 0; i < eMAX; i++)
    {
        if(eFlag[i] == 3 && eobj[i].active)
        {
            DrawRotaGraph(eobj[i].x,eobj[i].y,eobj[i].size,eobj[i].rot,eobj[i].enemyH[0],TRUE);
        }
    }
}


