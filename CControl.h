#pragma once
#include "Serial.h"

#define JOY_X 11
#define JOY_Y 4
#define BUTTON_1 1
#define BUTTON_2 2
#define GREEN_LED 39
#define RED_LED 38
#define BLUE_LED 37
#define COM5 5

enum { DIGITAL = 0, ANALOG, SERVO };

class CControl {
private:
   Serial _com;
   int type, channel, value;
   int current_button_state, previous_button_state;
   bool press_check;

public:
   CControl() {

   }; // CControl Constructor
   ~CControl() {}; // CControl Destructor

   void init_com(int comport);
   float get_analog(int result);
   bool get_data(int type, int channel, int& result);
   bool set_data(int type, int channel, int val);
   bool get_button(int type, int channel);
   void delay_timer(double delay);
};