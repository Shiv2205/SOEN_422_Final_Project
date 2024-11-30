#include <SPI.h>
#include <MFRC522.h>

/**Defines */
//RFID Sensor
#define RST_PIN   22   // Reset pin
#define SS_PIN    5    // Slave select pin

//Status LEDs
#define RED_PIN       12
#define YELLOW_PIN    13
#define GREEN_PIN     14

/**Declarations */
//RFID Sensor
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() 
{
    Serial.begin(115200);
    SPI.begin(18, 19, 23); // SCK, MISO, MOSI
    rfid.PCD_Init();

    Serial.println(F("RFID Reader Initialized"));
}

void loop() 
{
    // Look for a card
    if (( ! rfid.PICC_IsNewCardPresent()) || ( ! rfid.PICC_ReadCardSerial())) 
    {
        delay(50);
        return;
    }

    // Print card UID
    Serial.print(F("Card UID: "));
    for (byte i = 0; i < rfid.uid.size; i++) 
    {
        Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(rfid.uid.uidByte[i], HEX);
    }
    Serial.println();

    // Halt the card
    rfid.PICC_HaltA();
}
