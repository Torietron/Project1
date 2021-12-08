/***** MousePoll.h *****/
#ifndef DEF_MOUSEPOLL_H
#define DEF_MOUSEPOLL_H

class MousePoll {
	public:
		int Poll[4];
		MousePoll();
		int Update();
};

#endif