#ifndef boss_H
#define boss_H

#include "../../base.h"

class boss : public AdvImage {
	private:
		AdvImage* laserImageMode;
		double idleAngle;
		Vector2 idleOffset;
		Vector2 defaultPos;
	public:
		bool laser;
		boss(GameWindow* gamewindow);
		~boss();
		void update();
		void tilt(double angle);
		void setTilt(double angle);
		void setdefaultPos(float x, float y);
		void draw();
};



#endif // boss_H
