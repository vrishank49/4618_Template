#pragma once
#include "CGameObject.h"

#define INVADER_HEIGHT 5
#define INVADER_WIDTH 5
#define INVADER_LIVES 1

#define INVADER_ROWS 4
#define INVADER_COLUMNS 5

#define INVADER_INITIAL_VELOCITY cv::Point2f(100, 0)
#define INVADER_INITIAL_VELOCITY_INVERSE cv::Point2f(-100, 0)

class CInvader :
    public CGameObject
{
private:
public:
   CInvader::CInvader();
};

