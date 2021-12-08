#include "DxLib.h"
#include "FpsControl.h"

static const int N = 60;
static const int FPS = 60;

FpsControl::FpsControl()
{
    mStartTime = 0;
    mCount = 0;
    mFps = 0;
}

bool FpsControl::Update()
{
    if(mCount == 0)
    {
        mStartTime = GetNowCount();
    }
    if(mCount == N)
    {
        int t = GetNowCount();
        mFps = 1000.f/((t-mStartTime)/(float)N);
        mCount = 0;
        mStartTime = t;
    }
    mCount++;
    return true;
}

void FpsControl::Draw()
{
    DrawFormatString(0,0,GetColor(255,255,255),"%.1f",mFps);
}

void FpsControl::Timeout()
{
    int tookTime = GetNowCount() - mStartTime;
    int waitTime = mCount*1000/FPS -tookTime;
    if(waitTime > 0)
    {
        Sleep(waitTime);
    }
}
