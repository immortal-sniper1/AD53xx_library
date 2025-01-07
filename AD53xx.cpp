#include <AD53xx.h>
#include "Arduino.h"
#include <Wire.h>



//  CONSTRUCTOR

     AD53xx::AD53xx(uint8_t my_address, 
                    uint8_t channel_number,
                    uint8_t resolution, 
                    uint16_t Voltage_Reference,
                    TwoWire *wire) :

                    resolution(resolution), my_address(my_address),
                    channel_number(channel_number), Voltage_Reference(Voltage_Reference)
{
  //uint8_t address= 0x0C;
  //uint8_t channel_number=2;   // 1 2 4 channels 
  //uint8_t resolution=8;   // 8 10 12 bits
  //uint16_t Voltage_Reference= 2500 ; // in mV 
  _wire        = wire;
  Check_adress( my_address );
}
// (*_wire). is same as _wire->



bool AD53xx::Check_adress( uint8_t my_address )
{

if ( (my_address == 0x0C ) || (my_address ==0x0D ) )   //   12 or 13 decimal   
// 0x23  0x22 / 34 35 decimal for AD5338-1
{
  Serial.print("corect address");
  return true ;
}
else 
{
  if( (my_address == 0x23 ) || (my_address ==0x22 ) )
  {
    Serial.print("corect address IF it is AD5338-1");
    return true ;    
  }
  else 
  {
    Serial.print("WRONG ADDRESS");
    return false;
  }
}

}

//------------------------------------------------ WRITEING FUNCTIONS -----------------------------------------------------------------------------------

void AD53xx::DAC_Write_1ch(uint8_t my_address, uint16_t DAC_1_value)
{
/*
D is the decimal equivalent of the binary code, which is loaded
to the DAC register
0 to 255  for AD5301 (8 bits)
0 to 1023 for AD5311 (10 bits)
0 to 4095 for AD5321 (12 bits)


According to the datasheet the write process is
PD1 PD0 CLR LDAC D11 D10 D9 D8..... D0
*/



//uint8_t MSB_BYTE=0x80;   //bit 7 is no not care but set to 1 for ease of reading bytes
//uint8_t LSB_BYTE=0;
//uint8_t my_buffer=0;
POINTER_BYTE  =0xC0;
MSB_BYTE=0x0;
LSB_BYTE=0x0;





// bit 5 4 are for power down mode and defaul 00 is sormal operation mode


my_buffer = (DAC_1_value >> ( resolution -4 ))  & 0xF;  // extract first 4 bits and put them at the end of a buffer/carrier variable
MSB_BYTE = MSB_BYTE | my_buffer;

LSB_BYTE = DAC_1_value << 4 ; // copy the rest of the bits after you remove the first 4 , regardless of how many are left

//---------------------------------------------------------------------------------------------------------------------------------------------------

_wire->beginTransmission(my_address);  // Transmit to device number 44 (0x2C)
_wire->write(MSB_BYTE);                 // Sends value MSB_BYTE
_wire->write(LSB_BYTE);                 // Sends value LSB_BYTE   only for 2 & 4 channels
_wire->endTransmission();      // Stop transmitting

/*
Serial.print("MSB_BYTE: ");
Serial.println(MSB_BYTE, BIN);
Serial.print("LSB_BYTE: ");
Serial.println(LSB_BYTE, BIN);
*/
}




