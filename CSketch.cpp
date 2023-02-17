#include "stdafx.h"
#include "CSketch.h"
#include "CControl.h"
#include "cvui.h"
#include <conio.h>


CSketch::CSketch(int comport, cv::Point canvas_size) { // constructor - takes size object (for frame size), and initializes CControl COM port
   _sketchcontrol.init_com(comport); // initialize comport 5 for CControl object

   canvas = cv::Mat::zeros(canvas_size.x, canvas_size.y, CV_8UC3); // define parameters (sizing) for canvas and color
   _point_old = cv::Point(canvas_size.x/2, canvas_size.y/2);
   _point_new = cv::Point(canvas_size.x / 2, canvas_size.y / 2);
   _canvascolor = cv::Scalar(255,0,0);

   _reset = false;
   _colorchange = false;
   _colorselect = 0;

   cv::imshow("Etch-a-Sketch by Vrishank", canvas); // initialize canvas
   
   // TODO initialize LED colors (maybe I can do this in another spot outside of the constructor?
   // TODO create cursor
}

// TODO where does this go?
//cv::Point menu, gui_position;
//bool exit_flag = true;
//char exit_char = 'a';
//
////canvas_blank = cv::Mat(cv::Size(500, 500), CV_8UC3);
//
////sketch.run();
//
//cvui::init("Etch-a-Sketch by Vrishank");
////cvui::init("TEST EXIT");
//
////sketch.run();
//
//while (exit_flag && exit_char != 'q') {
//
//   //cvui::text(canvas, 50, 50, "Quit");
//   //cv::imshow("BLANK CANVAS", canvas_blank);
//   cv::imshow("Etch-a-Sketch by Vrishank", canvas); // TODO: is this line OK? button doesn't display if I don't include it.
//
//   if ((cvui::button(canvas, 50, 50, "Quit") == 1))
//   {
//      exit_flag = false;
//   }
//
//   cvui::update();
//
//   if (!kbhit()) {
//      exit_char = cv::waitKey(1);
//   }
//
//}
//
//cv::destroyAllWindows();
//
//exit(0);

void CSketch::update()
{
   _point_old = _point_new;
   int x_axis, y_axis;

   _sketchcontrol.get_data(ANALOG, JOY_X, x_axis);
   x_axis = (_sketchcontrol.get_analog(x_axis)*CANVAS_SIZE_X/100*1.5) - 127 ; // x axis position


   if (x_axis < 0)
      x_axis = 0;
   else if (x_axis > 500)
      x_axis = 499;

   std::cout << "X AXIS: " << x_axis << std::endl;
    
   _sketchcontrol.get_data(ANALOG, JOY_Y, y_axis);
   y_axis = (CANVAS_SIZE_Y*1.5) - _sketchcontrol.get_analog(y_axis)*CANVAS_SIZE_Y/100*1.5 - 122; // y axis position

   if (y_axis < 0)
      y_axis = 0;
   else if (y_axis > 500)
      y_axis = 499;

   std::cout << "Y AXIS: " << y_axis << std::endl;

   _point_new = cv::Point(x_axis, y_axis);


   //if ((cvui::button(canvas, 50, 15, "Reset") == 1))
   //{
   //   _reset = true;
   //}
   //cvui::update();


   if (_sketchcontrol.get_button(DIGITAL, BUTTON_1))
      _reset = true;
   else
      _reset = false;

   if (_sketchcontrol.get_button(DIGITAL, BUTTON_2))
      _colorchange = true;
   else
      _colorchange = false;

   //cv::waitKey(1);

   // else
   //  _colorchange = false;

   // _reset = _sketchcontrol.get_button(DIGITAL, BUTTON_1);

   // _colorchange = _sketchcontrol.get_button(DIGITAL, BUTTON_2);

   if (_colorchange)
      _colorselect++;

   if (_colorselect == 3) // if it increments past the red color go back to blue
      _colorselect = 0;

      switch (_colorselect) {
      case 0:
         _canvascolor = cv::Scalar(255, 0, 0);
         _sketchcontrol.set_data(DIGITAL, BLUE_LED, 1);
         _sketchcontrol.set_data(DIGITAL, RED_LED, 0);
         _sketchcontrol.set_data(DIGITAL, GREEN_LED, 0);
         _colorchange = false;
         break;
      case 1:
         _canvascolor = cv::Scalar(0, 255, 0);
         _sketchcontrol.set_data(DIGITAL, BLUE_LED, 0);
         _sketchcontrol.set_data(DIGITAL, RED_LED, 1);
         _sketchcontrol.set_data(DIGITAL, GREEN_LED, 0);
         _colorchange = false;
         break;
      case 2:
         _canvascolor = cv::Scalar(0, 0, 255);
         _sketchcontrol.set_data(DIGITAL, BLUE_LED, 0);
         _sketchcontrol.set_data(DIGITAL, RED_LED, 0);
         _sketchcontrol.set_data(DIGITAL, GREEN_LED, 1);
         _colorchange = false;
         break;
      }

   // update internal variables
   // -> read data from CControl & perform calculations
   // getanalog to get joystick position
   // button debounce for reset
   // 
   // -> use get_analog to get X/Y positions as percentages, and multiply by canvas size for coords

   // if button is pressed, change color
   // use debounce in CControl to check if the button press counter has incremented to initiate a color change
   // check here for _reset flag

}

void CSketch::draw()
{
  // cv::Mat canvas_blank, frame, edges;
  // do all drawing on the Mat image data type
   cv::line(canvas, _point_new, _point_old, _canvascolor, 1, cv::LINE_AA);

   if (_reset == true)
   {
      canvas = cv::Mat::zeros(CANVAS_SIZE_X, CANVAS_SIZE_Y, CV_8UC3);
   }
   
   // display image on screen
   // use imshow to display canvas
   // add GUI exit button to close program


   // when color changes, on GUI indicate what color it has changed to
   // check here for _reset flag
}
