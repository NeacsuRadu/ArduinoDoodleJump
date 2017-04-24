
#define DOODLE_JUMPING 1 
#define DOODLE_IDLE 2

#define JUMP_DURATION 2000 

#define ACCELERATED_JUMP_DURATION 600
#define SLOW_JUMP_DURATION 1800

#define SLOW_FALL_DURATION 200

#define IDLE_TIME 200 

#define ACC_UPDATE_INTERVAL 100
#define SLOW_UPDATE_INTERVAL 200

#define READ_SENSOR_INTERVAL 100

#define TIME_TO_START 500

#define YAXIS A0

struct paddle
{
  int8_t rowCoord;
  int8_t colCoord;
  paddle(int8_t rowCoord = 0, int8_t colCoord = 0):rowCoord(rowCoord), colCoord(colCoord){} 
};



paddle paddles[10];

const uint8_t row[8] = {2, 7, 19, 5, 13, 18, 12, 16};
const uint8_t col[8] = {6, 11, 10, 3, 17, 4, 8, 9};

int8_t last = 0;
int8_t doodleRowCoord = 7;
int8_t doodleColCoord = 3;

int8_t doodleState = 0;
int8_t distanceBetweenPaddles = 2;
int8_t sidewaysDirection = 1;
int8_t horizontalMovement = 0;

long acceleration = 0;

paddle cifre[10][13];

unsigned long doodleFallStartTime     = 0;
unsigned long doodleJumpStartTime     = 0;
unsigned long doodleLastUpdateTime    = 0;
unsigned long doodleLastReadOnSensor  = 0;
unsigned long startGameTime = 0;

long globalScore = 0;
long localScore  = 0;

bool inGameOverState = true;

long reverseScore;

