#include "stdafx.h"
#include "CGameObject.h"

void CGameObject::move()
{
   _position = _position + _velocity * ((cv::getTickCount() - _t_prev) / cv::getTickFrequency());
   _t_prev = cv::getTickCount();
}

void CGameObject::draw(cv::Mat& im, cv::Scalar color)
{
   //cv::rectangle(im, _position, cv::Point2f(_position.x + _shape.width, _position.y + _shape.height), color, cv::FILLED, cv::LINE_AA);
   cv::circle(im, _position, 10, color, cv::FILLED, cv::LINE_AA);
}
