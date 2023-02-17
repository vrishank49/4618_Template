#pragma once
#include "CBase4618.h"

#define PONG_CANVAS_SIZE_X 800
#define PONG_CANVAS_SIZE_Y 1000

/**
 * @brief CPong is a class that inherits from CBase4618 and provides an implementation
 * of the game Pong.
 */
class CPong :
   public CBase4618
{
private:
   cv::Mat tempframe; /**< A temporary cv::Mat object to hold frames */
   CControl _pongcontrol; /**< An object of CControl to read control inputs */
   cv::Point _pong_canvas, ball_center; /**< A cv::Point object to hold canvas coordinates */
   int ballsize; /**< The size of the ball */
   int ballspeed; /**< The speed of the ball */
   int _paddlepos_y_new; /**< The new position of the paddle */
   int _paddlepos_y_old; /**< The old position of the paddle */

   cv::Rect paddle_dim, paddle2_dim; /**< A cv::Rect object to hold paddle dimensions */
   cv::Scalar _white; /**< A cv::Scalar object to hold white color values */

public:
   /**
    * @brief Constructor of CPong class.
    * @param webcam_index Index of the webcam to use.
    * @param pong_canvas A cv::Point object to hold canvas coordinates.
    */
   CPong(int comport, cv::Point pong_canvas_size);

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
