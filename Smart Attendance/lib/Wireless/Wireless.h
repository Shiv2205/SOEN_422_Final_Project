#ifndef WIRELESS_H
#define WIRELESS_H

#include <WiFi.h>
#include <HTTPClient.h>


/**Defines */
#define TIMEOUT_MS                        20000
#define WIFI_CONNECTED                    (WiFi.status() == WL_CONNECTED) //Macro to verify wifi connection status
#define IS_TIMED_OUT(connection_start)    (((millis() - (connection_start)) > TIMEOUT_MS)) //Macro to verify if WiFi connection attempt has timed out
#define SERVER_BASE                       "https://backend-481246656974.northamerica-northeast1.run.app"


/**Types */
struct Credentials_t                                                           //Struct type to store WiFi Credentials
{
  std::string ssid;
  std::string password; 
};

 
namespace Wireless                                                             //Namespace to logically group WiFi functions
{

  extern HTTPClient     http;
  extern Credentials_t  wifi_cred;

  //static int Strip_JSON_quotes(String json_int);

  void WiFi_setup(Credentials_t*  wifi_credentials);                           //Setup WiFi and start connection
  

};


#endif // WIRELESS_H