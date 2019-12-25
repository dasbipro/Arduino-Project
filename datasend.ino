#include "esp_http_client.h"
#include <WiFi.h>
#include "Arduino.h"
#include <ArduinoJson.h>

const char* ssid = "ojgk2532";
const char* password = "jawad1245";

const char *post_url = "https://avijit666666.000webhostapp.com/HAW/api.php"; // Location to send POSTed data
const char *post_data = "temperature=13.9&humidity=25.0&raindrop=26.0&co=25.0&co2=30.0&lpg=25.6";  // Post data can be name value, or just value
//const char *post_data = "apiKey=1999&homeStatus=1";

bool internet_connected = false;

char json[]="[{\"id\":\"1\",\"light\":\"1\",\"fan\":\"1\",\"door\":\"1\"}]";




void setup()
{
  Serial.begin(115200);

  if (init_wifi()) { // Connected to WiFi
    internet_connected = true;
    Serial.println("Internet connected");
  }
}

bool init_wifi()
{
  int connAttempts = 0;
  Serial.println("\r\nConnecting to: " + String(ssid));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
    if (connAttempts > 10) return false;
    connAttempts++;
  }
  return true;
}


esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
  switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
      Serial.println("HTTP_EVENT_ERROR");
      break;
    case HTTP_EVENT_ON_CONNECTED:
      Serial.println("HTTP_EVENT_ON_CONNECTED");
      break;
    case HTTP_EVENT_HEADER_SENT:
      Serial.println("HTTP_EVENT_HEADER_SENT");
      break;
    case HTTP_EVENT_ON_HEADER:
      Serial.println();
      Serial.printf("HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
      break;
    case HTTP_EVENT_ON_DATA:
      Serial.println();
      Serial.printf("HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      if (!esp_http_client_is_chunked_response(evt->client)) {
        // Write out data
         Serial.printf("%.*s", evt->data_len, (char*)evt->data);
      }
      Serial.printf("%.*s", evt->data_len, (char*)evt->data);
      //json=evt->data;
      
      break;
    case HTTP_EVENT_ON_FINISH:
      Serial.println("");
      Serial.println("HTTP_EVENT_ON_FINISH");
      break;
    case HTTP_EVENT_DISCONNECTED:
      Serial.println("HTTP_EVENT_DISCONNECTED");
      break;
  }
  return ESP_OK;
}

static esp_err_t post_something()
{
  esp_err_t res = ESP_OK;

  esp_http_client_handle_t http_client;
  
  esp_http_client_config_t config_client = {0};
  config_client.url = post_url;
  config_client.event_handler = _http_event_handler;
  config_client.method = HTTP_METHOD_POST;

  http_client = esp_http_client_init(&config_client);
  
  esp_http_client_set_post_field(http_client , post_data, strlen(post_data));

  // esp_http_client_set_header(http_client, "Content-Type", "image/jpg"); // sending a jpg file

  esp_err_t err = esp_http_client_perform(http_client);
  if (err == ESP_OK) {
    Serial.print("esp_http_client_get_status_code: ");
    Serial.println(esp_http_client_get_status_code(http_client));
  }

  esp_http_client_cleanup(http_client);
}

void loop()
{
  if (1) { 
    post_something();
  }
  delay(10000);
}
