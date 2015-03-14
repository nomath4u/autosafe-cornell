#include <Wire.h>
#include <SoftwareSerial.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Adafruit_GPS.h>

#define OBDRX 10
#define OBDTX 11
#define NUMCOMMANDS 2
//#define GPSECHO true

/*****Packet Def***/
/*******************************************************
** This is the definition of the packet struct that we
** will be sending back to crash detection algorithms
** and the like. The idea is to assign your values into
** the global version of this packet so we can simply
** manipulate the packet to send values. Ideally 
** everything will be an int but feel free to add 
** and changes types/fields as we decide we need more 
** information
*******************************************************/
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
  int lat_degrees;
  int long_degrees;
  int knots;
  /*9DOF*/
  float gravity; //Dunno might need it?
  //String accel;
  //String gyro;
  double accx;
  double accy;
  double accz;
  double gyx;
  double gyy;
  double gyz;
  double mx;
  double my;
  double mz;
  //I don't know what we need here
  /*OBDII*/
  //char name[20]; //Idea here being we only send one piece of OBDII data per packet and this tells which one
  String name;
  int obdval;
};
  


/******GLOBALS*****/
//OBDII
char rxData[20];
int rxIndex=0;
int vehicleRPM=0; //Used as an example until we decide on commands
int command_index = 0 ; //For keeping track of iterations through loop
SoftwareSerial obd2(OBDRX,OBDTX);

//command format is MSByte = Mode LSByte = Device
char commands [NUMCOMMANDS][5] = {"010C", "010D"}; //This could be done better,NUMCOMMANDS must be updated, 5 because of null terminator
//This way we can use a case statement based on how many times through the command loop we have gone to get the name
enum cmd_names{ //The names of the commands with the same ordering as the commands array
  RPM,
  Speed
};

//**********I2C**********
// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

//**********GPS************
HardwareSerial mySerial = Serial3; 
Adafruit_GPS GPS(&mySerial);

boolean usingInterrupt = false;
void useInterrupt(boolean);  //Funct prototype keeps Arduino 0023

packet spacket;
  
void setup()
{ //9DOF
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(9600);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  //OBDII
  /*Serial.println("Initializing OBDII connection");
  obd2.begin(9600);
  delay(1500); //Have to give it a second to fire up, adapted value may be able to decrease
  obd2.println("ATZ"); //Get the bus ready
  delay(2000); //Same as before*/
  
  //Temp GPS setup for clean viewing
  Serial.println("Initializing GPS");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  
  //useInterrupt(true);

  delay(1000);
  Serial.println(PMTK_Q_RELEASE);
  
  /***Packet stuff, zero it out**/
  spacket.gravity = 9.8; //Mostly for testing purposes
  //spacket.accel = "BAD";
  //spacket.gyro = "BAD";
  for(int i = 0; i < 20; i++){
    spacket.name[i] = 0;
  }
  spacket.obdval = 0;
}

/*SIGNAL(TIMER0_COMPA_vect)
{
  char c = GPS.read();
}
void useInterrupt(boolean v)
{
  if (v) {
    //Timer0 is already used for millis() - we'll just interrupt
    // in the middle and call the Compare A function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}*/

