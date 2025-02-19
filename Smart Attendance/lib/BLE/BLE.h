#ifndef BLE_H
#define BLE_H

#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>
#include <vector>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <Wireless.h>


/* Macros */
#define BT_DEVICE_NAME        "Smart_Attendance_SOEN_422"
#define SERVICE_UUID          "3c6e2d67-5c1f-49b2-95f0-011d4de28d92"
#define CHARACTERISTIC_UUID   "bea41f13-dbb0-4b13-956c-f983c13e61b1"


/**Types */
using Course_List = std::vector<std::string>;

enum Command
{
  AUTH,
  WIFI,
  CONN_STATUS,
  COURSE_SELECT,
  UNKNOWN,
};


//Namespace to logically group BLE functions 
namespace BLE
{
  /**Declarations */
  //BLE
  extern BLEServer*           server_ptr;
  extern BLEService*          service_ptr;
  extern BLECharacteristic*   characteristic_ptr;
  extern BLEAdvertising*      advertising_ptr;

  //Wifi
  extern bool                 is_ssid_config;
  extern bool                 is_pass_config;

  //Config
  extern bool                 is_device_configured;
  extern bool                 is_auth;
  extern bool                 is_course_select;

  //Courses
  extern Course_List          course_list;
  extern std::string          course_menu;
  extern std::string          selected_course;


  /*************Functions*******************/
  //Setup Bluetooth BLE and start advertising
  void Init(void);
  //Setup advertising properties
  void Advertising_Setup(BLEAdvertising* advertising_ptr);

  //Getter for Characteristics pointer
  BLECharacteristic* Get_Characteristic(void);

  Course_List&       Get_Course_List(void);

  void DeInit(void);

};

Command Get_Command(std::string user_command);

// const char* Hexify_String(std::string& input_str);
// const char* String_To_Decimal(std::string& input_str);

#endif