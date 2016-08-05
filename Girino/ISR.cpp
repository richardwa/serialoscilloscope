//-----------------------------------------------------------------------------
// ISR.cpp
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

void sendData() {
  // Send buffer through serial port in the right order
  Serial.print("DATA");
  Serial.write( (uint8_t *)ADCBuffer + ADCCounter, ADCBUFFERSIZE - ADCCounter );
  Serial.write( (uint8_t *)ADCBuffer, ADCCounter );
  Serial.print(eof);
}

//-----------------------------------------------------------------------------
// ADC Conversion Complete Interrupt
//-----------------------------------------------------------------------------
uint8_t cycleCount;
ISR(ADC_vect)
{
    ADCBuffer[ADCCounter] = ADCH;
    ADCCounter = ( ADCCounter + 1 ) % ADCBUFFERSIZE;
    if ( stopIndex == ADCCounter )
    {
      // Disable ADC 
      cbi( ADCSRA, ADEN );
      sendData();
      stopIndex = -1;
    }
  
}

//-----------------------------------------------------------------------------
// Analog Comparator interrupt
//-----------------------------------------------------------------------------
ISR(ANALOG_COMP_vect)
{
  // Disable Analog Comparator interrupt
  cbi( ACSR, ACIE );
  //set stop index almost 1 buffer size ahead
  stopIndex = ( ADCCounter + ADCBUFFERSIZE - 1) % ADCBUFFERSIZE;
}