void setup() 
{
    cifre[0][0] = paddle(0,0);
    cifre[0][1] = paddle(0,1);
    cifre[0][2] = paddle(0,2);
    cifre[0][3] = paddle(1,0);
    cifre[0][4] = paddle(1,2);
    cifre[0][5] = paddle(2,0);
    cifre[0][6] = paddle(2,2);
    cifre[0][7] = paddle(3,0);
    cifre[0][8] = paddle(3,2);
    cifre[0][9] = paddle(4,0);
    cifre[0][10] = paddle(4,1);
    cifre[0][11] = paddle(4,2);
    cifre[0][12] = paddle(-1,-1);
    
    cifre[1][0] = paddle(0, 2);
    cifre[1][1] = paddle(1, 2);
    cifre[1][2] = paddle(2, 2);
    cifre[1][3] = paddle(3, 2);
    cifre[1][4] = paddle(4, 2);
    cifre[1][5] = paddle(-1,-1);
     
    cifre[2][0] = paddle(0,0);
    cifre[2][1] = paddle(0,1);
    cifre[2][2] = paddle(0,2);
    cifre[2][3] = paddle(1,2);
    cifre[2][4] = paddle(2,0);
    cifre[2][5] = paddle(2,1);
    cifre[2][6] = paddle(2,2);
    cifre[2][7] = paddle(3,0);
    cifre[2][8] = paddle(4,0);
    cifre[2][9] = paddle(4,1);
    cifre[2][10] = paddle(4,2);
    cifre[2][11] = paddle(-1,-1);

    cifre[3][0] = paddle(0,0);
    cifre[3][1] = paddle(0,1);
    cifre[3][2] = paddle(0,2);
    cifre[3][3] = paddle(1,2);
    cifre[3][4] = paddle(2,0);
    cifre[3][5] = paddle(2,1);
    cifre[3][6] = paddle(2,2);
    cifre[3][7] = paddle(3,2);
    cifre[3][8] = paddle(4,0);
    cifre[3][9] = paddle(4,1);
    cifre[3][10] = paddle(4,2);
    cifre[3][11] = paddle(-1,-1);

    cifre[4][0] = paddle(0,0);
    cifre[4][1] = paddle(0,2);
    cifre[4][2] = paddle(1,0);
    cifre[4][3] = paddle(1,2);
    cifre[4][4] = paddle(2,0);
    cifre[4][5] = paddle(2,1);
    cifre[4][6] = paddle(2,2);
    cifre[4][7] = paddle(3,2);
    cifre[4][8] = paddle(4,2);
    cifre[4][9] = paddle(-1,-1);
    
    cifre[5][0] = paddle(0,0);
    cifre[5][1] = paddle(0,1);
    cifre[5][2] = paddle(0,2);
    cifre[5][3] = paddle(1,0);
    cifre[5][4] = paddle(2,0);
    cifre[5][5] = paddle(2,1);
    cifre[5][6] = paddle(2,2);
    cifre[5][7] = paddle(3,2);
    cifre[5][8] = paddle(4,0);
    cifre[5][9] = paddle(4,1);
    cifre[5][10] = paddle(4,2);
    cifre[5][11] = paddle(-1,-1);
    
    cifre[6][0] = paddle(0,0);
    cifre[6][1] = paddle(0,1);
    cifre[6][2] = paddle(0,2);
    cifre[6][3] = paddle(1,0);
    cifre[6][4] = paddle(2,0);
    cifre[6][5] = paddle(2,1);
    cifre[6][6] = paddle(2,2);
    cifre[6][7] = paddle(3,0);
    cifre[6][8] = paddle(3,2);
    cifre[6][9] = paddle(4,0);
    cifre[6][10] = paddle(4,1);
    cifre[6][11] = paddle(4,2);
    cifre[6][12] = paddle(-1,-1);

    cifre[7][0] = paddle(0,0);
    cifre[7][1] = paddle(0,1);
    cifre[7][2] = paddle(0,2);
    cifre[7][3] = paddle(1,2);
    cifre[7][4] = paddle(2,2);
    cifre[7][5] = paddle(3,2);
    cifre[7][6] = paddle(4,2); 
    cifre[7][7] = paddle(-1,-1);

    cifre[8][0] = paddle(0,0);
    cifre[8][1] = paddle(0,1);
    cifre[8][2] = paddle(0,2);
    cifre[8][3] = paddle(1,0);
    cifre[8][4] = paddle(1,2);
    cifre[8][5] = paddle(2,0);
    cifre[8][6] = paddle(2,1);
    cifre[8][7] = paddle(2,2);
    cifre[8][8] = paddle(3,0);
    cifre[8][9] = paddle(3,2);
    cifre[8][10] = paddle(4,0);
    cifre[8][11] = paddle(4,1);
    cifre[8][12] = paddle(4,2);

    cifre[9][0] = paddle(0,0);
    cifre[9][1] = paddle(0,1);
    cifre[9][2] = paddle(0,2);
    cifre[9][3] = paddle(1,0);
    cifre[9][4] = paddle(1,2);
    cifre[9][5] = paddle(2,0);
    cifre[9][6] = paddle(2,1);
    cifre[9][7] = paddle(2,2);
    cifre[9][8] = paddle(3,2);
    cifre[9][9] = paddle(4,0);
    cifre[9][10] = paddle(4,1);
    cifre[9][11] = paddle(4,2);
    cifre[9][12] = paddle(-1,-1);
   
    for (int thisPin = 0; thisPin < 8; thisPin++) 
    {
        pinMode(col[thisPin], OUTPUT); pinMode(row[thisPin], OUTPUT);
    }
    setGameOverState();
}

// ------------ PADDLES FUNCTIONS ------------------ BEGIN


paddle generatePaddleBetween(int8_t r1, int8_t r2, int8_t c1, int8_t c2)
{
    return paddle(random(r1, r2), random(c1, c2));
}

void push_back(paddle pad)
{
    if (last < 10)
    {
        paddles[last++] = pad;
    }
}

void pop_front()
{
    for (int i = 1; i < last; ++i)
    {
        paddles[i-1] = paddles[i];
    }
    if (last > 0)
    {
        --last;
    }
}

// ------------ PADDLES FUNCTIONS ------------------ END



