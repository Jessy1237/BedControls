#include "arduino.h"
#include "Button.h"

#define DEBOUNCE_TIME 10
#define OVERFLOW_TIME 20000
#define MICROS_OVERFLOW 0xFFFFFFFF

Button::Button(int myPin, void (*myShortDetectedFunc)(), int myHoldTime)
{

  pin = myPin;
  holdTime = myHoldTime;
  shortDetectedFunc = myShortDetectedFunc;

  currentPinState = digitalRead(myPin);
}


void Button::updateHoldTime(int newHoldTime)
{
  holdTime = newHoldTime;
}

void Button::updateRiseTime(void)
{
    riseEventTime = micros() / 1000;
}

boolean Button::checkForHold()
{
  unsigned long currentTime = micros() / 1000;

  if (fallEventTime > riseEventTime)
  {
    if (currentPinState == 0 && digitalRead(pin) == 0)
    {
      if ( (currentTime - fallEventTime) > DEBOUNCE_TIME )
      {

        if ( (currentTime - fallEventTime) > OVERFLOW_TIME )
        {
          pressTime = MICROS_OVERFLOW / 1000 - fallEventTime + currentTime;
        }
        else
        {
          pressTime = currentTime - fallEventTime;
        }

        if ( pressTime >= holdTime )
        {
          return true;
        }
      }
    }
  }

  currentPinState = digitalRead(pin);

  return false;
}

void Button::checkForPress()
{
  if ( currentPinState == 1 && (currentPinState != digitalRead(pin)) )
  {

    fallEventTime = micros() / 1000;
    currentPinState = digitalRead(pin);
  }

  else if ( currentPinState == 0 && (currentPinState != digitalRead(pin)) )
  {
    riseEventTime = micros() / 1000;
    currentPinState = digitalRead(pin);

    if ( (riseEventTime - fallEventTime) > DEBOUNCE_TIME )
    {

      if ( (riseEventTime - fallEventTime) > OVERFLOW_TIME )
      {
        pressTime = MICROS_OVERFLOW / 1000 - fallEventTime + riseEventTime;
      }
      else
      {
        pressTime = riseEventTime - fallEventTime;
      }

      if ( pressTime < holdTime )
      {
        shortDetectedFunc();
      }
    }
  }
}
