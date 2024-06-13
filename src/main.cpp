/*
Board: LilyGo-T5 V2.3.1_2.13 Inch E-Paper
DriverChip:   GDEW0213T5
  - DEPG0213BN  : greylevel=2
  - GDEM0213B74 : greylevel=4
Product page: https://www.aliexpress.com/item/1005003063164032.html
Github: https://github.com/Xinyuan-LilyGO/LilyGo-T5-Epaper-Series/
Example:
  - Hello World: https://github.com/Xinyuan-LilyGO/LilyGo-T5-Epaper-Series/blob/master/examples/GxEPD_Hello_world/GxEPD_Hello_world.ino
*/

#include <Arduino.h>
#include <Ticker.h>
#include <SPI.h>
#include <SD.h>
#include "lilygo-t5_v23.h"
#include "FertigationOutput.h"

GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

SPIClass  SDSPI(VSPI);

#define NUM_OUTPUS 3
#define MOTOR_PIN 12
#define LED_BLINK_PIN 0
const uint8_t arOutputsPin[NUM_OUTPUS] = {27, 26, 25};

// ini adalah durasi output dalam milisecond
uint16_t arOutputOnDuration[NUM_OUTPUS] = {1000, 1000, 1000};

Task fertigationOutput;
Ticker ticker;
Ticker ledBlinkOff;

void onTimer() {
  Serial.println("onTimer");
  digitalWrite(LED_BLINK_PIN, HIGH);
  
  ledBlinkOff.once_ms(100, []() { // turn off led after 100ms
    digitalWrite(LED_BLINK_PIN, LOW);
  });
  fertigationOutput.start();
}

String message;

void setup() {
    pinMode(MOTOR_PIN, OUTPUT);
    pinMode(LED_BLINK_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, LOW);
    digitalWrite(LED_BLINK_PIN, LOW);
    delay(100);
    Serial.begin(115200);

    SDSPI.begin(TFCARD_SCLK, TFCARD_MISO, TFCARD_MOSI, TFCARD_CS);

    if (!SD.begin(TFCARD_CS, SDSPI)) {
        Serial.println("SD Card FAIL");
        message = "SD NULL";
    } else {
        Serial.println("SD Card PASS");
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        message = "SDCard:" + String(cardSize) + "MB";
        Serial.println(message);
    }  

    for (uint8_t i = 0; i < NUM_OUTPUS; i++) {
      fertigationOutput.addSolenoidOutput(arOutputsPin[i], arOutputOnDuration[i]);
    }
    // this will turn on motor pump after 200ms and turn off after 500ms
    fertigationOutput.setMotorPumpPin(MOTOR_PIN, 200, 500);
    ticker.attach(5, onTimer); // every 5s
    fertigationOutput.begin();

    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(); // enable diagnostic output on Serial
    display.setRotation(1);
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&FreeMonoBold9pt7b);
    display.setCursor(0, 45);
    display.println("Hello World");
    display.update();

    Serial.println("System running...");

}

void loop() {
  fertigationOutput.update();
}

