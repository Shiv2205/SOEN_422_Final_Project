#ifndef STATUS_LED_H
#define STATUS_LED_H

#include <Arduino.h>

#define RED_PIN      12
#define YELLOW_PIN   13
#define GREEN_PIN    14


namespace Status_LED
{
  void Init(void);

  void All_Off(void);

  void All_On(void);

  void Config(void);

  void Idle(void);

  void Fetching_Data(void);

  void Success(void);

  void Warn(void);

  void Failure(void);
}


#endif