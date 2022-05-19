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

float a0;
float a1;
float a2;
float a3;

int x_first_case = 0;
int y_first_case = 0;
int z_upon_case = 200;
int z_on_case = 5;


int longDelay;
int nb_pieces = 16;
int nb_row;

int x_cases[16];
int y_cases[16];
int z_cases[16];

int x_stock;
int y_stock;
int z_stock;

// Variables for manual input
String stringCase;
int chosenX;
int chosenY;
bool manual = true;
bool play = false;

// Variables for the game
int nbCasesAvailable;

// IA variables
int sizeStack;


enum Color
{
    BLUE = -1,
    BLACK = 1
};

typedef struct
{
    int x;
    int y;
} coords;


// ##########################################################
//            Braccio intern Functions
// ##########################################################

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

// Go to coordinate (x, y)
void moveTo(int x, int y, int z, float a0, float a1, float a2, float a3)
{
  if (InverseK.solve(x, y, z, a0, a1, a2, a3))
  {

    rad2brac(a0, a1, a2, a3);
    Vstep_shoulder -= 7;
    Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
  }
}

// ##########################################################
//            Braccio move Functions
// ##########################################################

// Go through all case with some limitations
void parcoursMoveAPiece(int xMax, int yMax)
{
  for(int i=0; i<xMax; i++)
  {
    for(int j=0; j<yMax; j++)
    {
      //grabPiece(j, i);
      takeStock(); 

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
  int zcase = z_cases[4*y+x];


  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);
  openGripper();
  moveTo(xcase, ycase, zcase, a0, a1, a2, a3);
  closeGripper();
  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);
}

// Release a piece at coordinates (x,y)
void releasePiece(int x, int y)
{
  int xcase = x_cases[4*y+x];
  int ycase = y_cases[4*y+x];
  int zcase = z_cases[4*y+x];


  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);
  moveTo(xcase, ycase, zcase, a0, a1, a2, a3);
  openGripper();
  moveTo(xcase, ycase, z_upon_case, a0, a1, a2, a3);
}

void takeStock()
{
  moveTo(x_cases[2], y_cases[2], z_upon_case, a0, a1, a2, a3); 
  
  moveTo(x_stock, y_stock, z_upon_case, a0, a1, a2, a3);
  openGripper(); 
  moveTo(x_stock, y_stock, z_stock, a0, a1, a2, a3);
  closeGripper(); 
  moveTo(x_stock, y_stock, z_upon_case, a0, a1, a2, a3);
}

void PlayAndPutPiece(int x, int y) 
{
  takeStock(); 
  releasePiece(x, y); 
}


void take() {
  openGripper();
  closeGripper();
}

void openGripper()
{
  Vstep_gripper = 46;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);

}

void closeGripper()
{
  Vstep_gripper = 73;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);
}




// ##########################################################
//            Gameplay Functions
// ##########################################################


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


// Check if a piece can be put at the coordinates (x,y) on the board
bool canPutPiece(int x, int y, int sizeBoard, int b[][4])
{
    if (x >= 0 && x < sizeBoard)
        if (y >= 0 && y < sizeBoard)
        {
            if (b[x][y] == 0)
                return true;
        }

    return false;
}

// Put a piece on the coordinate (x, y) if it is possible
void putRealPiece(int x, int y, int sizeBoard, enum Color color, int b[][4])
{
    if (canPutPiece(x, y, sizeBoard, b))
    {
        b[x][y] = color;
        nbCasesAvailable--;
        PlayAndPutPiece(x, y); 
        
    }
    else
        Serial.println("CASE OCCUPEE - Proposez une autre case");
}

int putPiece(int x, int y, int sizeBoard, enum Color color, int b[][4])
{
    if (canPutPiece(x, y, sizeBoard, b))
    {
        b[x][y] = color;
        nbCasesAvailable--;
        return 1;
    }
    else
    {
        Serial.println("CASE OCCUPEE - Proposez une autre case");
        return 0;
    }
}

