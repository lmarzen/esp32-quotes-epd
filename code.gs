/* Google script for esp32-quote-epd.
 * Copyright (C) 2023  Luke Marzen
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

var sheet_id = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
var ss = SpreadsheetApp.openById(sheet_id);
var sheet = ss.getSheets()[0];

/* Handles HTTP GET requests.
 */
function doGet(e) {
  var arg = e.parameter.read;
  if (arg == undefined){
    return ContentService.createTextOutput("error: invalid request");
  }
  if (arg == "rand"){
    return getRand();
  }
  if (arg == "shuffle"){
    return getShuffle();
  }
  if (arg == "iter"){
    return getIter();
  }
  return ContentService.createTextOutput("error: unexpected request"); 
}

/* Choose a quote from the first column completely at random.
 */
function getRand() {
  var range = sheet.getRange("A:A");
  var values = range.getValues();
  var cellCount = 0;
  for (var i = 0; i < values.length; i++) {
    if (values[i][0] !== "") {
      cellCount++;
    } else {
      break;
    }
  }
  
  var randomRowIndex = Math.floor(Math.random() * cellCount) + 1;
  var randomCell = sheet.getRange(randomRowIndex, 1);
  var quote = randomCell.getDisplayValue().toString();

  Logger.log(quote);
  return ContentService.createTextOutput(quote);
}

/* Each call reads the next quote. Once we have shown all the quotes once,
 * shuffle them.
 * Uses cell B1 as a pointer to the last read cell.
 */
function getShuffle() {
  var range = sheet.getRange("A:A");
  var values = range.getValues();
  var cellCount = 0;
  for (var i = 0; i < values.length; i++) {
    if (values[i][0] !== "") {
      cellCount++;
    } else {
      break;
    }
  }

  var pointer_cell = sheet.getRange("B1");
  var pointer_val = pointer_cell.getValue();
  if (pointer_val == null || Number(pointer_val) == NaN) {
    pointer_cell.setValue("0");
  }
  var pointer_pos = Number(pointer_cell.getValue());
  if (pointer_pos >= cellCount) {
    pointer_pos = 0;
    pointer_cell.setValue("0");
    range.randomize();
  }
  
  var quoteCell = sheet.getRange(pointer_pos + 1, 1);
  var quote = quoteCell.getDisplayValue().toString();

  pointer_cell.setValue(String(pointer_pos + 1));

  Logger.log(quote);
  return ContentService.createTextOutput(quote);
}

/* Loops through the quotes in order.
 * Uses cell B1 as a pointer to the last read cell.
 */
function getIter() {
  var range = sheet.getRange("A:A");
  var values = range.getValues();
  var cellCount = 0;
  for (var i = 0; i < values.length; i++) {
    if (values[i][0] !== "") {
      cellCount++;
    } else {
      break;
    }
  }

  var pointer_cell = sheet.getRange("B1");
  var pointer_val = pointer_cell.getValue();
  if (pointer_val == null || Number(pointer_val) == NaN) {
    pointer_cell.setValue("0");
  }
  var pointer_pos = Number(pointer_cell.getValue());
  if (pointer_pos >= cellCount) {
    pointer_pos = 0;
    pointer_cell.setValue("0");
  }
  
  var quoteCell = sheet.getRange(pointer_pos + 1, 1);
  var quote = quoteCell.getDisplayValue().toString();

  pointer_cell.setValue(String(pointer_pos + 1));

  Logger.log(quote);
  return ContentService.createTextOutput(quote);
}
