#pragma once
#include "stdafx.h"

#define DELTA_TIME 0.02

class CGameObject
{
protected:
   cv::Point2f _position, _velocity;
   cv::Rect _shape;

   int _lives;

public:
   // double _t_current, _t_prev, _t_delta; // timing

   CGameObject::CGameObject();

   void move();
   double _t_prev, _t_current;
   bool collide(CGameObject& obj);
   bool collide_wall(cv::Size board);
   void hit();
   int get_lives() { return _lives; }
   void set_lives(int lives) { _lives = lives; }
   void set_pos(cv::Point2f pos) { _position = pos; }
   cv::Point2f get_pos() { return _position; }
   void draw(cv::Mat& canvas, cv::Scalar); // draw the image itself (rectangle, just like one line of code)

   void set_velocity(cv::Point2f velocity) { _velocity = velocity; }
   cv::Point2f get_velocity() { return _velocity; }

};

