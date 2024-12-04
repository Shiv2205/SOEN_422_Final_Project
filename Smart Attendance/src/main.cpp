#include <Arduino.h>
#include <RFID.h>
#include <BLE.h>
#include <Wireless.h>
#include <HTTP.h>
#include <vector>


/**Types */
enum System_States
{
  CONFIG,
  ATTENDANCE_TAKING
};


/**Prototypes */
void Parse_Course_List(JSON* json_payload);
void Show_Course_Menu(void);


/**Global declarations */
System_States             attendance_sys_state;
std::string               selected_course;


/**Tick Function */
void Attendance_Tck(void)
{
  std::string card_id;
  std::string characteristic_value;

  switch (attendance_sys_state)
  {
    case CONFIG:
      //Status_lights->Blink()

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
          JSON json_payload;

          HTTP::Authenticate_Prof(&json_payload, card_id);

          if(nullptr == json_payload.Get_Json())
          {
            Serial.print(F("JSON payload empty\n"));
            break;
          }

          Parse_Course_List(&json_payload);

          Show_Course_Menu();
        }
      }
    break;

    case ATTENDANCE_TAKING:
      selected_course = BLE::selected_course;
      //Serial.print(F("Not Found"));
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

    RFID::Init();
    BLE ::Init();
}

void loop() 
{
  Attendance_Tck();
}


void Parse_Course_List(JSON* json_payload)
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

void Show_Course_Menu(void)
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