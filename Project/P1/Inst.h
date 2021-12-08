/***** Inst.h *****/
#ifndef DEF_INST_H
#define DEF_INST_H

#include "SceneVirt.h"
class Inst: public SceneVirt
{
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif