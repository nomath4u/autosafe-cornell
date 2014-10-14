#include <Wire.h>

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop()
{
  //while(Serial2.available()){
    
  //  Serial.print((char)Serial2.read());
 // }
  
  Wire.requestFrom(2,6);
  
  while(Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }
  
  delay(500);
}
