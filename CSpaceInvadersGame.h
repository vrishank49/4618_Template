#pragma once
#include "CBase4618.h"
#include "CControl.h"
#include "CShip.h"
#include "CInvader.h"
#include "CMissile.h"

#include <vector>
#include <iostream>

#define SPACEINVADERS_TITLE "Space Invaders by Vrishank"
#define SPACEINVADERS_CANVAS_SIZE_X 400
#define SPACEINVADERS_CANVAS_SIZE_Y 400

#define WHITE cv::Scalar(255, 255, 255)
#define RED cv::Scalar(0, 0, 255, 255)

#define POS_VELOCITY cv::Point2f(500,0)
#define NEG_VELOCITY cv::Point2f(-500,0)
#define ZERO_VELOCITY cv::Point2f(0,0)

class CSpaceInvadersGame :
    public CBase4618
{
private:
   CControl _invaderscontrol;
   cv::Mat _spaceinvaders_canvas;
   int _resetcount, _fireflag, _firecount, _scorecount;
   bool _debounce_fire, _debounce_reset;

   int _joy_input_raw;
   double _joy_perc;

   int _invader_row;
   cv::Point2f _invader_velocity;

public:
   CShip _ship;
   CInvader _tempinvader;
   CMissile _tempmissile;
   std::vector<CInvader> _invader_vector;
   std::vector<CMissile> _missile_vector;

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

   void button_input(); // Encapsulates all button input (S1&S2 on launchpad) in update() for simplicity.

   void init(); // initialize game state with starting locations and objects
};

