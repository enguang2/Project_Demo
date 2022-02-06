#include "bossMaster.h"

bossMaster::bossMaster(GameWindow* gamewindow, terrain* ter) : Object(gamewindow) {
	bossNavigator = new nav_graph(this);
	this->ter = ter;

	laserOffset.set(45,45);
	this->ufo = new boss(gamewindow);

	laser = new AdvImage(gamewindow);
	laser->loadTexture(RAYGUN_SPRITE);
	laser->center = new SDL_Point();
	laser_impact = new animation(gamewindow);
	laser_impact2 = new animation(gamewindow);
	laser_impact->setAnim(RAYGUN_IMPACT_SPRITE, 1, 4, 100.0f, 1);
	laser_impact->loop = true;
	laser_impact2->setAnim(RAYGUN_IMPACT_SPRITE, 1, 4, 90.0f, 1);
	laser_impact2->loop = true;
	laser_flare = new AdvImage(gamewindow);
	laser_flare->loadTexture(RAYGUN_IMPACT_SPRITE);
	defaultAngle = 0;
	raygunStarted = false;

}

bossMaster::~bossMaster() {
}

void bossMaster::update() {
	this->ufo->setPos(pos);
	this->ufo->update();

	
	if(raygunStarted) updateLaser();

	bossNavigator->update();
}

void bossMaster::draw() {

	this->ufo->draw();
	if(raygunStarted) {
		this->laser->draw();
		laser_flare->draw();
	}
}

void bossMaster::updateLaser() {
	this->laser->setPos(pos.x+laserOffset.x, pos.y+laserOffset.y);
	this->laser->angle = this->ufo->angle + 90;
	Vector2 lookat;
	lookat.fromAngle(laser->angle);
	Vector2 hit = ter->raycast(laser->getPos(), lookat, 0.1f);
	laser_flare->setPosCenter(hit);
	laser_flare->update();
	laser_impact->setPos(hit.x-laser_impact->getRect().w/2, hit.y-laser_impact->getRect().h+20);
	laser_impact2->setPos(hit.x-laser_impact->getRect().w/2, hit.y-laser_impact->getRect().h+20);
	this->laser->setFromTo(laser->getPos(), hit, true);
	this->laser->update();
	this->laser_impact->update();
	this->laser_impact2->update();
	this->laser_flare->angle += 50;
}