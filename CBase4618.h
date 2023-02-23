#pragma once

#include "Serial.h"
#include "CControl.h"

/**
 * @brief The CBase4618 class serves as the base class for different projects.
 *
 * This class contains a CControl object which handles data input and output via
 * serial communication. It also has a virtual update() and draw() method which can
 * be implemented differently by child classes.
 */
class CBase4618 {
private:
   CControl control; /**< CControl object for handling serial communication. */
   bool exit_flag;
   char exit_char;

public:
   cv::Mat _base_canvas; /**< OpenCV Mat object for drawing. */

   CBase4618();
   ~CBase4618();

   /**
    * @brief Starts the program execution.
    */
   void run();

   /**
    * @brief Virtual method for updating data.
    */
   virtual void update() = 0;

   /**
    * @brief Virtual method for drawing to the canvas.
    */
   virtual void draw() = 0;
};
