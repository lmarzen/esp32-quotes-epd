/* Client side utilities for esp32-quote-epd.
 * Copyright (C) 2022-2023  Luke Marzen
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// built-in C++ libraries
#include <cstring>
#include <vector>

// arduino/esp32 libraries
#include <Arduino.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <time.h>
#include <WiFi.h>

// header files
#include "_locale.h"
#include "client_utils.h"
#include "config.h"
#include "display_utils.h"
#include "renderer.h"

/* Power-on and connect WiFi.
 * Takes int parameter to store WiFi RSSI, or “Received Signal Strength
 * Indicator"
 *
 * Returns WiFi status.
 */
wl_status_t startWiFi(int &wifiRSSI)
{
  WiFi.mode(WIFI_STA);
  Serial.printf("Connecting to '%s'", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  // timeout if WiFi does not connect in 10s from now
  unsigned long timeout = millis() + 10000;
  wl_status_t connection_status = WiFi.status();

  while ((connection_status != WL_CONNECTED) && (millis() < timeout))
  {
    Serial.print(".");
    delay(50);
    connection_status = WiFi.status();
  }
  Serial.println();

  if (connection_status == WL_CONNECTED)
  {
    wifiRSSI = WiFi.RSSI(); // get WiFi signal strength now, because the WiFi
                            // will be turned off to save power!
    Serial.println("IP: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.printf("Could not connect to '%s'\n", WIFI_SSID);
  }
  return connection_status;
} // startWiFi

/* Disconnect and power-off WiFi.
 */
void killWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

/* Prints the local time to serial monitor.
 *
 * Returns true if getting local time was a success, otherwise false.
 */
bool printLocalTime(tm *timeInfo)
{
  int attempts = 0;
  while (!getLocalTime(timeInfo) && attempts++ < 3)
  {
    Serial.println("Failed to obtain time");
    return false;
  }
  Serial.println(timeInfo, "%A, %B %d, %Y %H:%M:%S");
  return true;
} // killWiFi

/* Connects to NTP server and stores time in a tm struct, adjusted for the time
 * zone specified in config.cpp.
 *
 * Returns true if success, otherwise false.
 *
 * Note: Must be connected to WiFi to get time from NTP server.
 */
bool setupTime(tm *timeInfo)
{
  // passing 0 for gmtOffset_sec and daylightOffset_sec and instead use setenv()
  // for timezone offsets
  configTime(0, 0, NTP_SERVER_1, NTP_SERVER_2);
  setenv("TZ", TIMEZONE, 1);
  tzset();
  return printLocalTime(timeInfo);
} // setupTime

/* Perform an HTTP GET request to a Google script.
 * If data is received, it will be stored in the string parameter.
 *
 * Returns the HTTP Status Code.
 */
int getGoogleSheetsQuote(String &r)
{
  int attempts = 0;
  bool rxSuccess = false;

  String url = "https://" + GS_ENDPOINT + "/macros/s/" + GS_DEPLOYMENT_ID 
               + "/exec?read=" + QUOTE_SELECTION;
  // This string is printed to terminal to help with debugging. The
  // GS_DEPLOYMENT_ID is censored to reduce the risk of users exposing their
  // key.
  String sanitizedUri = "https://" + GS_ENDPOINT
                        + "/macros/s/<GS_DEPLOYMENT_ID>/exec?read="
                        + QUOTE_SELECTION;

  Serial.println("Attempting HTTP Request: " + sanitizedUri);
  int httpResponse = 0;
  while (!rxSuccess && attempts < 3)
  {
    HTTPClient http;
    http.begin(url);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    httpResponse = http.GET();
    if (httpResponse == HTTP_CODE_OK)
    {
      r = http.getString();
      rxSuccess = true;
    }
    http.end();
    Serial.println("  " + String(httpResponse, DEC) + " "
                   + getHttpResponsePhrase(httpResponse));
    ++attempts;
  }

  return httpResponse;
} // getGoogleSheetsQuote

