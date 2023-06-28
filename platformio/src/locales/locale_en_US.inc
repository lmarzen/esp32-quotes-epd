/* American English locale data for esp32-quotes-epd.
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

#include <vector>
#include <Arduino.h>
#include "_locale.h"

// LC_TIME
// locale-based information,
// see https://man7.org/linux/man-pages/man3/nl_langinfo.3.html for more info.
// Note to Translators:
//   The LC_TIME definitions are included in the localedata/locales directory of
//   the glibc source tree, in files named after the locale codes and encoding
//   schemes. For example, the en_US locale data is stored in the
//   localedata/locales/en_US file, which contains the definitions for date and
//   time formats, month and day names, and other time-related settings for the
//   LC_TIME category.
//   https://www.gnu.org/software/libc/sources.html
// D_T_FMT                 string for formatting date and time
const char *LC_D_T_FMT     = "%a %d %b %Y %r %Z";
// LC_D_FMT                date format string
const char *LC_D_FMT       = "%m/%d/%y";
// T_FMT                   time format string
const char *LC_T_FMT       = "%r";
// T_FMT_AMPM              a.m. or p.m. time format string
const char *LC_T_FMT_AMPM  = "%I:%M:%S %p";
// AM_STR                  Ante Meridian affix
const char *LC_AM_STR      = "AM";
// PM_STR                  Post Meridian affix
const char *LC_PM_STR      = "PM";
// DAY_{1-7}               name of the n-th day of the week
const char *LC_DAY[7]      = {"Sunday", "Monday", "Tuesday", "Wednesday",
                              "Thursday", "Friday", "Saturday"};
// ABDAY_{1-7}             abbreviated name of the n-th day of the week
const char *LC_ABDAY[7]    = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
// MON_{1-12}              name of the n-th month of the year
const char *LC_MON[12]     = {"January",    "February", "March",    "April",
                               "May",       "June",     "July",     "August",
                               "September", "October",  "November", "December"};
// ABMON_{1-12}            abbreviated name of the n-th month of the year
const char *LC_ABMON[12]   = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                              "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
// ERA                     era description segments
const char *LC_ERA         = "";
// ERA_D_FMT               era date format string
const char *LC_ERA_D_FMT   = "";
// ERA_D_T_FMT             era date and time format string
const char *LC_ERA_D_T_FMT = "";
// ERA_T_FMT               era time format string
const char *LC_ERA_T_FMT   = "";

// WIFI
const char *TXT_WIFI_EXCELLENT     = "Excellent";
const char *TXT_WIFI_GOOD          = "Good";
const char *TXT_WIFI_FAIR          = "Fair";
const char *TXT_WIFI_WEAK          = "Weak";
const char *TXT_WIFI_NO_CONNECTION = "No Connection";

// LAST REFRESH
const char *TXT_UNKNOWN = "Unknown";

