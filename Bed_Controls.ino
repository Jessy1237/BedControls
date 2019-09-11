#define PORT_1_PIN 6
#define PORT_2_PIN 7
#define PORT_3_PIN 8
#define PORT_4_PIN 9
#define PORT_1_SEN 2
#define PORT_2_SEN 3
#define PORT_3_SEN 4
#define PORT_4_SEN 5

#define LED_PIN 10

#define ONE_SW 1
#define TWO_SW 2
#define FOUR_SW 3

int mode = 0;

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

  mode = getMode();

  Serial.begin(9600);
}

void loop()
{
  if (getMode() != mode)
  {
    Serial.println("MODE CHANGED!");
  }
  else
  {
    if (mode == ONE_SW)
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
    }
    else
    {
      Serial.println("UNKNOWN MODE");
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
