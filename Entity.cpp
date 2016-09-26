#include "Entity.h"

bool Entity::collides(Entity* other){
    if (  x_ <= other->x() && other->x() <= x_ + width_ &&
          y_<= other->y() && other->y() < y_ + height_)
        return true;
    if (  other->x() <= x_ && x_ <= other->x() + other->width() &&
          other->y() <= y_ && y_ <= other->y() + other->height())
        return true;
    return false;
}
