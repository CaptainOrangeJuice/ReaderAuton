/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       orang                                                     */
/*    Created:      12/20/2024, 5:35:26 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include "motors.h"
#include <fstream>
using namespace std;

using namespace vex;

// A global instance of competition
competition Competition;
#define LOOP_DELAY 20 //20 ms per loop
#define START_MERCY ((1000/LOOP_DELAY) * 0) //How many seconds to remove from the beginning [(1000/LOOP_DELAY) is the number of loops per second]
#define END_MERCY ((1000/LOOP_DELAY) * 4) //Give extra time? [1 second as of now]
#define MAX_INPUTS ((1000/LOOP_DELAY) * 20) //1000 for 20 seconds
#define forward vex::forward
uint8_t inputs[6][MAX_INPUTS];
uint8_t checkData[6][MAX_INPUTS];
uint8_t emptyData[0][0];
/*uint8_t     myTestData[ 100 ];
uint8_t     myReadBuffer[ 1000 ];*/

    
    // printToConsole("loop begun");
    // Brain.SDcard.
    // ofstream myfile;
    // myfile.open(Brain.SDcard.appendfile());
    // myfile.open ("C:\\Users\\orang\\Documents\\test.txt");
    // myfile << "Writing this to a file." << std::endl;
    // myfile.close();
    // printToConsole("loop completed");

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  int nRead = Brain.SDcard.loadfile( "V_BLUE_LEFT.dat", (uint8_t*)inputs, sizeof(inputs));
  
  if (nRead == sizeof(inputs)) {
    
    for (int i = 0; i < MAX_INPUTS; i++) {
      if (i <= START_MERCY) {
        continue;
      }
      int8_t fwd = inputs[0][i];
      int8_t lr = inputs[1][i];
      int8_t _slow = inputs[2][i];
      int8_t beltFwd = inputs[3][i];
      int8_t beltRev = inputs[4][i];
      int8_t Pneumatics = inputs[5][i];

      Left.spin(forward, (fwd + lr) * (_slow / 100) * 0.7, pct);
      Right.spin(forward, (fwd - lr) * (_slow / 100) * 0.7, pct);

      if (beltFwd) belt.spin(forward, 100, pct);
      else if (beltRev) belt.spin(reverse, 100, pct);
      else belt.stop(brake);

      if (Pneumatics != inputs[5][i-1]) {
        clampPneumatics.set(Pneumatics);
      }

      wait(LOOP_DELAY, msec);
    }
  } else { printToConsole("Load failed!"); Controller1.Screen.print("Load failed!"); }
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  int loopNum = 0;
  double slow = 1;
  bool pneumaticsBool = false;
  bool pressingBool = false;
  // User control code here, inside the loop
  /*for (int i = 0; i < 3; i++) {
    Controller1.rumble(".");
    wait(1, sec);
  }
  Controller1.Screen.print("go!");
  Controller1.rumble("-");*/
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................
  //if (Brain.SDcard.isInserted()) {
    // printToConsole("a");
    int8_t fwd = Controller1.Axis3.value();
    int8_t lr = Controller1.Axis1.value();
    int8_t _slow = slow * 100;
    int8_t beltFwd = Controller1.ButtonR1.pressing();
    int8_t beltRev = Controller1.ButtonR2.pressing();
    int8_t Pneumatics = pneumaticsBool;

    if (loopNum <= (MAX_INPUTS/* + START_MERCY + END_MERCY*/)) {
      inputs[0][loopNum] = fwd; //Left joystick (vertical)
      inputs[1][loopNum] = lr; //Right joystick (horizontal)
      inputs[2][loopNum] = _slow;
      inputs[3][loopNum] = beltFwd;
      inputs[4][loopNum] = beltRev;
      inputs[5][loopNum] = Pneumatics;

      loopNum++;
    } else {
      printToConsole("Recording finished.");
    }

    if (Controller1.ButtonUp.pressing()){
      slow = 0.4;
    } else {
      slow = 1;
    }

    Left.spin(forward, (Controller1.Axis3.position() + Controller1.Axis1.position()) * slow * 0.7, pct);
    Right.spin(forward, (Controller1.Axis3.position() - Controller1.Axis1.position()) * slow * 0.7, pct);


    if (Controller1.ButtonR1.pressing()) {
      //to the pen
      belt.spin(forward, 100, pct);
    } else if (Controller1.ButtonR2.pressing()) {
      //to the king
      belt.spin(reverse, 100, pct);
    } else {
      //where's my crown
      belt.stop(brake);
    }
    
    if (Controller1.ButtonA.pressing()) {
      //that's my bling
      if (pressingBool == false) {
        pneumaticsBool = !pneumaticsBool;
        clampPneumatics.set(pneumaticsBool);
        printToConsole(pneumaticsBool);
      }
      pressingBool = true;
    } else {
      //always drama when I ring
      pressingBool = false;
      // printToConsole("Pneumatics set to ");
    }
    
    
    // printToConsole("b");
    // for(int i=0;i<25;i++) { Brain.Screen.print(myData[i]); printToConsole(myData[i]); }
    //Brain.SDcard.savefile( "test", (uint8_t*)myData, sizeof(myData) );
    if (Controller1.ButtonDown.pressing() && Controller1.ButtonLeft.pressing()) {
      Brain.SDcard.savefile( "V_BLUE_LEFT.dat", (uint8_t*)inputs, sizeof(inputs) );
      printToConsole("Data saved."); Controller1.Screen.print("Data saved.");
      wait(1, sec);
    }

    if (Controller1.ButtonDown.pressing() && Controller1.ButtonRight.pressing()) {
      Brain.SDcard.savefile( "V_BLUE_LEFT.dat", (uint8_t*)emptyData, sizeof(emptyData) );
      int nRead = Brain.SDcard.loadfile( "V_BLUE_LEFT.dat", (uint8_t*)checkData, sizeof(checkData));
      printToConsole("DATA RESET. RUN PROGRAM AGAIN TO INPUT DATA."); Controller1.Screen.print("Data cleared.");
      if (nRead == sizeof(inputs)) {
        wait(0.5, sec);
        printToConsole("wait");
        wait(0.3, sec);
        printToConsole("never mind it's still there");
        wait(0.8, sec);
        printToConsole("uhhhhhhhhhhhhhhhhhhhhhhhhhh");
        wait(0.4, sec);
        printToConsole(".");
        wait(0.1, sec);
        printToConsole(".");
        wait(0.1, sec);
        printToConsole(".");
        wait(0.5, sec);
        printToConsole("Just remove it manually idk");
      }
      loopNum += MAX_INPUTS + 1;
      wait(1, sec);
    }

    // printToConsole("c");

    // int nRead = Brain.SDcard.loadfile( "1082VAuton.txt", (uint8_t*)myData, sizeof(myData));

    // printToConsole("d");
    /*for (int i = 0; i < nRead; i++) {
      Brain.Screen.print(readBuffer[i]);
      printToConsole(readBuffer[i]);
    }*/
  /*} else {

  }*/
    wait(LOOP_DELAY, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

