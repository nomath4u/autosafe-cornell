#include "parse_data.h"


int read_port()
{
    int fd = open("/dev/ttyACM0", O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
    /* Could not open the port. */
    perror("open_port: Unable to open /dev/ttyS0 - ");
    }
    char buffer[128];
    int n = read(fd, buffer, sizeof(buffer));
    if (n < 0)
    fputs("read failed!\n", stderr);
    //printf("Rec: %s\n", buffer);
    return (fd);
}

double strict_str2double(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

void print_data(){
    printf("Accelerometer X: %lf\n", accelerometer[1]);
    printf("Accelerometer Y: %lf\n", accelerometer[2]);
    printf("Accelerometer Z: %lf\n", accelerometer[3]);
    printf("Gyro X: %lf\n", gyro[1]);
    printf("Gyro Y: %lf\n", gyro[2]);
    printf("Gyro Z: %lf\n", gyro[3]);
    printf("Position Fix: %d\n", pos_fix);
    printf("Satellites: %d\n", satellites);
    printf("Time Hour: %d\n", (int)time_h);
    printf("Time Minutes: %d\n", (int)time_m);
    printf("Time Seconds: %d\n", (int)time_s);
    printf("Date Month: %d\n", (int)date_m);
    printf("Date Day: %d\n", (int)date_d);
    printf("Date Year: %d\n", (int)date_y);
    printf("Lattitude: %lf\n", coordinates[1]);
    printf("Longitude: %lf\n", coordinates[2]);
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

void parse_data()
{
    char *token = (char *) malloc(10 * sizeof(char));
    int i, n;

    char buf[128];
    int fd = read_port();

    while(1){
        n = read(fd, buf, sizeof(buf));
        if (n < 0)
            fputs("read failed!\n", stderr);

        token = strtok(buf, ",");
        for (i=0; token != NULL; i++) {
            if (i == 0) {
                accelerometer[1] = strict_str2double(token);
            }
            else if (i == 1) {
                accelerometer[2] = strict_str2double(token);
            }
            else if (i == 2) {
                accelerometer[3] = strict_str2double(token);
            }
            else if (i == 3) {
                gyro[1] = strict_str2double(token);
            }
            else if (i == 4) {
                gyro[2] = strict_str2double(token);
            }
            else if (i == 5) {
                gyro[3] = strict_str2double(token);
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
                coordinates[1] = strict_str2double(token);
            }
            else if (i == 15) {
                coordinates[2] = strict_str2double(token);
            }
            else if (i == 16) {
                speed = atoi(token);
            }
            else if (i == 17) {
                strncpy(obd_com, token, 10);
            }
            else if (i == 18) {
                obd_val = atoi(token);
            }
            token = strtok(NULL, ",");
        }
        print_data();
    }
}

