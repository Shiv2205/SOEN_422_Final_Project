#include "BLE.h"
#include <Arduino.h>

/**Declarations */
BLEServer*           BLE::server_ptr;
BLEService*          BLE::service_ptr;
BLECharacteristic*   BLE::characteristic_ptr;
BLEAdvertising*      BLE::advertising_ptr;

bool                 BLE::is_ssid_config = false;
bool                 BLE::is_pass_config = false;


/**Function Implementations */

//Custom callback class for the server
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

        case WIFI:
          if((BLE::is_ssid_config) && ( ! BLE::is_pass_config))
          {
            characteristic_ptr->setValue("Please insert wifi SSID: ");
            BLE::is_ssid_config = false;
            BLE::is_pass_config = true;
          }
          else if(( ! BLE::is_ssid_config) && (BLE::is_pass_config))
          {
            characteristic_ptr->setValue("Please insert wifi password: ");
            BLE::is_ssid_config = true;
          }
          else if((BLE::is_ssid_config) && (BLE::is_pass_config))
          {
            characteristic_ptr->setValue("Attempting connection to " + Wireless::wifi_cred.ssid);
            characteristic_ptr->notify();
            BLE::is_ssid_config = false;
            BLE::is_pass_config = false;
            Serial.printf("SSID: %s\nPassword: %s\n", 
                          Wireless::wifi_cred.ssid.c_str(),
                          Wireless::wifi_cred.password.c_str());
            Wireless::WiFi_setup(&Wireless::wifi_cred);                        //WiFi Setup

            if(WIFI_CONNECTED)
            {
              characteristic_ptr->setValue("Succesfully connected to " + Wireless::wifi_cred.ssid);
            }
            else
            {
              characteristic_ptr->setValue("Error connecting to "      + Wireless::wifi_cred.ssid);
            }
          }
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

Command Get_Command(std::string& user_command)
{
  
  if((BLE::is_pass_config) && ( ! BLE::is_ssid_config))
  {
    Wireless::wifi_cred.ssid = user_command;                                   //Set WiFi SSID

    return Command::WIFI;
  }
  else if((BLE::is_ssid_config) && (BLE::is_pass_config))
  {
    Wireless::wifi_cred.password = user_command;                               //Set WiFi password

    return Command::WIFI;                                                      
  }
  else
  {
    auto start_char         = user_command.begin();
    auto end_char           = user_command.end();
    auto output_start_char  = start_char;

    std::transform(start_char, end_char, output_start_char, ::tolower);        //Transform user_command to lowercase

    if("auth" == user_command)
    {
      return Command::AUTH;
    }
    if("wifi" == user_command)
    {
      BLE::is_ssid_config = true;
      return Command::WIFI;
    }
    if("status" == user_command)
    {
      return Command::CONN_STATUS;
    }

    return Command::UNKNOWN;                                                   //Default case
  }
}

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