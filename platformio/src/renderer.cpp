/* Renderer for esp32-quote-epd.
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

#include "_locale.h"
#include "_strftime.h"
#include "renderer.h"
#include "config.h"
#include "display_utils.h"

// fonts
#include "fonts/FreeSans.h"

// icon header files
#include "icons/icons_24x24.h"
#include "icons/icons_32x32.h"

#ifdef DISP_BW
GxEPD2_BW<GxEPD2_750_T7, GxEPD2_750_T7::HEIGHT> display(
  GxEPD2_750_T7(PIN_EPD_CS,
                PIN_EPD_DC,
                PIN_EPD_RST,
                PIN_EPD_BUSY));
#endif
#ifdef DISP_3C
GxEPD2_3C<GxEPD2_750c_Z08, GxEPD2_750c_Z08::HEIGHT / 2> display(
  GxEPD2_750c_Z08(PIN_EPD_CS,
                  PIN_EPD_DC,
                  PIN_EPD_RST,
                  PIN_EPD_BUSY));
#endif

#ifndef ACCENT_COLOR
  #define ACCENT_COLOR GxEPD_BLACK
#endif

/* Returns the string width in pixels
 */
uint16_t getStringWidth(String text)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return w;
}

/* Returns the string height in pixels
 */
uint16_t getStringHeight(String text)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  return h;
}

static const char printableASCII[] = " !\"#$%&`()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
/* Returns the maximum height of a font over the set of printable ascii
 * characters.
 */
uint16_t getCurrentFontHeight()
{
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(printableASCII, 0, 0, &x1, &y1, &w, &h);
  return h;
}

/* Draws a string with alignment
 */
void drawString(int16_t x, int16_t y, String text, alignment_t alignment,
                uint16_t color)
{
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextColor(color);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (alignment == RIGHT)
    x = x - w;
  if (alignment == CENTER)
    x = x - w / 2;
  display.setCursor(x, y);
  display.print(text);
} // end drawString

/* Draws a string that will flow into the next line when max_width is reached.
 * If a string exceeds max_lines an ellipsis (...) will terminate the last word.
 * Lines will break at spaces(' ') and dashes('-').
 *
 * Note: max_width should be big enough to accommodate the largest word that
 *       will be displayed. If an unbroken string of characters longer than
 *       max_width exist in text, then the string will be printed beyond
 *       max_width.
 */
void drawMultiLnString(int16_t x, int16_t y, String text, alignment_t alignment,
                       uint16_t max_width, uint16_t max_lines,
                       int16_t line_spacing, uint16_t color)
{
  uint16_t current_line = 0;
  String textRemaining = text;
  // print until we reach max_lines or no more text remains
  while (current_line < max_lines && !textRemaining.isEmpty())
  {
    int16_t  x1, y1;
    uint16_t w, h;

    display.getTextBounds(textRemaining, 0, 0, &x1, &y1, &w, &h);

    int endIndex = textRemaining.length();
    // check if remaining text is to wide, if it is then print what we can
    String subStr = textRemaining;
    int splitAt = 0;
    int keepLastChar = 0;
    while (w > max_width && splitAt != -1)
    {
      if (keepLastChar)
      {
        // if we kept the last character during the last iteration of this while
        // loop, remove it now so we don't get stuck in an infinite loop.
        subStr.remove(subStr.length() - 1);
      }

      // find the last place in the string that we can break it.
      if (current_line < max_lines - 1)
      {
        splitAt = std::max(subStr.lastIndexOf(" "),
                           subStr.lastIndexOf("-"));
      }
      else
      {
        // this is the last line, only break at spaces so we can add ellipsis
        splitAt = subStr.lastIndexOf(" ");
      }

      // if splitAt == -1 then there is an unbroken set of characters that is
      // longer than max_width. Otherwise if splitAt != -1 then we can continue
      // the loop until the string is <= max_width
      if (splitAt != -1)
      {
        endIndex = splitAt;
        subStr = subStr.substring(0, endIndex + 1);

        char lastChar = subStr.charAt(endIndex);
        if (lastChar == ' ')
        {
          // remove this char now so it is not counted towards line width
          keepLastChar = 0;
          subStr.remove(endIndex);
          --endIndex;
        }
        else if (lastChar == '-')
        {
          // this char will be printed on this line and removed next iteration
          keepLastChar = 1;
        }

        if (current_line < max_lines - 1)
        {
          // this is not the last line
          display.getTextBounds(subStr, 0, 0, &x1, &y1, &w, &h);
        }
        else
        {
          // this is the last line, we need to make sure there is space for
          // ellipsis
          display.getTextBounds(subStr + "...", 0, 0, &x1, &y1, &w, &h);
          if (w <= max_width)
          {
            // ellipsis fit, add them to subStr
            subStr = subStr + "...";
          }
        }

      } // end if (splitAt != -1)
    } // end inner while

    drawString(x, y + (current_line * line_spacing), subStr, alignment, color);

    // update textRemaining to no longer include what was printed
    // +1 for exclusive bounds, +1 to get passed space/dash
    textRemaining = textRemaining.substring(endIndex + 2 - keepLastChar);

    ++current_line;
  } // end outer while

  return;
} // end drawMultiLnString

