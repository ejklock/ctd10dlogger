//
// Created by klock on 24/09/16.
//

#ifndef AMDECAGONDLOGGER_AMDECAGONDLOGGER_H
#define AMDECAGONDLOGGER_AMDECAGONDLOGGER_H
#include "Arduino.h"
#include "SDISerial.h"
#include "SD.h"
#include "SPI.h"
#include "Wire.h"
#include "RTClib.h"
#include  "avr/wdt.h" 

void saveOnSd(char output_buffer[], char nomearquivo[], char dados[1][20]);
void getSensorData();
void initSensor();
void checkHour();
#endif //AMDECAGONDLOGGER_AMDECAGONDLOGGER_H

