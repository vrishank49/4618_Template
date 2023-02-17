#pragma once
#include "Serial.h"
#include "CControl.h"

class CBase4618
{
private:
   CControl control; // TODO check if I actually need this..

public:

   cv::Mat canvas;

   void run();
   virtual void update() = 0; // virtual void for multiple child method definitions of update and draw
   virtual void draw() = 0;

};