void removePiece(int x, int y, int b[][4])
{
    b[x][y] = 0;
    nbCasesAvailable++;
}

void push(coords* stack, int player, int x, int y, int b[][4])
{

    if (putPiece(x, y, 4, player, b))
    {
            coords c = {x, y};
        stack[sizeStack] = c;
        sizeStack++;
    }
}

void depush(coords* stack, int x, int y, int b[][4])
{
    sizeStack--;
    coords c = {0, 0};
    stack[sizeStack] = c;
    removePiece(x, y, b);
}

// Gets the name of the color from the enum
String nameColor(enum Color color)
{
    if (color == BLUE)
        return "Blue";
    else if (color == BLACK)
        return "Black";

    return "Color Unknown";
}

// Check at the end of the game the points
int checkPoints(int sizeBoard, enum Color color, int b[][4])
{
    int count = 0;
    int points = 0;

    // Horizontal verification
    for(int i=0; i<sizeBoard; i++)
    {
        count = 0;
        for(int j=0; j<sizeBoard; j++)
        {
            if (b[i][j] == color)
            {
                count++;
            }
        }

        if (count == 4)
        {
            points++;
        }
    }

    count = 0;
    // Vertical verification
    for(int i=0; i<sizeBoard; i++)
    {
        count = 0;
        for(int j=0; j<sizeBoard; j++)
        {
            if (b[j][i] == color)
            {
                count++;
            }
        }

        if (count == 4)
        {
            points++;
        }
    }

    // Diagonal verification left
    count = 0;
    for(int i=0; i<sizeBoard; i++)
    {
        if (b[i][i] == color)
            count++;

        if (count == 4)
        {
            points++;
        }
    }

    // Diagonal verification right
    count = 0;
    for(int i=sizeBoard-1; i>-1; i--)
    {
        if (b[i][i] == color)
            count++;

        if (count == 4)
        {
            points++;
        }
    }

    return points;
}

// Display the board (usefull for bebug especially)
void printBoard(int b[][4], int sizeBoard)
{
    for(int i=0; i<sizeBoard; i++)
    {
        for(int j=0; j<sizeBoard; j++)
        {
            Serial.print(b[j][i]);
            if (j == 3)
                Serial.print("\n");
        }
    }

    Serial.print("----------------\n");
}


coords* avalaibleMovements(int b[][4], int sizeBoard)
{
    int position = 0;
    int size_movements = 0;

    coords* movements = (coords*) malloc(nbCasesAvailable * sizeof(coords));

    for(int i=0; i<sizeBoard; i++)
    {
        for(int j=0; j<sizeBoard; j++)
        {
            if (canPutPiece(i, j, sizeBoard, b))
            {
                coords c = {i, j};
                movements[position] = c;
                position++;
            }
        }
    }

    return movements;
}

int getWinner(int pointsBlue, int pointsBlack)
{
  if (pointsBlue == pointsBlack)
  {
    Serial.print(pointsBlue);
    Serial.println(" points for blue and black !");
    Serial.println("No winner !");
    return 0;
  }
  else if (pointsBlue > pointsBlack)
  {
    Serial.print(pointsBlue);
    Serial.print(" points for blue !");
    Serial.println("Blue win !");
  }
  else
  {
    Serial.print(pointsBlack);
    Serial.print(" points for black !");
    Serial.println("Black win !");
  }
}

// ##########################################################
//            Dump AI
// ##########################################################

