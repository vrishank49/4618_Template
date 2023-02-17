#include "stdafx.h"
#include "CBase4618.h"
#include "cvui.h"
#include <conio.h>

void CBase4618::run()
{
   bool exit_flag = true;
   char exit_char = 'a';
   
   //canvas_blank = cv::mat(cv::size(500, 500), cv_8uc3);
   
   //sketch.run();
   
   cvui::init("Etch-a-Sketch by Vrishank");
   //cvui::init("test exit");
   
   //sketch.run();
   
   while (exit_flag && exit_char != 'q') {
   
      //cvui::text(canvas, 50, 50, "quit");
      //cv::imshow("blank canvas", canvas_blank);

      cv::imshow("Etch-a-Sketch by Vrishank", canvas); // todo: is this line ok? button doesn't display if i don't include it.
   
      if ((cvui::button(canvas, 50, 50, "Quit") == 1))
      {
         exit_flag = false;
      }
   
      cvui::update();
   
      if (!kbhit()) {
         exit_char = cv::waitKey(1);
      }

      update();
      draw();
   
   }
   
   cv::destroyAllWindows();
   
   exit(0);

}