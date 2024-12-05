#include <Arduino.h>
#include <RFID.h>
#include <BLE.h>
#include <Wireless.h>
#include <HTTP.h>
#include <Status_LED.h>
#include <vector>


/**Types */
enum System_States
{
  CONFIG,
  ATTENDANCE_TAKING
};


/**Prototypes */
static inline void Parse_Course_List(JSON* json_payload);
static inline void Parse_Log_Response(JSON* json_payload);
static inline void Show_Course_Menu(void);


/**Global declarations */
System_States             attendance_sys_state;
std::string               selected_course       = "";


/**Tick Function */
void Attendance_Tck(void)
{
  std::string card_id;
  std::string characteristic_value;

  switch (attendance_sys_state)
  {
    case CONFIG:
      Status_LED::Config();

      if(BLE::is_auth)
      {
        card_id = RFID::Watch_For_Cards();

        if(NOT_FOUND == card_id)
        {
          break;
        }
        else
        {
          Serial.printf("Card ID: %s\n", card_id.c_str());
          Status_LED::Fetching_Data();

          JSON json_payload;

          HTTP::Authenticate_Prof(&json_payload, card_id);

          if(nullptr == json_payload.Get_Json())
          {
            Serial.print(F("JSON payload empty\n"));
            Status_LED::Failure();
            break;
          }

          Parse_Course_List(&json_payload);

          Show_Course_Menu();
        }
      }
    break;

    case ATTENDANCE_TAKING:
      Status_LED::Idle();
      if(selected_course.empty())
      {
        selected_course = BLE ::selected_course.substr(3);
      }

      card_id = RFID::Watch_For_Cards();

      if(NOT_FOUND == card_id)
      {
        break;
      }
      else
      {
        Serial.printf("Card ID: %s\n", card_id.c_str());
        JSON json_payload;

        HTTP::Log_Attendance(&json_payload, selected_course, card_id);
        Status_LED::Fetching_Data();

        if(nullptr == json_payload.Get_Json())
        {
          Serial.print(F("JSON payload empty\n"));
          break;
        }
        
        Parse_Log_Response(&json_payload);
      }
    break;
    
    default:
      Serial.print(F("Not Found"));
    break;
  }

  switch (attendance_sys_state)
  {
    case CONFIG:
      if(BLE::is_device_configured)
      {
        attendance_sys_state = System_States::ATTENDANCE_TAKING;
        delay(5000);
        BLE::DeInit();
      }
    break;
  }
}


void setup() 
{
    Serial.begin(115200);

    attendance_sys_state = System_States::CONFIG;                              //Startup State = CONFIG

    RFID      ::Init();
    BLE       ::Init();
    Status_LED::Init();
}

void loop() 
{
  Attendance_Tck();
}


inline void Parse_Course_List(JSON* json_payload)
{
  cJSON* course_list_json = json_payload->Get_Item("course_list");

  if(cJSON_Array == course_list_json->type)
  {
    cJSON*        course          = NULL;
    int           course_count    = 0;

    cJSON_ArrayForEach(course, course_list_json)
    {
      course_count++;

      cJSON* course_code = cJSON_GetObjectItem(course, "code");

      BLE::course_list.emplace_back
      ( std::to_string(course_count) + 
        ". "                         +
        course_code->valuestring   
      );
    }
  }
}

inline void Parse_Log_Response(JSON* json_payload)
{
  cJSON* response_json = json_payload->Get_Item("response");

  if(cJSON_Number == response_json->type)
  {
    switch (response_json->valueint)
    {
      case HTTP::CREATED_ :
        Serial.print(F("Attendance logged successfully"));
        Status_LED::Success();
      break;
      
      case HTTP::SERVER_ERR_ :
        Serial.print(F("Attendance already logged"));
        Status_LED::Warn();
      break;

      default:
        Serial.print(F("Default case Parse_Log_Response"));
      break;
    } 
  }
  else if (cJSON_String == response_json->type)
  {
    Serial.print(response_json->valuestring);
    Status_LED::Failure();
  }
}

inline void Show_Course_Menu(void)
{
  BLE::is_auth          = false;
  BLE::is_course_select = true;

  BLE::course_menu = "Select the course to record attendance for:\n";
  for(auto course : BLE::course_list)
  {
    BLE::course_menu += course + "\n";
  }

  BLE::Get_Characteristic()->setValue(BLE::course_menu);
  BLE::Get_Characteristic()->notify(); 
}