/* Initialize e-paper display
 */
void initDisplay()
{
  display.init(115200, true, 2, false);
  // display.init(); for older Waveshare HAT's
  SPI.begin(PIN_EPD_SCK,
            PIN_EPD_MISO,
            PIN_EPD_MOSI,
            PIN_EPD_CS);

  display.setRotation(0);
  display.setTextSize(1);
  display.setTextColor(GxEPD_BLACK);
  display.setTextWrap(false);
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
} // end initDisplay

/* Splits the input text at spaces(' ') and dashes('-'). Appends each line into
 * into the dest vector.
 *
 * Returns false if the entire quote does not fit within the width and height
 * requirements based on the current font size.
 *
 * Note: max_width should be big enough to accommodate the largest word that
 *       will be displayed. If an unbroken string of characters longer than
 *       max_width exist in text, then the string will be printed beyond
 *       max_width.
 */
bool splitQuote(std::vector<String> *dest, String quote,
                uint16_t max_width, uint16_t max_lines)
{
  uint16_t current_line = 0;
  String textRemaining = quote;
  // print until we reach max_lines or no more text remains
  while (current_line < max_lines && !textRemaining.isEmpty())
  {
    int16_t  x1, y1;
    uint16_t w, h;

    display.getTextBounds(textRemaining, 0, 0, &x1, &y1, &w, &h);

    int endIndex = textRemaining.length();
    // check if remaining text is to wide, if it is then print what we can
    String subStr = textRemaining;
    int splitAt = 0;
    int nl = textRemaining.indexOf("\n");
    if (nl != -1)
    {
      String tmpStr = textRemaining.substring(0, nl);
      display.getTextBounds(tmpStr, 0, 0, &x1, &y1, &w, &h);
      if (w <= max_width)
      {
        endIndex = nl - 1;
      }
    }

    int keepLastChar = 0;
    while (w > max_width && splitAt != -1)
    {
      // Serial.println("\"" + textRemaining + "\"");
      if (keepLastChar)
      {
        // if we kept the last character during the last iteration of this while
        // loop, remove it now so we don't get stuck in an infinite loop.
        subStr.remove(subStr.length() - 1);
      }

      // find the last place in the string that we can break it.
      if (current_line < max_lines - 1)
      {
        splitAt = std::max(subStr.lastIndexOf(" "),
                           subStr.lastIndexOf("-"));
      }
      else
      {
        // this is the last line, only break at spaces so we can add ellipsis
        splitAt = subStr.lastIndexOf(" ");
      }

      // if splitAt == -1 then there is an unbroken set of characters that is
      // longer than max_width. Otherwise if splitAt != -1 then we can continue
      // the loop until the string is <= max_width
      if (splitAt != -1)
      {
        endIndex = splitAt;
        subStr = subStr.substring(0, endIndex + 1);

        char lastChar = subStr.charAt(endIndex);
        if (lastChar == ' ' || lastChar == '\n')
        {
          // remove this char now so it is not counted towards line width
          keepLastChar = 0;
          subStr.remove(endIndex);
          --endIndex;
          // Serial.println("end removed subStr = " + subStr);
        }
        else if (lastChar == '-')
        {
          // this char will be printed on this line and removed next iteration
          keepLastChar = 1;
        }

        if (current_line < max_lines - 1)
        {
          // this is not the last line
          display.getTextBounds(subStr, 0, 0, &x1, &y1, &w, &h);
        }
        else
        {
          // this is the last line, we need to make sure there is space for
          // ellipsis
          display.getTextBounds(subStr + "...", 0, 0, &x1, &y1, &w, &h);
          if (w <= max_width)
          {
            // ellipsis fit, add them to subStr
            subStr = subStr + "...";
          }
        }

      } // end if (splitAt != -1)
    } // end inner while

    dest->push_back(textRemaining.substring(0, endIndex + 1 - keepLastChar));
    // update textRemaining to no longer include what was printed
    // +1 for exclusive bounds, +1 to get passed space/dash
    textRemaining = textRemaining.substring(endIndex + 2 - keepLastChar);

    ++current_line;
  } // end outer while

  if (!textRemaining.isEmpty()) {
    return false;
  }

  return true;
} // end splitQuote

