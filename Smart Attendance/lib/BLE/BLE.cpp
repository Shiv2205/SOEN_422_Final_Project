#include "BLE.h"
#include <Arduino.h>

/**Declarations */
BLEServer*           BLE::server_ptr;
BLEService*          BLE::service_ptr;
BLECharacteristic*   BLE::characteristic_ptr;
BLEAdvertising*      BLE::advertising_ptr;


/**Function Implementations */

class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer* pServer) override
  {
    Serial.println("Client connected!");
  }

  void onDisconnect(BLEServer* pServer) override
  {
    Serial.println("Client disconnected!");
    pServer->startAdvertising();
  }
};

//Custom callback class for when the characteristic value is changed
class Command_Callbacks : public BLECharacteristicCallbacks 
{
  void onWrite(BLECharacteristic* characteristic_ptr) override
  {
    std::string value = characteristic_ptr->getValue();

    if (value.length() > 0)                                                    //Ensure data is avaliable before processing
    {
      switch (Get_Command(value))
      {
        case AUTH:
          characteristic_ptr->setValue("Please tap your card on the reader for authentication");
        break;

        case CONN_STATUS:
          characteristic_ptr->setValue("Device connected! Awaiting commands");
        break;

        default:
          characteristic_ptr->setValue("Unrecognized! Try \"auth\" or \"status\"");
        break;
      }
      characteristic_ptr->notify();
    }
  }

};

void BLE::Init(void)
{
  BLEDevice::init(BT_DEVICE_NAME);                                             //Init BLE device and specify device name
  BLE::server_ptr         = BLEDevice::createServer();                         //Create the BLE server
  BLE::service_ptr        = BLE::server_ptr->createService(SERVICE_UUID);      //Create service

  BLE::characteristic_ptr = BLE::service_ptr->createCharacteristic
                            ( CHARACTERISTIC_UUID, 
                              BLECharacteristic::PROPERTY_WRITE | 
                              BLECharacteristic::PROPERTY_READ  |
                              BLECharacteristic::PROPERTY_NOTIFY);             //Specify Service characteristics
  

  BLE::server_ptr        ->setCallbacks (new ServerCallbacks());
  BLE::characteristic_ptr->setCallbacks (new Command_Callbacks());

  BLE::service_ptr->start();                                                   //Start BLE service

  BLE::Advertising_Setup(BLE::advertising_ptr);                                //Advertising specific setup

  BLEDevice::startAdvertising();                                               //Start advertising
}

void BLE::Advertising_Setup(BLEAdvertising* advertising_ptr)
{
  advertising_ptr = BLEDevice::getAdvertising();
  advertising_ptr->addServiceUUID(SERVICE_UUID);
  advertising_ptr->setScanResponse(true);
  advertising_ptr->setMinPreferred(0x06);                                      //functions that help with iPhone connections issue
  advertising_ptr->setMaxPreferred(0x12);
}

BLECharacteristic* BLE::Get_Characteristic(void)
{
  return BLE::characteristic_ptr;
}

Command Get_Command(std::string& user_command)
{
  auto start_char         = user_command.begin();
  auto end_char           = user_command.end();
  auto output_start_char  = start_char;

  std::transform(start_char, end_char, output_start_char, ::tolower);          //Transform user_command to lowercase

  if(("auth" == user_command) || ("authenticate" == user_command))
  {
    return Command::AUTH;
  }
  if(("status" == user_command) || ("check connection" == user_command))
  {
    return Command::CONN_STATUS;
  }

  return Command::UNKNOWN;                                                     //Default case
}

// const char* Hexify_String(std::string& input_str)
// {
//   std::ostringstream hexStream;
//   int count = 1;
//   for (unsigned char c : input_str) {
//     hexStream << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(c) << " ";
//   }
//   return hexStream.str().c_str();
// }

// const char* String_To_Decimal(std::string& input_str)
// {
//   std::ostringstream dec_stream;
//   for (unsigned char c : input_str) {
//     dec_stream << "Char: " << c << " | Decimal: " << static_cast<int>(c) << "\n";
//     if (isprint(c)) 
//     {
//       Serial.print(c);
//     } 
//     else 
//     {
//       Serial.print("[0x");
//       Serial.print((int)c, HEX); // Print non-printable as hex
//       Serial.print("]");
//     }
//   }
//   return dec_stream.str().c_str();
// }