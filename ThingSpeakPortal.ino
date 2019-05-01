/*
 * This should be flashed to an ESP8266-01 which is connected to another 
 * client device through a serial port.  We expect to see data on the 
 * serial port in the form of a JSON string. 
 * 
 * example:  {"destination": "thingspeak", "api_key": "YOURAPIKEYHERE", "channel": xyz, "fields": [nn.nn, mm.mm, oo.oo, ...]}
 * 
 * To make this easier use the ThingSpeakPortalClient
 */



/*
 * If the client is able to send data over hardware serial this should be 0.
 * If the client needs to use SoftwareSerial set this to 1
 */
#define _USING_SOFT_SERIAL_ 0
#define HARDWARE_SERIAL_BAUD_RATE 115200
#define SOFTWARE_SERIAL_BAUD_RATE 38400


#include "ThingSpeak.h"
#include "GardenMeshCommonDefines.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>


char ssid[] = STATION_SSID;   // your network SSID (name) 
char pass[] = STATION_PASSWORD;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

bool sendThingSpeakData( String json );

void setup() {
  //On Arduino or Genuino 101 the current maximum RX speed is 57600 bps
  #if _USING_SOFT_SERIAL_ 
    Serial.begin(SOFTWARE_SERIAL_BAUD_RATE);  // Initialize serial
  #else 
    Serial.begin(HARDWARE_SERIAL_BAUD_RATE);  // Initialize serial
  #endif

  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}


void loop() {
  String json = Serial.readString();
  if( json != NULL && json != "" ) {
    sendThingSpeakData(json);
  }
}











/*****************************************************************************************************
 * 
 * SEND DATA FROM JSON TO THING SPEAK
 * 
 * {"destination": "thingspeak", "api_key": "G7OER4H5RKBVS7R5", "channel": 770064, "fields": [197670.00, 20.64, 45.62]}
 * 
 *****************************************************************************************************/
const char* k_destination_key = "destination";
const char* k_destination_thingspeak = "thingspeak";
const char* k_api_key = "api_key";
const char* k_channel_key = "channel";
const char* k_fields_key = "fields";
const char* k_status_key = "status";

bool sendThingSpeakData( String json ) {
  const int capacity = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(8) + 200; 
  StaticJsonBuffer<capacity> jb;
  JsonObject& root = jb.parseObject(json);

  // make sure we have all the parts we need to send to thing speak
  if( !root.success() ) { return false; }

  // make sure we have a destination and it is thingspeak
  if( !root.containsKey(k_destination_key) ) { return false; }
  if( root[k_destination_key] != k_destination_thingspeak ) { return false; }

  // make sure we have a destination and it is thingspeak
  if( !root.containsKey(k_api_key) ) { return false; }
  const char *api_key = root[k_api_key].as<const char*>();

  // make sure we have the channel and it is an int value
  if( !root.containsKey(k_channel_key) ) { return false; }
  if( !root[k_channel_key].is<int>() ) { return false; }
  unsigned long channel = root[k_channel_key];

  if( !root.containsKey(k_fields_key) ) { return false; }
  if( !root[k_fields_key].is<JsonArray>() ) { return false; }
  JsonArray& fields = root[k_fields_key];
  if( fields.size() < 1 ) { return false; }

  // Connect or reconnect to WiFi
  if( WiFi.status() != WL_CONNECTED ) {
    while( WiFi.status() != WL_CONNECTED ) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      delay(5000);     
    } 
  }

  unsigned int index = 1;
  for( auto field : fields ) {
    float fieldValue = field.as<float>();
    ThingSpeak.setField(index, fieldValue);
    index += 1;
  }

  if( root.containsKey(k_status_key) ) {
    const char *status = root[k_status_key].as<const char*>();
    ThingSpeak.setStatus(status);
  }

  // write to the ThingSpeak channel
  int res = ThingSpeak.writeFields(channel, api_key);
  if( res == 200 ) {
    return true;
  } 
  
  return false;
}
