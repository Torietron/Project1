/***** GOscene.h *****/
#ifndef DEF_GOSCENE_H
#define DEF_GOSCENE_H

#include "SceneVirt.h"
class GOscene: public SceneVirt
{
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif