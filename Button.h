#ifndef buttonDetect_h
#define buttonDetect_h

//#include "arduino.h"

class Button
{

  protected:
    void (*shortDetectedFunc)();
    volatile unsigned long fallEventTime = 0;
    volatile unsigned long riseEventTime = 0;
    int pin;
    int pressTime;
    bool currentPinState;
    int holdTime;

  public:
    Button (int myPin, void (*myShortDetectedFunc)(), int myHoldTime );
    void updateHoldTime (int newHoldTime);
    boolean checkForHold(void);
    void checkForPress(void);
    void updateRiseTime(void);

};
#endif
