/***** EnemyData.h *****/
#ifndef DEF_ENEMYDATA_H
#define DEF_ENEMYDATA_H

#include <cstdint>
#include "PlayerData.h"
#include "Projectiles.h"

//sprite array won't be combined with + operator
typedef struct Enemy_t {
    uint_fast8_t type, magnitude, charge, charge2, frame; 
    int_fast8_t state, nextstate, hp, hpmax;
    int_fast16_t x, y, ax, ay, tx, ty, range, cycletime, w, h;
    int_fast32_t enemyH[6], pace;
    char direction;
    float fx, fy, rotx;
    double rot, size, vel;
    bool active, warp, destroyed, chaser, edge, degrade;
    Enemy_t operator+(Enemy_t &eobj)
    {
        Enemy_t combine;
        //array
        combine.enemyH[0] = this->enemyH[0] + eobj.enemyH[0];
        combine.magnitude = this->magnitude + eobj.magnitude;
        combine.type = this->type + eobj.type;
        combine.charge = this->charge + eobj.charge;
        combine.charge2 = this->charge2 + eobj.charge2;
        combine.hp = this->hp + eobj.hp;
        combine.hpmax = this->hpmax + eobj.hpmax;
        combine.x = this->x + eobj.x;
        combine.y = this->y + eobj.y;
        combine.ax = this->ax + eobj.ax;
        combine.ay = this->ay + eobj.ay;
        combine.tx = this->tx + eobj.tx;
        combine.ty = this->ty + eobj.ty;
        combine.range = this->range + eobj.range;
        combine.cycletime = this->cycletime + eobj.cycletime;
        combine.w = this->w + eobj.w;
        combine.h = this->h + eobj.h;
        combine.pace = this->pace + eobj.pace;
        combine.direction = this->direction + eobj.direction;
        combine.fx = this->fx + eobj.fx;
        combine.fy = this->fy + eobj.fy;
        combine.rot = this->rot + eobj.rot;
        combine.size = this->size + eobj.size;
        combine.vel = this->vel + eobj.vel;
        combine.active = this->active + eobj.active;
        combine.warp = this->warp + eobj.warp;
        combine.destroyed = this->destroyed + eobj.destroyed;
        combine.chaser = this->chaser + eobj.chaser;
        combine.edge = this->edge + eobj.edge;
        return combine;
    }
};

class EnemyData
{
    public:
        uint_fast8_t sizeDiv;
        int_fast16_t x, y, w, h, bw, bh, sw, sh;
        int_fast32_t eShotH[3];
        double rot, size, vel, shotSize;
        EnemyData();
        void Update(uint_fast8_t type, Enemy_t *eobj, const uint_fast8_t MAX, uint_fast8_t *eFlag);
        void Fire(Enemy_t *eobj, Shotobj_t * sobj, PlayerData &P, const uint_fast8_t eMAX, const uint_fast8_t sMAX, uint_fast8_t *sFlag);
        void Draw(Enemy_t *eobj, Shotobj_t *sobj, const uint_fast8_t eMAX, const uint_fast8_t sMAX, uint_fast8_t *eFlag, uint_fast8_t *sFlag);
        void Draw(Enemy_t *eobj, const uint_fast8_t eMAX, uint_fast8_t *eFlag);
};

#endif