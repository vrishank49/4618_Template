/**
 * @file CControl.h
 * @brief Declaration of CControl class.
 */

#pragma once
#include "Serial.h"

 /**
  * @brief Joystick X axis channel.
  */
#define JOY_X 11

  /**
   * @brief Joystick Y axis channel.
   */
#define JOY_Y 4

   /**
    * @brief Button 1 channel.
    */
#define BUTTON_1 1

    /**
     * @brief Button 2 channel.
     */
#define BUTTON_2 2

     /**
      * @brief Green LED channel.
      */
#define GREEN_LED 39

      /**
       * @brief Red LED channel.
       */
#define RED_LED 38

       /**
        * @brief Blue LED channel.
        */
#define BLUE_LED 37

        /**
         * @brief COM port for communication.
         */
#define COM5 5
#define COM9 9

         /**
          * @brief Enumeration of possible data types.
          */
enum { DIGITAL = 0, ANALOG, SERVO };

/**
 * @brief The CControl class provides methods for interacting with a microcontroller.
 */
class CControl {
private:
   Serial _com; ///< The communication port.
   int type; ///< The type of data.
   int channel; ///< The channel of the data.
   int value; ///< The value of the data.
   int current_button_state; ///< The current button state.
   int previous_button_state; ///< The previous button state.
   bool press_check; ///< Flag indicating if button is pressed.

public:
   /**
    * @brief Constructor for the CControl class.
    */
   CControl() {};

   /**
    * @brief Destructor for the CControl class.
    */
   ~CControl() {};

   /**
    * @brief Initializes the communication port.
    * @param comport The communication port number.
    */
   void init_com(int comport);

   /**
    * @brief Gets the analog value from a channel.
    * @param result The analog result.
    * @return The analog value.
    */
   float get_analog(int result);

   /**
    * @brief Gets data from a channel.
    * @param type The type of data.
    * @param channel The channel of the data.
    * @param result The data result.
    * @return True if successful, false otherwise.
    */
   bool get_data(int type, int channel, int& result);

   /**
    * @brief Sets data on a channel.
    * @param type The type of data.
    * @param channel The channel of the data.
    * @param val The value of the data.
    * @return True if successful, false otherwise.
    */
   bool set_data(int type, int channel, int val);

   /**
    * @brief Gets the state of a button.
    * @param type The type of button.
    * @param channel The channel of the button.
    * @return True if the button is pressed, false otherwise.
    */
   bool get_button(int type, int channel);

   /**
    * @brief Delays for a specified time.
    * @param delay The delay time.
    */
   void delay_timer(double delay);
};
