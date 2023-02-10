#pragma once
#include "Serial.h"
#include "CControl.h"

class CBase4618
{
private:
   CControl control;

public:
   void run();
   virtual void update() = 0; // virtual void for multiple child method definitions of update and draw
   virtual void draw() = 0;

};

