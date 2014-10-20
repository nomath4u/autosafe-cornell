//Adapted from https://learn.sparkfun.com/tutorials/obd-ii-uart-hookup-guide

#include <SoftwareSerial.h>

SoftwareSerial obd2(10,11); //TX on SF -> 10 Only certain ones work, see http://arduino.cc/en/Reference/softwareSerial

char rxData[20];
char rxIndex=0;

int vehicleSpeed = 0;
int vehicleRPM=0;

void setup(){
  obd2.begin(9600);
  Serial.begin(9600); //To show back on arduino terminal
  Serial.println("Reseting OBD2");
  delay(1500);
  obd2.println("ATZ");
  delay(2000);
  Serial.println("Done");
}

void loop(){
   obd2.flush(); //Just in case
   obd2.println("010D"); //The command to get the vehicle speed
   //Get response twice because the board echos back our command
   getResponse();
   getResponse();
   vehicleSpeed = strtol(&rxData[6],0,16); //Need to understand this still
   //Serial.print(rxData);
   Serial.print(vehicleSpeed);
   Serial.print( "km/h\n");
   delay(100);
   obd2.flush(); //Just in case
   obd2.println("010C"); //Getting the RPM this time
   //Get 2 responses again
   getResponse();
   getResponse();
   //RPM data is 2 bytes long and delivered in 1/4 RPM, for some weird reason
   //Serial.println(rxData);
   vehicleRPM = ((strtol(&rxData[6],0,16)*256)+strtol(&rxData[9],0,16))/4;
   Serial.print(vehicleRPM);
   Serial.print("\n");
   obd2.flush();
   delay(100); //Reset the CAN bus
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
   
