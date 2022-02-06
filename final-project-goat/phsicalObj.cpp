#include "phsicalObj.h"

PhsicalObj::PhsicalObj(const Vector2& start_pos, const Window*& w) : Object(start_pos, w) {
    vel.x = 0;
    vel.y = 0;
    acc.x = 0;
    acc.y = 0;
}

PhsicalObj::PhsicalObj(const int& start_x, const int& start_y, const Window*& w) : Object(start_x, start_y, w) {
    pos.x = start_x;
    pos.y = start_y;
    vel.x = 0;
    vel.y = 0;
    acc.x = 0;
    acc.y = 0;
}

// update the position and velosity and keep accerlation the same
void PhsicalObj::update(terrain* ter) {
    pos.add(vel);
    vel.add(acc);
}

void PhsicalObj::setAcc(const Vector2& new_acc) {
    acc = new_acc;
}

void PhsicalObj::setAcc(const int& x, const int& y) {
    acc.x = x;
    acc.y = y;
}

void PhsicalObj::setVel(const Vector2& new_vel) {
    vel = new_vel;
}

void PhsicalObj::setVel(const int& x, const int& y) {
    vel.x = x;
    vel.y = y;
}

Vector2 PhsicalObj::getVel() const {
    return vel;
}

Vector2 PhsicalObj::getAcc() const {
    return acc;
}

bool physicsObj::isOnGround(terrain* ter) const {
	if(ter->getSolid((int)(pos.x+col.x-1), (int)(pos.y+col.y))) return true; //bottom right
	else if(ter->getSolid((int)(pos.x+1), (int)(pos.y+col.y))) return true; //bottom left
	return false;
}

bool physicsObj::isAgainstWallLeft(terrain* ter) const {
	if(ter->getSolid((int)(pos.x), (int)(pos.y+col.y-1))) return true; //bottom left
	else if(ter->getSolid((int)(pos.x), (int)(pos.y))) return true; //top left
	return false;
}

bool physicsObj::isAgainstWallRight(terrain* ter) const {
	if(ter->getSolid((int)(pos.x+col.x), (int)(pos.y+col.y-1))) return true; //bottom right
	else if(ter->getSolid((int)(pos.x+col.x), (int)(pos.y))) return true; //top right
	return false;
}