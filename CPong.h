#pragma once
#include "CBase4618.h"

#define PONG_CANVAS_SIZE_X 800
#define PONG_CANVAS_SIZE_Y 600
#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 30

#define PONG_TITLE "Pong by Vrishank"

/**
 * @brief CPong is a class that inherits from CBase4618 and provides an implementation
 * of the game Pong.
 */
class CPong :
   public CBase4618
{
private:
   CControl _pongcontrol; /**< An object of CControl to read control inputs */
   cv::Point _pong_canvas, ball, ball_direction; /**< A cv::Point object to hold canvas coordinates and ball position */
   int ballsize; /**< The size of the ball */
   int ballspeed; /**< The speed of the ball */
   int _player_paddle_pos_new; /**< The new position of the paddle */
   int _player_paddle_pos_old; /**< The old position of the paddle */

   int _winreset;

   int _playerscore, _botscore;

   double _current_time, _delta_time, _prev_time;
   double frequency, fps, initialframe;

   bool _resetflag;


   cv::Rect _player_paddle_dim, _bot_paddle_dim; /**< A cv::Rect object to hold paddle dimensions */
   cv::Scalar _white; /**< A cv::Scalar object to hold white color values */

public:
   /**
    * @brief Constructor of CPong class.
    * @param webcam_index Index of the webcam to use.
    * @param pong_canvas A cv::Point object to hold canvas coordinates.
    */
   CPong(int comport);

   ~CPong() {};

   /**
    * @brief Update the state of the game.
    */
   void update();

   /**
    * @brief Draw the game.
    */
   void draw();
};
