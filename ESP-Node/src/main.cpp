#include <Arduino.h>
#include "Gyroscope/Gyro.h"
#include "Communication/Network.h"
#include "Communication/WebSocket.h"
#include "Communication/ESP_Now.h"

unsigned long SendingTimer = 0;
unsigned long HearbeatTimer = 0;
const int16_t DATA_PER_SECOND = 100;
int16_t TIME_BETWEEN_CALLS = 1000;

ConnectionType connection = NOTHING;

#define ESP_PORT 81 // WebSocket-Port
bool WiFiSetup = false;
const uint8_t DEVICE_NUMBER = '2';

void setup()
{
  Serial.begin(115200);

  // Gyro
  Gyro_Setup();

  // Connection
  connection = WIFI;

  // Timing
  TIME_BETWEEN_CALLS = (int)(1000 / DATA_PER_SECOND);
  SendingTimer = millis();
}

void WebSocket_Loop()
{
  if (!WiFiSetup)
  {
    WiFi_Manager_Setup(ESP_PORT);
    WebSocket_Setup(ESP_PORT);
    WiFiSetup = true;
    Serial.print("Try to connect to Client");
  }

  webSocket.loop();
  if (!WebSocket_Connected)
  {
    WebSocket_ConnectToClient();
  }
  else if (millis() - SendingTimer > TIME_BETWEEN_CALLS)
  {
    Gyro_Update();
    WebSocket_SendGyroData(gyroX);
    SendingTimer = millis();
  }
}

void loop()
{
  WebSocket_Loop();
}