int heuristic(int b[][4], int sizeBoard, int x, int y, int player)
{
    int val = 0;
    int c = 0;
    int c_minus = 0;

    if (x > 0)
        if (b[x-1][y] == player)
            val++;

    if (x < sizeBoard-1)
        if (b[x+1][y] == player)
            val++;

    if (y > 0)
        if (b[x][y-1] == player)
            val++;

    if (y < sizeBoard-1)
        if (b[x][y+1] == player)
            val++;

    // Line
    for(int i=0; i<sizeBoard; i++)
    {
        if (b[i][y] == player)
            c++;

        if (b[i][y] == -1 * player)
            c_minus++;
    }

    if (c == 4)
    {
        val += 100;
    }

    if (c_minus == 3)
    {
        val+= 100;
    }


    c = 0;
    c_minus = 0;
    for(int i=0; i<sizeBoard; i++)
    {
        if (b[x][i] == player)
            c++;

        if (b[x][i] == -1 * player)
            c_minus++;
    }

    if (c == 4)
    {
        val += 100;
    }

    if (c_minus == 3)
    {
        val+= 100;
    }

    c = 0;
    c_minus = 0;
    // Diagonal
    if (x == y)
    {
         for(int i=0; i<sizeBoard; i++)
        {
            if (b[i][i] == player)
                c++;

            if (b[i][i] == -1 * player)
                c_minus++;
        }

        if (c == 4)
        {
            val += 100;
        }

        if (c_minus == 3)
        {
            val+= 100;
        }
        c=0;
        c_minus = 0;


        for(int i=sizeBoard-1; i>-1; i--)
        {
            if (b[i][i] == player)
                c++;

            if (b[i][i] == -1 * player)
                c_minus++;
        }

        if (c == 4)
        {
            val += 100;
        }

        if (c_minus == 3)
        {
            val+= 100;
        }
        c = 0;
        c_minus = 0;
    }


    return val;
}

coords minmax_move(int b[][4], coords* stack, int player)
{
    int val = 0;
    int new_val = 0;
    int c = 0;
    coords best_move = {0, 0};

    if (player)
    {
        val = -10000;
        coords* test_case = avalaibleMovements(b, 4);
        for (int i=0; i<nbCasesAvailable; i++)
        {
            coords move = {test_case[i].x, test_case[i].y};
            push(stack, player, move.x, move.y, b);

            new_val = heuristic(b, 4, move.x, move.y, player);
            if (new_val > val)
            {
                val = new_val;
                best_move = move;
            }
            depush(stack, test_case[i].x, test_case[i].y, b);
        }
    }


    return best_move;
}


