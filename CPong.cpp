#include "stdafx.h"
#include "CPong.h"
#include "cvui.h"
#include <chrono>
#include <string>
#include <conio.h>
#include "time.h"
#include <iostream>

CPong::CPong(int comport) {
   _pongcontrol.init_com(comport); // initialize comport 5 for CControl object
   _base_canvas = cv::Mat::zeros(PONG_CANVAS_SIZE_Y, PONG_CANVAS_SIZE_X, CV_8UC3); // initialize canvas and define parameters (sizing) for canvas and color

   cvui::init(PONG_TITLE);
   cv::imshow(PONG_TITLE, _base_canvas); // initialize canvas
   ballsize = 25;
   ballspeed = 10;

   _player_paddle_dim = cv::Rect(1, 400, 10, 105);
   _bot_paddle_dim = cv::Rect((_base_canvas.cols - _player_paddle_dim.width - 1), _player_paddle_dim.y, _player_paddle_dim.width, _player_paddle_dim.height);
   _white = cv::Scalar(255, 255, 255);

   _player_paddle_pos_old = 400;
   _playerscore = 0;
   _botscore = 0;

   ball = cv::Point(PONG_CANVAS_SIZE_X / 2, PONG_CANVAS_SIZE_Y / 2);

   // timings
   _prev_time = cv::getTickCount();
   frequency = cv::getTickFrequency();
   initialframe = 0;

   // determining ball positioning
   ball_direction = cv::Point(1, 1); // adjust the ballspeed for how fast you want the ball to move

}

void CPong::update()
{
   _player_paddle_pos_old = _player_paddle_pos_new; // to move user paddle (left paddle)
   _pongcontrol.get_data(ANALOG, JOY_Y, _player_paddle_pos_new);
   _player_paddle_pos_new = PONG_CANVAS_SIZE_Y - (_pongcontrol.get_analog(_player_paddle_pos_new) / 100 * PONG_CANVAS_SIZE_Y);

   if (_player_paddle_pos_new < 0) // borders for user paddle
      _player_paddle_pos_new = 0;
   else if (_player_paddle_pos_new > 695)
      _player_paddle_pos_new = 695;

   _player_paddle_dim = cv::Rect(1, _player_paddle_pos_new, 10, 105);

   ball.x += ball_direction.x * ballspeed;
   ball.y += ball_direction.y * ballspeed;

   // collision detection:
   // if the ball is at the same position as the player's paddle, make the direction negative
   if ((ball.x - ballsize <= _player_paddle_dim.x + _player_paddle_dim.width) && (ball.x + ballsize > _player_paddle_dim.x + _player_paddle_dim.width)) // check for the radius thing after
   {
      if ((ball.y + ballsize >= _player_paddle_dim.y) && (ball.y - ballsize <= _player_paddle_dim.y + _player_paddle_dim.height))
         ball_direction.x *= -1;
   }

   else if ((ball.x - ballsize <= _bot_paddle_dim.x + _bot_paddle_dim.width) && (ball.x + ballsize > _bot_paddle_dim.x + _bot_paddle_dim.width)) // check for the radius thing after
   {
      if ((ball.y + ballsize >= _bot_paddle_dim.y) && (ball.y - ballsize <= _bot_paddle_dim.y + _bot_paddle_dim.height))
         ball_direction.x *= -1;
   }
   
   // check for score (if ball position passes the boundary of the paddle's position

   if (ball.x - ballsize < 0) // left side score check
   {
      _botscore++;
      ball = cv::Point(PONG_CANVAS_SIZE_X / 2, PONG_CANVAS_SIZE_Y / 2);
      //_resetflag = true;
   }

   if (ball.x + ballsize > PONG_CANVAS_SIZE_X) // right side score check DOESN'T WORK PLS HELP
   {
      //_playerscore++;
      //_resetflag = true;
   }

   // check for the ball hitting the left or right border
   if (ball.y > PONG_CANVAS_SIZE_Y - ballsize || ball.y < ballsize)
      ball_direction.y *= -1;
   else
      ball.y = ball.y;

   // player 2 (bot) movement IDK WHAT IT DOES BUT IT DOES SOMETHING DOUBLE CHECK THIS AND UNDERSTAND IT LATER
   if (_bot_paddle_dim.y + _bot_paddle_dim.height / 2 < ball.y) 
   {
      _bot_paddle_dim.y += PONG_CANVAS_SIZE_Y / 100;
      if (_bot_paddle_dim.y > PONG_CANVAS_SIZE_Y - _bot_paddle_dim.height)
         _bot_paddle_dim.y = PONG_CANVAS_SIZE_Y - _bot_paddle_dim.height;

   }
   else if (_bot_paddle_dim.y + _bot_paddle_dim.height / 2 > ball.y)
   {
      _bot_paddle_dim.y -= PONG_CANVAS_SIZE_Y / 100;
      if (_bot_paddle_dim.y < 0)
         _bot_paddle_dim.y = 0;
   }
}

void CPong::draw()
{
   _base_canvas = cv::Mat::zeros(PONG_CANVAS_SIZE_Y, PONG_CANVAS_SIZE_X, CV_8UC3); // clear canvas every cycle to refresh positions of everything

   // reset canvas button
   if ((cvui::button(_base_canvas, 30, 10, "Reset") == 1)) // enable reset flag if GUI button is pressed
      _resetflag = true;

   if (_resetflag == true)
   {
      _base_canvas = cv::Mat::zeros(PONG_CANVAS_SIZE_Y, PONG_CANVAS_SIZE_X, CV_8UC3);  // clear screen if reset flag
      ball = cv::Point(PONG_CANVAS_SIZE_X / 2, PONG_CANVAS_SIZE_Y / 2);
      _playerscore = 0;
      _botscore = 0;
   }

   _resetflag = false; // reset the reset flag 

   if (_botscore >= 5) {

         _base_canvas = cv::Mat::zeros(PONG_CANVAS_SIZE_Y, PONG_CANVAS_SIZE_X, CV_8UC3);
         putText(_base_canvas, "YOU LOSE", cv::Size(100, 300), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 200, 100), 2);
         putText(_base_canvas, "Press S1 to reset game.", cv::Size(100, 300), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 200, 255), 2);

      _playerscore = 0;
      _botscore = 0;
      _resetflag = true;
   }

   // cv::Mat img = _base_canvas.clone();  // clone the base canvas to avoid modifying the original
   cv::rectangle(_base_canvas, _player_paddle_dim, _white, -1, cv::LINE_AA);
   cv::rectangle(_base_canvas, _bot_paddle_dim, _white, -1, cv::LINE_AA);
   cv::circle(_base_canvas, ball, ballsize, _white, -1, cv::LINE_AA);

   // framerate
   while ((cv::getTickCount() - initialframe) / cv::getTickFrequency() < 0.0333) {}
   fps = 1 / ((cv::getTickCount() - initialframe) / cv::getTickFrequency());
   initialframe = cv::getTickCount();

   putText(_base_canvas, "FPS : " + std::to_string(fps), cv::Size(600, 600), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 200), 2); // display FPS
   putText(_base_canvas, "YOU : " + std::to_string(_playerscore), cv::Size(300, 300), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 0, 255), 2); // display FPS
   putText(_base_canvas, "BOT : " + std::to_string(_botscore), cv::Size(100, 300), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 0, 0), 2); // display FPS

   cvui::trackbar(_base_canvas, 40, 40, 220, &ballsize, (int)1, int(25));
   cvui::trackbar(_base_canvas, 40, 100, 220, &ballspeed, (int)1, int(25));
   cv::imshow(PONG_TITLE, _base_canvas);

}

// TODOS
// bot paddle not detecting score properly
// score pausing the win screen when 5 is reached
// multithreaded pong
// 