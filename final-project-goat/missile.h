#ifndef MISSILE_H
#define MISSILE_H

#include "physicsObj.h"
#include "animation.h"


class missile : public AdvImage {
	private:
		Vector2 vel;
		Timer timer;
		animation* trail;
		
		Image* hitMarker;
		void setHitMarker(terrain* ter);
	public:
		animation* explosion;
		void update(terrain* ter);
		void draw();
		missile(GameWindow* gw);
		missile(GameWindow* gw, int spawnX, int spawnY, float velX, float velY, bool drop);
		~missile();
		void reset();
		void launch(Vector2 vel);
		bool destroy;
		void setPos(Vector2 pos);
		Vector2 getPos();
		bool collided;
		bool flameTrail;
		
		bool markHit;
		float maxTime;
};



#endif // MISSILE_H
