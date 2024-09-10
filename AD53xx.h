#ifndef AD53xx
#define AD53xx

#include "Arduino.h"
#include <Wire.h>


 // #define AD53xx_LIB_VERSION              (F("0.1.0"))

class AD53xx
{
  public:

    //constructor
              AD53xx (uint8_t my_address, 
                      uint8_t channel_number,
                      uint8_t resolution, 
                      uint16_t Voltage_Reference);


       
       void DAC_Write_1ch(uint8_t my_address,                                                              uint16_t DAC_1_value);
       void DAC_Write_2ch(uint8_t my_address ,bool CLR, bool LDAC,                     uint8_t DAC_number, uint16_t DAC_1_value);
       void DAC_Write_4ch(uint8_t my_address ,bool gain, bool buff, bool CLR, bool PD, uint8_t DAC_number, uint16_t DAC_1_value);
       uint16_t DAC_Read_2ch(uint8_t my_address, uint8_t DAC_number);
       uint16_t DAC_Read_4ch(uint8_t my_address, uint8_t DAC_number);
       uint16_t DAC_Voltage_to_Value(float input_voltage);    // turn a voltage value into the DAC input seting / 
                                                              // like if you want 1V out this will tell you what to write in teh DAC
       uint16_t DAC_Value_to_Voltage(uint16_t DAC_value);    // turn a decimal value into the DAC output voltage
 

// debug 
       bool Check_adress(uint8_t my_address);

  private:

    uint8_t MSB_BYTE=0x0;
    uint8_t LSB_BYTE=0x0;
    uint8_t my_buffer=0;
    uint8_t POINTER_BYTE=0x0;

};


#endif