uint32_t timer = millis(); //Going to use this to only get GPS sometimes
void loop()
{
 /*This section is for reading in the UART/GPS data*/
 /*TODO: Figure out what information we need here */
 // if a sentence is received, we can check the checksum, parse it...
 if (GPS.newNMEAreceived()) {
   // a tricky thing here is if we print the NMEA sentence, or data
   // we end up not listening and catching other sentences!
   // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
   if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
     return;  // we can fail to parse a sentence in which case we should just wait for anoth
 }

 if(timer > millis()) timer = millis(); //If something wraps around, fix it
 if(millis() - timer > 2000) { //Do this ever 2 seconds
   timer = millis();
   spacket.lat_degrees = GPS.latitudeDegrees;
   spacket.long_degrees = GPS.longitudeDegrees;
   spacket.fix = (int)GPS.fix;
   spacket.num_satellite = GPS.satellites;
   spacket.knots = GPS.speed;

   spacket.day.month = GPS.month;
   spacket.day.day = GPS.day;
   spacket.day.year = GPS.year;
   spacket.time.hour = GPS.hour;
   spacket.time.minute = GPS.minute;
   spacket.time.seconds = GPS.seconds;
 }
 
 /******This is for the I2C 9 degrees of freedom sensor******/
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    /**************************************************
    **** NEED to send parsed information to packet ****/
    //Serial.print("a/g/m:\t");
    //Serial.print(double(ax)/16384); Serial.print("\t");
    //Serial.print(double(ay)/16384); Serial.print("\t");
    //Serial.print(double(az)/16384); Serial.print("\t");
      //double acx = double(ax)/16384;
      spacket.accx =double(ax)/16384;
      //double acy = double(ay)/16384;
      spacket.accy = double(ay)/16384;
      //double acz = double(az)/16384;
      spacket.accz = double(az)/16384;
      //spacket.accel = (String)acx + "," + (String)acy + "," + (String)acz;
    //Serial.print(double(gx)/131); Serial.print("\t");
    //Serial.print(double(gy)/131); Serial.print("\t");
    //Serial.print(double(gz)/131); Serial.print("\t");
      //double gyx = double(gx)/131);
      spacket.gyx = double(gx)/131;
      //double gyy = double(gy)/131);
      spacket.gyy = double(gy)/131;
      //double gyz = double(gz)/131);
      spacket.gyz = double(gz)/131;
      //spacket.accel = (String)gyx + "," + (String)gyy + "," + (String)gyz;
      spacket.mx = double (mx);
      spacket.my = double (my);
      spacket.mz = double (mz);
      
//    Serial.print(double(mx)); Serial.print("\t");
//    Serial.print(my); Serial.print("\t");
//    Serial.print(mz);
    Serial.println();

 /******End of the I2C 9 degrees of freedom sensor code******/


  /*Need to add OBD2 decode data here*/

  obd2.flush(); //Just in case
  obd2.println(commands[command_index]); //Vehicle speed command
  //Serial.println(commands[command_index]); //Testing what I'm sending out
  //getResponse(); //Responds first with the command you sent
  //getResponse(); //The actual information we want
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
  delay(100); //So we don't query the CANbus too fast and cause a buffer overflow
  obd2.flush();
  command_index++;
  if(command_index >= NUMCOMMANDS){ //Just so we don't accidentally overflow the int if it was running for a long time
    command_index = 0;
  }
  //delay(1000);
  int error = send_packet();
  /*If there was an error do something*/
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
    //}
  }
}

int send_packet(){
  /*Serial.print("LaD: "); Serial.println(spacket.lat_degrees);
  Serial.print("LoD: "); Serial.println(spacket.long_degrees);
  Serial.print("Fix: "); Serial.println(spacket.fix);
  Serial.print("Sat: "); Serial.println(spacket.num_satellite);
  Serial.print("Spd: "); Serial.println(spacket.knots);
  Serial.print("Dat: "); Serial.print(spacket.day.month);
  Serial.print("/"); Serial.print(spacket.day.day);
  Serial.print("/"); Serial.println(spacket.day.year);
  Serial.print("Tim: "); Serial.print(spacket.time.hour);
  Serial.print(":"); Serial.print(spacket.time.minute);
  Serial.print(":"); Serial.print(spacket.time.seconds);
  Serial.print("Grv: "); Serial.println(spacket.gravity);
  //Serial.print("ACL: "); Serial.println(spacket.accel);
  //Serial.print("GYR: "); Serial.println(spacket.gyro);
  Serial.print("Nam: "); Serial.println(spacket.name);
  Serial.print("Val: "); Serial.println(spacket.obdval);*/
  

  //Serial.print("X: "); Serial.println(mx);
  //Serial.print("Y: "); Serial.println(my);
  //Serial.print("Z: "); Serial.println(mz);
  Serial.print(spacket.accx); Serial.print(",");
  Serial.print(spacket.accy); Serial.print(",");
  Serial.print(spacket.accz); Serial.print(",");
  Serial.print(spacket.gyx); Serial.print(",");
  Serial.print(spacket.gyy); Serial.print(",");
  Serial.print(spacket.gyz); Serial.print(",");
  Serial.print(spacket.mx); Serial.print(",");
  Serial.print(spacket.my); Serial.print(",");
  Serial.print(spacket.mz); Serial.print(",");
//  Serial.print(spacket.); Serial.print(",");
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
  Serial.println(spacket.obdval);

  return 0;
}
