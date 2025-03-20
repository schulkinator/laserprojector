/*
 * HelTec Automation(TM) WIFI_LoRa_32 factory test code which includes
 *  the follow functions:
 *
 * - Basic OLED function test;
 *
 * - Basic serial port test(in baud rate 115200);
 *
 * - Basic LED test;
 *
 * - WIFI join and scan test;
 *
 * - ArduinoOTA By Wifi;
 *
 * - Timer test and some other Arduino basic functions.
 *
 * by lxyzn from HelTec AutoMation, ChengDu, China
 * 成都惠利特自动化科技有限公司¸
 * www.heltec.cn
 *
 * This project is also available on GitHub:
 * https://github.com/HelTecAutomation/Heltec_ESP32
*/


#include <ArduinoOTA.h>
#include <WiFi.h>
#include <Wire.h>
#include "HT_SSD1306Wire.h"
#include <LiquidCrystal.h>

#define LASER_CONTROL_PIN 46

static bool laser_state = false;
/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/



// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 45, en = 42, d4 = 6, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


/**********************************************  WIFI Client *********************************

 * wifi client
 */
const char* ssid = "xxx"; //replace "xxxxxx" with your WIFI's ssid
const char* password = "xxx"; //replace "xxxxxx" with your WIFI's password


//WiFi&OTA 
//#define HOSTNAME "HelTec_OTA_OLED"
#define PASSWORD "HT.123456" //the password for OTA upgrade, can set it in any char you want

/************************************************  *********************************
 * Whether to use static IP
 */

#define USE_STATIC_IP false
#if USE_STATIC_IP
  IPAddress staticIP(192,168,1,22);
  IPAddress gateway(192,168,1,9);
  IPAddress subnet(255,255,255,0);
  IPAddress dns1(8, 8, 8, 8);
  IPAddress dns2(114,114,114,114);
#endif

/*******************************************************************
 * OLED Arguments
 */

//#define RST_OLED 16                     //OLED Reset pin, requires manual reset, otherwise it will not be displayed
#define OLED_UPDATE_INTERVAL 500        //OLED screen refresh interval ms
//SSD1306 display(0x3C, 4, 15);           //Pins 4 and 5 are bound to the motherboard of Kit 32 and cannot be used for other purposes
static SSD1306Wire  display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst


/********************************************************************
 * OTA upgrade configuration

 */
