//Fishinator 2020 - Remote Sense Cluster

//This microcontroller code should be paired with the sister-code, 
//running on node-red on the other side of the bluetooth connection
//controlling the Fishinator 2020.

//TFT code based on sample code Written by Limor Fried/Ladyada for Adafruit Industries.
//(redistribution text is found at the bottom of file for my clarity)

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <stdio.h>

#include "Ultrasonic.h"  //hc-sr04 ultrasonic module library

#define myTrigger (5)
#define myEcho  (4)

Ultrasonic myPing(myTrigger, myEcho);

// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8

// For 1.3", 1.54", and 2.0" TFT with ST7789:
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

//thermistor inputs (analog input pins)
#define TMPSNS1 A5  //ambient
#define TMPSNS2 A4  //tank
#define TMPSNS3 A3 //growspace

//10k combined with 1k-300k cds light sensor
#define LIGHTSNS1 A0  //ambient (above tank below growspace)
#define LIGHTSNS2 A1  //growspace  (might want to change this to a 1k for better high-light sense)

//pins to watch system voltages (from resistive bridge to power 3.3v/5v)
#define VOLTWATCH33 A6  //1k pulled high, 10k pulled low.  sample 3.3v(10/11) voltage
#define VOLTWATCH5  A7  //30k from 5v to gnd, tap at 20k for 5v*(2/3)=3.3v

#define UNDEFINEDANALOGPIN  A2  //as yet unused

#define MAXBLITDIM (127)
boolean blit[MAXBLITDIM * MAXBLITDIM * 2];

//lasttime for detecting the period for 1 cycle of loop()
uint16_t lasttime = 0;

//clear TFT by default, but any 8 bit value OK
void clear_blit(uint8_t num, uint8_t value = false) {
  //tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(3);
  drawtext("starting...", ST77XX_WHITE);
  delay(1500);

  switch (num) {
    //clear first bank
    case 0: //memset(blit,value, MAXBLITDIM*MAXBLITDIM);
      break;
    //clear second bank
    case 1: memset(blit + MAXBLITDIM * MAXBLITDIM, value, MAXBLITDIM * MAXBLITDIM);
      break;
    default: break; //do nothing
  }//*/
}


void setup(void) {
  pinMode(0, INPUT);
  pinMode(1, OUTPUT); //which are the rx/tx pins
  lasttime = millis();
  
  Serial.begin(9600);  while (!Serial) {}    ;  //wait for it
  
  //setup bluetooth
  Serial.println(F("AT+DEFAULT")); //soft reset
  delay(500);
  /*
    Serial.println("AT+PIN4444");
    delay(500);
    Serial.println("AT+BAUD3");
    delay(500);
    Serial.println("AT+NAMEfishinator");
    delay(500);
    Serial.println("AT+START");*/

  //use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240);           // Init ST7789 240x240

  clear_blit(false);
  tft.fillScreen(ST77XX_BLACK);

  pinMode(TMPSNS1, INPUT );
  pinMode(TMPSNS2, INPUT );
  pinMode(TMPSNS3, INPUT );
  pinMode(LIGHTSNS1, INPUT );
  //pinMode(LIGHTSNS2, INPUT );
  pinMode(VOLTWATCH33, INPUT );
  pinMode(VOLTWATCH5, INPUT );

  drawtext("Ultrasonic/Graphics", ST77XX_WHITE);
  delay(750);
}

