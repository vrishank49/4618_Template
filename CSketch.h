#pragma once
#include "CBase4618.h"

class CSketch : public CBase4618
{
private:


public:
   CSketch() {
      cv::Size; // no COM port defined, as we use init_com in CControl class
   };
   ~CSketch() {};

   void update();
   void draw();

};

