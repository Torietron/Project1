/***** Entry.h *****/
#ifndef DEF_ENTRY_H
#define DEF_ENTRY_H

#include "SceneVirt.h"
class EntryScene: public SceneVirt
{
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif