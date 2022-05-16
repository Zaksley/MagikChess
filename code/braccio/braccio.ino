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
#include <InverseK.h>


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

int x; 
int y;
int z; 

int z_take = -5; 

float a0;
float a1;
float a2;
float a3;

int x_first_case = 0; 
int y_first_case = 0;
int z_upon_case = 200; 
int z_on_case = -21;


int longDelay; 
int nb_pieces = 16;
int nb_row; 

int x_cases[16]; 
int y_cases[16]; 


// Quick conversion from the Braccio angle system to radians
float b2a(float b){
  return b / 180.0 * PI - HALF_PI;
}

// Quick conversion from radians to the Braccio angle system
float a2b(float a) {
  return (a + HALF_PI) * 180 / PI;
}

void rad2brac(float a0, float a1, float a2, float a3)
{
  Vstep_base = a2b(a0);
  Vstep_shoulder = a2b(a1);
  Vstep_elbow = a2b(a2); 
  Vstep_wrist_ver = a2b(a3); 
}


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



  // Setup the lengths and rotation limits for each link
  Link base, upperarm, forearm, hand;

  base.init(0, b2a(0.0), b2a(180.0));
  upperarm.init(200, b2a(15.0), b2a(165.0));
  forearm.init(200, b2a(0.0), b2a(180.0));
  hand.init(270, b2a(0.0), b2a(180.0));

  // Attach the links to the inverse kinematic model
  InverseK.attach(base, upperarm, forearm, hand);


  Braccio.begin();
  
  Vstep_base = 0;
  Vstep_shoulder = 3;
  Vstep_elbow = 180;
  Vstep_wrist_ver = 170;
  Vstep_wrist_rot = 0;
  Vstep_gripper = 73;

  longDelay = 30; 
  nb_row = sqrt(nb_pieces); 
 
  // Place le braccio à l'angle 270 pour notre repère 
  //Vstep_base = 30;
  //Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_wrist_rot = 90;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  for(int Y=0; Y<nb_row; Y++)
  {
    for(int X=0; X<nb_row; X++)
    {
      x_cases[nb_row*Y + X] = 0;
      y_cases[nb_row*Y + X] = 0;
    }
  }


  int X_decalage = 35; 
  int Y_decalage = 35; 
  
  /*
  x_cases[0] = 168; 
  y_cases[0] = 236; 
  
  x_cases[1] = 212; 
  y_cases[1] = 191; 

  x_cases[2] = 236; 
  y_cases[2] = 161; 
   
  x_cases[3] = 260; 
  y_cases[3] = 125; 
  */
  
  x_cases[0] = 266; 
  y_cases[0] = 200; 
  
  x_cases[1] = 300; 
  y_cases[1] = 200; 

  x_cases[2] = 333; 
  y_cases[2] = 200;
    
  x_cases[3] = 366; 
  y_cases[3] = 200; 


  

  x_cases[4] = 192; 
  y_cases[4] = 256; 

  x_cases[5] = 238; 
  y_cases[5] = 207; 

  x_cases[6] = 274; 
  y_cases[6] = 184; 

  x_cases[7] = 280; 
  y_cases[7] = 175; 

  x_cases[8] = 191; 
  y_cases[8] = 287; 

  x_cases[9] = 252; 
  y_cases[9] = 261; 

  x_cases[10] = 272; 
  y_cases[10] = 241; 

  x_cases[11] = 293; 
  y_cases[11] = 196;



    grabPiece(0, 0);
    delay(1);
    releasePiece(1, 0);
    delay(1);
    grabPiece(1, 0);
    delay(1);
    releasePiece(2, 0);
    delay(1);
    grabPiece(2, 0);
    delay(1);
    releasePiece(3, 0);
    grabPiece(3, 0);
    /*releasePiece(0, 1);
    grabPiece(0, 1);
    releasePiece(1, 1);
    grabPiece(1, 1);
    releasePiece(1, 2);
    grabPiece(1, 2);
    releasePiece(1, 3
    );*/



  /*
  for(int i=1; i<3; i++)
  {
    for(int j=0; j<4; j++) 
    {
      grabPiece(j, i); 

      if (j+1 % 4 == 0) releasePiece(0, i+1);
      else              releasePiece(j+1, i);
    }
  }
  */

  /*
  grabPiece(3, 2); 

  openGripper(); */

  /*
  grabPiece(0, 2); 

  openGripper(); 
  
  grabPiece(1, 2);

  openGripper(); 
  
  grabPiece(2, 2);

  openGripper(); 
  
  grabPiece(3, 2);

  openGripper(); 
  
  grabPiece(4, 2);

  openGripper();
  */

