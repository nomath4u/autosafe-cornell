#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

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
};

/******GLOBALS*****/
//**********GPS************
HardwareSerial mySerial = Serial;  //This might have been the problem
//SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  true  //This might have been the problem

// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;
void useInterrupt(boolean); // Func prototype keeps Arduino 0023 happy

packet spacket;
  
void setup()
{ 
  //initialize serial communication
  Serial.begin(9600);
  
  //Temp GPS setup for clean viewing
  Serial.println("Initializing GPS");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PGCMD_ANTENNA);
  
  useInterrupt(true);
  
  delay(1000);
  Serial.println(PMTK_Q_RELEASE);
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
}
void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis(); //Going to use this to only get GPS sometimes
void loop()
{  
  //if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
    //char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    //if (GPSECHO)
      //if (c) Serial.print(c);
  //}
  
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }
  
  
 /*This section is for reading in the UART/GPS data*/
 /*TODO: Figure out what information we need here */
 //char c = GPS.read();
 if(timer > millis()) timer = millis(); //If something wraps around, fix it
 if(millis() - timer > 2000) { //Do this ever 2 seconds
   timer = millis();
   
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
 delay(1000);
 int error = send_packet();
}

int send_packet(){
  Serial.print("LaD: "); Serial.println(spacket.lat_degrees);
  Serial.print("loD: "); Serial.println(spacket.long_degrees);
  Serial.print("Fix: "); Serial.println(spacket.fix);
  Serial.print("Sat: "); Serial.println(spacket.num_satellite);
  Serial.print("Spd: "); Serial.println(spacket.knots);
  Serial.print("Dat: "); Serial.print(spacket.day.month);
  Serial.print("/"); Serial.print(spacket.day.day);
  Serial.print("/"); Serial.println(spacket.day.year);
  Serial.print("Tim: "); Serial.print(spacket.time.hour);
  Serial.print(":"); Serial.print(spacket.time.minute);
  Serial.print(":"); Serial.println(spacket.time.seconds);
  return 0;
}
