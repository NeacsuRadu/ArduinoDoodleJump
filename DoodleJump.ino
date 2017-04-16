
#define DOODLE_JUMPING 1 
#define DOODLE_IDLE 2

#define JUMP_DURATION 500 

#define ACCELERATED_JUMP_DURATION 200
#define SLOW_JUMP_DURATION 400

#define SLOW_FALL_DURATION 200

#define IDLE_TIME 100 

#define ACC_UPDATE_INTERVAL 50
#define SLOW_UPDATE_INTERVAL 100

#define READ_SENSOR_INTERVAL 100

#define YAXIS A0



#include <ArduinoSTL.h>

struct pereche
{
  int8_t first;
  int8_t second;

};

std::vector<pereche > paddles;

// 2-dimensional array of row pin numbers:
const uint8_t row[8] = {
  2, 7, 19, 5, 13, 18, 12, 16
};

// 2-dimensional array of column pin numbers:
const uint8_t col[8] = {
  6, 11, 10, 3, 17, 4, 8, 9
};

uint8_t doodleRowCoord = 7;
uint8_t doodleColCoord = 3;

int8_t doodleState = 0;

long acceleration = 0;

unsigned long doodleFallStartTime = 0;
unsigned long doodleJumpStartTime = 0;
unsigned long doodleLastUpdateTime = 0;

unsigned long doodleLastReadOnSensor = 0;


void setup() {
  // initialize the I/O pins as outputs
  // iterate over the pins:
  for (int thisPin = 0; thisPin < 8; thisPin++) {
    // initialize the output pins:
    pinMode(col[thisPin], OUTPUT);
    pinMode(row[thisPin], OUTPUT);
    // take the col pins (i.e. the cathodes) high to ensure that
    // the LEDS are off:
    digitalWrite(col[thisPin], HIGH);
    digitalWrite(row[thisPin], LOW);
  }

  initGame();

  Serial.begin(9600);
}

void initGame()
{
    long col;
    pereche per;
    col = random(0, 7); 
    per.first = 6; 
    per.second = col;
    paddles.push_back(per);
    col = random(0, 7); 
    per.first = 4; 
    per.second = col;
    paddles.push_back(per);
    col = random(0, 7); 
    per.first = 2; 
    per.second = col;
    paddles.push_back(per);
    col = random(0, 7); 
    per.first = 0; 
    per.second = col;
    paddles.push_back(per);
}

void startLed(int c, int r)
{
  digitalWrite(col[c], LOW);
  digitalWrite(row[r], HIGH);

  digitalWrite(col[c], HIGH);
  digitalWrite(row[r], LOW);
}

void loop() {
  readSensors();
  updateObjects();
  displayObjects();
}


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
          --doodleRowCoord;
          doodleLastUpdateTime = millis();
          return;
      }
  }
  else
  {
    if(doodleRowCoord == 7)
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
}

void displayObjects()
{
    startLed(doodleColCoord, doodleRowCoord);
    for(std::vector<pereche>::iterator it = paddles.begin(); it != paddles.end(); ++it)
    {
        startLed(it->second, it->first);
        startLed(it->second + 1, it->first);
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