// ##########################################################
//            Main loops Arduino
// ##########################################################


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

  // Rotate to get wrist in the good orientation 
  Vstep_wrist_rot = 90;
  Braccio.ServoMovement(longDelay, Vstep_base, Vstep_shoulder, Vstep_elbow, Vstep_wrist_ver, Vstep_wrist_rot, Vstep_gripper);



  for(int Y=0; Y<nb_row; Y++)
  {
    for(int X=0; X<nb_row; X++)
    {
      x_cases[nb_row*Y + X] = 0;
      y_cases[nb_row*Y + X] = 0;
      z_cases[nb_row*Y + X] = z_on_case;
    }
  }

  // Define the coordinates of all cases

  // Réserve de pièces
  x_stock = 187; 
  y_stock = 163;
  z_stock = -1;


  // Ligne 1
  x_cases[0] = 171;
  y_cases[0] = 239;
  z_cases[0] -= 1;

  x_cases[1] = 198;
  y_cases[1] = 207;

  x_cases[2] = 233;
  y_cases[2] = 166;
  z_cases[2] -= 2;

  x_cases[3] = 255;
  y_cases[3] = 136;
  z_cases[3] -= 2;

  // Ligne 2
  x_cases[4] = 194;
  y_cases[4] = 256;

  x_cases[5] = 218;
  y_cases[5] = 235;

  x_cases[6] = 240;
  y_cases[6] = 213;
  z_cases[6] += 2;

  x_cases[7] = 289;
  y_cases[7] = 152;
  z_cases[7] += 4;

  // Ligne 3
  x_cases[8] = 221;
  y_cases[8] = 286;

  x_cases[9] = 244;
  y_cases[9] = 262;
  z_cases[9] += 3;

  x_cases[10] = 267;
  y_cases[10] = 238;
  z_cases[10] += 2;

  x_cases[11] = 316;
  y_cases[11] = 177;

  // Ligne 4
  x_cases[12] = 244;
  y_cases[12] = 308;
  z_cases[12] += 10;

  x_cases[13] = 273;
  y_cases[13] = 277;
  z_cases[13] += 10;

  x_cases[14] = 302;
  y_cases[14] = 234;
  z_cases[14] += 10;

  x_cases[15] = 339;
  y_cases[15] = 191;
  z_cases[15] += 15;



  // ###########################
  // ###########################
  /*
    int sizeBoard = 4;
    nbCasesAvailable = sizeBoard * sizeBoard;

    int Board[][4] = { {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}
                    };

    
    int move = 0;
    int player = 1;
    int searchCase = true;

    printBoard(Board, 4);
    while(nbCasesAvailable > 0)
    {
        if (move = 0)
            player = 1;
        else
            player *= -1;

        coords* test_case;
        test_case = avalaibleMovements(Board, sizeBoard);

        int x = 0;
        int y = 0;

        int random = rand() % nbCasesAvailable;

        x = test_case[random].x;
        y = test_case[random].y;
        free(test_case);

        putPiece(x, y, sizeBoard, player, Board);
        printBoard(Board, 4);
    }

    int pointsBlue = checkPoints(sizeBoard, BLUE, Board);
    int pointsBlack = checkPoints(sizeBoard, BLACK, Board);

    getWinner(pointsBlue, pointsBlack);*/
    



  //takeStock(); 
  


  //parcoursMoveAPiece(4, 4);

  /*
  grabPiece(1, 0);
  releasePiece(0, 0);*/

  /*for (int i = 0; i < 4; i++){
    for (int j = 0; j < 4; j++){
      grabPiece(0, 0);
      releasePiece(i, j);
    }
  }*/
}




void loop() {


  // If we want to do a real game, else we don't do anything
  int sizeBoard = 4;
  nbCasesAvailable = sizeBoard * sizeBoard;

  int Board[][4] = { {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}
                    };

  int player = 1;
  int searchCase = true;

    // Stack
  coords* stack = malloc(sizeBoard * sizeBoard * sizeof(coords));
  sizeStack = 0;
  
  if (manual)
  {
       while(nbCasesAvailable > 0)
      {
        if (Serial.available() )
        {
          stringCase = Serial.readString();
          Serial.println(stringCase);
  
          play = changeStringToCase(stringCase);
          if (play)
          {
            // Play
            putRealPiece(chosenX, chosenY, sizeBoard, player, Board);
            printBoard(Board, 4);
            player *= -1; 
          }
        }
      }
  }
  else 
  {
      printBoard(Board, 4);
      while(nbCasesAvailable > 0)
      {
        // AI
        if (player == 1)
        {
          Serial.println("Coup IA"); 
          coords move = minmax_move(Board, stack, player);
          putRealPiece(move.x, move.y, sizeBoard, player, Board);
        }
        else 
        {
          
          Serial.println("Coup Bot"); 
          coords* test_case;
          test_case = avalaibleMovements(Board, sizeBoard);
  
          int x = 0;
          int y = 0;
  
          int rand = random(0, nbCasesAvailable); 
          Serial.println(rand);
          x = test_case[rand].x;
          y = test_case[rand].y;
          free(test_case);
  
          putRealPiece(x, y, sizeBoard, player, Board);
          printBoard(Board, 4);
        }

        player *= -1;
      }

  }

  free(stack); 

 // Bug display! 
  int pointsBlue = checkPoints(sizeBoard, BLUE, Board);
  int pointsBlack = checkPoints(sizeBoard, BLACK, Board);

  getWinner(pointsBlue, pointsBlack);

  exit(0); 
  
}





  
