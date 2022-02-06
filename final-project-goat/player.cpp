#include "player.h"

Player::Player(const int& start_x, const int& start_y, const Window*& w) : PhsicalObj(start_x, start_y, w) {
    vel.x = 0;
    vel.y = 0;
    acc.x = 0;
    acc.y = GRAVITY;
    // health = 3
}

Player::Player(const Vector2& start_pos, const Window*& w) : PhsicalObj(start_pos, w) {
    vel.x = 0;
    vel.y = 0;
    acc.x = 0;
    acc.y = -GRAVITY;
    // health = 3
}

void Player::jump(){
    vel.y -= 15
}

void Player::move(const bool& dir) {
    if(dir) {
        if(vel.x < 3.0) {
            acc.x = 0.5;
        } else {
            acc.x = 0.0;
            vel.x = 3.0;
        }
    } else {
        if(vel.x > -3.0) {
            acc.x = -0.5;
        } else {
            acc.x = 0.0;
            vel.x = -3.0;
        }
    }
}

void Player::update(terrain* ter) {
    if(!(this->isOnGround(ter)) && vel.y < 0) {
        pos.y += vel.y;
        vel.y += acc.y;
    } else if(this->isOnGround(ter)){
        pos.y += vel.y;
        vel.y += acc.y;
    }
    if(!(this->isAgainstWallLeft(ter)) && vel.x < 0) {
        pos.x += vel.x;
    } else if(!(this->isAgainstWallRight(ter)) && vel.x > 0) {
        pos.x += vel.x;
    }
    vel.x += acc.x;

    playerSprite->setPos(this->pos);
    playerSprite->update();
}