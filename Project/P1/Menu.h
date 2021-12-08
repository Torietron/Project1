/***** Menu.h *****/
#ifndef DEF_MENU_H
#define DEF_MENU_H

#include "SceneVirt.h"
class Menu: public SceneVirt
{
    public:
        void Init();
        void End();
        void Update();
        void Draw();
};

#endif
