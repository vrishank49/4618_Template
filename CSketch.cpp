#include "stdafx.h"
#include "CSketch.h"
#include "CControl.h"
#include "cvui.h"
#include <conio.h>


CSketch::CSketch(int comport, cv::Point canvas_size) { // constructor - takes size object (for frame size), and initializes CControl COM port
   _sketchcontrol.init_com(comport); // initialize comport 5 for CControl object

  _base_canvas= cv::Mat::zeros(canvas_size.x, canvas_size.y, CV_8UC3); // define parameters (sizing) for canvas and color
   _point_old = cv::Point(canvas_size.x/2, canvas_size.y/2);
   _point_new = cv::Point(canvas_size.x / 2, canvas_size.y / 2);
   _canvascolor = cv::Scalar(255,0,0);

   cvui::init("Etch-a-Sketch by Vrishank");
   cv::imshow("Etch-a-Sketch by Vrishank", _base_canvas); // initialize and display canvas

   _resetflag = false; // initialize variables for debounce, reset and color change
   _colorchange = false;
   _colorselect = 0;
   _colorflag = 0;
   debounce = false;
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

   _sketchcontrol.get_data(ANALOG, JOY_X, x_axis);
   x_axis = (_sketchcontrol.get_analog(x_axis)*CANVAS_SIZE_X/100*1.5) - 127 ; // x axis position, scaled and moved over to fit within screen

   if (x_axis < 0) // x boundaries
      x_axis = 0;
   else if (x_axis > 500)
      x_axis = 499;
    
   _sketchcontrol.get_data(ANALOG, JOY_Y, y_axis);
   y_axis = (CANVAS_SIZE_Y*1.5) - _sketchcontrol.get_analog(y_axis)*CANVAS_SIZE_Y/100*1.5 - 122; // y axis position (similar to x_axis parameters)

   if (y_axis < 0) // y boundaries
      y_axis = 0;
   else if (y_axis > 500)
      y_axis = 499;

   _point_new = cv::Point(x_axis, y_axis);

   if (_sketchcontrol.get_button(DIGITAL, BUTTON_1) || (cvui::button(_base_canvas, 50, 20, "Reset") == 1)) // reset if button is pressed on microcontroller or GUI
      _resetflag = true;
   else
      _resetflag = false;

   _sketchcontrol.get_data(DIGITAL, BUTTON_2, _colorflag); // check if button is pressed on microcontroller for color change

   if (!debounce && !_colorflag) {

      if (_colorflag == 0)
         _colorselect++;

      if (_colorselect == 3) // if it increments past the green color go back to blue
         _colorselect = 0;

      switch (_colorselect) {
         case 0: // BLUE
            _canvascolor = cv::Scalar(255, 0, 0);
            _sketchcontrol.set_data(DIGITAL, BLUE_LED, 1);
            _sketchcontrol.set_data(DIGITAL, RED_LED, 0);
            _sketchcontrol.set_data(DIGITAL, GREEN_LED, 0);
            break;
         case 1: // RED
            _canvascolor = cv::Scalar(0, 255, 0);
            _sketchcontrol.set_data(DIGITAL, BLUE_LED, 0);
            _sketchcontrol.set_data(DIGITAL, RED_LED, 1);
            _sketchcontrol.set_data(DIGITAL, GREEN_LED, 0);
            break;
         case 2: // GREEN
            _canvascolor = cv::Scalar(0, 0, 255);
            _sketchcontrol.set_data(DIGITAL, BLUE_LED, 0);
            _sketchcontrol.set_data(DIGITAL, RED_LED, 0);
            _sketchcontrol.set_data(DIGITAL, GREEN_LED, 1);
            break;
      }

      debounce = true;
   }

   if (_colorflag && debounce)
      debounce = false;
}

void CSketch::draw()
{
   cv::line(_base_canvas, _point_new, _point_old, _canvascolor, 1, cv::LINE_AA); // draw a little line between the old and new point (between joystick inputs)

   if ((cvui::button(_base_canvas, 50, 20, "Reset") == 1)) // enable reset flag if GUI button is pressed
      _resetflag = true;

   if (_resetflag == true)
   {
     _base_canvas= cv::Mat::zeros(CANVAS_SIZE_X, CANVAS_SIZE_Y, CV_8UC3);  // clear screen if reset flag 
   }
   
   _resetflag = false; // reset the reset flag (lol)

   cv::imshow("Etch-a-Sketch by Vrishank", _base_canvas); // update image
}