/*
  grabPiece(0, 1); 
  openGripper(); 

  grabPiece(1, 1); 

  openGripper(); 

  grabPiece(2, 1); 

  openGripper(); 

  grabPiece(3, 1); 

  openGripper(); 
  */


  // TURN ON 2 FIRST ROWS
/*
  grabPiece(0, 0); 
  
  releasePiece(1, 0); 

  grabPiece(1, 0); 

  releasePiece(2, 0); 

  grabPiece(2, 0); 

  releasePiece(3, 0); 

  grabPiece(3, 0); 

  releasePiece(3, 1); 

  grabPiece(3, 1); 

  releasePiece(2, 1); 

  grabPiece(2, 1); 

  releasePiece(1, 1); 

  grabPiece(1, 1); 

  releasePiece(0, 1); 

  grabPiece(0, 1); 

  releasePiece(0, 0); 
  */
  

  /*
  int x_first = 161;
  int y_first = 226; 

  
  //x_first += 42;
  //y_first -= 42;
  
  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);
  

  openGripper();

  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3); 

  
  delay(200);

  closeGripper();

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);
  */


  /*

  x_first += 42;
  y_first -= 42;

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3);

  openGripper();

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  delay(200);
  
  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3);

  closeGripper();

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  moveTo(50, 50, 100, a0, a1, a2, a3);

  x_first -= 42;
  y_first += 42;

  delay(200);
  
  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3);

  openGripper();

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);
  */
}

void grabPiece(int x, int y)
{
  int xcase = x_cases[4*y+x]; 
  int ycase = y_cases[4*y+x]; 


  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);

  openGripper();

  moveTo(xcase, ycase, z_on_case, a0, a1, a2, a3);

  closeGripper();

  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);
}

void releasePiece(int x, int y)
{
  int xcase = x_cases[4*y+x]; 
  int ycase = y_cases[4*y+x]; 

  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);

  //openGripper();

  moveTo(xcase, ycase, z_on_case, a0, a1, a2, a3);

  openGripper();

  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);
}


void take() {
  
  openGripper(); 
  closeGripper(); 
}


void moveTo(int x, int y, int z, float a0, float a1, float a2, float a3)
{
  if (InverseK.solve(x, y, z, a0, a1, a2, a3))
  {
    rad2brac(a0, a1, a2, a3); 
    Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  }
}

/*
void getBack()
{
  Vstep_elbow = 90;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  Vstep_shoulder = 90; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
}

void pointSky(bool openIt) 
{
  Vstep_wrist_ver = 90;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  Vstep_elbow = 90;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_shoulder = 90; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_wrist_rot = 90;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  if (openIt) 
    openGripper(); 
  else
    closeGripper(); 
}
*/

void openGripper() 
{
  Vstep_gripper = 48; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
 
}

void closeGripper()
{
  Vstep_gripper = 73; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
}

/*
void grab()
{
  Vstep_wrist_ver = 180;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_elbow = 177; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  
  Vstep_shoulder = 85; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
}

void grabWithPiece()
{
  Vstep_wrist_ver = 180;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  
  Vstep_elbow = 170; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

  
  Vstep_shoulder = 90; 
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
}
*/

void loop() {

  /*
  int x_first = 161;
  int y_first = 226; 
  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);
  

  openGripper();

  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3); 

  delay(200);

  closeGripper();

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  moveTo(50, 50, 100, a0, a1, a2, a3);

  x_first += 42;
  y_first -= 43;

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3);

  openGripper();

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  delay(200);
  
  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3);

  closeGripper();

  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  moveTo(50, 50, 100, a0, a1, a2, a3);

  x_first -= 42;
  y_first += 43;

  delay(200);
  
  moveTo(x_first, y_first, z_upon_case, a0, a1, a2, a3);

  moveTo(x_first, y_first, z_on_case, a0, a1, a2, a3);

  openGripper();
  delay(200);
  */

  
}
