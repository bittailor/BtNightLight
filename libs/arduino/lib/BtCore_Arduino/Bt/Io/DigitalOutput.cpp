//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//  
//-------------------------------------------------------------------------------------------------
//
//  Bt::Io::DigitalOutput
//
//*************************************************************************************************

#include "Bt/Io/DigitalOutput.hpp"

#include <avr/interrupt.h>

#include "Arduino.h"

namespace Bt {
namespace Io {


//-------------------------------------------------------------------------------------------------

DigitalOutput::DigitalOutput(uint8_t iPin)
: mPin(iPin)
, mMirror(false)
{

   pinMode(iPin,OUTPUT);
}

//-------------------------------------------------------------------------------------------------

DigitalOutput::~DigitalOutput()
{

}

//-------------------------------------------------------------------------------------------------

void DigitalOutput::set(bool iValue)
{
   mMirror = iValue;
   digitalWrite(mPin, iValue ? HIGH : LOW);
}

//-------------------------------------------------------------------------------------------------

} // namespace Io
} // namespace Bt