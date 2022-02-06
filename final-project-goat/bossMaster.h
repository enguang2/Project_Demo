#ifndef bossMaster_H
#define bossMaster_H

#include "boss.h"

#include "../terrain.h"
#include "../animation.h"
#include "../NAV/nav_graph.h"

class bossMaster : public Object {
	private:
	terrain* ter;
	
	Vector2 laserOffset;
	double defaultAngle;
	
	AdvImage* laser;
	animation* laser_impact;
	animation* laser_impact2;
	AdvImage* laser_flare;
	void updateLaser();

	public:
		bool raygunStarted;
		nav_graph* bossNavigator;
		boss* ufo;
		void draw();
		void update();
		bossMaster(GameWindow* gamewindow, terrain* ter);
		~bossMaster();
		
};



#endif // bossMaster_H
