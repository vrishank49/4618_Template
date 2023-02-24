#include "stdafx.h"
#include "CGameObject.h"

CGameObject::CGameObject()
{
   _t_prev = cv::getTickCount();
}

void CGameObject::move()
{
   _t_current = cv::getTickCount();
   _position = _position + _velocity * ((_t_current - _t_prev) / cv::getTickFrequency());
   _t_prev = _t_current;
}

void CGameObject::draw(cv::Mat& canvas, cv::Scalar color)
{
   cv::rectangle(canvas, _position, cv::Point2f(_position.x + _shape.width, _position.y + _shape.height), color, cv::FILLED, cv::LINE_AA);
}
