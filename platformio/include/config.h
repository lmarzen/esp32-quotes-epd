/* Configuration option declarations for esp32-quote-epd.
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

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <cstdint>
#include <Arduino.h>
#include <gfxfont.h>

// E-PAPER PANEL
// This project currently supports the following E-Paper panels:
//   DISP_BW - WaveShare 800x480, 7.5inch E-Ink display, Black/White
//             https://www.waveshare.com/product/7.5inch-e-paper-hat.htm
//   DISP_3C - Waveshare 800x480, 7.5inch E-Ink display, Red/Black/White
//             https://www.waveshare.com/product/7.5inch-e-paper-hat-b.htm
// Uncomment the macro that identifies your physical panel.
#define DISP_BW
// #define DISP_3C

// 3 COLOR E-INK ACCENT COLOR
// Defines the 3rd color to be used when a 3 color display is selected.
#ifdef DISP_3C
  // #define ACCENT_COLOR GxEPD_BLACK
  #define ACCENT_COLOR GxEPD_RED
#endif

// FONTS
// A handful of popular Open Source typefaces have been included with this
// project for your convenience.
//
//   FONT                   HEADER FILE              FAMILY        LICENSE
//   Allerta Stencil        AllertaStencil_Regular.h               SIL OFL v1.1
//   Bangers Regular        Bangers_Regular.h                      SIL OFL v1.1
//   Covered By Your Grace  CoveredByYourGrace_Regular.h           SIL OFL v1.1
//   Graduate               Graduate_Regular.h                     SIL OFL v1.1
//   FreeMono               FreeMono.h               GNU FreeFont  GNU GPL v3.0
//   FreeSans               FreeSans.h               GNU FreeFont  GNU GPL v3.0
//   FreeSerif              FreeSerif.h              GNU FreeFont  GNU GPL v3.0
//   Lato                   Lato_Regular.h           Lato          SIL OFL v1.1
//   Montserrat             Montserrat_Regular.h     Montserrat    SIL OFL v1.1
//   Open Sans              OpenSans_Regular.h       Open Sans     SIL OFL v1.1
//   Orbit                  Orbit_Regular.h                        SIL OFL v1.1
//   Patrick Hand SC        PatrickHandSC_Regular.h                SIL OFL v1.1
//   Permanent Marker       PermanentMarker_Regular.h              Apache v2.0
//   Poppins                Poppins_Regular.h        Poppins       SIL OFL v1.1
//   Quicksand              Quicksand_Regular.h      Quicksand     SIL OFL v1.1
//   Raleway                Raleway_Regular.h        Raleway       SIL OFL v1.1
//   Roboto                 Roboto_Regular.h         Roboto        Apache v2.0
//   Roboto Mono            RobotoMono_Regular.h     Roboto Mono   Apache v2.0
//   Roboto Slab            RobotoSlab_Regular.h     Roboto Slab   Apache v2.0
//   Share Tech Mono        ShareTechMono_Regular.h                SIL OFL v1.1
//   Syne Mono              SyneMono_Regular.h                     SIL OFL v1.1
//   Ubuntu                 Ubuntu_R.h               Ubuntu font   UFL v1.0
//   Ubuntu Mono            UbuntuMono_R.h           Ubuntu font   UFL v1.0
//   VT323                  VT323_Regular.h                        SIL OFL v1.1
//
// Included sizes: 6pt, 12pt, 16pt, 20pt, 26pt, 32pt, 40pt, 48pt.
//
// Adding new fonts or new sizes is relatively straightforward, see
// fonts/README.
//
// #include the fonts you want to use.
#include "fonts/FreeSerif.h"
#include "fonts/Roboto_Regular.h"
#include "fonts/Ubuntu_R.h"

// LOCALE
// If your locale is not here, you can add it by copying and modifying one of
// the files in src/locales. Please feel free to create a pull request to add
// official support for your locale.
//   Language (Territory)            code
//   English (United States)         en_US
#define LOCALE en_US

// Set the below constants in "config.cpp"
extern const uint8_t PIN_BAT_ADC;
extern const uint8_t PIN_EPD_BUSY;
extern const uint8_t PIN_EPD_CS;
extern const uint8_t PIN_EPD_RST;
extern const uint8_t PIN_EPD_DC;
extern const uint8_t PIN_EPD_SCK;
extern const uint8_t PIN_EPD_MISO;
extern const uint8_t PIN_EPD_MOSI;
extern const char *WIFI_SSID;
extern const char *WIFI_PASSWORD;
extern const String GS_DEPLOYMENT_ID;
extern const String GS_ENDPOINT;
extern const int MARGIN_X;
extern const int MARGIN_Y;
extern const int QUOTE_JUSTIFICATION;
extern const int AUTHOR_JUSTIFICATION;
extern const String QUOTE_SELECTION;
extern const GFXfont *QUOTE_FONTS[];
extern const size_t QUOTE_FONTS_SZ;
extern const char *TIMEZONE;
extern const char *DATE_FORMAT;
extern const char *REFRESH_TIME_FORMAT;
extern const char *NTP_SERVER_1;
extern const char *NTP_SERVER_2;
extern const long SLEEP_DURATION;
extern const int BED_TIME;
extern const int WAKE_TIME;
extern const float BATTERY_WARN_VOLTAGE;
extern const float LOW_BATTERY_VOLTAGE;
extern const float VERY_LOW_BATTERY_VOLTAGE;
extern const float CRIT_LOW_BATTERY_VOLTAGE;
extern const unsigned long LOW_BATTERY_SLEEP_INTERVAL;
extern const unsigned long VERY_LOW_BATTERY_SLEEP_INTERVAL;

#endif

