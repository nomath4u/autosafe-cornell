#include <Wire.h>
#include <SoftwareSerial.h>

#define OBDRX 10
#define OBDTX 11
#define NUMCOMMANDS 1

/******GLOBALS*****/
//OBDII
char rxData[20];
int rxIndex=0;
int vehicleRPM=0; //Used as an example until we decide on commands
SoftwareSerial obd2(OBDRX,OBDTX);
char commands [NUMCOMMANDS][5] = {"010C"}; //This could be done better,NUMCOMMANDS must be updated, 5 because of null terminator

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

   
//The getResponse function collects incoming data from the UART into the rxData buffer
// and only exits when a carriage return character is seen. Once the carriage return
// string is detected, the rxData buffer is null terminated (so we can treat it as a string)
// and the rxData index is reset to 0 so that the next string can be copied.
void getResponse(void){
  char inChar=0;
  //Keep reading characters until we get a carriage return
  while(inChar != '\r'){ 
    //while(1){
    //If a character comes in on the serial port, we need to act on it.
    if(obd2.available() > 0){
      //Start by checking if we've received the end of message character ('\r').
      if(obd2.peek() == '\r'){
        //Clear the Serial buffer
        inChar=obd2.read();
        //Put the end of string character on our data string
        rxData[rxIndex]='\0';
        //Reset the buffer index so that the next character goes back at the beginning of the string.
        rxIndex=0;
      }
      //If we didn't get the end of message character, just add the new character to the string.
      else{
        //Get the new character from the Serial port.
        inChar = obd2.read();
        //Serial.print(inChar);
        //Add the new character to the string, and increment the index variable.
        rxData[rxIndex++]=inChar;
      }
    }
  }
}
