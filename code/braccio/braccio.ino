/*
  testBraccio90.ino

 testBraccio90 is a setup sketch to check the alignment of all the servo motors
 This is the first sketch you need to run on Braccio
 When you start this sketch Braccio will be positioned perpendicular to the base
 If you can't see the Braccio in this exact position you need to reallign the servo motors position

 Created on 18 Nov 2015
 by Andrea Martino

 This example is in the public domain.
 */

#include <Braccio.h>
#include <Servo.h>


Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;



int Vstep_base;
int Vstep_shoulder;
int Vstep_elbow;
int Vstep_wrist_rot;
int Vstep_wrist_ver;
int Vstep_gripper;

int longDelay; 

void setup() {  
  //Initialization functions and set up the initial position for Braccio
  //All the servo motors will be positioned in the "safety" position:
  //Base (M1):90 degrees
  //Shoulder (M2): 45 degrees
  //Elbow (M3): 180 degrees
  //Wrist vertical (M4): 180 degrees
  //Wrist rotation (M5): 90 degrees
  //gripper (M6): 10 degrees
  Serial.begin(9600); 
  
  Braccio.begin();
  
  Vstep_base = 0;
  Vstep_shoulder = 40;
  Vstep_elbow = 180;
  Vstep_wrist_ver = 170;
  Vstep_wrist_rot = 0;
  Vstep_gripper = 73;

  longDelay = 30; 
 
  //Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper); 

  // Place le braccio à l'angle 270 pour notre repère 
  Vstep_base = 30;
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  
  pointSky(); 

  grab(); 
  


  take(); 

  delay(1000);

  getBack(); 
  

}

void take() {
  
  openGripper(); 
  closeGripper(); 
}

void getBack()
{
  Vstep_elbow = 90;
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  Vstep_shoulder = 90; 
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
}

void pointSky() 
{
  Vstep_wrist_ver = 90;
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  Vstep_elbow = 90;
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_shoulder = 90; 
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_wrist_rot = 90;
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  openGripper(); 
}

void openGripper() 
{
  Vstep_gripper = 10; 
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
 
}

void closeGripper()
{
  Vstep_gripper = 60; 
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
}

void grab()
{
  Vstep_wrist_ver = 180;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_elbow = 177; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  
  Vstep_shoulder = 85; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
}

void loop() {
  /*
   Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
  
  // the arm is aligned upwards  and the gripper is closed
                     //(step delay, M1, M2, M3, M4, M5, M6);
  
  //Braccio.ServoMovement(20,         90, 90, 90, 90, 90,  73);  



  /*
  Vstep_elbow = 130; 
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  Vstep_wrist_ver = 130; 
  Braccio.ServoMovement(20, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);*/
  
}
