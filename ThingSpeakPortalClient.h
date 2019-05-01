
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
 *
 * The ThingSpeakPortalClient is intended for use with the ThingSpeakPortal device that
 * is running the ThingSpeakPortal sketch.  The device should be an ESP8266-01 which is all
 * that's necessery for this purpose.
 * 
 * The client is constructed with the details for a channel
 * 
 * 
 */


#include <Arduino.h>
#include <SoftwareSerial.h>

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









class ThingSpeakPortalClient {
  public: 
    
    /*
     * Initialize the client for use with HardwareSerial.  This is preferable if you have multiple
     * hardware serial streams such as the arduino mega has.
     * 
     * paramaters: 
     *  api_key: the api write key for the ThingSpeak channel
     *  channel: the channel id for the ThingSpeak channel
     *  fieldCount: how many fields in the ThingSpeak channel
     *  device: the hardware serial stream
     *  
     */
    ThingSpeakPortalClient( String api_key, unsigned int channel, int fieldCount, HardwareSerial& device );
  
    /*
     * Initialize the client for use with SoftSerial.  This is useful when you only
     * have a single hardware serial port that you need for something else.
     * 
     * paramaters: 
     *  api_key: the api write key for the ThingSpeak channel
     *  channel: the channel id for the ThingSpeak channel
     *  fieldCount: how many fields in the ThingSpeak channel
     *  rx_pin: the rx pin for the SoftSerial stream
     *  tx_pin: the tx pin for the SoftSerial stream
     */
    ThingSpeakPortalClient( String api_key, unsigned int channel, int fieldCount, uint8_t rx_pin, uint8_t tx_pin ); // RX, TX

    /*
     * Start the serial port at the appropriate baud rate.  
     * 
     * In the case of the SoftwareSerial connection the ThingSpeakPortal device 
     * should be set up to use a 38400 baud rate since SoftwareSerial is unreliable 
     * at high boud rates.  The client should be set to the same rate.
     * 
     * In the case of HardwareSerial connection the ThingSpeakPortal device defaults
     * to be set up to use 115200. The client should be set to the same rate.
     */
    void begin( unsigned long baud_rate );

    /*
     * The send function takes a variadic list of floats as it's arguments.  The function
     * expects the list of variables to be the same number as specified in fieldCount in the
     * constructor.
     */
    void send( String status, float args[] );

  private: 
    HardwareSerial* _hwSerial = NULL;
    SoftwareSerial* _swSerial = NULL;
    int _fieldCount = 0;
    String _apiKey;
    String _channel;
};






#endif
