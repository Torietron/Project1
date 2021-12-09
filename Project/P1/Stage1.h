/****** Stage1.h ******/
#ifndef DEF_STAGE1_H
#define DEF_STAGE1_H

#include "SceneVirt.h"
class Stage1: public SceneVirt
{
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif