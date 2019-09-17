#include "Button.h"

//Input pins for buttons
#define PORT_1_PIN 6
#define PORT_2_PIN 7
#define PORT_3_PIN 8
#define PORT_4_PIN 9

//Sensing pins for ports
#define PORT_1_SEN 2
#define PORT_2_SEN 3
#define PORT_3_SEN 4
#define PORT_4_SEN 5

//Output pins for buttons
#define PORT_1_OUT A0
#define PORT_2_OUT A1
#define PORT_3_OUT A2
#define PORT_4_OUT A3

#define LED_PIN 10

//Mode indexes
#define ONE_SW 1
#define TWO_SW 2
#define FOUR_SW 3

//How long until a button is considered being held down
#define HOLD_TIME 1000

//How long should the relay be active for a short press
#define PRESS_DELAY 200

//Bed Controller Button Modes
#define FEET_UP 1
#define FEET_DOWN 2
#define HEAD_UP 3
#define HEAD_DOWN 4

#define MAX_FUNCS 4

int mode = 0;
int selection = 1;

//Functions for a hold for the buttons
void oneSWHold(void);
void twoSWHold(void);
void fourSWHold(void);

void unknown(void);
void changed(void);

//Functions for a short press for the buttons
void button1Pressed(void);
void button2Pressed(void);
void button3Pressed(void);
void button4Pressed(void);

void changeSelection(void);
int getOutPin(void);

Button b1 = Button(PORT_1_PIN, button1Pressed, HOLD_TIME);
Button b2 = Button(PORT_2_PIN, button2Pressed, HOLD_TIME);
Button b3 = Button(PORT_3_PIN, button3Pressed, HOLD_TIME);
Button b4 = Button(PORT_4_PIN, button4Pressed, HOLD_TIME);

void setup()
{
  pinMode(PORT_1_PIN, INPUT_PULLUP);
  pinMode(PORT_2_PIN, INPUT_PULLUP);
  pinMode(PORT_3_PIN, INPUT_PULLUP);
  pinMode(PORT_4_PIN, INPUT_PULLUP);
  pinMode(PORT_1_SEN, INPUT_PULLUP);
  pinMode(PORT_2_SEN, INPUT_PULLUP);
  pinMode(PORT_3_SEN, INPUT_PULLUP);
  pinMode(PORT_4_SEN, INPUT_PULLUP);
  pinMode(PORT_1_OUT, OUTPUT);
  pinMode(PORT_2_OUT, OUTPUT);
  pinMode(PORT_3_OUT, OUTPUT);
  pinMode(PORT_4_OUT, OUTPUT);

  mode = getMode();

  Serial.begin(9600);
}

void loop()
{
  char str[100] = "";
  sprintf(str, "Selection: %d", selection);
  Serial.println(str);
  if (getMode() != mode)
  {
    Serial.println("MODE CHANGED!");
    changed();
  }
  else
  {
    if (mode == ONE_SW)
    {
      Serial.println("ONE SW");
      b1.checkForPress();
      oneSWHold();
    }
    else if (mode == TWO_SW)
    {
      Serial.println("TWO SW");
      b1.checkForPress();
      b2.checkForPress();
      twoSWHold();
    }
    else if (mode == FOUR_SW)
    {
      Serial.println("FOUR SW");
      b1.checkForPress();
      b2.checkForPress();
      b3.checkForPress();
      b4.checkForPress();
      fourSWHold();
    }
    else
    {
      Serial.println("UNKNOWN MODE");
      unknown();
    }
  }
}

int getMode(void)
{

  if (digitalRead(PORT_1_SEN) == HIGH && digitalRead(PORT_2_SEN) == LOW && digitalRead(PORT_3_SEN) == LOW && digitalRead(PORT_4_SEN) == LOW)
  {
    return ONE_SW;
  }

  if (digitalRead(PORT_1_SEN) == HIGH && digitalRead(PORT_2_SEN) == HIGH && digitalRead(PORT_3_SEN) == LOW && digitalRead(PORT_4_SEN) == LOW)
  {
    return TWO_SW;
  }

  if (digitalRead(PORT_1_SEN) == HIGH && digitalRead(PORT_2_SEN) == HIGH && digitalRead(PORT_3_SEN) == HIGH && digitalRead(PORT_4_SEN) == HIGH)
  {
    return FOUR_SW;
  }

  return 0;
}

void oneSWHold(void)
{
  //Check for long hold on the button
  if (b1.checkForHold())
  {
    int pin =  getOutPin();
    digitalWrite(pin, HIGH);
  }
  else
  {
    int pin = getOutPin();
    digitalWrite(pin, LOW);
  }
}

void twoSWHold(void)
{

  //Check our activation button is being held
  if (b2.checkForHold() && !b1.checkForHold())
  {
    int pin = getOutPin();
    digitalWrite(pin, HIGH);
  }
  else
  {
    int pin =  getOutPin();
    digitalWrite(pin, LOW);
  }

}

void fourSWHold(void)
{

  //Check if our button is being held and its paired control button isn't being held then write a HIGH to the relay otherwise write a LOW
  if (b1.checkForHold() && !b2.checkForHold())
  {
    digitalWrite(PORT_1_OUT, HIGH);
  }
  else
  {
    digitalWrite(PORT_1_OUT, LOW);
  }

  if (b2.checkForHold() && !b1.checkForHold())
  {
    digitalWrite(PORT_2_OUT, HIGH);
  }
  else
  {
    digitalWrite(PORT_2_OUT, LOW);
  }

  if (b3.checkForHold() && !b4.checkForHold())
  {
    digitalWrite(PORT_3_OUT, HIGH);
  }
  else
  {
    digitalWrite(PORT_3_OUT, LOW);
  }

  if (b4.checkForHold() && !b3.checkForHold())
  {
    digitalWrite(PORT_4_OUT, HIGH);
  }
  else
  {
    digitalWrite(PORT_4_OUT, LOW);
  }

}

void unknown(void)
{
  changed();
}

void changed(void)
{
  digitalWrite(PORT_1_OUT, LOW);
  digitalWrite(PORT_2_OUT, LOW);
  digitalWrite(PORT_3_OUT, LOW);
  digitalWrite(PORT_4_OUT, LOW);

  //Update our rise time so it resets our hold timer. i.e. virtually acting if the action button was released
  b1.updateRiseTime();
  b2.updateRiseTime();
  b3.updateRiseTime();
  b4.updateRiseTime();
}

void button1Pressed(void)
{
  if (getMode() != mode)
  {
    Serial.println("MODE CHANGED!");
    changed();
  }
  else
  {
    if (mode == ONE_SW) //Button 1 is selector button
    {
      Serial.println("ONE SW");
      changeSelection();
    }
    else if (mode == TWO_SW)
    {
      Serial.println("TWO SW");
      changeSelection();
    }
    else if (mode == FOUR_SW) //Button 1 is now activation for pin 1
    {
      Serial.println("FOUR SW");
      digitalWrite(PORT_1_OUT, HIGH);
      delay(PRESS_DELAY);
      digitalWrite(PORT_1_OUT, LOW);
    }
    else
    {
      Serial.println("UNKNOWN MODE");
      unknown();
    }
  }
}

void button2Pressed(void)
{
  if (getMode() != mode)
  {
    Serial.println("MODE CHANGED!");
    changed();
  }
  else
  {
    if (mode == ONE_SW)
    {
      Serial.println("ONE SW");
    }
    else if (mode == TWO_SW) //Button 2 is our activation button in this scenario
    {
      Serial.println("TWO SW");
      int pin =  getOutPin();
      digitalWrite(pin, HIGH);
      delay(PRESS_DELAY);
      digitalWrite(pin, LOW);
    }
    else if (mode == FOUR_SW)
    {
      Serial.println("FOUR SW");
      digitalWrite(PORT_2_OUT, HIGH);
      delay(PRESS_DELAY);
      digitalWrite(PORT_2_OUT, LOW);
    }
    else
    {
      Serial.println("UNKNOWN MODE");
      unknown();
    }
  }
}

void button3Pressed(void)
{
  if (getMode() != mode)
  {
    Serial.println("MODE CHANGED!");
    changed();
  }
  else
  {
    if (mode == ONE_SW) //Does not exist in this scenario
    {
      Serial.println("ONE SW");
    }
    else if (mode == TWO_SW)
    {
      Serial.println("TWO SW");
    }
    else if (mode == FOUR_SW)
    {
      Serial.println("FOUR SW");
      digitalWrite(PORT_3_OUT, HIGH);
      delay(PRESS_DELAY);
      digitalWrite(PORT_3_OUT, LOW);
    }
    else
    {
      Serial.println("UNKNOWN MODE");
      unknown();
    }
  }
}

void button4Pressed(void)
{
  if (getMode() != mode)
  {
    Serial.println("MODE CHANGED!");
    changed();
  }
  else
  {
    if (mode == ONE_SW) //Does not exist in this scenario
    {
      Serial.println("ONE SW");
    }
    else if (mode == TWO_SW)
    {
      Serial.println("TWO SW");
    }
    else if (mode == FOUR_SW)
    {
      Serial.println("FOUR SW");
      digitalWrite(PORT_4_OUT, HIGH);
      delay(PRESS_DELAY);
      digitalWrite(PORT_4_OUT, LOW);
    }
    else
    {
      Serial.println("UNKNOWN MODE");
      unknown();
    }
  }
}

void changeSelection(void)
{
  if (selection == MAX_FUNCS )
  {
    selection = 1;
  }
  else
  {
    selection++;
  }

  //As this method can be called from an interrupt we want to set all the pins to LOW incase the person changes selection while holding a button down
  changed();
}

int getOutPin(void)
{
  switch (selection)
  {
    case 1:
      return PORT_1_OUT;
      break;

    case 2:
      return PORT_2_OUT;
      break;
    case 3:
      return PORT_3_OUT;
      break;
    case 4:
      return PORT_4_OUT;
      break;
  }
}
