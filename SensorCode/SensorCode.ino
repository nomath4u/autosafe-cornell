#include <Wire.h>
#include <SoftwareSerial.h>

#define OBDRX 10
#define OBDTX 11
#define NUMCOMMANDS 1

/******GLOBALS*****/
//OBDII
char rxData[20]
rxIndex=0;
int vehicleRPM=0; //Used as an example until we decide on commands
SoftwareSerial obd2(OBDRX,OBDTX);
char commands [NUMCOMMANDS][4] = {"010C"}; //This could be done better,NUMCOMMANDS must be updated

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
  for(int i= 0; i < NUMCOMMANDS; i++){
    obd2.flush(); //Just in case
    obd2.println(commands[i]); //Vehicle speed command
    getResponse(); //Responds first with the command you sent
    getResponse(); //The actual information we want
    /*Case statement depending on command assuming in loop*/ 
    delay(100); //So we don't query the CANbus too fast and cause a buffer overflow
  }
}