void initGame()
{
    last = 0;
    doodleRowCoord = 7;
    doodleColCoord = 3;
    doodleState = 0;
    distanceBetweenPaddles = 2;
    sidewaysDirection = 1;
    horizontalMovement = 0;
    acceleration = 0;
    doodleFallStartTime     = 0;
    doodleJumpStartTime     = 0;
    doodleLastUpdateTime    = 0;
    doodleLastReadOnSensor  = 0;
    globalScore = 0;
    localScore  = 0;
    
    for (int i = 6; i >= -2; i -= 2)
    {
        push_back(generatePaddleBetween(i,i+1,0,7)); 
    }
    for (int thisPin = 0; thisPin < 8; thisPin++) 
    {
        digitalWrite(col[thisPin], HIGH); digitalWrite(row[thisPin], LOW
        );
    }
    
    startJumpingLittleDoodle();
}

void startLed(int c, int r)
{
    digitalWrite(col[c], LOW);
    digitalWrite(row[r], HIGH);

    digitalWrite(col[c], HIGH);
    digitalWrite(row[r], LOW);
}

void digit(int8_t digit, int8_t nr)
{
    for(int8_t i = 0; i <= 12 && cifre[digit][i].rowCoord != -1; ++i)
    {
        startLed(cifre[digit][i].colCoord + nr*3 + 1, cifre[digit][i].rowCoord);
    }
}

void loop() 
{
    if (!inGameOverState)
    {
        displayObjects();
        readSensors();
        updateObjects();
    }
    else
    {
        long sc = globalScore;
        sc = sc/10;
        digit(sc%10, 1);
        sc = sc/10;
        if(sc != 0)
          digit(sc%10, 0);
        if (gameShouldStart())
        {
            startGame();
        }
    }
}

// ------- GAME STATES BEGIN -------------

void startGame()
{
    inGameOverState = false;
    initGame();
}

void enterGameOverState() // 
{
    inGameOverState = true;
    setGameOverState();
}

void getRevScore()
{
    long sc = globalScore;
    reverseScore = 0;
    while(sc)
    {
      reverseScore = reverseScore*10 + sc%10;
      sc /= 10;
    }
}

void setGameOverState() // make the matrix red 
{
    getRevScore();
    for (int thisPin = 0; thisPin < 8; thisPin++) 
    {
        digitalWrite(col[thisPin], HIGH); digitalWrite(row[thisPin], LOW);
    }
}

// ---------- GAME STATES END ------------------

int8_t minn(int8_t m1, int8_t m2)
{
    return (m1 - m2 < 0 ? m1 : m2);
}

// ------------- UPDATE BEGIN ---------------------

void updateObjects()
{
    updateDoodlePosition();
}


void startJumpingLittleDoodle()
{
    acceleration = ACC_UPDATE_INTERVAL;
    doodleJumpStartTime = millis();
    doodleLastUpdateTime = millis();
    doodleState |= DOODLE_JUMPING;
}

bool doodleShouldJump() // check if doodle can jump, i.e has a paddle below him 
{
    for (int i = 0; i < last; ++i)
    {
       if (paddles[i].rowCoord == doodleRowCoord + 1 && (paddles[i].colCoord == doodleColCoord || paddles[i].colCoord == doodleColCoord - 1))
           return true;
    }
    return false;
}

void addModePaddles() // generate a new paddle and add it into the vector 
{
    push_back(generatePaddleBetween(-distanceBetweenPaddles, 0, 0, 7));
}

bool shouldAddMorePaddles() 
{
    if (paddles[last-1].rowCoord >= 0)
    {
        return true;
    }
    return false;
}

void updatePaddles() // just increasing the paddles' row coord and pop the ones that are out of the matrix 
{
    while (paddles[0].rowCoord >= 7)
    {
        pop_front();
    }
    for (int i = 0; i < last; ++i)
    {
        paddles[i].rowCoord ++;
    }
}

void updateHorizontalPosition()
{
    doodleColCoord += horizontalMovement;
    if (doodleColCoord == -1) // if doodle is out of bounds we make it appear on the other side 
    {
        doodleColCoord = 7;
    }
    else if (doodleColCoord == 8)
    {
        doodleColCoord = 0;
    }
}

