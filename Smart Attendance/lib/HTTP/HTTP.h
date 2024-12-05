#ifndef HTTP_H
#define HTTP_H

#include <HTTPClient.h>
#include <JSON.h>


/**Defines */
#define SERVER_BASE                       "https://backend-481246656974.northamerica-northeast1.run.app"



namespace HTTP
{
  /**Types */
  enum Code
  {
    OK_           = 200,
    CREATED_      = 201,
    ACCEPTED_     = 202,
    BAD_REQUEST_  = 400,
    UNAUTHORIZED_ = 401,
    FORBIDDEN_    = 403,
    NOT_FOUND_    = 404,
    SERVER_ERR_   = 500
  };

  extern HTTPClient request;
  extern JSON       json_helper;

  void Authenticate_Prof(JSON* payload_ptr, std::string RFID_Sig);

  void Log_Attendance(JSON* payload_ptr, std::string course_code, std::string RFID_Sig);
}


void Test_Request(void);

#endif