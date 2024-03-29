////////////////////////////////////////////////////////////////
// ELEX 4618 Template project for BCIT
// Created Oct 5, 2016 by Craig Hennessey
// Last updated Dec 2, 2022
////////////////////////////////////////////////////////////////
#include "stdafx.h"

// Add simple GUI elements
#define CVUI_DISABLE_COMPILATION_NOTICES
#define CVUI_IMPLEMENTATION
#include "cvui.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <conio.h>

#include "Client.h"
#include "Server.h"

#include "CControl.h"
#include "CSketch.h"
#include "CPong.h"
#include "CBase4618.h"
#include "CSpaceInvadersGame.h"
#include "CGameObject.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Must include Windows.h after Winsock2.h, so Serial must be included after Client/Server
#include "Serial.h" 

#define CANVAS_NAME "Display Image"

////////////////////////////////////////////////////////////////
// Can be used as a replacement for cv::waitKey() to display cv::imshow() images, Windows Only
////////////////////////////////////////////////////////////////
void process_msg()
{
  MSG msg;
  while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
  }
}

////////////////////////////////////////////////////////////////
// Generate ARUCO markers
////////////////////////////////////////////////////////////////
void generate_marks()
{
  std::string str;
  cv::Mat im;
  int mark_size = 250;

  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  for (int marker_id = 0; marker_id < 250; marker_id++)
  {
    cv::aruco::drawMarker(dictionary, marker_id, mark_size, im, 1);
    str = "mark" + std::to_string(marker_id) + ".png";
    cv::imwrite(str, im);
  }
}

////////////////////////////////////////////////////////////////
// Serial Communication
////////////////////////////////////////////////////////////////
void test_com()
{
  // Comport class (change port to your MSP device port)
  Serial com;
  com.open("COM5");

  // TX and RX strings
  std::string tx_str = "G 0 33\n";
  std::string rx_str;

  // temporary storage
  char buff[2];
  do
  {
    // Send TX string
		com.write(tx_str.c_str(), tx_str.length());
    Sleep(10); // wait for ADC conversion, etc. May not be needed?
  
    rx_str = "";
    // start timeout count
    double start_time = cv::getTickCount();

    buff[0] = 0;
		// Read 1 byte and if an End Of Line then exit loop
    // Timeout after 1 second, if debugging step by step this will cause you to exit the loop
    while (buff[0] != '\n' && (cv::getTickCount() - start_time) / cv::getTickFrequency() < 1.0)
    {
      if (com.read(buff, 1) > 0)
      {
        rx_str = rx_str + buff[0];
      }
    }

    printf ("\nRX: %s", rx_str.c_str());
    cv::waitKey(1);
  } 
  while (1);
}

////////////////////////////////////////////////////////////////
// Display Image on screen
////////////////////////////////////////////////////////////////
void do_image()
{
  cv::Mat im;

  // initialize GUI system
  cvui::init(CANVAS_NAME);
  cv::Point gui_position;

  // Load test image
  im = cv::imread("BCIT.jpg");

  // Seed random number generator with time
  srand(time(0));

  //Draw 500 circles and dots on the image
  for (int i = 0; i < 500; i++)
  {
    gui_position = cv::Point(10, 10);
    cvui::window(im, gui_position.x, gui_position.y, 200, 40, "Image Test");
    gui_position += cv::Point(5, 25);
    cvui::text(im, gui_position.x, gui_position.y, "Number of Circles: " + std::to_string(i));

    float radius = 50 * rand() / RAND_MAX;
    cv::Point center = cv::Point(im.size().width*rand() / RAND_MAX, im.size().height*rand() / RAND_MAX);
    
    cv::circle(im, center, radius, cv::Scalar(200, 200, 200), 1, cv::LINE_AA);
    
    im.at<char>(i,i) = 255;
    
    // Update the CVUI GUI system
    cvui::update();

    // Show image and delay (all display image to update)
    cv::imshow(CANVAS_NAME, im);
    cv::waitKey(1);
  }
}

