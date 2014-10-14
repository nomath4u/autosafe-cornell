#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop()
{
 /*This section is for reading in the UART/GPS data
  //while(Serial2.available()){
    
  //  Serial.print((char)Serial2.read());
 // }
 
 /*This is for the I2C 9 degrees of freedom sensor
  Wire.requestFrom(2,6);
  
  while(Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }

  /*Need to add OBD2 decode data here
  
  delay(500);
}
