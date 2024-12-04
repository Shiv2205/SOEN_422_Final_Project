#include "JSON.h"


JSON::JSON(void)
{
  this->json = NULL;
}

JSON::JSON(cJSON* cjson_ptr)
{
  this->json = cjson_ptr;
}

void  JSON::Set_Json(cJSON* json_ptr)
{
  this->json = json_ptr;
}

cJSON* JSON::Get_Json(void)
{
  return this->json;
}

cJSON* JSON::Parse(const char* payload)
{
  this->json = cJSON_Parse(payload);

  return this->json;
}

cJSON* JSON::Get_Item(const char* item)
{
  return cJSON_GetObjectItemCaseSensitive(this->json, item);
}

Response_Code JSON::JSON_Validation(void)
{
  if(NULL == this->json)
  {
    const char* error_msg = cJSON_GetErrorPtr(); 
    if (NULL != error_msg) 
    { 
      Serial.printf("Error: %s\n", error_msg); 
    } 
    cJSON_Delete(json); 

    return Response_Code::FAILURE; 
  }
  else
  {
    return Response_Code::SUCCESS;
  }
}

JSON::~JSON(void)
{
  if(NULL != this->json)
  {
    cJSON_Delete(this->json);
  }
}