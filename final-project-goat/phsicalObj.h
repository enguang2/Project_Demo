#ifndef PHSICALOBJ_H
#define PHSICALOBJ_H

#include "base.h"

class PhsicalObj : public Object {
  protected:
    Vector2 vel; // velocity
    Vector2 acc; // acceleration
  public:
    PhsicalObj(const Vector2& start_pos);
    PhsicalObj(const float& start_x, const float& start_y);
    void update(terrain* ter);
    void setAcc(const Vector2& new_acc);
    void setAcc(const float& x, const float& y);
    void setVel(const Vector2& new_vel);
    void setVel(const float& x, const float& y);
    Vector2 getAcc() const;
    Vector2 getVel() const;
    bool isOnGround(terrain* ter) const;
    bool isAgainstWallLeft(terrain* ter) const;
    bool isAgainstWallLeft(terrain* ter) const;
};

#endif