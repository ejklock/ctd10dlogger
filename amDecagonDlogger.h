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

#define DATALINE_PIN 9
#define CS 10
#define INVERTED 1

void saveOnSd(char output_buffer[], char nomearquivo[], char dados[1][20]);
void getSensorData();
void initSensor();
#endif //AMDECAGONDLOGGER_AMDECAGONDLOGGER_H
