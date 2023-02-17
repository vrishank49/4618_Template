#include "stdafx.h"
#include "CBase4618.h"
#include "cvui.h"
#include <conio.h>

/**
 * @brief Runs the base of the program.
 *
 * This function runs the main loop of the program until the user clicks on the "Quit" button or presses the 'q' key.
 */
void CBase4618::run()
{
   bool exit_flag = true;
   char exit_char = 'a';

   while (exit_flag && exit_char != 'q') { // loop when exit conditions are not met

      if ((cvui::button(_base_canvas, 50, 50, "Quit") == 1)) // display and check Quit button
      {
         exit_flag = false;
      }

      cvui::update();

      if (!kbhit()) { // check for button press
         exit_char = cv::waitKey(1);
      }

      update();
      draw();
   }

   cv::destroyAllWindows();

   exit(0);
}
