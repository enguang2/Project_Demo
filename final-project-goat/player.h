#ifndef PLAYER_H
#define PLAYER_H

#include "phsicalObj.h"
#include "animation.h"

class Player : public PhsicalObj{
  private:
    // TODO: add private vars and funcs
    // int health;
    animation* playerSprite;
  public:
    Player(const float& start_x, const float& start_y);
    Player(const Vector2& start_pos);
    void jump();
    void move(const bool& dir);
    void update(terrain* ter);
};

#endif