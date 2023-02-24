#include "stdafx.h"
#include "CBase4618.h"
#include "cvui.h"
#include <conio.h>

/**
 * @brief Runs the base of the program.
 *
 * This function runs the main loop of the program until the user clicks on the "Quit" button or presses the 'q' key.
 */

CBase4618::CBase4618() {
   exit_flag = true;
   exit_char = 'a'; // not important what character this is, just for intializing

}

void CBase4618::run()
{

   do { // loop when exit conditions are not met

      if (!kbhit()) { // check for button press
         exit_char = cv::waitKey(1);
      }

      update();
      draw();

      cvui::update();

   } while (exit_flag && exit_char != 'q');

}

CBase4618::~CBase4618() {
   cv::destroyAllWindows();
   exit(0);
}