void setupOTA()
{
  //Port defaults to 8266
  //ArduinoOTA.setPort(8266);

  //Hostname defaults to esp8266-[ChipID]
//  ArduinoOTA.setHostname(HOSTNAME);

  //No authentication by default
  ArduinoOTA.setPassword(PASSWORD);

  //Password can be set with it's md5 value as well
  //MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  //ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]()
  {

    display.clear();
    display.setFont(ArialMT_Plain_10);        //Set font size
    display.setTextAlignment(TEXT_ALIGN_LEFT);//Set font alignment
    display.drawString(0, 0, "Start Updating....");



    Serial.printf("Start Updating....Type:%s\n", (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem");
  });

  ArduinoOTA.onEnd([]()
  {
    display.clear();
    display.drawString(0, 0, "Update Complete!");
    Serial.println("Update Complete!");

    ESP.restart();
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    String pro = String(progress / (total / 100)) + "%";
    int progressbar = (progress / (total / 100));
    //int progressbar = (progress / 5) % 100;
    //int pro = progress / (total / 100);

    display.clear();
#if defined (WIRELESS_STICK)
    display.drawProgressBar(0, 11, 64, 8, progressbar);    // draw the progress bar
    display.setTextAlignment(TEXT_ALIGN_CENTER);          // draw the percentage as String
    display.drawString(10, 20, pro);
#else
    display.drawProgressBar(0, 32, 120, 10, progressbar);    // draw the progress bar
    display.setTextAlignment(TEXT_ALIGN_CENTER);          // draw the percentage as String
    display.drawString(64, 15, pro);
#endif
    display.display();

    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("Error[%u]: ", error);
    String info = "Error Info:";
    switch(error)
    {
      case OTA_AUTH_ERROR:
        info += "Auth Failed";
        Serial.println("Auth Failed");
        break;

      case OTA_BEGIN_ERROR:
        info += "Begin Failed";
        Serial.println("Begin Failed");
        break;

      case OTA_CONNECT_ERROR:
        info += "Connect Failed";
        Serial.println("Connect Failed");
        break;

      case OTA_RECEIVE_ERROR:
        info += "Receive Failed";
        Serial.println("Receive Failed");
        break;

      case OTA_END_ERROR:
        info += "End Failed";
        Serial.println("End Failed");
        break;
    }

    display.clear();
    display.drawString(0, 0, info);
    ESP.restart();
  });

  ArduinoOTA.begin();
}

/*********************************************************************
 * setup wifi
 */
void setupWIFI()
{
  display.clear();
  display.drawString(0, 0, "Connecting...");
  display.drawString(0, 10, String(ssid));
  display.display();


  //Connect to WiFi, delete old configuration, turn off WiFi, prepare to reconfigure

  WiFi.disconnect(true);
  delay(1000);

  WiFi.mode(WIFI_STA);
  //WiFi.onEvent(WiFiEvent);
  WiFi.setAutoReconnect(true);

  WiFi.setAutoReconnect(true);    //Automatically reconnect after disconnecting WiFi ESP32 is not available

  //WiFi.setHostname(HOSTNAME);
  WiFi.begin(ssid, password);
#if USE_STATIC_IP
  WiFi.config(staticIP, gateway, subnet);
#endif


  //Wait for 5000ms, if not connected, continue


  byte count = 0;
  while(WiFi.status() != WL_CONNECTED && count < 10)
  {
    count ++;
    delay(500);
    Serial.print(".");
  }

  display.clear();
  if(WiFi.status() == WL_CONNECTED)
    display.drawString(0, 0, "Connected");
  else
    display.drawString(0, 0, "Connect False");
  display.display();
  
}

void setup_lcd() {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop_lcd() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}

/******************************************************
 * arduino setup
 */
void setup()
{
  pinMode(LASER_CONTROL_PIN, OUTPUT);
  setup_lcd();
  display.init();
  display.clear();
  display.display();
  
  display.setContrast(255);
   Serial.begin(115200);
  // pinMode(36, OUTPUT);
  // digitalWrite(36,HIGH);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Initialize...");

  setupWIFI();
  setupOTA();
  
}

/******************************************************
 * arduino loop
 */
void loop()
{
  ArduinoOTA.handle();
  unsigned long ms = millis();
  if(ms % 1000 == 0)
  {
    Serial.println("helloï¼ŒOTA now");

    laser_state = !laser_state;
    if (laser_state) {
      digitalWrite(LASER_CONTROL_PIN, HIGH); // turn the laser on
    } else {
      digitalWrite(LASER_CONTROL_PIN, LOW); // turn the laser off
    }
  }
  loop_lcd();
}

/****************************************************

 * [Universal Function] ESP32 WiFi Kit 32 Event Handling

 */
void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
switch (event) {
        case ARDUINO_EVENT_WIFI_READY: 
            Serial.println("WiFi interface ready");
            break;
        case ARDUINO_EVENT_WIFI_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case ARDUINO_EVENT_WIFI_STA_START:
            Serial.println("WiFi client started");
            break;
        case ARDUINO_EVENT_WIFI_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case ARDUINO_EVENT_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case ARDUINO_EVENT_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("WiFi access point started");
            break;
        case ARDUINO_EVENT_WIFI_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case ARDUINO_EVENT_WIFI_AP_GOT_IP6:
            Serial.println("AP IPv6 is preferred");
            break;
        case ARDUINO_EVENT_WIFI_STA_GOT_IP6:
            Serial.println("STA IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP6:
            Serial.println("Ethernet IPv6 is preferred");
            break;
        case ARDUINO_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case ARDUINO_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
        default: break;
    }}
