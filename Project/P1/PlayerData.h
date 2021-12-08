/***** PlayerData.h *****/
#ifndef DEF_PLAYERDATA_H
#define DEF_PLAYERDATA_H

#include <cstdint>
#include "Projectiles.h"
class PlayerData
{
    public:
        uint_fast8_t charge, jFlag, jGrace, vMax, vBase;
        int_fast8_t hp, hpmax, shp, shpmax;
        int_fast16_t mx, my, px, py, pw, ph, sw, sh;
        int_fast16_t opx, opy, lx;
        int_fast32_t playerH[9], playershotH[3], playershot2H, shieldH[2], TriH;
        float accel, vel_x, vel_y;
        double rot, shieldrot, playerSize, shotSize, shieldSize;
        char moved;
        PlayerData();
        void Update(uint_fast8_t a);
        void CreateFire(uint_fast8_t type, Shotobj_t *sobj, const uint_fast8_t MAX, uint_fast8_t *sFlag);
        void DrawFire(Shotobj_t *sobj, const uint_fast8_t MAX, uint_fast8_t *sFlag);
        void DrawAim(Shotobj_t &aimObj, int_fast32_t imgH, uint_fast16_t distance);
};

#endif