/* This function is responsible for drawing the quote in the center of the
 * screen. A quote's author is preceeded by the '@' symbol. 
 */
void drawQuote(const String &quote)
{
  String quoteBody;
  String author;

  // seperate quote body and author
  if (quote[0] == '@') {
    author = quote;
  }
  else
  {
    for (int i = 0; i < quote.length(); ++i)
    {
      if (quote[i - 1] != '\\' && quote[i] == '@')
      {
        quoteBody = quote.substring(0, i);
        author = quote.substring(i + 1, quote.length());
      }
    }
  }
  if (author.isEmpty())
  {
    quoteBody = quote;
  }

  quoteBody.trim();
  author.trim();

  bool fitsScreen = false;
  int i = 0;
  std::vector<String> lines;

  uint16_t qLineHeight = 0;
  uint16_t aLineHeight = 0;
  uint16_t q_dy = 0;
  uint16_t a_dy = 0;
  uint16_t maxLines = 0;

  while(!fitsScreen && i + 1 < QUOTE_FONTS_SZ) {
    if (!author.isEmpty())
    {
      display.setFont(QUOTE_FONTS[i + 1]);
      aLineHeight = getCurrentFontHeight();
      a_dy = aLineHeight + (aLineHeight / 4);
    }

    display.setFont(QUOTE_FONTS[i]);
    qLineHeight = getCurrentFontHeight();
    q_dy = qLineHeight + (qLineHeight / 4);

    maxLines = ((DISP_HEIGHT - MARGIN_Y * 2) - a_dy) / q_dy;

    lines.clear();
    fitsScreen = splitQuote(&lines, quoteBody,
                            DISP_WIDTH - MARGIN_X * 2, maxLines);
    i += 2;
  }

  Serial.println("Drawing " + String(lines.size()) + " line(s): ");

  int totalHeight = qLineHeight + (lines.size() - 1) * q_dy;
  totalHeight += a_dy;
  int y = (DISP_HEIGHT / 2) - (totalHeight / 2)
          + (qLineHeight / 2) + (qLineHeight / 3);

  for (int l = 0; l < lines.size(); ++l) {
    switch (static_cast<alignment_t>(QUOTE_JUSTIFICATION))
    {
    case LEFT:
      drawString(MARGIN_X, y, lines[l], LEFT, ACCENT_COLOR);
      break;
    case RIGHT:
      drawString(DISP_WIDTH - MARGIN_X, y, lines[l], RIGHT, ACCENT_COLOR);
      break;
    case CENTER:
    default:
      drawString(DISP_WIDTH / 2, y, lines[l], CENTER, ACCENT_COLOR);
      break;
    }

    y += q_dy;
    Serial.println("  " + lines[l]);
  }

  if (!author.isEmpty()) {
    y = y - q_dy + a_dy;
    display.setFont(QUOTE_FONTS[i - 1]);
    switch (static_cast<alignment_t>(AUTHOR_JUSTIFICATION))
    {
    case LEFT:
      drawString(MARGIN_X, y, author, LEFT, ACCENT_COLOR);
      break;
    case RIGHT:
      drawString(DISP_WIDTH - MARGIN_X, y, author, RIGHT, ACCENT_COLOR);
      break;
    case CENTER:
    default:
      drawString(DISP_WIDTH / 2, y, author, CENTER, ACCENT_COLOR);
      break;
    }
  }


  return;
} // end drawQuote


