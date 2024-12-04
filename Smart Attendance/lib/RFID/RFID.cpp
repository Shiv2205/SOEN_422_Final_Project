#include "RFID.h"

/**Declarations */
MFRC522 RFID::rfid(SS_PIN, RST_PIN);


/**Function Implementations */
void RFID::Init(void)
{
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);
  rfid.PCD_Init();

  Serial.println(F("RFID Reader Initialized"));
}

std::string RFID::Watch_For_Cards(void)
{
  std::string card_id = "";
  
  if (( ! rfid.PICC_IsNewCardPresent()) || ( ! rfid.PICC_ReadCardSerial()))    // Look for a card
  {
    delay(50);
    return NOT_FOUND;
  }

  for (byte i = 0; i < rfid.uid.size; i++) 
  {
    card_id += Byte_To_Hex(rfid.uid.uidByte[i]) 
            + ((i + 1) < rfid.uid.size ? " " : "");
  }

  rfid.PICC_HaltA();                                                           // Halt the card

  return card_id;
}

void RFID::Print_Card_UID(void)
{
  Serial.print(F("Card UID: "));
  for (byte i = 0; i < rfid.uid.size; i++)                                     // Print card UID
  {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
}

std::string Byte_To_Hex(uint8_t input_byte)
{
  char buffer[3];                                                              //Two HEX characters + null terminator
  std::sprintf(buffer, "%02X", input_byte);

  return std::string(buffer);
}

void RFID::Halt_Card(void)
{
  // Halt the card
  rfid.PICC_HaltA();
}