void updateDoodlePosition()
{
  updateHorizontalPosition(); // we update the horizontal position based on the sensor reading 
  if ((doodleState & DOODLE_JUMPING)) // doodle is jumping 
  {
      if (millis() - doodleJumpStartTime > JUMP_DURATION) // jump is done, start falling 
      {
          doodleState &= !DOODLE_JUMPING;
          doodleLastUpdateTime = millis();
          doodleFallStartTime = millis();
          acceleration = ACC_UPDATE_INTERVAL;
      }
      else if (millis() - doodleJumpStartTime > SLOW_JUMP_DURATION) // changing the update interval, realistic jump :D 
      {
          doodleLastUpdateTime = millis();
      }
      else if (millis() - doodleJumpStartTime > ACCELERATED_JUMP_DURATION)
      {
          acceleration = SLOW_UPDATE_INTERVAL;
      }

      if (millis() - doodleLastUpdateTime > acceleration) //updating the doodle position 
      {
          localScore += 5;
          if (doodleRowCoord > 4) // if doodle is below middle we decrease doodle's row coord 
          { 
              --doodleRowCoord;
          }
          else // doodle is above, we increase paddles' row, doodle remains in position, same visual result
          {
              updatePaddles();
              if (shouldAddMorePaddles())
              {
                  addModePaddles();
              }
          }
          doodleLastUpdateTime = millis();
      }
  }
  else // doodle is falling 
  {
     if (doodleRowCoord == 7)
     {
        enterGameOverState();
     }
     if (doodleShouldJump())
     {
        startJumpingLittleDoodle();
        return;
     }

      if (millis() - doodleFallStartTime > SLOW_FALL_DURATION)
      {
         acceleration = SLOW_UPDATE_INTERVAL;
      }

      if (millis() - doodleLastUpdateTime > acceleration)
      {
         localScore -= 5;
         ++doodleRowCoord;
         doodleLastUpdateTime = millis();
      }
  }
  if (localScore > globalScore)
  {
      globalScore = localScore;
      updateDifficulty();
  }
}

void updateDifficulty()
{
    if (globalScore % 250 == 0) // change the horizontal direction by 180 deg
    {
        sidewaysDirection *= (-1);
    }
    if (globalScore % 200 == 0) // increases the maximum distance between paddles 
    {
        distanceBetweenPaddles = minn(10, distanceBetweenPaddles + 1);
    }
}

// ------------- UPDATE END -----------------------



// ------------ DISPLAY BEGIN ---------------

void displayObjects()
{
    startLed(doodleColCoord, doodleRowCoord);
    for (int i = 0; i < last; ++i)
    {
        if (paddles[i].rowCoord >= 0)
        {
            startLed(paddles[i].colCoord, paddles[i].rowCoord);
            startLed(paddles[i].colCoord + 1, paddles[i].rowCoord);
        }
    }
}

// --------------- DISPLAY END --------------------------------------------


// --------------------- READING SENSORS BEGIN ------------------------------

void readSensors()
{
    if (millis() - doodleLastReadOnSensor > READ_SENSOR_INTERVAL)// we read the sensors periodically :D 
    {
        doodleLastReadOnSensor = millis();
        int reading = analogRead(YAXIS);

        if (reading < 100) // updating the variable wich contains the horizontal movement
        {
            horizontalMovement = -1 * sidewaysDirection; // sidewaysDirection changes the direction depending on difficulty
        }
        else if (reading > 900)
        {
            horizontalMovement = 1 * sidewaysDirection;
        }
        else
        {
            horizontalMovement = 0;
        }
    }
    else 
    {
        horizontalMovement = 0;
    }
}

bool gameShouldStart()
{
    int reading = analogRead(YAXIS);
    if (reading > 900 || reading < 100)
    {
        if (startGameTime == 0)
        {
            startGameTime = millis();
        }
        if (millis() - startGameTime > TIME_TO_START)
        {
            startGameTime = 0;
            return true;
        }
    }
    else 
    {
        startGameTime = 0;
    }
    return false;
}

// ------------ READING SENSORS END ------------------
