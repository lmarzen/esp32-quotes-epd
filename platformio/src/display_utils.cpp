/* Display helper utilities for esp32-quote-epd.
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

#include <cmath>
#include <vector>
#include <Arduino.h>
#include <HTTPClient.h>

#include "_locale.h"
#include "_strftime.h"
#include "config.h"
#include "display_utils.h"

// icon header files
#include "icons/icons_16x16.h"
#include "icons/icons_24x24.h"

/* Returns battery percentage, rounded to the nearest integer.
 * Takes a voltage and uses a pre-calculated polynomial to find an approximation
 * of the battery life percentage remaining.
 */
int calcBatPercent(double v)
{
  // this formula was calculated using samples collected from a lipo battery
  double y = -  144.9390 * v * v * v
             + 1655.8629 * v * v
             - 6158.8520 * v
             + 7501.3202;

  // enforce bounds, 0-100
  y = max(y, 0.0);
  y = min(y, 100.0);

  y = round(y);
  return static_cast<int>(y);
} // end calcBatPercent

/* Returns 24x24 bitmap incidcating battery status.
 */
const uint8_t *getBatBitmap24(int batPercent)
{
  if (batPercent >= 93)
  {
    return battery_full_90deg_24x24;
  }
  else if (batPercent >= 79)
  {
    return battery_6_bar_90deg_24x24;
  }
  else if (batPercent >= 65)
  {
    return battery_5_bar_90deg_24x24;
  }
  else if (batPercent >= 50)
  {
    return battery_4_bar_90deg_24x24;
  }
  else if (batPercent >= 36)
  {
    return battery_3_bar_90deg_24x24;
  }
  else if (batPercent >= 22)
  {
    return battery_2_bar_90deg_24x24;
  }
  else if (batPercent >= 8)
  {
    return battery_1_bar_90deg_24x24;
  }
  else
  {  // batPercent < 8
    return battery_0_bar_90deg_24x24;
  }
} // end getBatBitmap24

/* Gets string with the current date.
 */
void getDateStr(String &s, tm *timeInfo)
{
  char buf[48] = {};
  _strftime(buf, sizeof(buf), DATE_FORMAT, timeInfo);
  s = buf;

  // remove double spaces. %e will add an extra space, ie. " 1" instead of "1"
  s.replace("  ", " ");
  return;
} // end getDateStr

/* Gets string with the current date and time of the current refresh attempt.
 */
void getRefreshTimeStr(String &s, bool timeSuccess, tm *timeInfo)
{
  if (timeSuccess == false)
  {
    s = TXT_UNKNOWN;
    return;
  }

  char buf[48] = {};
  _strftime(buf, sizeof(buf), REFRESH_TIME_FORMAT, timeInfo);
  s = buf;

  // remove double spaces.
  s.replace("  ", " ");
  return;
} // end getRefreshTimeStr

/* Returns the wifi signal strength descriptor text for the given RSSI.
 */
const char *getWiFidesc(int rssi)
{
  if (rssi == 0)
  {
    return TXT_WIFI_NO_CONNECTION;
  }
  else if (rssi >= -50)
  {
    return TXT_WIFI_EXCELLENT;
  }
  else if (rssi >= -60)
  {
    return TXT_WIFI_GOOD;
  }
  else if (rssi >= -70)
  {
    return TXT_WIFI_FAIR;
  }
  else
  {  // rssi < -70
    return TXT_WIFI_WEAK;
  }
} // end getWiFidesc

/* Returns 16x16 bitmap incidcating wifi status.
 */
const uint8_t *getWiFiBitmap16(int rssi)
{
  if (rssi == 0)
  {
    return wifi_x_16x16;
  }
  else if (rssi >= -50)
  {
    return wifi_16x16;
  }
  else if (rssi >= -60)
  {
    return wifi_3_bar_16x16;
  }
  else if (rssi >= -70)
  {
    return wifi_2_bar_16x16;
  }
  else
  {  // rssi < -70
    return wifi_1_bar_16x16;
  }
} // end getWiFiBitmap24

/* This function returns a pointer to a string representing the meaning for a
 * HTTP response status code or an arduino client error code.
 * ArduinoJson DeserializationError codes are also included here and are given a
 * negative 100 offset to distinguish them from other client error codes.
 *
 * HTTP response status codes (100 to 511)
 * https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
 *
 * HTTP client errors (-1 to -11)
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/HTTPClient/src/HTTPClient.h
 *
 * ArduinoJson DeserializationError codes (-100 to -105)
 * https://arduinojson.org/v6/api/misc/deserializationerror/
 */
