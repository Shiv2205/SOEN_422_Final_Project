#ifndef JSON_H
#define JSON_H

#include <Arduino.h>
#include <cJSON.h>


/**Types */
enum Response_Code
{
  SUCCESS = 0,
  FAILURE = 1
};


class JSON
{
private:
  cJSON* json;

public:

  JSON(void);

  JSON(cJSON* cjson_ptr);

  void   Set_Json(cJSON* json_ptr);
  
  cJSON* Get_Json(void);

  cJSON* Parse(const char* payload);

  cJSON* Get_Item(const char* item);

  Response_Code JSON_Validation(void);

  ~JSON(void);
};

#endif