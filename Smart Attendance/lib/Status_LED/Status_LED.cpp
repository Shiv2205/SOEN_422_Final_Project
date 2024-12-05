#include "Status_LED.h"


void Status_LED::Init(void)
{
  pinMode(RED_PIN   , OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN , OUTPUT);
}

void Status_LED::All_Off(void)
{
  digitalWrite(RED_PIN   , LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN , LOW);
}

void Status_LED::All_On(void)
{
  digitalWrite(RED_PIN   , HIGH);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(GREEN_PIN , HIGH);
}

void Status_LED::Config(void)
{
  Status_LED::All_On();
  delay(500);

  Status_LED::All_Off();
  delay(500);
}

void Status_LED::Idle(void)
{
  Status_LED::All_Off();
  digitalWrite(GREEN_PIN, HIGH);
}

void Status_LED::Fetching_Data(void)
{
  Status_LED::All_Off();
  for(int count = 0; count < 2; count ++)
  {
    digitalWrite(YELLOW_PIN, HIGH);
    delay(400);
    digitalWrite(YELLOW_PIN, LOW);
    delay(400);
  }
}

void Status_LED::Success(void)
{
  Status_LED::Idle();
  delay(1000);
}

void Status_LED::Warn(void)
{
  Status_LED::All_Off();
  digitalWrite(YELLOW_PIN, HIGH);
  delay(1000);
}

void Status_LED::Failure(void)
{
  Status_LED::All_Off();
  digitalWrite(RED_PIN, HIGH);
  delay(1000);
}