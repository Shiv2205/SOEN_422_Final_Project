#include "HTTP.h"

/**Variables */
HTTPClient      HTTP::request;
JSON            HTTP::json_helper;


/**Implementations */

void HTTP::Authenticate_Prof(JSON* payload_ptr, std::string RFID_Sig)
{
  std::string auth_route = SERVER_BASE + std::string("/auth");

  cJSON* request_body = cJSON_CreateObject();

  cJSON_AddStringToObject(request_body, "RFID_Sig", RFID_Sig.c_str());

  char* req_body_str = cJSON_PrintUnformatted(request_body);

  HTTP::request.begin(auth_route.c_str());
  HTTP::request.addHeader("Content-Type", "application/json");

  int response_code = HTTP::request.POST(req_body_str);

  if(response_code > 0)
  {
    String      response  = HTTP::request.getString();
    const char* payload   = response.c_str();

    if(HTTP::UNAUTHORIZED_ == response_code)
    {
      payload_ptr->Set_Json(nullptr);
    }
    else
    {
      payload_ptr->Set_Json(cJSON_Parse(payload));
    }
  }

  HTTP::request.end();
  cJSON_Delete(request_body);
}



void Test_Request(void)
{
  std::string test_route = SERVER_BASE + std::string("/test");

  HTTP::request.begin(test_route.c_str());

  //Send HTTP GET request
  int http_response_code = HTTP::request.GET();
  
  if (http_response_code > 0) 
  {
    Serial.print  ("HTTP Response code: "); 
    Serial.println(http_response_code);

    String payload_string = HTTP::request.getString();
    const char* payload   = payload_string.c_str();

    HTTP::json_helper.Parse(payload);
    
    if(SUCCESS == HTTP::json_helper.JSON_Validation())
    {
      cJSON* message = HTTP::json_helper.Get_Item("message");

      if(cJSON_String == message->type)//(cJSON_IsString(message)) && (NULL != message->valuestring)
      {
        Serial.printf("Message: %s\n", message->valuestring);
      }
    }
  }
  else
  {
    Serial.print  ("Error code: "  );
    Serial.println(http_response_code);
  }
  // Free resources
  HTTP::request.end();
}