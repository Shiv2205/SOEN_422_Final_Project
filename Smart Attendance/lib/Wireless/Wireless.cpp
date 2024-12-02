#include "Wireless.h"

HTTPClient      Wireless::http;
Credentials_t   Wireless::wifi_cred;


// int Wireless::Strip_JSON_quotes(String json_int)
// {
//   json_int.replace("\"", "");
//   return (int)json_int.toInt();
// }

void Wireless::WiFi_setup(Credentials_t*  wifi_credentials)
{
  WiFi.mode (WIFI_MODE_STA);
  WiFi.begin(wifi_credentials->ssid.c_str(), 
             wifi_credentials->password.c_str());

  unsigned long connection_start = millis();
  while((! WIFI_CONNECTED) && (! IS_TIMED_OUT(connection_start)))
  {
    Serial.printf("Attempting connection to: %s\n", wifi_credentials->ssid.c_str());
    delay(500);
  }

  if(WIFI_CONNECTED)
  {
    Serial.print(F("WiFi Connected!\n"));
    Serial.printf("Device IP: %s\n", WiFi.localIP().toString());
  }
}