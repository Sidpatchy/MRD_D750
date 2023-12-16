/**
 * @file epd7in5b_V2.cpp
 * @brief 7.5inch EPD driver
 * @version 1.0
 * @date 2023-10-03
 * 
 * @author Sidpatchy
 */

#include "epd7in5b_V2.h"
#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include "config.h"

/**
 * @brief Perform a software reset on the e-Paper.
 */
static void resetEPaper() {
    digitalWrite(RST_Pin, HIGH);
    delay(200);
    digitalWrite(RST_Pin, LOW);
    delay(4);
    digitalWrite(RST_Pin, HIGH);
    delay(200);
}

/**
 * @brief Send a command to the e-Paper.
 * 
 * @param command Command to be sent.
 */
static void sendCommand(uint8_t command) {
    digitalWrite(DC_Pin, LOW);
    digitalWrite(CS_Pin, LOW);
    SPI.transfer(command);
    digitalWrite(CS_Pin, HIGH);
}

/**
 * @brief Send data to the e-Paper.
 * 
 * @param data Data to be sent.
 */
static void sendData(uint8_t data) {
    digitalWrite(DC_Pin, HIGH);
    digitalWrite(CS_Pin, LOW);
    SPI.transfer(data);
    digitalWrite(CS_Pin, HIGH);
}

/**
 * @brief Wait until the e-Paper is no longer busy (High).
 */
static void waitForNotBusyHigh() {
    int count = 0;
    while (!digitalRead(BUSY_Pin)) {
        delay(10);
        count++;
        if (count > 5000) {
            return;
        }
    }
}

/**
 * @brief Wait until the e-Paper is no longer busy (Low).
 */
static void waitForNotBusyLow() {
    while (digitalRead(BUSY_Pin)) {
        delay(5);
    }
}

/**
 * @brief Turn on the e-Paper display.
 */
static void turnOnDisplay() {
    sendCommand(0x12);  // DISPLAY_REFRESH
    //sendData(0x00);
    delay(1);
    waitForNotBusyHigh();
}

/**
 * @brief Initialize the e-Paper register.
 */
void EPD7IN5B_V2::init() {
    resetEPaper();
    waitForNotBusyHigh();

    // Hereafter, configuration commands can be sent to initialize the e-Paper.
    sendCommand(0x01);      //POWER SETTING
    sendData (0x07);
    sendData (0x07);        //VGH=20V,VGL=-20V
    sendData (0x3f);        //VDH=15V
    sendData (0x3f);        //VDL=-15V

    
    //Enhanced display drive(Add 0x06 command)
    //sendCommand(0x06);      //Booster Soft Start 
    //sendData (0x17);
    //sendData (0x17);   
    //sendData (0x28);   
    //sendData (0x17);   
  
    sendCommand(0x04);      //POWER ON
    waitForNotBusyHigh();        //waiting for the electronic paper IC to release the idle signal
  
    sendCommand(0X00);      //PANNEL SETTING
    sendData(0x0F);         //KW-3f   KWR-2F BWROTP 0f BWOTP 1f

    sendCommand(0x61);     //resolution setting
    sendData (display_width/256); 
    sendData (display_width%256);  
    sendData (display_height/256);
    sendData (display_height%256); 

    sendCommand(0X15);   
    sendData(0x00);    

    sendCommand(0X50);     //VCOM AND DATA INTERVAL SETTING
    sendData(0x11);
    sendData(0x07);

    sendCommand(0X60);     //TCON SETTING
    sendData(0x22);
    waitForNotBusyHigh();
}

/**
 * @brief Initialize the e-Paper register.
 */
