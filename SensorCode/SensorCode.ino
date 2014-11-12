#include <Wire.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "MPU6050.h"

#define OBDRX 10
#define OBDTX 11
#define NUMCOMMANDS 1

/******GLOBALS*****/
//OBDII
char rxData[20];
int rxIndex=0;
int vehicleRPM=0; //Used as an example until we decide on commands
SoftwareSerial obd2(OBDRX,OBDTX);
//char commands [NUMCOMMANDS][4] = {"010C"}; //This could be done better,NUMCOMMANDS must be updated

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
  obd2.begin(9600);
  delay(1500); //Have to give it a second to fire up, adapted value may be able to decrease
  obd2.println("ATZ"); //Get the bus ready
  delay(2000); //Same as before 
}

void loop()
{
  delay(100);
 /*This section is for reading in the UART/GPS data*/
 /*TODO: Figure out what information we need here */
  //while(Serial2.available()){
    //Serial.print((char)Serial2.read());
  //}
 
 /******This is for the I2C 9 degrees of freedom sensor******/
    // read raw accel/gyro measurements from device
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);

    // display tab-separated accel/gyro x/y/z values
    Serial.print("a/g/m:\t");
    Serial.print(double(ax)/16384); Serial.print("\t");
    Serial.print(double(ay)/16384); Serial.print("\t");
    Serial.print(double(az)/16384); Serial.print("\t");
    Serial.print(double(gx)/131); Serial.print("\t");
    Serial.print(double(gy)/131); Serial.print("\t");
    Serial.print(double(gz)/131); Serial.print("\t");
//    Serial.print(double(mx)); Serial.print("\t");
//    Serial.print(my); Serial.print("\t");
//    Serial.print(mz);
    Serial.println();

 /******End of the I2C 9 degrees of freedom sensor code******/


  /*Need to add OBD2 decode data here*/
/*  for(int i= 0; i < NUMCOMMANDS; i++){
    obd2.flush(); //Just in case
    obd2.println(commands[i]); //Vehicle speed command
    getResponse(); //Responds first with the command you sent
    getResponse(); //The actual information we want
    Case statement depending on command assuming in loop
    delay(100); //So we don't query the CANbus too fast and cause a buffer overflow
  }
  */
}

  
  
  
