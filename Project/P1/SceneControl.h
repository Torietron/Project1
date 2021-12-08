/****** SceneControl.h *****/
#ifndef DEF_SCENECONTROL_H
#define DEF_SCENECONTROL_H

#include <cstdint>
class SceneControl
{
    public:
        SceneControl(uint_fast8_t startup = 1);
        void Change(uint_fast8_t nextscene);
        void Init();
        void End();
        int Update();
        int Draw();
    private:
        uint_fast8_t lScene;
        uint_fast8_t nScene;
};

typedef enum {
    NO_SCENE,
    DEBUG_SCENE,
    MENU_SCENE,
    INST_SCENE,
    ENTRY_SCENE,
    S1_SCENE,
    S2_SCENE,
    GO_SCENE,
    W_SCENE
} scenes_t;

#endif