void AD53xx::DAC_Write_2ch(uint8_t my_address , bool CLR, bool LDAC, uint8_t DAC_number, uint16_t DAC_1_value)
{
/*
D is the decimal equivalent of the binary code, which is loaded
to the DAC register
0 to 255  for AD5337 (8 bits)
0 to 1023 for AD5338 and AD5338-1 (10 bits)
0 to 4095 for AD5339 (12 bits)


According to the datasheet the write process is
PD1 PD0 CLR LDAC D11 D10 D9 D8..... D0
*/


//Serial.print("DAC_number: ");
//Serial.println(DAC_number);


if ( (DAC_number != 1) && (DAC_number != 2) )
{
  Serial.print("WRONG DAC NUMBER");
}

//uint8_t POINTER_BYTE=0;
//uint8_t MSB_BYTE=0;
//uint8_t LSB_BYTE=0;
//uint8_t my_buffer=0;

POINTER_BYTE  =0xC0;
MSB_BYTE=0x0;
LSB_BYTE=0x0;
//POINTER_BYTE |= 1<< 7;    //no not care bits but nice to see stuff at debug
//POINTER_BYTE |= 1<< 6;
POINTER_BYTE |= 1<< ( DAC_number -1);  // set bitmask for used DAC

//Serial.print("DAC_number: ");
//Serial.println(DAC_number);
// bit 7 6 are for power down mode and defaul 00 is sormal operation mode



// set CLR  
// 1 normal operations 
// 0 All DAC registers and input registers are filled with 0s on completion of the write sequence.
if ( CLR )
{
  MSB_BYTE |= 0x20 ; 
}



// set LDAC
// 0 The two DAC registers and, therefore, all DAC outputs, simultaneously updated on completion of the write sequence.
// 1 Addressed input register only is updated. There is no change in the contents of the DAC registers.
if ( LDAC )
{
  MSB_BYTE |= 0x10 ; 
}
 


my_buffer = (DAC_1_value >> ( resolution -4 ))  & 0xF;  // extract first 4 bits and put them at the end of a buffer/carrier variable
MSB_BYTE = MSB_BYTE | my_buffer;

LSB_BYTE = DAC_1_value << 4 ; // copy the rest of the bits after you remove the first 4 , regardless of how many are left

//---------------------------------------------------------------------------------------------------------------------------------------------------

_wire->beginTransmission(my_address);  // Transmit to device number 44 (0x2C)
_wire->write(POINTER_BYTE);             // Sends value POINTER_BYTE
_wire->write(MSB_BYTE);                 // Sends value MSB_BYTE
_wire->write(LSB_BYTE);                 // Sends value LSB_BYTE   only for 2 & 4 channels
_wire->endTransmission();      // Stop transmitting



Serial.print("POINTER_BYTE: ");
Serial.println(POINTER_BYTE, BIN);
Serial.print("MSB_BYTE: ");
Serial.println(MSB_BYTE, BIN);
Serial.print("LSB_BYTE: ");
Serial.println(LSB_BYTE, BIN);

}




void AD53xx::DAC_Write_4ch(uint8_t my_address ,bool gain, bool buff, bool CLR, bool PD, uint8_t DAC_number, uint16_t DAC_1_value)
{
/*
D is the decimal equivalent of the binary code, which is loaded
to the DAC register
0 to 255  for AD5305 (8 bits)
0 to 1023 for AD5315 (10 bits)
0 to 4095 for AD5325 (12 bits)

0 to 255  for AD5306 (8 bits)
0 to 1023 for AD5316 (10 bits)
0 to 4095 for AD5326 (12 bits)


According to the datasheet the write process is
PD1 PD0 CLR LDAC D11 D10 D9 D8..... D0
*/

if ( (DAC_number < 1) && (DAC_number > 4) )
{
  Serial.print("WRONG DAC NUMBER");
}


//uint8_t POINTER_BYTE=0;
//uint8_t MSB_BYTE=0;
//uint8_t LSB_BYTE=0;
//uint8_t my_buffer=0;
POINTER_BYTE  =0xC0;
MSB_BYTE=0x0;
LSB_BYTE=0x0;

POINTER_BYTE |= 1<< 7;    //no not care bits but nice to see stuff at debug
POINTER_BYTE |= 1<< 6;
POINTER_BYTE |= ( 1<<( DAC_number -1) ) ;  // set bitmask for used DAC


// bit 7 6 are for power down mode and defaul 00 is sormal operation mode






// set PD
// 0: On completion of the write sequence, all four DACs go into power-down mode. The DAC outputs enter a high impedance state
// 1: Normal operation.
if ( PD )
{
  MSB_BYTE |= 0x10 ; 
}


// set CLR  
// 1 normal operations 
// 0 All DAC registers and input registers are filled with 0s on completion of the write sequence.
if ( CLR )
{
  MSB_BYTE |= 0x20 ; 
}


// set buff
//0  Reference input for that DAC is unbuffered.
//1  Reference input for that DAC is buffered.
if ( buff )
{
  MSB_BYTE |= 0x40 ; 
}


// set gain  
//0  Output range for that DAC set at 0 V to VREF.
//1  Output range for that DAC set at 0 V to 2 VREF.
if ( gain )
{
  MSB_BYTE |= 0x80 ; 
}







my_buffer = (DAC_1_value >> ( resolution -4 ))  & 0xF;  // extract first 4 bits and put them at the end of a buffer/carrier variable
MSB_BYTE = MSB_BYTE | my_buffer;

LSB_BYTE = DAC_1_value << 4 ; // copy the rest of the bits after you remove the first 4 , regardless of how many are left

//---------------------------------------------------------------------------------------------------------------------------------------------------

_wire->beginTransmission(my_address);  // Transmit to device number 44 (0x2C)
_wire->write(POINTER_BYTE);             // Sends value POINTER_BYTE
_wire->write(MSB_BYTE);                 // Sends value MSB_BYTE
_wire->write(LSB_BYTE);                 // Sends value LSB_BYTE   only for 2 & 4 channels
_wire->endTransmission();      // Stop transmitting

/*
Serial.print("POINTER_BYTE: ");
Serial.println(POINTER_BYTE, BIN);
Serial.print("MSB_BYTE: ");
Serial.println(MSB_BYTE, BIN);
Serial.print("LSB_BYTE: ");
Serial.println(LSB_BYTE, BIN);
*/

}



