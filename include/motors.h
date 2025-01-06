#pragma once
#include "vex.h"
#include <iostream>
#include <fstream>
#define SCREAM(TEXT) std::cout<<TEXT<<std::endl;
#define yap(text) std::cout<<text<<std::endl;
#define printToConsole(text) std::cout<<text<<std::endl;
#define BOOM(text) std::cout<<TEXT<<std::endl;

inline extern vex::brain Brain = vex::brain();
inline extern vex::motor Fl = vex::motor(vex::PORT6, vex::ratio18_1, true); 
inline extern vex::motor Fr = vex::motor(vex::PORT7, vex::ratio18_1, false);
inline extern vex::motor Tl = vex::motor(vex::PORT11, vex::ratio18_1, false); //Motor on the left side above the others
inline extern vex::motor Tr = vex::motor(vex::PORT3, vex::ratio18_1, true); //Motor on the right side above the others
inline extern vex::motor Bl = vex::motor(vex::PORT2, vex::ratio18_1, true);
inline extern vex::motor Br = vex::motor(vex::PORT1, vex::ratio18_1, false);
inline extern vex::motor belt = vex::motor(vex::PORT10, vex::ratio18_1, true);
inline extern vex::motor manlySilverArmL = vex::motor(vex::PORT16, vex::ratio36_1, true);
inline extern vex::motor manlySilverArmR = vex::motor(vex::PORT13, vex::ratio36_1, false);
inline extern vex::motor_group manlySilverArm = vex::motor_group(manlySilverArmL, manlySilverArmR); //Not the lady brown frfr
inline extern vex::digital_out clampPneumatics = vex::digital_out(Brain.ThreeWirePort.H);
inline extern vex::controller Controller1 = vex::controller(vex::primary);
inline extern vex::motor_group Left = vex::motor_group(Fl, Tl, Bl);
inline extern vex::motor_group Right = vex::motor_group(Fr, Tr, Br);
inline extern vex::inertial InertialSensor = vex::inertial(vex::PORT17);
inline extern vex::optical OpticalSensor = vex::optical(vex::PORT15);
inline extern vex::rotation RotationSensor = vex::rotation(vex::PORT14);
// inline extern vex::rotation rotationL = new vex::rotation(PORT18, false);
//extern kDeviceTypeMotorSensor