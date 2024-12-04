#ifndef RFID_H
#define RFID_H

#include <cstdio>
#include <SPI.h>
#include <MFRC522.h>


/**Defines */
//RFID Sensor Pins
#define RST_PIN       22   
#define SS_PIN        5  
#define SCK_PIN       18
#define MISO_PIN      19
#define MOSI_PIN      23

//Status LEDs
#define RED_PIN       12
#define YELLOW_PIN    13
#define GREEN_PIN     14

//Util
#define NOT_FOUND     "Not Found"

/**Namespace to logically group RFID functions */
namespace RFID
{
  /**Declarations */
  //RFID Sensor
  extern MFRC522 rfid;


  /**Functions */
  void Init(void);

  std::string Watch_For_Cards(void);

  void Print_Card_UID(void);

  void Halt_Card(void);
};


std::string Byte_To_Hex(uint8_t input_byte);

#endif