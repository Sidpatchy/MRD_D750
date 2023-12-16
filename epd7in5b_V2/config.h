/**
 * @file config.h
 * @brief Configuration section
 * @version 1.0
 * @date 2023-10-11
 * 
 * @author Sidpatchy
 */

#pragma once

// Specify custom SPI pins
constexpr int CLK_Pin = 18;    // Clock
constexpr int MISO_Pin = -1;   // MISO (not used here, hence -1)
constexpr int DIN_Pin = 23;    // MOSI
constexpr int RST_Pin = 22;    // Reset
constexpr int DC_Pin = 21;     // DC
constexpr int CS_Pin = 5;      // CS/SS
constexpr int BUSY_Pin = 25;   // BUSY
