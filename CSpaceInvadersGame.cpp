#include "stdafx.h"
#include "cvui.h"
#include "CSpaceInvadersGame.h"

CSpaceInvadersGame::CSpaceInvadersGame(int comport) {
   _invaderscontrol.init_com(comport);

   _spaceinvaders_canvas = cv::Mat::zeros(SPACEINVADERS_CANVAS_SIZE_X, SPACEINVADERS_CANVAS_SIZE_Y, CV_8UC3);
   cvui::init(SPACEINVADERS_TITLE);
   cv::imshow(SPACEINVADERS_TITLE, _spaceinvaders_canvas);

   _resetcount = 0;
   _firecount = 0;
   _debounce_fire = false;
   _debounce_reset = false;

   // initialize delta t by setting current time
   _t_current = cv::getTickCount();

   _ship.set_pos(cv::Point2f(SPACEINVADERS_CANVAS_SIZE_X / 2, SPACEINVADERS_CANVAS_SIZE_Y / 2));
}



void CSpaceInvadersGame::update() {

   CSpaceInvadersGame::button_input();

   //_joy_input_old = _joy_input_new;
   _invaderscontrol.get_data(ANALOG, JOY_X, _joy_input_raw);
   _joy_perc = _invaderscontrol.get_analog(_joy_input_raw); 

   if ((_joy_perc < 60) && (_joy_perc > 40)) // deadzone for movement direction (velocity)
      _ship.set_velocity(ZERO_VELOCITY);
   else if (_joy_perc > 60)
      _ship.set_velocity(POS_VELOCITY);
   else if (_joy_perc < 40)
      _ship.set_velocity(NEG_VELOCITY);

   _ship.move(); // change the position according to velocity & time

   // get delta t
   _t_prev = _t_current;
   _t_current = cv::getTickCount();
   _t_delta = (_t_current - _t_prev) / cv::getTickFrequency();
   _t_current = _t_prev;


   // update missile and vector stuffs
   for (int i = 0; i < _invader_vector.size(); i++) {
      //update invaders that exist using CGameObject methods
   }

   for (int i = 0; i < _missile_vector.size(); i++) {
      //update missiles that exist using the CGameObject methods
   }

   // check for button 1 (fire)
   // if button 1 is pressed, pushback a missile into the vector
   // check for button 2 (reset)
   // -> erase entire missile vector
   // -> reset invaders to default
   // ->
   // print out state (reset/fire) to cout

   // remove any elements of the vectors whose life = 0
   // -> if ship life = 0, go to lose state

}

void CSpaceInvadersGame::draw() {
   _spaceinvaders_canvas = cv::Mat::zeros(SPACEINVADERS_CANVAS_SIZE_X, SPACEINVADERS_CANVAS_SIZE_Y, CV_8UC3); // clear canvas of old drawings after positions are updated from update()

   std::cout << _ship.get_pos().x << std::endl; // test to see ship position
   //std::cout << _joy_perc << std::endl; // test to see ship position


   // move ship based on joy input
   _ship.draw(_spaceinvaders_canvas, WHITE); // draw the ship that has moved according to move() in update()

   for (int i = 0; i < _invader_vector.size(); i++) {
      //draw invaders that exist using the CGameObject draw method
   }

   for (int i = 0; i < _missile_vector.size(); i++) {
      //draw missiles that exist using the CGameObject draw method
   }

   cv::imshow(SPACEINVADERS_TITLE, _spaceinvaders_canvas);
}

void CSpaceInvadersGame::button_input() {

   _invaderscontrol.get_data(DIGITAL, BUTTON_1, _firecount);

   if (!_debounce_fire && !_firecount)
   {
      std::cout << "fire\n";

      _debounce_fire = true;
   }

   if (_debounce_fire && _firecount)
      _debounce_fire = false;

   _invaderscontrol.get_data(DIGITAL, BUTTON_2, _resetcount);

   if (!_debounce_reset && !_resetcount)
   {
      std::cout << "reset\n";

      _debounce_reset = true;
   }

   if (_debounce_reset && _resetcount)
      _debounce_reset = false;
}