#pragma once
#include "CBase4618.h"

#define CANVAS_SIZE_X 500
#define CANVAS_SIZE_Y 500

class CSketch : public CBase4618
{
private:
   CControl _sketchcontrol;
   cv::Point _point_old, _point_new;
   cv::Point _canvas;
   cv::Scalar _canvascolor;
   bool _reset;
   bool _colorchange;
   int _colorselect;

public:
   CSketch(int, cv::Point);
   ~CSketch() {};

   // 

   void update();
   void draw();

};

