#include "stdafx.h"
#include "CInvader.h"

CInvader::CInvader()
{
   _shape.height = INVADER_HEIGHT;
   _shape.width = INVADER_WIDTH;
   _lives = INVADER_LIVES;
   _velocity = INVADER_INITIAL_VELOCITY;
}
