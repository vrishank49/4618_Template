#pragma once
#include "Serial.h"

#define JOY_X 11
#define JOY_Y 4
#define BUTTON_1 1
#define BUTTON_2 2

enum { DIGITAL = 0, ANALOG, SERVO };

class CControl {
private:
   Serial _com;
   int type, channel, value;

public:
   CControl() {

   }; // CControl Constructor
   ~CControl() {}; // CControl Destructor
   void init_com(int comport);
   float get_analog(int result);
   void get_data(int type, int channel, int& result);
   bool set_data(int type, int channel, int val);
   void get_button(int type, int channel);
   void delay_timer(double delay);
};