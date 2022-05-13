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

// Variables for manual input 
String stringCase; 
int chosenX; 
int chosenY; 
bool manual = true; 
bool play = false; 


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
 

  for(int Y=0; Y<nb_row; Y++)
  {
    for(int X=0; X<nb_row; X++)
    {
      x_cases[nb_row*Y + X] = 0;
      y_cases[nb_row*Y + X] = 0;
    }
  }

  // Define the coordinates of all cases   
  x_cases[0] = 171; 
  y_cases[0] = 228; 
  
  x_cases[1] = 202; 
  y_cases[1] = 195; 

  x_cases[2] = 223; 
  y_cases[2] = 168;  

  x_cases[3] = 254; 
  y_cases[3] = 132; 

  x_cases[4] = 190; 
  y_cases[4] = 245; 

  x_cases[5] = 234; 
  y_cases[5] = 205; 

  x_cases[6] = 252; 
  y_cases[6] = 196; 

  x_cases[7] = 285; 
  y_cases[7] = 155; 

  x_cases[8] = 213; 
  y_cases[8] = 280; 

  x_cases[9] = 263; 
  y_cases[9] = 241; 

  x_cases[10] = 272; 
  y_cases[10] = 241; 

  x_cases[11] = 293; 
  y_cases[11] = 196;



  //parcoursMoveAPiece(2, 4); 

  /*
  grabPiece(0, 0);
  openGripper(); 
  grabPiece(0, 1); 
  openGripper();
  */
  /*
  grabPiece(2, 2);
  openGripper();
  grabPiece(3, 2);
  */
}

// Go through all case with some limitations
void parcoursMoveAPiece(int xMax, int yMax)
{
  for(int i=0; i<xMax; i++)
  {
    for(int j=0; j<yMax; j++) 
    {
      grabPiece(j, i); 

      if (j+1 % 4 == 0) releasePiece(0, i+1);
      else              releasePiece(j+1, i);
    }
  }
}

// Grab a piece at coordinates (x,y)
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

// Release a piece at coordinates (x,y)
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

// Go to coordinate (x, y) 
void moveTo(int x, int y, int z, float a0, float a1, float a2, float a3)
{
  if (InverseK.solve(x, y, z, a0, a1, a2, a3))
  {
    rad2brac(a0, a1, a2, a3); 
    Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  }
}


/*
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

bool changeStringToCase(String stringCase) 
{ 
  switch(stringCase[0])
  {
    case 'A':
    case 'a':
      Serial.println("DETECT A"); 
      chosenX = 0; 
      break; 

    case 'B':
    case 'b':
      Serial.println("DETECT B"); 
      chosenX = 1; 
      break; 

    case 'C':
    case 'c':
      Serial.println("DETECT C"); 
      chosenX = 2; 
      break; 

    case 'D':
    case 'd':
      Serial.println("DETECT D"); 
      chosenX = 3;
      break; 

    default:
      Serial.println("ERROR");
      return false; 
      break; 
  }

  switch(stringCase[1])
  {
    case '1':
      Serial.println("DETECT 1"); 
      chosenY = 0; 
      break; 

    case '2':
      Serial.println("DETECT 2"); 
      chosenY = 1; 
      break; 

    case '3':
      Serial.println("DETECT 3"); 
      chosenY = 2; 
      break; 

    case '4':
      Serial.println("DETECT 4"); 
      chosenY = 3;
      break; 

    default:
      Serial.println("ERROR");
      return false; 
      break; 
  }

  return true; 
}

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

void loop() {


  // If we want to do a real game, else we don't do anything 
  if (manual) 
  {
    if (Serial.available() )
    { 
   
        stringCase = Serial.readString();
        Serial.println(stringCase);
  
        play = changeStringToCase(stringCase); 
        if (play)
        {
          // Play
          Serial.println(chosenX); 
          Serial.print(chosenY); 
        }

    }
  }
}
