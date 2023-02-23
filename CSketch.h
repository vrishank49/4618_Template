#pragma once
#include "CBase4618.h"

#define CANVAS_SIZE_X 500
#define CANVAS_SIZE_Y 500

#define ETCH_TITLE "Etch-a-Sketch by Vrishank"

/**
 * @brief CSketch is a class that inherits from CBase4618 and provides an implementation
 * of the game Etch-a-Sketch.
 */
class CSketch : public CBase4618 {
private:
   CControl _sketchcontrol; ///< instance of CControl class
   cv::Point _point_old; ///< previous point in canvas
   cv::Point _point_new; ///< new point in canvas
   cv::Point _canvas; ///< canvas point
   cv::Scalar _canvascolor; ///< color of canvas
   bool _resetflag, debounce; ///< reset flag for canvas
   bool _colorchange; ///< color change flag
   int _colorselect, _colorflag, x_axis, y_axis; ///< selected colors, flags and intermediate axis variables

public:
   /**
    * @brief Construct a new CSketch object
    *
    * @param comport COM port
    * @param canvas_size decide frame size
    */
   CSketch(int comport, cv::Point canvas_size);

   /**
    * @brief Destroy the CSketch object
    *
    */
   ~CSketch() {};

   /**
    * @brief update the canvas
    *
    */
   void update();

   /**
    * @brief draw on the canvas
    *
    */
   void draw();
};
