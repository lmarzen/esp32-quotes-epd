/* Locale data declarations for esp32-quote-epd.
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

#ifndef ___LOCALE_H__
#define ___LOCALE_H__

#include <vector>
#include <Arduino.h>

// LC_TIME
extern const char *LC_D_T_FMT;
extern const char *LC_D_FMT;
extern const char *LC_T_FMT;
extern const char *LC_T_FMT_AMPM;
extern const char *LC_AM_STR;
extern const char *LC_PM_STR;
extern const char *LC_DAY[7];
extern const char *LC_ABDAY[7];
extern const char *LC_MON[12];
extern const char *LC_ABMON[12];
extern const char *LC_ERA;
extern const char *LC_ERA_D_FMT;
extern const char *LC_ERA_D_T_FMT;
extern const char *LC_ERA_T_FMT;

// WIFI
extern const char *TXT_WIFI_EXCELLENT;
extern const char *TXT_WIFI_GOOD;
extern const char *TXT_WIFI_FAIR;
extern const char *TXT_WIFI_WEAK;
extern const char *TXT_WIFI_NO_CONNECTION;

// LAST REFRESH
extern const char *TXT_UNKNOWN;

#endif

