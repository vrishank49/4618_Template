#pragma once
#include "CGameObject.h"

#define MISSILE_VELOCITY cv::Point2f(0, -100)

#define MISSILE_HEIGHT 4
#define MISSILE_WIDTH 2
#define MISSILE_LIVES 1

class CMissile :
    public CGameObject
{
private:
public:
   CMissile::CMissile();
};

