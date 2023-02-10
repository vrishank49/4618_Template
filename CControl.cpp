#include "stdafx.h"
#include "CControl.h"
#include <conio.h>

void CControl::init_com(int comport)
{
   std::string comport_name = "COM" + std::to_string(comport);
   _com.open(comport_name); // open COM port for MSP
}

float CControl::get_analog(int result)
{
   // get the analog value from 0-4096 and return as a percentage
   // result / 4096 * 100 = percentage
   // round to 1 decimal place
   float temp_int = result * 100;
   return (temp_int / 4096);
}

bool CControl::set_data(int type, int channel, int val)
{
   _com.flush();
   // TX and RX strings
   std::string type_string, channel_string, val_string;
   type_string = std::to_string(type);
   channel_string = std::to_string(channel);
   val_string = std::to_string(val);
   std::string tx_str = "S" + type_string + " " + channel_string + " " + val_string + "\n";
   // bool type to check if it was sent as a servo or not
   // com write to servo
   // return true if it sent properly, false if it didn't send
   _com.write(tx_str.c_str(), tx_str.length());
   return false;
}


void CControl::get_data(int type, int channel, int& result, bool debounce_check)
{
   _com.flush();
   // TX and RX strings
   std::string type_string, channel_string;
   type_string = std::to_string(type);
   channel_string = std::to_string(channel);
   std::string tx_str = "G" + type_string + " " + channel_string + " \n";
   std::string rx_str;

   std::string receive_token;
   std::stringstream token_ss(receive_token);
   std::vector<std::string> s_buff;

   // temporary storage
   char buff[2];
      // Send TX string
      _com.write(tx_str.c_str(), tx_str.length());
      //Sleep(10); // wait for ADC conversion, etc. May not be needed?

      rx_str = "";
      // start timeout count
      double start_time = cv::getTickCount();

      buff[0] = 0;
      // Read 1 byte and if an End Of Line then exit loop
    // Timeout after 1 second, if debugging step by step this will cause you to exit the loop

      while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
      {
         if (_com.read(buff, 1) > 0)
         {
            rx_str = rx_str + buff[0];

         }

      }

      token_ss << rx_str; // puts rx string into token_ss stringstream



  /*    while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
      {
         if (_com.read(buff, 1) > 0)
         {
            rx_str = rx_str + buff[0];
         
         }
      }*/
      
      //


      while (std::getline(token_ss, receive_token, ' ')) {
         s_buff.push_back(receive_token);
      }

      if (s_buff.size() >= 4) {
         result = std::stoi(s_buff[3]);
      }


      //int temp_result = std::stoi(s_buff.at(3));
      //cv::waitKey(1);

      //if (debounce_check == false) {

         //if (type == 1) {
         //  //int result_old = std::stoi(rx_str.substr(6, 7).c_str());
         //  // printf("ANALOG TEST:CH%d", channel);
         //  // printf(" = %s", rx_str.substr(6, 7).c_str());

         //   std::cout << "ANALOG TEST : CH" << channel << " " << result << " (" << trunc(get_analog(result)) << "%)\n";

         //  /// printf(" (%.1f", get_analog(result_old));
         //   //printf("%)");

         //}
         //else if (type == 0) {

         //   //int led_state = std::stoi(rx_str.substr(6));
         //}
         //else if (type == 2) {
         //   std::string test = (rx_str.substr(6).c_str());

         //      printf("SERVO POS:CH%d", channel);
         //      printf(" = %s", rx_str.substr(6).c_str());
         //   
         //}
     // }
      //else 
        // result = std::stoi(rx_str.substr(6).c_str());


      cv::waitKey(1);
}

void CControl::get_button(int type, int channel)
{
   int current_button_state = 0, previous_button_state = 1, press_count = 0;
   bool press_check;

   std::cout << "Button Presses: 0\n";

   do {
      if (previous_button_state == 1)
      {
         press_check = true;
      }

      get_data(type, channel, previous_button_state, true); // does this sent the result (button state) to previous_button_state??????
      delay_timer(10);

      if (press_check == true) {
         get_data(type, channel, current_button_state, true);
         if ((current_button_state == 0) && (previous_button_state == 0)) {
            press_count++;
            std::cout << "Button Presses: " << press_count << std::endl;
            press_check = false;
         }
      }
   } while (!kbhit());

}

void CControl::delay_timer(double delay)
{
   double start_time = cv::getTickCount(), current_time = 0, period;

   while (delay > start_time) {
      current_time = (1000 * (cv::getTickCount() - start_time) / cv::getTickFrequency());
   }

}
