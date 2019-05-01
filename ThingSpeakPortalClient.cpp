

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
