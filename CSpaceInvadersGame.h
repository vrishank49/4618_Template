#pragma once
#include "CBase4618.h"
#include "CControl.h"
#include "CShip.h"
#include "CInvader.h"
#include "CMissile.h"

#define SPACEINVADERS_TITLE "Space Invaders by Vrishank"
#define SPACEINVADERS_CANVAS_SIZE_X 700
#define SPACEINVADERS_CANVAS_SIZE_Y 700

#define WHITE cv::Scalar(0,0,0)

#define POS_VELOCITY cv::Point2f(10,0)
#define NEG_VELOCITY cv::Point2f(-10,0)
#define ZERO_VELOCITY cv::Point2f(0,0)

class CSpaceInvadersGame :
    public CBase4618
{
private:
   CControl _invaderscontrol;
   cv::Mat _spaceinvaders_canvas;
   int _resetcount, _firecount;
   bool _debounce_fire, _debounce_reset;

   int _joy_input_old, _joy_input_new, _joy_input_raw;
   double _joy_perc;


public:
   CShip _ship;
   std::vector<CInvader> _invader_vector;
   std::vector<CMissile> _missile_vector;

   cv::Point2f ShipVelocity;

   double _t_current, _t_prev, _t_delta; // timing

   CSpaceInvadersGame(int comport);

   ~CSpaceInvadersGame() {};

   /**
    * @brief Update the state of the game.
    */
   void update();

   /**
    * @brief Draw the game.
    */
   void draw();

   void button_input(); //< Encapsulates all button input (S1&S2 on launchpad) in update() for simplicity.

   void init();
};

