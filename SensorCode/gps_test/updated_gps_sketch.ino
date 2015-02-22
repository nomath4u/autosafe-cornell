// Code for Adafruit GPS module
     
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

/*****Packet Def*****/
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
HardwareSerial mySerial = Serial;

Adafruit_GPS GPS(&mySerial);
     
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

packet spacket;
     
void setup()
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(9600);
  Serial.println("Initializing GPS");
     
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
     
  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  // Ask for firmware version
  //Serial.println(PMTK_Q_RELEASE);
}
       
uint32_t timer = millis();
void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
//    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
//    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();
     
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer    
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

