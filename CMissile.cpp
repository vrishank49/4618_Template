#include "stdafx.h"
#include "CMissile.h"

CMissile::CMissile()
{
   _shape.height = MISSILE_HEIGHT;
   _shape.width = MISSILE_WIDTH;
   _lives = MISSILE_LIVES;
   _velocity = MISSILE_VELOCITY;
}
