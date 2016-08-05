//-----------------------------------------------------------------------------
// Girino.ino
//-----------------------------------------------------------------------------
// Copyright 2012 Cristiano Lino Fontana
//
// This file is part of Girino.
//
//	Girino is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	Girino is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with Girino.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Girino.h"

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

volatile uint16_t stopIndex;
volatile uint16_t ADCCounter;
volatile  uint8_t ADCBuffer[ADCBUFFERSIZE];

uint8_t prescaler;
uint8_t skipModulo;
uint8_t triggerType;
uint8_t threshold;
uint8_t voltageReferenceType;

const String eof = "EOF\n"; //message delimiter

//-----------------------------------------------------------------------------
// Main routines
//-----------------------------------------------------------------------------
//
// The setup function initializes registers.
//
void setup (void) {		// Setup of the microcontroller
  Serial.begin(BAUDRATE);
  
  ADCCounter = 0;
  stopIndex = -1;
  prescaler = 128;
  skipModulo = 0;
  triggerType = 3;
  threshold = 25;
  voltageReferenceType = 1;

  // Activate interrupts
  sei();

  initPins();
  initADC();
  initAnalogComparator();
  
  pinMode( thresholdPin, OUTPUT );
  analogWrite( thresholdPin, threshold );

  Serial.print("READY");
  Serial.print(eof);

  Serial.print("{ buffer: ");
  Serial.print(ADCBUFFERSIZE);
  Serial.print(", baud: ");
  Serial.print(BAUDRATE);
  Serial.print("}");
  Serial.print(eof);
  printStatus();
}


void serialEventRun(void) {
  String inputString = Serial.readString();  
  if (inputString != ""){
    commandRecieved(inputString);
  }
}
void commandRecieved(String cmd){
  char chars[6];
  uint8_t cmdVal;
  switch (cmd.charAt(0)) {
      case 's':      // start ADC conversions
        startADC();
        startAnalogComparator();
        break;

      case 'S':     // Stop ADC conversions
        stopAnalogComparator();
        stopADC();
        break;

      case 'k':     // skip setting, for slower samples
      case 'K':
        cmd.substring(1).toCharArray(chars, sizeof(chars));
        cmdVal = atoi( chars );
        skipModulo = cmdVal;
        printStatus();
        break;
        
      case 'p':     // prescaler setting
      case 'P':
        cmd.substring(1).toCharArray(chars, sizeof(chars));
        cmdVal = atoi( chars );
        prescaler = cmdVal;
        setADCPrescaler(cmdVal);
        printStatus();
        break;

      case 'r':  
      case 'R':
        cmd.substring(1).toCharArray(chars, sizeof(chars));
        cmdVal = atoi( chars );
        voltageReferenceType = cmdVal;
        setVoltageReference(cmdVal);
        printStatus();
        break;

      case 'e':     // trigger event setting
      case 'E':
        cmd.substring(1).toCharArray(chars, sizeof(chars));
        cmdVal = atoi( chars );
        triggerType = cmdVal;
        setTriggerEvent(cmdVal);
        printStatus();
        break;

      case 't':     // trigger threshold setting
      case 'T':
        cmd.substring(1).toCharArray(chars, sizeof(chars));
        cmdVal = atoi( chars );
        threshold = cmdVal;
        analogWrite( thresholdPin, cmdVal);
        printStatus();
        break;

      case 'd':     // 'd' for display status
      case 'D':
        printStatus();
        break;

      default:
        // Display error message
        Serial.print("ERROR: ");
        Serial.print(cmd);
        Serial.print(eof);
    }
}
  
void printStatus( void )
{
  Serial.print("{ prescaler: ");
  Serial.print(prescaler);
  Serial.print(", skipModulo: ");
  Serial.print(skipModulo);
  Serial.print(", triggerType: ");
  Serial.print(triggerType);
  Serial.print(", threshold: ");
  Serial.print(threshold);
  Serial.print(", voltageReferenceType: ");
  Serial.print(voltageReferenceType);
  Serial.print("}");
  Serial.print(eof);
}


void loop (void) {

}
