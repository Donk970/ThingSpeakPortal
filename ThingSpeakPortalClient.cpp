/*
 * MIT License
 * Copyright (c) 2019 Daniel E Steward
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */


#include "ThingSpeakPortalClient.h"

ThingSpeakPortalClient:: ThingSpeakPortalClient( String api_key, unsigned int channel, int fieldCount, uint8_t rx_pin, uint8_t tx_pin ) { 
  _swSerial = new SoftwareSerial(rx_pin, tx_pin);
  _fieldCount = fieldCount; 
  _apiKey = api_key;
  _channel = channel;
} // RX, TX

ThingSpeakPortalClient:: ThingSpeakPortalClient( String api_key, unsigned int channel, int fieldCount, HardwareSerial& device ) { 
  _hwSerial = &device; 
  _fieldCount = fieldCount; 
  _apiKey = api_key;
  _channel = channel;
}

void ThingSpeakPortalClient:: begin( unsigned long baud_rate ) { 
  if( _hwSerial != NULL ) { 
    _hwSerial->begin(baud_rate); 
  } else if( _swSerial != NULL ) { 
    _swSerial->begin(baud_rate); 
  } 
}

void ThingSpeakPortalClient:: send( String status = "", ... ) {
  va_list arguments;
  va_start( arguments, _fieldCount );  

  String json = "\"destination\": \"thingspeak\", \"api_key\": \"" + _apiKey + ", \"channel\": " + _channel + ", \"status\": " + status + ", \"fields\": [";
  bool first = true;
  for( int a = 0; a < _fieldCount; a++ ) {
    float arg = va_arg( arguments, float); 
    json += String(first ? "" : ", ") + String(arg);
    first = false;
  }
  va_end( arguments );

  json += "]}";
  if( _hwSerial != NULL ) { 
    _hwSerial->print(json);
  } else if( _swSerial != NULL ) { 
    _swSerial->print(json);
  } 
}
