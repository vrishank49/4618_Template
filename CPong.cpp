#include "stdafx.h"
#include "CPong.h"
#include "cvui.h"

CPong::CPong(int comport, cv::Point pong_canvas_size) {
   _pongcontrol.init_com(comport); // initialize comport 5 for CControl object
   _base_canvas = cv::Mat::zeros(pong_canvas_size.x, pong_canvas_size.y, CV_8UC3); // define parameters (sizing) for canvas and color

   cvui::init("Pong by Vrishank");
   cv::imshow("Pong by Vrishank", _base_canvas); // initialize canvas

   ballsize = 25;
   ballspeed = 10;

   paddle_dim = cv::Rect(1, 400, 10, 105);
   _white = cv::Scalar(255, 255, 255);
   
   _paddlepos_y_old = 400;

   ball_center = cv::Point(_base_canvas.cols / 2, _base_canvas.rows / 2);

   paddle2_dim = cv::Rect((_base_canvas.cols - paddle_dim.width - 1), paddle_dim.y, paddle_dim.width, paddle_dim.height);

}

void CPong::update()
{
   _paddlepos_y_old = _paddlepos_y_new;
   _pongcontrol.get_data(ANALOG, JOY_Y, _paddlepos_y_new);
   _paddlepos_y_new = PONG_CANVAS_SIZE_Y - (_pongcontrol.get_analog(_paddlepos_y_new)/100*PONG_CANVAS_SIZE_Y);

   if (_paddlepos_y_new < 0)
      _paddlepos_y_new = 0;
   else if (_paddlepos_y_new > 695)
      _paddlepos_y_new = 695;

   //_paddlepos = cv::Point(1, _paddlepos_y);
    std::cout << _paddlepos_y_new << std::endl;

   paddle_dim = cv::Rect(1, _paddlepos_y_new, 10, 105);

   // overlap the frame
   tempframe = cv::Mat(_base_canvas.size(), _base_canvas.type());
   tempframe.setTo(cv::Scalar(0, 0, 0));
}

void CPong::draw() // the cloning and masking to simulate the paddles moving came from ChatGPT
{
   cv::Mat img = _base_canvas.clone();  // clone the base canvas to avoid modifying the original
   cv::rectangle(img, paddle_dim, _white, -1, cv::LINE_AA);
   cv::rectangle(img, paddle2_dim, _white, -1, cv::LINE_AA);
   cv::circle(img, ball_center, ballsize, _white, -1, cv::LINE_AA);

   cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);  // create a mask for the rectangular paddle
   mask(paddle_dim) = 255;

   cv::Mat mask2 = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);  // create a mask for the second paddle
   mask2(paddle2_dim) = 255;

   // split the image into its color channels
   std::vector<cv::Mat> channels;
   cv::split(img, channels);

   // create an alpha channel from the inverse of the mask
   cv::Mat alpha = cv::Mat::ones(img.rows, img.cols, CV_8UC1) * 255;
   alpha.setTo(0, mask);

   // create an alpha channel from the inverse of the mask
   cv::Mat alpha2 = cv::Mat::ones(img.rows, img.cols, CV_8UC1) * 255;
   alpha2.setTo(0, mask2);

   // merge the color channels and alpha channel into a 4-channel image
   channels.push_back(alpha);
   cv::Mat img_with_alpha;
   cv::merge(channels, img_with_alpha);

   // merge the color channels and alpha channel into a 4-channel image
   channels.push_back(alpha2);
   cv::Mat img_with_alpha2;
   cv::merge(channels, img_with_alpha2);

   cv::imshow("Pong by Vrishank", img_with_alpha);
   cv::createTrackbar("Ball size", "Pong by Vrishank", &ballsize, 50, 0);
   cv::createTrackbar("Ball speed", "Pong by Vrishank", &ballspeed, 50, 0);
}