//------------------------------------------------ READING FUNCTIONS -----------------------------------------------------------------------------------


uint16_t AD53xx::DAC_Read_2ch(uint8_t my_address, uint8_t DAC_number)
{
/*
D is the decimal equivalent of the binary code, which is loaded
to the DAC register
0 to 255  for AD5337 (8 bits)
0 to 1023 for AD5338 and AD5338-1 (10 bits)
0 to 4095 for AD5339 (12 bits)


According to the datasheet the write process is
PD1 PD0 CLR LDAC D11 D10 D9 D8..... D0
*/

if ( (DAC_number != 1) || (DAC_number != 2) )
{
  Serial.print("WRONG DAC NUMBER");
}

//uint8_t POINTER_BYTE=0;
//uint8_t MSB_BYTE=0;
//uint8_t LSB_BYTE=0;
//uint16_t my_buffer=0;

POINTER_BYTE |= 1<< 7;    //no not care bits but nice to see stuff at debug
POINTER_BYTE |= 1<< 6;
POINTER_BYTE |= ( 1<<( DAC_number -1) ) ;  // set bitmask for used DAC





//---------------------------------------------------------------------------------------------------------------------------------------------------

_wire->beginTransmission(my_address);  // Transmit to device number 44 (0x2C)
_wire->write(POINTER_BYTE);             // Sends value POINTER_BYTE
_wire->write(  (my_address << 1)| 0x01); 
MSB_BYTE = _wire->read();    // Receive a byte as character
LSB_BYTE = _wire->read();    // Receive a byte as character

my_buffer = MSB_BYTE <<8 ;
my_buffer |= LSB_BYTE ;
my_buffer |= my_buffer >> (16- resolution );


_wire->endTransmission();      // Stop transmitting

/*
Serial.print("POINTER_BYTE: ");
Serial.println(POINTER_BYTE, BIN);
Serial.print("MSB_BYTE: ");
Serial.println(MSB_BYTE, BIN);
Serial.print("LSB_BYTE: ");
Serial.println(LSB_BYTE, BIN);
*/
return my_buffer;

}



uint16_t AD53xx::DAC_Read_4ch(uint8_t my_address, uint8_t DAC_number)
{
  return DAC_Read_2ch(my_address, DAC_number);
}


uint16_t AD53xx::DAC_Voltage_to_Value(float input_voltage)    // input_voltage is   dac output , function output is the value that gets the voltage at the output
{
  return input_voltage* ( pow(2, resolution ) ) / Voltage_Reference ;
}


uint16_t AD53xx::DAC_Value_to_Voltage(uint16_t DAC_value)   //  DAC_value is the bits set in DAC , output is mV expected at DAC output
{
  return ( Voltage_Reference * DAC_value ) / ( pow(2, resolution ) );
}



