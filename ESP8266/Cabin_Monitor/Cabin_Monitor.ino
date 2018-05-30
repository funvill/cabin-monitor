/**
 * Created by: Steven Smethurst 
 * Created on: May 29, 2018 
 * Version: 
 * 
 */

#include <Esp.h>
#include <SPI.h>
#include <SD.h>
#include <WEMOS_SHT3X.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

 static const unsigned char VERSION_MAJOR = 0;
 static const unsigned char VERSION_MINOR = 0;
 static const unsigned char VERSION_PATCH = 1;
 
 static const unsigned int SETTING_SERIAL_BAUD = 9600;
 static const unsigned int SETTING_SLEEP_CYCLE = 3000; // In MS 
 static const bool SETTING_ENABLE_SD_CARD = false; 
 #define SETTING_DATA_LOG "datalog.txt"

// PINS 
 static const unsigned int PIN_SD_CARD_CS = D8;

// Globals
SHT3X sht30(0x45);

#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(SETTING_SERIAL_BAUD);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Print version infomation 
  Serial.print("Cabin Monitor v");
  Serial.print(VERSION_MAJOR);
  Serial.print(".");
  Serial.print(VERSION_MINOR);
  Serial.print(".");
  Serial.println(VERSION_PATCH);
  Serial.println("https://github.com/funvill/cabin-monitor");
  Serial.println(""); 

if( SETTING_ENABLE_SD_CARD) {
  // see if the card is present and can be initialized:
  Serial.println("FYI: Attempting to initialize SD Card.");
  if (!SD.begin(PIN_SD_CARD_CS)) {
    Serial.println("ERROR: Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("FYI: SD Card initialized.");
}

  Serial.println("FYI: Attempting to initialize Dispay");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Print Settings 
  Serial.print ("FYI: data file: ");
  Serial.println( SETTING_DATA_LOG );
  Serial.print ("FYI: Sleep Cycle: ");
  Serial.println( SETTING_SLEEP_CYCLE );

  Serial.println(); 
  Serial.print("FYI: Starting...\n");
}

void loop() {

  if(sht30.get()==0){
    Serial.print("Temperature in Celsius : ");
    Serial.println(sht30.cTemp);
    Serial.print("Relative Humidity : ");
    Serial.println(sht30.humidity);
    Serial.println();



    // Clear the buffer.
    display.clearDisplay();
    display.setTextSize(0);
    display.setCursor(0, 0);
    display.setTextColor(WHITE);

    display.println("T: ");
    display.setTextSize(0);
    display.println(sht30.cTemp);

    display.setCursor(0, 15);
    display.setTextSize(0);
    display.println("H: ");
    display.setTextSize(0);
    display.println(sht30.humidity);

    display.display();





    if(SETTING_ENABLE_SD_CARD) {
        // open the file. note that only one file can be open at a time,
      // so you have to close this one before opening another.
      File fileDataLog = SD.open(SETTING_DATA_LOG, FILE_WRITE);
      if (! fileDataLog) {
      // if the file isn't open, pop up an error:
        Serial.println("ERROR: Could not open data file.");
        return ; 
      }  

        fileDataLog.print("Celsius: ");   
        fileDataLog.print(sht30.cTemp);   
        fileDataLog.print(", Relative Humidity: ");   
        fileDataLog.println(sht30.humidity);   
        fileDataLog.close();
    }
    
  }



 // convert to microseconds
  ESP.deepSleep(SETTING_SLEEP_CYCLE * 1000);

}
