/* Configuration options for esp32-quote-epd.
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

#include <Arduino.h>
#include "config.h"

// PINS
// ADC pin used to measure battery voltage
const uint8_t PIN_BAT_ADC  = A2;
// Pins for Waveshare e-paper Driver Board
const uint8_t PIN_EPD_BUSY = 13;
const uint8_t PIN_EPD_CS   =  2;
const uint8_t PIN_EPD_RST  = 21;
const uint8_t PIN_EPD_DC   = 22;
const uint8_t PIN_EPD_SCK  = 18;
const uint8_t PIN_EPD_MISO = 19; // 19 Master-In Slave-Out not used, as no data from display
const uint8_t PIN_EPD_MOSI = 23;

// WIFI CREDENTIALS
const char *WIFI_SSID     = "ssid";
const char *WIFI_PASSWORD = "password";

// GOOGLE SCRIPT
const String GS_DEPLOYMENT_ID = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const String GS_ENDPOINT = "script.google.com";

// MARGINS, JUSTIFICATION, & FONT SIZES
const int MARGIN_X = 40;
const int MARGIN_Y = 24;
// Left   = 0
// Right  = 1
// Center = 2
const int QUOTE_JUSTIFICATION = 2;
const int AUTHOR_JUSTIFICATION = 1;

// QUOTE SELECTION METHOD
// Choose from the following selection methods.
//   "rand"    - Choose a quote from the first column completely at random.
//   "shuffle" - Each call read the next quote. Once we have shown all the
//               quotes once, shuffle them.
//               Uses cell B1 as a pointer to the last read cell.
//   "iter"    - Loops through the quotes in order.
//               Uses cell B1 as a pointerto the last read cell.
const String QUOTE_SELECTION = "shuffle";

// FONT SELECTION
// Quotes will be drawn in the largest font that will fit the screen.
// Below you can change what fonts are used to draw the quote body and the
// author's name.
// The array is organized to contain the font for the quote body immediately
// followed by the corresponding font that the author's name should be drawn.
// The QUOTE_FONTS array must therefore ALWAYS have an EVEN number of elements.
// Additionally, the array must be organized in order from LARGEST to SMALLEST
// in order for the scaling feature to work correctly.
//
// If you always want the same size (disable scaling), then QUOTE_FONTS should
// contain just two fonts.
// Alternatively, if you want more granularity then add more pairs of fonts to
// QUOTE_FONTS.
//
// See config.h for a list of include fonts.
const GFXfont *QUOTE_FONTS[] =
{//quote body font         author font
  &FreeSerif_48pt7b,      &FreeSerif_26pt7b,
  &FreeSerif_40pt7b,      &FreeSerif_26pt7b,
  &Roboto_Regular_32pt7b, &Roboto_Regular_26pt7b,
  &Roboto_Regular_26pt7b, &Roboto_Regular_20pt7b,
  &Ubuntu_R_20pt7b,       &Ubuntu_R_20pt7b,
  &Ubuntu_R_16pt7b,       &Ubuntu_R_16pt7b
  &Ubuntu_R_12pt7b,       &Ubuntu_R_12pt7b
};
const size_t QUOTE_FONTS_SZ = sizeof(QUOTE_FONTS) / sizeof(QUOTE_FONTS[0]);

// TIME
// For list of time zones see
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char *TIMEZONE = "EST5EDT,M3.2.0,M11.1.0";
// Date format used when when printing to serial connection.
// For more information about formatting see
// https://man7.org/linux/man-pages/man3/strftime.3.html
const char *DATE_FORMAT = "%A, %B %e"; // Saturday, January 1
// Date/Time format used when displaying the last refresh time along the bottom
// of the screen.
// For more information about formatting see
// https://man7.org/linux/man-pages/man3/strftime.3.html
const char *REFRESH_TIME_FORMAT = "%x %H:%M";
// NTP_SERVER_1 is the primary time server, while NTP_SERVER_2 is a fallback.
// In most cases it's best to use pool.ntp.org to find an NTP server
// The system will try finding the closest available servers for you.
const char *NTP_SERVER_1 = "us.pool.ntp.org";
const char *NTP_SERVER_2 = "time.nist.gov";
// Sleep duration in minutes. (aka how often esp32 will wake for an update)
// Aligned to the nearest minute boundary and must evenly divide 60.
// For example, if set to 30 (minutes) the display will update at 00 or 30
// minutes past the hour.
const long SLEEP_DURATION = 24 * 60; // 24 hours
// If BED_TIME == WAKE_TIME, then this battery saving feature will be disabled.
// (range: [0-23])
const int BED_TIME  = 00; // Last update at 00:00 (midnight) until WAKE_TIME.
const int WAKE_TIME = 06; // Hour of first update after BED_TIME, 06:00.

// BATTERY
// To protect the battery upon LOW_BATTERY_VOLTAGE, the display will cease to
// update until battery is charged again. The ESP32 will deep-sleep (consuming
// < 11μA), waking briefly check the voltage at the corresponding interval (in
// minutes). Once the battery voltage has fallen to CRIT_LOW_BATTERY_VOLTAGE,
// the esp32 will hibernate and a manual press of the reset (RST) button to
// begin operating again.
const float BATTERY_WARN_VOLTAGE     = 3.40; // (volts) ~ 10%
const float LOW_BATTERY_VOLTAGE      = 3.20; // (volts)
const float VERY_LOW_BATTERY_VOLTAGE = 3.10; // (volts)
const float CRIT_LOW_BATTERY_VOLTAGE = 3.00; // (volts)
const unsigned long LOW_BATTERY_SLEEP_INTERVAL      = 30;  // (minutes)
const unsigned long VERY_LOW_BATTERY_SLEEP_INTERVAL = 120; // (minutes)

