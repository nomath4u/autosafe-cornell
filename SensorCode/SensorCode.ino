#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#define GPSECHO false

#define NONE 0
#define LEFT 1
#define RIGHT 2

#define zero_zero 0
#define zero_one 1
#define one_zero 2
#define one_one 3

#define OBDRX 7
#define OBDTX 8
#define NUMCOMMANDS 2

typedef struct packet packet;

struct date{
  int month;
  int day;
  int year;
};

struct time{
  int hour;
  int minute;
  int seconds;
};

struct packet{
  /*GPS*/
  int fix;
  int num_satellite;
  struct time time;
  struct date day;
  float lat_degrees;
  float long_degrees;
  int knots;  
  /*9DOF*/
  //String accel;
  double accx;
  double accy;
  double accz;
  //String gyro;
  double gyx;
  double gyy;
  double gyz;
  /*OBD-II*/
  String name;
  int obdval;
  //encoder
  int spin;
  int pressed;
};

/*Globals*/
//Encoder
const int buttonPin = 0;    // the number of the pushbutton pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

//9DOF
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

//GPS
Adafruit_GPS GPS(&Serial2);

//OBD-II
char rxData[20];
int rxIndex=0;
int vehicleRPM=0; //Used as an example until we decide on commands
int command_index = 0 ; //For keeping track of iterations through loop
//HardwareSerial obd2(OBDRX,OBDTX);
SoftwareSerial obd2(OBDRX,OBDTX);
//command format is MSByte = Mode LSByte = Device
char commands [NUMCOMMANDS][5] = {"010C", "010D"}; //This could be done better,NUMCOMMANDS must be updated, 5 because of null terminator
//This way we can use a case statement based on how many times through the command loop we have gone to get the name
enum cmd_names{ //The names of the commands with the same ordering as the commands array
  RPM,
  Speed
};

packet spacket;

void setup(){
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);
    
  //9DOF
  // join I2C bus
  Wire.begin();
  // initialize serial communication
  Serial.begin(9600);
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  //OBDII
  Serial.println("Initializing OBDII connection");
  obd2.begin(9600);
  delay(1500); //Have to give it a second to fire up, adapted value may be able to decrease
  obd2.println("ATZ"); //Get the bus ready
  delay(2000); //Same as before
  
  //GPS
  Serial2.begin(9600);
  Serial.println("Initializing GPS!");
     
  GPS.begin(9600);  
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ); // 1 Hz update rate
  GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  
  /***Packet stuff, zero it out**/
  spacket.accx = 0;
  spacket.accy = 0;
  spacket.accz = 0;
  spacket.gyx = 0;
  spacket.gyy = 0;
  spacket.gyz = 0;
  
  for(int i = 0; i < 20; i++){
    spacket.name[i] = 0;
  }
  spacket.obdval = 0;
}

int concatenate(int x, int y){ 
  int pow = 2;
  return x * pow + y;
}

//*********************************************************************** 
//                        deal_with_encoder 	 
//This function masks to get the value of the encoders to determines if  
//the encoder was turned left or right. It saves the value of the 
//previous value to compare to the new value 
//***********************************************************************
int deal_with_encoder(void){
  int pin_one = 0;
  int pin_two = 0;
  int encoder = 0;
  int spin = NONE;
  static int old_encoder = 0;
    
  pin_one = digitalRead(1);
  pin_two = digitalRead(2);
    
  encoder = concatenate(pin_one,pin_two); 

  switch(encoder){
    case 0:
      if (old_encoder == zero_one){
        spin = RIGHT;
      } else if (old_encoder == one_zero){
        spin = LEFT;
      } else{
        spin= NONE;
      }
      break;
      
    case 1:
      if (old_encoder == zero_zero){
        spin = LEFT;
      } else if (old_encoder == one_one){
        spin = RIGHT;
      } else{
        spin= NONE;
      }
      break;
      
    case 2:
      if (old_encoder == zero_zero){
        spin = RIGHT;
      } else if (old_encoder == one_one){
        spin = LEFT;
      } else{
        spin= NONE;
      }
      break;
        
    case 3:
      if (old_encoder == one_zero){
        spin = RIGHT;
      } else if (old_encoder == zero_one){
        spin = LEFT;
      } else{
        spin= NONE;
      }
      break;
  }     
  old_encoder = encoder;  //Save the encoder values to compare to the new one
  return spin;
} 

