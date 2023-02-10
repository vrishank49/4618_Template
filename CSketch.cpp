#include "stdafx.h"
#include "CSketch.h"
#include "cvui.h"
#include <conio.h>


void CSketch::update()
{
   std::cout << "Update\n";
   // update internal variables
   // -> read data from CControl & perform calculations
   // -> use get_analog to get X/Y positions as percentages, and multiply by canvas size for coords

   // if button is pressed, change color
   // use debounce in CControl to check if the button press counter has incremented to initiate a color change
   // check here for _reset flag

}

void CSketch::draw()
{
   std::cout << "Draw\n";
   // do all drawing on the Mat image data type
   // display image on screen
   // use imshow to display canvas
   // add GUI exit button to close program

   // when color changes, on GUI indicate what color it has changed to
   // check here for _reset flag
}
