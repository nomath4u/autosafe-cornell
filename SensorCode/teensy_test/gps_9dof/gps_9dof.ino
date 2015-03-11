#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_GPS.h>
#define GPSECHO false

Adafruit_GPS GPS(&Serial2);
     
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
  
  //String accel;
  double accx;
  double accy;
  double accz;
  //String gyro;
  double gyx;
  double gyy;
  double gyz;
};

packet spacket;

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

void setup()
{   
    Serial2.begin(9600);
    Serial.println("Initializing GPS!");
     
    GPS.begin(9600);
  
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_ALLDATA);
    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_10HZ); // 1 Hz update rate
    GPS.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);
    GPS.sendCommand(PGCMD_ANTENNA);

    delay(1000);
    
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

    /***Packet stuff, zero it out**/
    spacket.accx = 0;
    spacket.accy = 0;
    spacket.accz = 0;
    spacket.gyx = 0;
    spacket.gyy = 0;
    spacket.gyz = 0;
}

uint32_t timer = millis();
void loop() // run over and over again
{
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
  
    /*char c = GPS.read();
    if (GPS.newNMEAreceived()) {
      if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        return; // we can fail to parse a sentence in which case we should just wait for another
    }*/
    char c;
    while(1){
      c = GPS.read();
      if(c == '\n'){
        if(GPS.newNMEAreceived()){
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
    // if millis() or timer wraps around, we'll just reset it
    //if (timer > millis()) timer = millis();
     
    // approximately every 2 seconds or so, print out the current stats
    //if (millis() - timer > 2000) {
//      timer = millis(); // reset the timer
//      spacket.lat_degrees = GPS.latitudeDegrees;
//      spacket.long_degrees = GPS.longitudeDegrees;
//      spacket.fix = GPS.fix;
//      spacket.num_satellite = GPS.satellites;
//      spacket.knots = GPS.speed;   
//      spacket.day.month = GPS.month;
//      spacket.day.day = GPS.day;
//      spacket.day.year = GPS.year;
//      spacket.time.hour = GPS.hour;
//      spacket.time.minute = GPS.minute;
//      spacket.time.seconds = GPS.seconds;
  
      
    //}
      delay(100);      
      int error = send_packet();
      //send_packet();
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
  Serial.print(spacket.accx); Serial.print(",");
  Serial.print(spacket.accy); Serial.print(",");
  Serial.print(spacket.accz); Serial.print(",");
  Serial.print(spacket.gyx); Serial.print(",");
  Serial.print(spacket.gyy); Serial.print(",");
  Serial.println(spacket.gyz);
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






