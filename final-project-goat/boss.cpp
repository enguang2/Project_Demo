#include "boss.h"

boss::boss(GameWindow* gamewindow) : AdvImage(gamewindow) {
	this->loadTexture(BOSS_SPRITE);
	defaultAngle = 0;
	defaultPos.set(3,3);
	laserImageMode = new AdvImage(gamewindow);
	laserImageMode->loadTexture(BOSS_SPRITE_ALTERNATIVE);
	laser = false;
}

boss::~boss() {
}

void boss::update() {
	Vector2 position1 = pos;
	defaultAngle += BOSS_IDLEANIMATION_SPEED;
	defaultOffset.fromAngle(defaultAngle);
	defaultOffset.mul(BOSS_IDLEANIMATION_RADIUS);
	defaultOffset.x = defaultOffset.x* defaultPos.x;
	defaultOffset.y = defaultOffset.y*defaultPos.y;
	position1.add(defaultOffset);
	this->setPos(position1);

	laserImageMode->angle = this->angle;
	laserImageMode->setPos(pos);
	laserImageMode->update();

	AdvImage::update();
}

void boss::tilt(double angle) {
	this->angle += angle;
}

void boss::setTilt(double angle) {
	this->angle = angle;
}

void boss::setdefaultPos(float x, float y) {
	defaultPos.set(x,y);
}

void boss::draw() {
	if(laser) laserImageMode->draw();
	else AdvImage::draw();
}