////////////////////////////////////////////////////////////////
// Display Video on screen
////////////////////////////////////////////////////////////////
void do_video()
{
  cv::VideoCapture vid;

  vid.open(0, cv::CAP_DSHOW);
    
  bool do_canny = true;
  bool do_aruco = false;
  int canny_thresh = 30;
  bool do_exit = false;

  cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);

  // initialize GUI system
  cvui::init(CANVAS_NAME);
  cv::Point gui_position;
  
  std::vector<cv::Scalar> color_vec;
  color_vec.push_back(cv::Scalar(255, 255, 255));
  color_vec.push_back(cv::Scalar(255, 0, 0));
  color_vec.push_back(cv::Scalar(0, 255, 0));
  color_vec.push_back(cv::Scalar(0, 0, 255));
  int color_index = 0;
  
  if (vid.isOpened() == TRUE)
  {
    do
    {
      cv::Mat frame, edges;

      // Capture video frame
      vid >> frame;

      // Make sure video frame exists
      if (frame.empty() == false)
      { 
        // ARUCO marker tracking
        if (do_aruco == true)
        {
          std::vector<int> ids;
          std::vector<std::vector<cv::Point2f> > corners;
          cv::aruco::detectMarkers(frame, dictionary, corners, ids);
          if (ids.size() > 0)
          {
            cv::aruco::drawDetectedMarkers(frame, corners, ids);
          }
        }

        // Canny edge detection
        if (do_canny == true)
        {
          cv::cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
          cv::GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
          cv::Canny(edges, edges, 0, canny_thresh, 3);
          cv::add(frame, color_vec.at(color_index), frame, edges);
        }

        // GUI Menu
        gui_position = cv::Point(10, 10);
        cvui::window(frame, gui_position.x, gui_position.y, 200, 190, "Video Test");
        gui_position += cv::Point(5, 25);
        cvui::checkbox(frame, gui_position.x, gui_position.y, "Canny Filter", &do_canny);
        gui_position += cv::Point(0, 25);
        cvui::checkbox(frame, gui_position.x, gui_position.y, "ArUco", &do_aruco);
        gui_position += cv::Point(0, 25);
        cvui::text(frame, gui_position.x, gui_position.y, "Canny Threshold");
        gui_position += cv::Point(0, 15);
        cvui::trackbar(frame, gui_position.x, gui_position.y, 180, &canny_thresh, 5, 120);
        gui_position += cv::Point(0, 50);
        if (cvui::button(frame, gui_position.x, gui_position.y, 100, 30, "Colour Switch"))
        {
          color_index++;
          if (color_index >= color_vec.size()) { color_index = 0; }
        }
        gui_position += cv::Point(120, 0);
        if (cvui::button(frame, gui_position.x, gui_position.y, 50, 30, "Exit"))
        {
          do_exit = true;
        }

        // Update the CVUI GUI system
        cvui::update();
        cv::imshow(CANVAS_NAME, frame);
      }
    }
    while (cv::waitKey(1) != 'q' && do_exit == false);
  }      
}		

////////////////////////////////////////////////////////////////
// Demo client server communication
////////////////////////////////////////////////////////////////
void serverthread(CServer* server)
{
  // Start server
  server->start(4618);
}

void do_clientserver()
{
  char inputchar = 0;
  std::vector<std::string> cmds;

  cv::VideoCapture vid;
  CServer server;

  // Start server thread
  std::thread t(&serverthread, &server);
  t.detach();

  vid.open(0);

  while (inputchar != 'q')
  {
    inputchar = cv::waitKey(100);

    server.get_cmd(cmds);
    if (cmds.size() > 0)
    {
      // Process different commands received by the server
      for (int i = 0; i < cmds.size(); i++)
      {
        if (cmds.at(i) == "im")
        {
          std::cout << "\nServer Rx: " << cmds.at(i);

          std::string reply = "Hi there from Server";
          server.send_string(reply);
        }
        else
        {
          std::cout << "\nServer Rx: " << cmds.at(i);

          std::string reply = "Got some other message";
          server.send_string(reply);
        }
      }
    }

		// Update server image with the latest camera image
		if (vid.isOpened() == true)
		{
			cv::Mat frame;
			vid >> frame;
			if (frame.empty() == false)
			{
				imshow("Server Image", frame);
				process_msg();
				server.set_txim(frame);
			}
		}
  };

  server.stop();
  
  Sleep(100);
}

////////////////////////////////////////////////////////////////
// Lab 1
////////////////////////////////////////////////////////////////
void lab1()
{
}

////////////////////////////////////////////////////////////////
// Lab 2
////////////////////////////////////////////////////////////////
void lab2()
{
}

