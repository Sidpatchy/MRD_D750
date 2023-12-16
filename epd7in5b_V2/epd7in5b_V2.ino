/**
 * @file epd7in5b_V2.ino
 * @brief Main file for testing the display.
 * @version 1.0
 * @date 2023-10-03
 * 
 * @author Sidpatchy
 */

#include "epd7in5b_V2.h"
#include "ImageData.h"
#include <SPI.h>
#include "config.h"

void setup() {
  Serial.begin(9600);

  pinMode(RST_Pin, OUTPUT);
  pinMode(DC_Pin, OUTPUT);
  pinMode(CS_Pin, OUTPUT);
  pinMode(BUSY_Pin, INPUT);

  // Initialize the SPI bus with custom pins
  SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
  SPI.begin(CLK_Pin, MISO_Pin, DIN_Pin, CS_Pin);

  // Clear the screen with white
  Serial.println("Clear display white...");
  //EPD7IN5B_V2::init180();
  //EPD7IN5B_V2::clearWhite();
  //EPD7IN5B_V2::enterDeepSleep();
  //delay(5000);

  // Clear the screen with black
  Serial.println("Clear display black...");
  //EPD7IN5B_V2::init180();
  //EPD7IN5B_V2::clearBlack();
  //EPD7IN5B_V2::enterDeepSleep();
  //delay(5000);

  // Clear the screen with red
  Serial.println("Clear display red...");
  //EPD7IN5B_V2::init180();
  //EPD7IN5B_V2::clearRed();
  //EPD7IN5B_V2::enterDeepSleep();
  //delay(5000);

  // Show 3 colour block test
  //Serial.println("Show 3 block test...");
  //EPD7IN5B_V2::show3Block();
  //delay(10000);

  // Show image
  Serial.println("Show test image...");
  EPD7IN5B_V2::init180();
  EPD7IN5B_V2::display(gImage_7in5_V2_b, gImage_7in5_V2_ry);
  EPD7IN5B_V2::enterDeepSleep();
  delay(5000);

  // Enter sleep mode
  esp_deep_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:

}
