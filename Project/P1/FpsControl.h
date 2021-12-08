/***** FpsControl.h *****/
#ifndef DEF_FPSCONTROL_H
#define DEF_FPSCONTROL_H

class FpsControl{
public:
	FpsControl();
	bool Update();
	void Draw();
	void Timeout();
private:
	int mStartTime;
	int mCount;
	float mFps;
};

#endif