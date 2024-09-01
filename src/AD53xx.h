#include <Wire.h>
#include "Arduino.h"

#define AD53xx_LIB_VERSION              (F("0.1.0"))

class AD53xx
{
  public:

    //constructor
      void AD53xx();


       
       void DAC_Write_1ch();
       void DAC_Write_2ch();
       void DAC_Write_4ch();
       uint16_t DAC_Read_2ch();
       uint16_t DAC_Read_4ch();
       uint16_t DAC_Voltage_to_Value();    // turn a voltage value into the DAC input seting / 
                                           // like if you want 1V out this will tell you what to write in teh DAC
       uint16_t DAC_Value_to_Voltage();    // turn a decimal value into the DAC output voltage
 

// debug 
       bool Check_adress( );

  private:

    uint8_t MSB_BYTE=0x0;
    uint8_t LSB_BYTE=0x0;
    uint8_t my_buffer=0;
    uint8_t POINTER_BYTE=0x0

};