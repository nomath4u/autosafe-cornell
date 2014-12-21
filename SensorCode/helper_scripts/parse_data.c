#include <stdio.h>
#include <string.h>
#include <stdlib.h>



//[{accelerometer xyz},{gyroscope xyz},{magnetometer}, {satellites},{time HMS}, {date MDY},{latitude},{longitude}, {position fix}, {speed knots}, {OBDII command}, {OBDII value}]
//

#define X 0;
#define Y 1;
#define Z 2;
#define LAT 0;
#define LONG 1;

double accelormeter[3];
double gyro[3];
double magnet;
int satellites;
time_t time;
time_t date;
double cordnates[2];
int pos_fix;
double speed;
int obd_com;
int obd_result;

int parse_data(){
    char *token = (char *) malloc(10 * sizeof(char));
    int i;
    char * set;

    char buf[] = "0.26,-0.17,1.17,-1.15,-5.50,0.31,0,0,0,0,0,0,0,0,0,0,0,RPM,694";
    
    token = strtok(buf, ",");
    for (i=0; token != NULL; i++) {
            if (i == 0) 
                printf("Accelerometer X: ");
            else if (i == 1)
                printf("Accelerometer Y: ");
            else if (i == 2)
                printf("Accelerometer Z: ");
            else if (i == 3)
                printf("Gyro X: ");
            else if (i == 4)
                printf("Gyro Y: ");
            else if (i == 5)
                printf("Gyro Z: ");
            else if (i == 6)
                printf("Magnetometer: ");
            else if (i == 7)
                printf("Satellites: ");
            else if (i == 8)
                printf("Time: ");
            else if (i == 9)
                printf("Date: ");
            else if (i == 10)
                printf("Lattitude: ");
            else if (i == 11)
                printf("Longitude: ");
            else if (i == 12)
                printf("Position Fix: ");
            else if (i == 13)
                printf("Speed: ");
            else if (i == 14)
                printf("OBDII Command: ");
            else if (i == 15)
                printf("OBDII Value: ");
        printf("%s\n", token);
      token = strtok(NULL, ",");
    }
}

int main() {
    parse_data();
    return 0;
}
