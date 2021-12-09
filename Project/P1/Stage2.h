/****** Stage2.h ******/
#ifndef DEF_STAGE2_H
#define DEF_STAGE2_H

#include "SceneVirt.h"
class Stage2: public SceneVirt
{
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif