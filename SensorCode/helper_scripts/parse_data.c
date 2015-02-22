#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


//[{accelerometer xyz},{gyroscope xyz},{magnetometer}, {satellites},{time HMS}, {date MDY},{latitude},{longitude}, {position fix}, {speed knots}, {OBDII command}, {OBDII value}]
//

#define X 0
#define Y 1
#define Z 2
#define LAT 0;
#define LONG 1;
#define BAUDRATE B9600

double accelormeter[3];
double gyro[3];
double magnet;
int satellites;
time_t time_h;
time_t time_m;
time_t time_s;
time_t date_d;
time_t date_m;
time_t date_y;
double cordnates[2];
int pos_fix;
double speed;
char *obd_com;
int obd_val;
int gc = 0;

int read_port(void)
{
    struct termios oldtio,newtio;
    int fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
        /* Could not open the port. */
        perror("open_port: Unable to open Serial Port.");
    }

    tcgetattr(fd,&oldtio); /* save current serial port settings */
    bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR | ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = ICANON;
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);

    return (fd);
}

double strict_str2double(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

int print_data(){
    printf("Accelerometer X: %.2f\n", accelormeter[X]);
    printf("Accelerometer Y: %.2f\n", accelormeter[Y]);
    printf("Accelerometer Z: %.2f\n", accelormeter[Z]);
    printf("Gyro X: %.2f\n", gyro[X]);
    printf("Gyro Y: %.2f\n", gyro[Y]);
    printf("Gyro Z: %.2f\n", gyro[Z]);
    printf("Position Fix: %d\n", pos_fix);
    printf("Satellites: %d\n", satellites);
    printf("Time Hour: %d\n", (int)time_h);
    printf("Time Minutes: %d\n", (int)time_m);
    printf("Time Seconds: %d\n", (int)time_s);
    printf("Date Month: %d\n", (int)date_m);
    printf("Date Day: %d\n", (int)date_d);
    printf("Date Year: %d\n", (int)date_y);
    printf("Lattitude: %lf\n", cordnates[1]);
    printf("Longitude: %lf\n", cordnates[2]);
    printf("Speed: %lf\n", speed);
    printf("OBDII Command: %s\n", obd_com);
    printf("OBDII Value: %d\n", obd_val);
}

void copy_string(char *target, char *source)
{
    while(*source)
    {
        *target = *source;
        source++;
        target++;
    }
    *target = '\0';
}

int parse_data(){
    char *token = (char *) malloc(10 * sizeof(char));
    int i, n;
    char * set;
    char buf[128];
    int fd = read_port();

    while(1){
        gc++;
        n = read(fd, &buf, sizeof(buf));
        if (n < 0)
            fputs("read failed!\n", stderr);
        //char bufP[128] = "-0.06,0.00,0.98,0.50,-0.05,-0.40,0,0,0,27,31,1,6,80,0,0,0,None,0\n";
        
        //buf[n] = 0; 

        token = strtok(buf, ",");
        //printf("TOK %s\n", token);
        for (i=0; token != NULL; i++) {
            if (i == 0) { 
                //printf("X %s\n", token);
                accelormeter[X] = strict_str2double(token);
            }
            else if (i == 1) {
                accelormeter[Y] = strict_str2double(token);
            }
            else if (i == 2) {
                accelormeter[Z] = strict_str2double(token);
            }
            else if (i == 3) {
                gyro[X] = strict_str2double(token);
            }
            else if (i == 4) {
                gyro[Y] = strict_str2double(token);
            }
            else if (i == 5) {
                gyro[Z] = strict_str2double(token);
            }
            else if (i == 6) {
                pos_fix = atoi(token);
            }
            else if (i == 7) {
                satellites = atoi(token);
            }
            else if (i == 8) {
                time_h = (time_t)atoi(token);
            }
            else if (i == 9) { 
                time_m = (time_t)atoi(token);
            }
            else if (i == 10) {
                time_s = (time_t)atoi(token);
            }
            else if (i == 11) {
                date_m = (time_t)atoi(token);
            }
            else if (i == 12) {
                date_d = (time_t)atoi(token);
            }
            else if (i == 13) {
                date_y = (time_t)atoi(token);
            }
            else if (i == 14) {
                cordnates[1] = strict_str2double(token);
            }
            else if (i == 15) {
                cordnates[2] = strict_str2double(token);
            }
            else if (i == 16) {
                speed = atoi(token);
            }
            else if (i == 17) {
                obd_com = "None";
                //strncpy(obd_com, token, 5);
            }
            else if (i == 18) {
                obd_val = atoi(token);
            }
            token = strtok(NULL, ",");
        }
        printf("--------------------------------------------------------------------------------\n");
        print_data();
        fflush(stdout);
    }
}



int main() {
    parse_data();
    return 0;
}
