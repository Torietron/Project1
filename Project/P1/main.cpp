//test
#define _USE_MATH_DEFINES
#include "DxLib.h"
#include <cstdint>
#include <cmath>
#include <iostream>
#include "SceneControl.h"
#include "KeyPoll.h"
#include "MousePoll.h"
#include "PlayerData.h"
#include "EnemyData.h"
#include "FpsControl.h"
#include "PhysicsData.h"
#include "Interface.h"

bool debugflag = false;
SceneControl Scene(MENU_SCENE);
FpsControl Fps;
KeyPoll Key;
MousePoll Mouse;
PlayerData P;
EnemyData E;
PhysicsData Physics;
Delta_t Delta;
Interface Ui(0);

int_fast16_t Width = 640, Height = 480;
uint_fast32_t sTime = 0, dTime = 0;
double imgsize = 0.25;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
	SetGraphMode(640, 480, 16); //640 x 480, bit depth, refresh rate
	if( DxLib_Init() == -1 ) return -1;

	ChangeFont("Lucidia Sans Unicode");     //Georgia, Lucidia Sans Unicode                
    ChangeFontType(Ui.FontType);
	SetFontSize(Ui.FontSize);                             
    SetFontThickness(Ui.FontThickness);    
	
	//preload setup
	SetDrawScreen(DX_SCREEN_BACK);
	
	while(1)
	{
	    ClearDrawScreen();
		{
			//update
			{
				if(debugflag) Fps.Update();
				Scene.Update();

			}
			//draw
			{
				if(debugflag) Fps.Draw();
				Scene.Draw();
			}
		}
		ScreenFlip();
		if(ProcessMessage() < 0 ) break;
		if(CheckHitKey(KEY_INPUT_ESCAPE)) break;
	}
	DxLib_End();
	return 0;					
}
