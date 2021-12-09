/***** Wscene.h *****/
#ifndef DEF_WSCENE_H
#define DEF_WSCENE_H

#include "SceneVirt.h"
class Wscene: public SceneVirt
{
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif