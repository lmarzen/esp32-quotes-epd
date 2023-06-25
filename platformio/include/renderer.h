/* Renderer declarations for esp32-quote-epd.
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

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <Arduino.h>
#include <time.h>
#include "config.h"

#define DISP_WIDTH  800
#define DISP_HEIGHT 480

#ifdef DISP_BW
#include <GxEPD2_BW.h>
extern GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display;
#endif
#ifdef DISP_3C
#include <GxEPD2_3C.h>
extern GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> display;
#endif

typedef enum alignment
{
  LEFT,
  RIGHT,
  CENTER,
  JUSTIFY
} alignment_t;

uint16_t getStringWidth(const String &text);
uint16_t getStringHeight(const String &text);
void drawString(int16_t x, int16_t y, const String &text, alignment_t alignment,
                uint16_t color=GxEPD_BLACK, uint16_t justify_width=DISP_WIDTH);
void drawMultiLnString(int16_t x, int16_t y, const String &text, 
                       alignment_t alignment, uint16_t max_width,
                       uint16_t max_lines, int16_t line_spacing, 
                       uint16_t color=GxEPD_BLACK);
void initDisplay();
void drawQuote(const String &quote);
void drawStatusBar(const String &statusStr, const String &refreshTimeStr,
                   int rssi, double batVoltage);
void drawError(const uint8_t *bitmap_196x196,
               const String &errMsgLn1, const String &errMsgLn2);

#endif