////////////////////////////////////////////////////////////////
// Lab 3
////////////////////////////////////////////////////////////////
void lab3()
{
   CControl com1;

   int comport = 5;
   std::cout << "Which comport: ";
   std::cin >> comport; // foolproof this input

   com1.init_com(comport); // initialize comport (to 5 for my case)

   int user_input = 0, type, channel, value, button_sel, press_count = 0;

   while (1) {
      std::cout << "\n(1) - Joystick Input\n(2) - Button Input\n(3) - Button Debounce Test\n(4) - Servo Test\nPlease select an option: ";
      std::cin >> user_input;

      switch (user_input) {
      case 1: // test joystick output
         do {
            if (com1.get_data(ANALOG, JOY_X, value)) {
               std::cout << "ANALOG TEST : CH" << JOY_X << " " << value << " (" << trunc(com1.get_analog(value)) << "%)\n";
            }
            if (com1.get_data(ANALOG, JOY_Y, value)) {
               std::cout << "ANALOG TEST : CH" << JOY_Y << " " << value << " (" << trunc(com1.get_analog(value)) << "%)\n";
            }

            std::cout << std::endl;
            cv::waitKey(50);
            //std::cout << std::endl;
         } while (!kbhit());
         break;
      case 2: // test button press
            std::cout << "Which button? ";
            std::cin >> button_sel;
            if (button_sel == BUTTON_1) {
               do {
                  if (com1.get_data(DIGITAL, BUTTON_1, value)) {
                     std::cout << "DIGITAL TEST : CH" << BUTTON_1 << " " << value;
                  }

                  if (value == 0)
                     com1.set_data(DIGITAL, 39, 1);
                  else if(value == 1)
                     com1.set_data(DIGITAL, 39, 0);

                  cv::waitKey(10);
                  std::cout << std::endl;
               } while (!kbhit());
            }
            else if (button_sel == BUTTON_2)
            {
               do {
                  if (com1.get_data(DIGITAL, BUTTON_2, value)) {
                     std::cout << "DIGITAL TEST : CH" << BUTTON_2 << " " << value;
                  }

                  if (value == 0)
                     com1.set_data(DIGITAL, 39, 1);
                  else if (value == 1)
                     com1.set_data(DIGITAL, 39, 0);

                  cv::waitKey(10);
                  std::cout << std::endl;
               } while (!kbhit());
            }
            else
               std::cout << "Please enter a valid button, 1 or 2.";

            std::cout << std::endl;

         break;
      case 3: // test debounce/increment button press
         press_count = 1;
            std::cout << "Which button? ";
            std::cin >> button_sel;
            if (button_sel == 1) {
               do {
                  if (com1.get_button(DIGITAL, BUTTON_1)) { // check if button has been pressed on falling edge of active low signal
                     std::cout << "Press Count = " << press_count << std::endl;
                     press_count++;
                  }
               } while (!kbhit());
               
            }
            else if (button_sel == 2)
            {
               do {
                  if (com1.get_button(DIGITAL, BUTTON_2)) { // check if button has been pressed on falling edge of active low signal
                     std::cout << "Press Count = " << press_count << std::endl;
                     press_count++;
                  }
               } while (!kbhit());
            }
            else
               std::cout << "Please enter a valid button, 1 or 2.";
            std::cout << std::endl;
         break;
      case 4: // servo test (find boundaries before gears grind)
         do {
            for (value = 0; value < 180; value += 5)
            {
               com1.set_data(SERVO, 0, value);
               cv::waitKey(10);
               std::cout << "SERVO POS:CH0 = " << value << std::endl;
            }
            for (value = 180; value > 0; value -= 5)
            {
               com1.set_data(SERVO, 0, value);
               cv::waitKey(10);
               std::cout << "SERVO POS:CH0 = " << value << std::endl;
            }
         } while (!kbhit());
         break;
      default:
         std::cout << "\nNo such case exists.\n\n";
         break;
      }
   }

}

////////////////////////////////////////////////////////////////
// Lab 4
////////////////////////////////////////////////////////////////
void lab4()
{
   cv::Point canvas_size = cv::Point(CANVAS_SIZE_X, CANVAS_SIZE_Y); // decide how big to make the canvas by defining a cv::Point variable
   CSketch sketch(COM9, canvas_size); // construct CSketch with given parameters
   sketch.run();  // run created sketch object
}

////////////////////////////////////////////////////////////////
// Lab 5
////////////////////////////////////////////////////////////////
void lab5()
{
   //cv::Point pong_canvas_size = cv::Point(PONG_CANVAS_SIZE_X, PONG_CANVAS_SIZE_Y); // decide how big to make the canvas by defining a cv::Point variable
   CPong pong(COM9); // construct CSketch with given parameters
   pong.run();  // run created sketch object
}

////////////////////////////////////////////////////////////////
// Lab 6
////////////////////////////////////////////////////////////////
void lab6()
{
   CSpaceInvadersGame spaceinvaders(COM9);
   spaceinvaders.run();
}

void print_menu()
{
	std::cout << "\n***********************************";
	std::cout << "\n* ELEX4618 Template Project";
	std::cout << "\n***********************************";
  std::cout << "\n(1) Lab 1 - User Input";
  std::cout << "\n(2) Lab 2 - Grading";
  std::cout << "\n(3) Lab 3 - Embedded Control";
  std::cout << "\n(4) Lab 4 - Etch-A-Sketch";
  std::cout << "\n(5) Lab 5 - Pong";
  std::cout << "\n(6) Lab 6 - Classic Arcade Game";
  std::cout << "\n(7) Lab 7 - Linux Port";
  std::cout << "\n(8) Lab 8 - Sorting";
  std::cout << "\n(9) Lab 9 - Sockets";
  std::cout << "\n(10) Test serial COM communication";
	std::cout << "\n(11) Show image manipulation";
	std::cout << "\n(12) Show video manipulation";
	std::cout << "\n(13) Test client/server communication";
	std::cout << "\n(0) Exit";
	std::cout << "\nCMD> ";
}

int main(int argc, char* argv[])
{
  // generate_marks();

	int cmd = -1;
	do
	{
		print_menu();
		std::cin >> cmd;
		switch (cmd)
		{
    case 1: lab1();
    case 2: lab2();
    case 3: lab3();
    case 4: lab4();
    case 5: lab5();
    case 6: lab6();
    case 10: test_com(); break;
		case 11: do_image(); break;
		case 12: do_video(); break;
    case 13: do_clientserver(); break;
		}
	} while (cmd != 0);
}
