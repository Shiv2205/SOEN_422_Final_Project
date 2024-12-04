#include "BLE.h"
#include <Arduino.h>

/**Declarations */
BLEServer*           BLE::server_ptr;
BLEService*          BLE::service_ptr;
BLECharacteristic*   BLE::characteristic_ptr;
BLEAdvertising*      BLE::advertising_ptr;

bool                 BLE::is_ssid_config        = false;
bool                 BLE::is_pass_config        = false;

bool                 BLE::is_device_configured  = false;
bool                 BLE::is_auth               = false;
bool                 BLE::is_course_select      = false;

Course_List          BLE::course_list;
std::string          BLE::course_menu;
std::string          BLE::selected_course;


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
    std::string value       = characteristic_ptr->getValue();
    std::string conn_status = "Bluetooth: Connected\nWiFi: ";
    std::string response;

    if (value.length() > 0)                                                    //Ensure data is avaliable before processing
    {
      switch (Get_Command(value))
      {
        case AUTH:                                                             //**Authentication Command**
          if( ! WIFI_CONNECTED)
          {
            response = "Wifi connection required. Use command \"wifi\"to configure connection.";
          }
          else
          {
            response = "Please tap your card on the reader for authentication";
            BLE::is_auth = true;
          }
        break;

        case WIFI:                                                             //**WIFI Command**
          if((BLE::is_ssid_config) && ( ! BLE::is_pass_config))
          {
            response = "Please insert wifi SSID: ";                            //Set SSID
            BLE::is_ssid_config = false;
            BLE::is_pass_config = true;
          }
          else if(( ! BLE::is_ssid_config) && (BLE::is_pass_config))
          {
            response = "Please insert wifi password: ";                        //Set password
            BLE::is_ssid_config = true;
          }
          else if((BLE::is_ssid_config) && (BLE::is_pass_config))
          {
            BLE::is_ssid_config = false;
            BLE::is_pass_config = false;

            Wireless::Init(&Wireless::wifi_cred);                              //WiFi Setup

            characteristic_ptr->setValue("Attempting connection to " 
                                          + Wireless::wifi_cred.ssid);
            characteristic_ptr->notify();

            if(WIFI_CONNECTED)
            {
              response = "Succesfully connected to " +                         //Successful connection
                          Wireless::wifi_cred.ssid;
            }
            else
            {
              response = "Error connecting to " +                              //unsuccessful connection
                          Wireless::wifi_cred.ssid;
            }
          }
        break;

        case COURSE_SELECT:                                                    //**Course select Command**
          BLE::selected_course = "";
          for(int index = 0; index < BLE::course_list.size(); index++)
          {
            if(std::to_string(index + 1) == value)
            {
              BLE::selected_course = BLE::course_list.at(index);
            }
          }

          if(BLE::selected_course.empty())
          {
            response = "Choice should be a number representing the course.\n";
            response += BLE::course_menu;
          }
          else
          {
            response = "Taking attendance for " + BLE::selected_course + "\n";
            response += "Device will be disconnected from bluetooth soon.";

            BLE::is_course_select     = false;
            BLE::is_device_configured = true;
          }

        break;

        case CONN_STATUS:                                                      //**Status Command**
          conn_status += WIFI_CONNECTED ? "Connected" : "Disconnected"; 

          response = conn_status;
        break;

        default:                                                               //**Default Case Handling**
          response = "Unrecognized! Try \"auth\" or \"status\"";
        break;
      }
      characteristic_ptr->setValue(response);
      characteristic_ptr->notify();
    }
  }

};

Command Get_Command(std::string user_command)
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
  else if(BLE::is_course_select)
  {
    return Command::COURSE_SELECT;
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

Course_List& BLE::Get_Course_List(void)
{
  return BLE::course_list;
}

void BLE::DeInit(void)
{
  if (BLE::advertising_ptr) 
  {
    BLE::advertising_ptr->stop();                                              //Stop advertising
  }

  if (BLE::server_ptr) 
  {
    BLE::server_ptr->disconnect(0);                                            //Disconnect all active clients
  }

  BLEDevice::deinit();                                                         //Deinitialize BLE

  esp_bt_controller_disable();
  esp_bt_controller_mem_release(ESP_BT_MODE_BLE);                              //Release BLE memory
}