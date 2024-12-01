#include <Arduino.h>
#include <RFID.h>
#include <BLE.h>


void setup() 
{
    Serial.begin(115200);

    RFID::Init();
    BLE ::Init();
}

void loop() 
{
    std::string card_id;

    // Look for a card
    card_id = RFID::Watch_For_Cards();

    if(NOT_FOUND != card_id)
    {
      Serial.println(card_id.c_str());
    }
    else
    {
      //BLE::Get_Characteristic()->setValue("Card ID: " + card_id);
      //BLE::Get_Characteristic()->notify();
    }
}
