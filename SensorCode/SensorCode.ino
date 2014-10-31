#include <Wire.h>
#include <SoftwareSerial.h>

#define OBDRX 10
#define OBDTX 11

/******GLOBALS*****/
//OBDII
char rxData[20]
rxIndex=0;
int vehicleRPM=0; //Used as an example until we decide on commands
SoftwareSerial obd2(OBDRX,OBDTX);

void setup()
{ //9DOF
  Wire.begin();
  Serial.begin(9600);
  Serial2.begin(9600);
  //OBDII
  obd2.begin(9600);
  delay(1500); //Have to give it a second to fire up, adapted value may be able to decrease
  obd2.println("ATZ"); //Get the bus ready
  delay(2000); //Same as before 
}

void loop()
{
 /*This section is for reading in the UART/GPS data*/
 /*TODO: Figure out what information we need here */
  //while(Serial2.available()){
    //Serial.print((char)Serial2.read());
  //}
 
 /*This is for the I2C 9 degrees of freedom sensor*/
  Wire.requestFrom(2,6);
  
  while(Wire.available())
  {
    char c = Wire.read();
    Serial.print(c);
  }

  /*Need to add OBD2 decode data here*/
  
  delay(500);
}