void loop()
{
  spacket.spin = deal_with_encoder();
  int depressed = digitalRead(buttonPin);  
  //Serial.print("Spin: ");
  //Serial.println(spacket.spin);
  //Serial.print("Switch: ");
  if(depressed == 1);{
    //Serial.println("Not Pressed");
    spacket.pressed = 0;
  }
  if (depressed != lastButtonState) {    
    lastDebounceTime = millis();  // reset the debouncing timer
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (depressed != buttonState) {  // if the button state has changed:
      buttonState = depressed;
      if(depressed == 0){
        //Serial.println("Pressed");
        spacket.pressed = 1;
      }
    }
  }
  //delay(100);
  lastButtonState = depressed;
  
  // read raw accel/gyro/mag measurements from device
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

  //store raw accel data scalled in "g"s
  spacket.accx = double(ax)/16384;
  spacket.accy = double(ay)/16384;
  spacket.accz = double(az)/16384;
    
  //store raw gyro data scalled in degrees       
  spacket.gyx = double(gx)/131;
  spacket.gyy = double(gy)/131;
  spacket.gyz = double(gz)/131; 
 
  char c;
  if(GPS.newNMEAreceived()){
  while(1){
    c = GPS.read();
    if(c == '\n'){
      //if(GPS.newNMEAreceived()){
      // Only want to do this at the end of the line.
        if(!GPS.parse(GPS.lastNMEA())){
        //Serial.println("Dangit");
        break;
        }
        else{ //Success
          update_gps();
          break;
        }
      }
    }
  }
  
  //OBD-II
  obd2.flush(); //Just in case
  obd2.println(commands[command_index]); //Vehicle speed command
  //Serial.println(commands[command_index]); //Testing what I'm sending out
  getResponse(); //Responds first with the command you sent
  getResponse(); //The actual information we want
  switch(command_index){
    case RPM:
      //Serial.println("Do RPM stuff here");
      spacket.name = "RPM";
      //spacket.obdval = 42; //Need to actually do the math eventually
      spacket.obdval = ((strtol(&rxData[6],0,16)*256)+strtol(&rxData[9],0,16))/4;
      break;
    case Speed:
      //Serial.println("Do Speed stuff here");
      spacket.name = "Speed";
      //spacket.obdval = 777; //Need to actually do the math eventually
      spacket.obdval = strtol(&rxData[6],0,16);
      break;
    default:
      Serial.println("This shoudln't happen");
  }  
  //delay(100); //So we don't query the CANbus too fast and cause a buffer overflow
  //obd2.flush();
  command_index++;
  if(command_index >= NUMCOMMANDS){ //Just so we don't accidentally overflow the int if it was running for a long time
    command_index = 0;
  }
  
  delay(100);      
  int error = send_packet();
}

/*The getResponse function collects incoming data from the UART into the rxData buffer
 and only exits when a carriage return character is seen. Once the carriage return
 string is detected, the rxData buffer is null terminated (so we can treat it as a string)
 and the rxData index is reset to 0 so that the next string can be copied.*/
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
    //}
  }
}

int send_packet(){
  Serial.print(spacket.accx); Serial.print(",");
  Serial.print(spacket.accy); Serial.print(",");
  Serial.print(spacket.accz); Serial.print(",");
  Serial.print(spacket.gyx); Serial.print(",");
  Serial.print(spacket.gyy); Serial.print(",");
  Serial.print(spacket.gyz); Serial.print(",");
  Serial.print(spacket.fix); Serial.print(",");
  Serial.print(spacket.num_satellite); Serial.print(",");
  Serial.print(spacket.time.hour); Serial.print(",");
  Serial.print(spacket.time.minute); Serial.print(",");
  Serial.print(spacket.time.seconds); Serial.print(",");
  Serial.print(spacket.day.month); Serial.print(",");
  Serial.print(spacket.day.day); Serial.print(",");
  Serial.print(spacket.day.year); Serial.print(",");
  Serial.print(spacket.lat_degrees); Serial.print(",");
  Serial.print(spacket.long_degrees); Serial.print(",");
  Serial.print(spacket.knots); Serial.print(",");
  Serial.print(spacket.name); Serial.print(",");
  Serial.print(spacket.obdval);Serial.print(",");
  Serial.print(spacket.spin); Serial.print(",");
  Serial.println(spacket.pressed);
  return 0;
}

void update_gps(){
  spacket.lat_degrees = GPS.latitudeDegrees;
  spacket.long_degrees = GPS.longitudeDegrees;
  spacket.fix = GPS.fix;
  spacket.num_satellite = GPS.satellites;
  spacket.knots = GPS.speed;   
  spacket.day.month = GPS.month;
  spacket.day.day = GPS.day;
  spacket.day.year = GPS.year;
  spacket.time.hour = GPS.hour;
  spacket.time.minute = GPS.minute;
  spacket.time.seconds = GPS.seconds;
}