void drawtext(char *text, uint16_t color) {
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

/* THE FOLLOWING TWO FUNCTIONS ARE THE SAME FUNCTION!!!! */
//read the 1k - 300k CdS in series with 10k fixed resistor
//using the 3.3v supply as source.
//returns: voltage in millivolts
float readLightBridgeVoltageMV(int senseNum) { return 3300.0 * analogRead(senseNum) / 1024.0; }

//read the 10k Thermistor voltage, half bridge with 10k fixed resistor to 3.3v
float readThermistorBridgeVoltageMV(int senseNum) { return 3300.0 * analogRead(senseNum) / 1024.0; }




void loop() {
  char report[80]; //re-useable string - bigger than needed for now - watch for buffer overflows
  tft.fillScreen(ST77XX_BLACK);  //clear the TFT
  unsigned int millivolts = 0;

  uint16_t nowtime = millis();
  uint16_t looptime = nowtime - lasttime; 
  sprintf(report, "SenseClusterXLoopTime,%u,ms\r\n", looptime  );
  Serial.print(report);  //send out  bluetooth serial port

  //Report Local Ultrasonic Distance Sensor
  //long durat = myPing.Timing();//get a new ultrasonic reading, in milliseconds
  //float fdist_cm = durat / 58.3; //58.3 ms/cm is 1/speedofsound * 2 traversals (1 each way)
  float fdist_cm = myPing.read(CM);

  tft.setCursor(0, 125);
  // out-of-range values bad
  if (fdist_cm < 400) {
    //there's some legacy cruft here, but whatever, just leave it.
    unsigned int decimetres = 10 * fdist_cm;
    sprintf(report, "WaterDistance,%u.%1u,cm\r\n", decimetres / 10, decimetres - 10 * (decimetres / 10) );
    tft.print(report);
    Serial.print(report);
  }
  else
  {
    Serial.print("WaterDistance,OutOfRange,cm\r\n");
    tft.print("WaterDistance,OutOfRange,cm\r\n");
  }
  tft.setCursor(0, 25);
  tft.print(fdist_cm); tft.print(F(" cm   "));


  //Report Local Thermistor Temperature Voltages
  
  //thermistor voltage, 10k@25C thermistor in series with 10k fixed resistor
  //regulated 3.3volts source voltage

  millivolts = readThermistorBridgeVoltageMV(TMPSNS1);
  sprintf(report, "Tvolts,%u.%.3u,v\r\n", millivolts / 1000, millivolts - 1000 * (millivolts / 1000)  );
  Serial.print(report);  //send out  bluetooth serial port
  
  tft.setCursor(0, 50);
  tft.print(millivolts/1000.0); tft.print(F(" volts "));
  tft.setCursor(0, 100);
  tft.print(report); //send to screen

  //this is the SECOND thermistor temperature
  millivolts = readThermistorBridgeVoltageMV(TMPSNS2);
  sprintf(report, "T2volts,%u.%.3u,v\r\n", millivolts / 1000, millivolts - 1000 * (millivolts / 1000)  );
  Serial.print(report);  //send out  bluetooth serial port
  
  //this is the THIRD thermistor temperature
  millivolts = readThermistorBridgeVoltageMV(TMPSNS3);
  sprintf(report, "T3volts,%u.%.3u,v\r\n", millivolts / 1000, millivolts - 1000 * (millivolts / 1000)  );
  Serial.print(report);  //send out  bluetooth serial port



  //Report Local Light Sensors (1k - 300k CdS in series with 10k fixed resistor)
  millivolts = readLightBridgeVoltageMV(LIGHTSNS1);
  sprintf(report, "Lvolts,%u.%.3u,v\r\n", millivolts / 1000, millivolts - 1000 * (millivolts / 1000)  );
  
  tft.setCursor(0, 150);
  tft.print(report); //send to screen   //just report this one for now.
  Serial.print(report);  //send out  bluetooth serial port

  //second  light sensor -
  millivolts = readLightBridgeVoltageMV(LIGHTSNS2);
  sprintf(report, "L2volts,%u.%.3u,v\r\n", millivolts / 1000, millivolts - 1000 * (millivolts / 1000)  );
  Serial.print(report);  //send out  bluetooth serial port


  //Report Local Power Supply/Battery/Bus Voltages
  
  //regulated 3.3 volts
  float vcc33 = 11 * 3.3 * analogRead(VOLTWATCH33) / 1024 / 10; //compensated for 10/11 resistor bridge
  millivolts = vcc33 * 1000;
  sprintf(report, "VCCvolts,%u.%.3u,v\r\n", millivolts / 1000, millivolts - 1000 * (millivolts / 1000)  );
  tft.setCursor(0, 175);
  tft.print(report); //send to screen
  Serial.print(report);  //send out  bluetooth serial port

  //USB (5v) bus voltage
  float vcc5 = 3 * 3.3 * analogRead(VOLTWATCH5) / 1024 / 2; //compensated for 2/3 resistor bridge
  millivolts = vcc5 * 1000;
  sprintf(report, "BUSvolts,%u.%.3u,v\r\n", millivolts / 1000, millivolts - 1000 * (millivolts / 1000)  );
  tft.setCursor(0, 200);
  tft.print(report); //send to screen
  Serial.print(report);  //send out  bluetooth serial port
}


//** use the stuff below here as an example, but it does nothing in this project
/*
void tftSecondsCount() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
  tft.setTextSize(0);
}

void printRandomBoxOnTFT() 
{
    //this continuously prints a randomly coloured box, on pixel per loop
    tft.setCursor(0, 0);
    if (92 > random(100))
      tft.drawPixel((56 + (int16_t) random(MAXBLITDIM)), (56 + (int16_t) random(MAXBLITDIM)), 0); //turn off pixel
    else
      tft.drawPixel((56 + (int16_t) random(MAXBLITDIM)), (56 + (int16_t) random(MAXBLITDIM)), (uint16_t) random(65538));
    delay(0);
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(3);
  tft.println("Hello World!");
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(4);
  tft.print(1234.567);
  delay(1500);
  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_GREEN);
  tft.print(p, 6);
  tft.println(" Want pi?");
  tft.println(" ");
  tft.print(8675309, HEX); // print 8,675,309 out in HEX!
  tft.println(" Print HEX!");
  tft.println(" ");

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(3);
  tft.println("Sketch has been");
  tft.println("running for: ");
  tft.setTextColor(ST77XX_MAGENTA);
  tft.print(millis() / 1000);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(" seconds.");
  tft.setTextSize(0);
}*/

/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  The 1.3" TFT breakout
  ----> https://www.adafruit.com/product/4313
  The 1.54" TFT breakout
    ----> https://www.adafruit.com/product/3787
  The 2.0" TFT breakout
    ----> https://www.adafruit.com/product/4311
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/
