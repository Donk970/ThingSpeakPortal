



#ifndef _THINGSPEAK_PORTAL_H_ 
#define _THINGSPEAK_PORTAL_H_



/*****************************************************************************************************
 * 
 * SEND DATA FROM JSON TO THING SPEAK
 * 
 * {destination: "thingspeak", api_key: "G7OER4H5RKBVS7R5", channel: 770064, fields: [123.45, 234.56, 00.00], status: ""}
 * {"destination": "thingspeak", "api_key": "G7OER4H5RKBVS7R5", "channel": 770064, "fields": [197670.00, 20.64, 45.62]}
 * 
 *****************************************************************************************************/
const String k_thingspeak_prefix = "{\"destination\": \"thingspeak\", \"api_key\": \"G7OER4H5RKBVS7R5\", \"channel\": 770064, \"fields\": [";
const String k_thingspeak_suffix = "]}";

String jsonForReadings( double newCO2, double newTemp = 0.0, double newHum = 0.0 ) {
  return k_thingspeak_prefix + newCO2 + ", " + newTemp + ", " + newHum + k_thingspeak_suffix;
}

void addReading( double newCO2, double newTemp = 0.0, double newHum = 0.0 ) {
  String json = jsonForReadings( newCO2, newTemp, newHum );
  Serial.println(json); // send json to the little esp8266 thingspeak portal device
}


String tsPortalJSON( String api_key, unsigned int channel, int fieldCount, ... ) {
  va_list arguments;
  va_start( arguments, fieldCount );  
  
  for( int a = 0; a < fieldCount; a++ ) {
    float arg = va_arg( arguments, float); 
  }
  va_end( arguments );

}











#endif