void EPD7IN5B_V2::init180() {
    resetEPaper();
    waitForNotBusyHigh();

    // Hereafter, configuration commands can be sent to initialize the e-Paper.
    sendCommand(0x01);			//POWER SETTING
    sendData(0x07);
    sendData(0x07);    //VGH=20V,VGL=-20V
    sendData(0x3f);		//VDH=15V
    sendData(0x3f);		//VDL=-15V

    sendCommand(0x04); //POWER ON
    delay(100);
    waitForNotBusyHigh();

    sendCommand(0X00);			//PANNEL SETTING
    sendData(0x0F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    sendCommand(0x61);        	//tres
    sendData(0x03);		//source 800
    sendData(0x20);
    sendData(0x01);		//gate 480
    sendData(0xE0);

    sendCommand(0X15);
    sendData(0x00);

    sendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    sendData(0x11);
    sendData(0x07);

    sendCommand(0X60);			//TCON SETTING
    sendData(0x22);

    sendCommand(0x65);  // Resolution setting
    sendData(0x00);
    sendData(0x00);//800*480
    sendData(0x00);
    sendData(0x00);
    waitForNotBusyHigh();
}

/**
 * @brief Clear the e-Paper screen to white.
 */
void EPD7IN5B_V2::clearWhite() {
    uint16_t width =(display_width % 8 == 0)?(display_width / 8 ):(display_width / 8 + 1);

    sendCommand(0x10);    // Send black/white data
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }

    sendCommand(0x13);    // Send red/white data
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0x00);
        }
    }

    turnOnDisplay();
}

/**
 * @brief Clear the e-Paper screen to black.
 */
void EPD7IN5B_V2::clearBlack() {
    uint16_t width =(display_width % 8 == 0)?(display_width / 8 ):(display_width / 8 + 1);

    sendCommand(0x10);    // Send black/white data
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0x00);
        }
    }

    sendCommand(0x13);    // Send red/white data
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0x00);
        }
    }

    turnOnDisplay();
}

/**
 * @brief Clear the e-Paper screen to red.
 */
void EPD7IN5B_V2::clearRed() {
    uint16_t width =(display_width % 8 == 0)?(display_width / 8 ):(display_width / 8 + 1);

    sendCommand(0x10);    // Send black/white data
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }

    sendCommand(0x13);    // Send red/white data
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < width; i++) {
            sendData(0xFF);
        }
    }

    turnOnDisplay();
}

/**
 * @brief Display a colour block pattern on the e-Paper.
 */
/*void EPD7IN5B_V2::show3Block() {
    const unsigned long segmentWidth = 100;
    const unsigned long segmentHeight = 240;

    Colour const colors[3] = {
        Colour::WHITE,
        Colour::BLACK,
        Colour::RED
    };

    sendCommand(0x10);
    for (unsigned long segment = 0; segment < 2; ++segment) {
        for (unsigned long i = 0; i < segmentHeight; i++) {
            for (unsigned long k = segment * 4; k < (segment + 1) * 4; k++) {
                for (unsigned long j = 0; j < segmentWidth; j++) {
                    uint8_t colorValue = static_cast<uint8_t>(colors[k]);
                    sendData((colorValue << 4) | colorValue);
                }
            }
        }
    }

    turnOnDisplay();
}*/

/**
 * @brief Display an image on the e-Paper.
 * 
 * @param image Image data to be displayed.
 */
void EPD7IN5B_V2::display(const uint8_t *blackimage, const uint8_t *ryimage) {
    uint16_t Width =(display_width % 8 == 0)?(display_width / 8 ):(display_width / 8 + 1);

    sendCommand(0x10); //black
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            sendData(blackimage[i + j * Width]);
        }
    }

    sendCommand(0x13); //red
    for (uint16_t j = 0; j < display_height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            sendData(~ryimage[i + j * Width]);
        }
    }
    turnOnDisplay();
}

/**
 * @brief Enter deep sleep mode for the e-Paper.
 */
void EPD7IN5B_V2::enterDeepSleep() {
    sendCommand(0x02);    // power off
    waitForNotBusyHigh(); // wait for display to not be busy
    sendCommand(0x07);    // deep sleep
    sendData(0xA5);
}