/* This function is responsible for drawing the status bar along the bottom of
 * the display.
 */
void drawStatusBar(String statusStr, String refreshTimeStr, int rssi,
                   double batVoltage)
{
  String dataStr;
  uint16_t dataColor = GxEPD_BLACK;
  display.setFont(&FreeSans_6pt7b);
  int pos = DISP_WIDTH - 2;
  const int sp = 2;

  // battery
  int batPercent = calcBatPercent(batVoltage);
  if (batVoltage < BATTERY_WARN_VOLTAGE) {
    dataColor = ACCENT_COLOR;
  }
  dataStr = String(batPercent) + "% ("
            + String( round(100.0 * batVoltage) / 100.0, 2 ) + "v)";
  drawString(pos, DISP_HEIGHT - 1 - 2, dataStr, RIGHT, dataColor);
  pos -= getStringWidth(dataStr) + 25;
  display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 17,
                             getBatBitmap24(batPercent), 24, 24, dataColor);
  pos -= sp + 9;

  // WiFi
  dataStr = String(getWiFidesc(rssi));
  dataColor = rssi >= -70 ? GxEPD_BLACK : ACCENT_COLOR;
  if (rssi != 0)
  {
    dataStr += " (" + String(rssi) + "dBm)";
  }
  drawString(pos, DISP_HEIGHT - 1 - 2, dataStr, RIGHT, dataColor);
  pos -= getStringWidth(dataStr) + 19;
  display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 13, getWiFiBitmap16(rssi),
                             16, 16, dataColor);
  pos -= sp + 8;

  // last refresh
  dataColor = GxEPD_BLACK;
  drawString(pos, DISP_HEIGHT - 1 - 2, refreshTimeStr, RIGHT, dataColor);
  pos -= getStringWidth(refreshTimeStr) + 25;
  display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 21, wi_refresh_32x32,
                             32, 32, dataColor);
  pos -= sp;

  // status
  dataColor = ACCENT_COLOR;
  if (!statusStr.isEmpty())
  {
    drawString(pos, DISP_HEIGHT - 1 - 2, statusStr, RIGHT, dataColor);
    pos -= getStringWidth(statusStr) + 24;
    display.drawInvertedBitmap(pos, DISP_HEIGHT - 1 - 18, error_icon_24x24,
                               24, 24, dataColor);
  }

  return;
} // end drawStatusBar

/* This function is responsible for drawing prominent error messages to the
 * screen.
 */
void drawError(const uint8_t *bitmap_196x196,
               const String &errMsgLn1, const String &errMsgLn2)
{
  display.setFont(&FreeSans_26pt7b);
  drawString(DISP_WIDTH / 2,
             DISP_HEIGHT / 2 + 196 / 2 + 21,
             errMsgLn1, CENTER);
  drawString(DISP_WIDTH / 2,
             DISP_HEIGHT / 2 + 196 / 2 + 76,
             errMsgLn2, CENTER);
  display.drawInvertedBitmap(DISP_WIDTH / 2 - 196 / 2,
                             DISP_HEIGHT / 2 - 196 / 2 - 21,
                             bitmap_196x196, 196, 196, ACCENT_COLOR);
  return;
} // end drawError