const char *getHttpResponsePhrase(int code)
{
  switch (code)
  {
  // HTTP client errors
  case HTTPC_ERROR_CONNECTION_REFUSED:  return "connection refused";
  case HTTPC_ERROR_SEND_HEADER_FAILED:  return "send header failed";
  case HTTPC_ERROR_SEND_PAYLOAD_FAILED: return "send payload failed";
  case HTTPC_ERROR_NOT_CONNECTED:       return "not connected";
  case HTTPC_ERROR_CONNECTION_LOST:     return "connection lost";
  case HTTPC_ERROR_NO_STREAM:           return "no stream";
  case HTTPC_ERROR_NO_HTTP_SERVER:      return "no HTTP server";
  case HTTPC_ERROR_TOO_LESS_RAM:        return "too less ram";
  case HTTPC_ERROR_ENCODING:            return "Transfer-Encoding not supported";
  case HTTPC_ERROR_STREAM_WRITE:        return "Stream write error";
  case HTTPC_ERROR_READ_TIMEOUT:        return "read Timeout";

  // 1xx - Informational Responses
  case 100: return "Continue";
  case 101: return "Switching Protocols";
  case 102: return "Processing";
  case 103: return "Early Hints";

  // 2xx - Successful Responses
  case 200: return "OK";
  case 201: return "Created";
  case 202: return "Accepted";
  case 203: return "Non-Authoritative Information";
  case 204: return "No Content";
  case 205: return "Reset Content";
  case 206: return "Partial Content";
  case 207: return "Multi-Status";
  case 208: return "Already Reported";
  case 226: return "IM Used";

  // 3xx - Redirection Responses
  case 300: return "Multiple Choices";
  case 301: return "Moved Permanently";
  case 302: return "Found";
  case 303: return "See Other";
  case 304: return "Not Modified";
  case 305: return "Use Proxy";
  case 307: return "Temporary Redirect";
  case 308: return "Permanent Redirect";

  // 4xx - Client Error Responses
  case 400: return "Bad Request";
  case 401: return "Unauthorized";
  case 402: return "Payment Required";
  case 403: return "Forbidden";
  case 404: return "Not Found";
  case 405: return "Method Not Allowed";
  case 406: return "Not Acceptable";
  case 407: return "Proxy Authentication Required";
  case 408: return "Request Timeout";
  case 409: return "Conflict";
  case 410: return "Gone";
  case 411: return "Length Required";
  case 412: return "Precondition Failed";
  case 413: return "Content Too Large";
  case 414: return "URI Too Long";
  case 415: return "Unsupported Media Type";
  case 416: return "Range Not Satisfiable";
  case 417: return "Expectation Failed";
  case 418: return "I'm a teapot";
  case 421: return "Misdirected Request";
  case 422: return "Unprocessable Content";
  case 423: return "Locked";
  case 424: return "Failed Dependency";
  case 425: return "Too Early";
  case 426: return "Upgrade Required";
  case 428: return "Precondition Required";
  case 429: return "Too Many Requests";
  case 431: return "Request Header Fields Too Large";
  case 451: return "Unavailable For Legal Reasons";

  // 5xx - Server Error Responses
  case 500: return "Internal Server Error";
  case 501: return "Not Implemented";
  case 502: return "Bad Gateway";
  case 503: return "Service Unavailable";
  case 504: return "Gateway Timeout";
  case 505: return "HTTP Version Not Supported";
  case 506: return "Variant Also Negotiates";
  case 507: return "Insufficient Storage";
  case 508: return "Loop Detected";
  case 510: return "Not Extended";
  case 511: return "Network Authentication Required";

  default:  return "";
  }
} // end getHttpResponsePhrase


/* This function returns a pointer to a string representing the meaning for a
 * WiFi status (wl_status_t).
 *
 * wl_status_t type definition
 * https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFiType.h
 */
const char *getWifiStatusPhrase(wl_status_t status)
{
  switch (status)
  {
  case WL_NO_SHIELD:       return "No Shield";
  case WL_IDLE_STATUS:     return "Idle";
  case WL_NO_SSID_AVAIL:   return "No SSID Available";
  case WL_SCAN_COMPLETED:  return "Scan Complete";
  case WL_CONNECTED:       return "Connected";
  case WL_CONNECT_FAILED:  return "Connection Failed";
  case WL_CONNECTION_LOST: return "Connection Lost";
  case WL_DISCONNECTED:    return "Disconnected";

  default:  return "";
  }
} // end getWifiStatusPhrase
