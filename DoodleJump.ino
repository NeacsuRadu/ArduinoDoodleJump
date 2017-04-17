
#define DOODLE_JUMPING 1 
#define DOODLE_IDLE 2

#define JUMP_DURATION 1000 

#define ACCELERATED_JUMP_DURATION 200
#define SLOW_JUMP_DURATION 800

#define SLOW_FALL_DURATION 200

#define IDLE_TIME 200 

#define ACC_UPDATE_INTERVAL 100
#define SLOW_UPDATE_INTERVAL 200

#define READ_SENSOR_INTERVAL 100

#define YAXIS A0

struct paddle
{
  int8_t rowCoord;
  int8_t colCoord;
  paddle(int8_t rowCoord = 0, int8_t colCoord = 0):rowCoord(rowCoord), colCoord(colCoord){} 
};

paddle paddles[10];
int8_t first = 0;
int8_t last = 0;

const uint8_t row[8] = {2, 7, 19, 5, 13, 18, 12, 16};
const uint8_t col[8] = {6, 11, 10, 3, 17, 4, 8, 9};

uint8_t doodleRowCoord = 7;
uint8_t doodleColCoord = 3;

int8_t doodleState = 0;
int8_t distanceBetweenPaddles = 2;

long acceleration = 0;

unsigned long doodleFallStartTime     = 0;
unsigned long doodleJumpStartTime     = 0;
unsigned long doodleLastUpdateTime    = 0;
unsigned long doodleLastReadOnSensor  = 0;

unsigned long globalScore = 0;
unsigned long localScore  = 0;

void setup() 
{
    for (int thisPin = 0; thisPin < 8; thisPin++) 
    {
        pinMode(col[thisPin], OUTPUT); pinMode(row[thisPin], OUTPUT);
        digitalWrite(col[thisPin], HIGH); digitalWrite(row[thisPin], LOW);
    }
    initGame();
    Serial.begin(9600);
}

// ------------ PADDLES FUNCTIONS ------------------ BEGIN


paddle generatePaddleBetween(int8_t r1, int8_t r2, int8_t c1, int8_t c2)
{
    return paddle(random(r1, r2), random(c1, c2));
}

void push_back(paddle pad)
{
    if(last < 10)
    {
        paddles[last++] = pad;
    }
}

void pop_front()
{
    for(int i = 1; i < last; ++i)
    {
        paddles[i-1] = paddles[i];
    }
    if(last > 0)
    {
        --last;
    }
}

// ------------ PADDLES FUNCTIONS ------------------ END


void initGame()
{
    push_back(paddle(8, 3));
    for(int i = 6; i >= 0; i -= 2)
    {
        push_back(generatePaddleBetween(i,i+1,0,7)); 
    }
}

void startLed(int c, int r)
{
    digitalWrite(col[c], LOW);
    digitalWrite(row[r], HIGH);

    digitalWrite(col[c], HIGH);
    digitalWrite(row[r], LOW);
}

void loop() 
{
    readSensors();
    updateObjects();
    displayObjects();
}


void updateObjects()
{
    updateDoodlePosition();
    updateDifficulty();
}

void startJumpingLittleDoodle()
{
    acceleration = ACC_UPDATE_INTERVAL;
    doodleJumpStartTime = millis();
    doodleLastUpdateTime = millis();
    doodleState |= DOODLE_JUMPING;
}

bool doodleShouldJump()
{
    for(int i = 0; i < last; ++i)
    {
       if(paddles[i].rowCoord == doodleRowCoord + 1 && (paddles[i].colCoord == doodleColCoord || paddles[i].colCoord == doodleColCoord - 1))
           return true;
    }
    return false;
}

bool shouldAddMorePaddles()
{
    if(paddles[last-1].rowCoord >= distanceBetweenPaddles)
    {
        return true;
    }
    return false;
}

void addModePaddles()
{
    push_back(generatePaddleBetween(0, distanceBetweenPaddles, 0, 7));
}

void updatePaddles()
{
    while(paddles[0].rowCoord >= 7)
    {
        pop_front();
    }
    for(int i = 0; i < last; ++i)
    {
        paddles[i].rowCoord ++;
    }
}

void updateDoodlePosition()
{
  if((doodleState & DOODLE_JUMPING))
  {
      if(millis() - doodleJumpStartTime > JUMP_DURATION)
      {
          doodleState &= !DOODLE_JUMPING;
          doodleLastUpdateTime = millis();
          doodleFallStartTime = millis();
          acceleration = ACC_UPDATE_INTERVAL;
      }
      else if(millis() - doodleJumpStartTime > SLOW_JUMP_DURATION)
      {
          doodleLastUpdateTime = millis();
      }
      else if(millis() - doodleJumpStartTime > ACCELERATED_JUMP_DURATION)
      {
          acceleration = SLOW_UPDATE_INTERVAL;
      }

      if(millis() - doodleLastUpdateTime > acceleration)
      {
          if(doodleRowCoord > 4)
          { 
              ++localScore;
              --doodleRowCoord;
          }
          else
          {
              updatePaddles();
              if(shouldAddMorePaddles())
              {
                  addModePaddles();
              }
          }
          doodleLastUpdateTime = millis();
      }
  }
  else
  {
    --localScore;
    if(doodleShouldJump())
    {
       startJumpingLittleDoodle();
       return;
    }

    if(millis() - doodleFallStartTime > SLOW_FALL_DURATION)
    {
        acceleration = SLOW_UPDATE_INTERVAL;
    }


    if(millis() - doodleLastUpdateTime > acceleration)
    {
        ++doodleRowCoord;
        doodleLastUpdateTime = millis();
    }
  }
  if(localScore > globalScore)
  {
      globalScore = localScore;
  }
}

int8_t minn(int8_t m1, int8_t m2)
{
    return (m1 - m2 < 0 ? m1 : m2);
}

void updateDifficulty()
{
    if(globalScore % 100 == 0)
    {
        distanceBetweenPaddles = minn(5, distanceBetweenPaddles + 1);
    }
}

void displayObjects()
{
    startLed(doodleColCoord, doodleRowCoord);
    for(int i = 0; i < last; ++i)
    {
        startLed(paddles[i].colCoord, paddles[i].rowCoord);
        startLed(paddles[i].colCoord + 1, paddles[i].rowCoord);
    }
}

void readSensors()
{
    if(millis() - doodleLastReadOnSensor > READ_SENSOR_INTERVAL)
    {
        doodleLastReadOnSensor = millis();
        int reading = analogRead(YAXIS);

        if(reading < 100)
        {
            if(doodleColCoord == 0)
            {
                doodleColCoord = 8;
            }
            --doodleColCoord;
        }
        else if(reading > 900)
        {
            if(doodleColCoord == 7)
            {
                doodleColCoord = -1;
            }
            ++doodleColCoord;
        }
    }
}

