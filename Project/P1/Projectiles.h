/***** Projectiles.h *****/
#ifndef DEF_PROJECTILES_H
#define DEF_PROJECTILES_H

typedef struct{
    float x, y;
    float sx, sy;
    float angle, rotx;
    uint_fast8_t divisor;
    double size, rot;
} Shotobj_t;

#endif