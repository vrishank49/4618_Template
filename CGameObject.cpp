#include "stdafx.h"
#include "CGameObject.h"

CGameObject::CGameObject()
{
   _t_prev = cv::getTickCount();
}

void CGameObject::move() // the delta time is broken so I've hardcoded it in. Fix when I have time.
{
   //_t_prev = cv::getTickCount();
   //_t_current = cv::getTickCount();
   //_position = _position + _velocity * ((_t_current - _t_prev) / cv::getTickFrequency());
   _position = _position + _velocity * DELTA_TIME;
   //_t_prev = _t_current;
}

bool CGameObject::collide(CGameObject& obj)
{
   if (_position.y == obj.get_pos().y && _position.x == obj.get_pos().x) // if the object is on the same y position as the other object, register a hit
      hit();

   return false;
}

bool CGameObject::collide_wall(cv::Size board)
{
   if (_position.y == 0) // if the object is on the same y position as the other object, register a hit
      hit();
   return false;
}

void CGameObject::hit()
{
   set_lives(get_lives() - 1);
}

void CGameObject::draw(cv::Mat& canvas, cv::Scalar color)
{
   cv::rectangle(canvas, _position, cv::Point2f(_position.x + _shape.width, _position.y + _shape.height), color, cv::FILLED, cv::LINE_AA);
}
