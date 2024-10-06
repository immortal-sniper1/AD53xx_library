#include <Wire.h>
#include <AD53xx.h>


AD53xx My_DAC(0x0C, 2, 8, 2470, &Wire);

void setup() 
{
  // put your setup code here, to run once:
Serial.begin(115200);

pinMode(36, OUTPUT);
digitalWrite(36, 1);





Wire.begin(2,1);

// test if DAC is detected

byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
 
      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");


    ////////////////////////////////////


}

void loop() 
{
//Single voltage set on DAC CH2
uint16_t yy= My_DAC.DAC_Voltage_to_Value( 1);    // calculate value needed to get 1V at the output
  My_DAC.DAC_Write_2ch(0x0C , 1, 0, 2, yy );
delay(50000);





// voltage RAMPon DAC CH1
uint16_t xx= 0;
while (xx<255) 
{

  xx = xx+10 ;  
  My_DAC.DAC_Write_2ch(0x0C , 1, 0, 1, xx );
}

delay(50000);
  Serial.println(" ");
  Serial.println("-----------------------------------------------------------------------------------------------------------");



}








