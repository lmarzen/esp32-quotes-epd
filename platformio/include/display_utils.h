/* Display helper utility declarations for esp32-quote-epd.
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

#ifndef __DISPLAY_UTILS_H__
#define __DISPLAY_UTILS_H__

#include <vector>
#include <time.h>
#include <WiFi.h>

int calcBatPercent(double v);
const uint8_t *getBatBitmap24(int batPercent);
void getDateStr(String &s, tm *timeInfo);
void getRefreshTimeStr(String &s, bool timeSuccess, tm *timeInfo);
const char *getWiFidesc(int rssi);
const uint8_t *getWiFiBitmap16(int rssi);
const char *getHttpResponsePhrase(int code);
const char *getWifiStatusPhrase(wl_status_t status);

#endif

