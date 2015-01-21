#include <Wire.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"

/*****Packet Def***/
/*******************************************************
** This is the definition of the packet struct that we
** will be sending back to crash detection algorithms
** and the like. The idea is to assign your values into
** the global version of this packet so we can simply
** manipulate the packet to send values. 
********************************************************/
typedef struct packet packet;

struct packet{
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
 
//**********I2C**********
// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz; 
  
  
void setup()
{ //9DOF
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


void loop(){
 /******This is for the I2C 9 degrees of freedom sensor******/
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
    
    Serial.println(); //new line to mark new packet
    send_packet();    //send new packet to our from microcontroller (aslo displayed in terminal 
    
 /******End of the I2C 9 degrees of freedom sensor code******/
}

//This is our standardized format for sending information, comma seperated
// accell first, then gyro, then the other groups packet data
int send_packet(){
  Serial.print(spacket.accx); Serial.print(",");
  Serial.print(spacket.accy); Serial.print(",");
  Serial.print(spacket.accz); Serial.print(",");
  Serial.print(spacket.gyx); Serial.print(",");
  Serial.print(spacket.gyy); Serial.print(",");
  Serial.print(spacket.gyz); Serial.print(",");
  return 0;
}

