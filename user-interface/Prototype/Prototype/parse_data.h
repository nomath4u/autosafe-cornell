#ifndef PARSE_DATA_H
#define PARSE_DATA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*
 *[{accelerometer xyz},
 * {gyroscope xyz},
 * {magnetometer},
 * {satellites},
 * {time HMS},
 * {date MDY},
 * {latitude},
 * {longitude},
 * {position fix},
 * {speed knots},
 * {OBDII command},
 * {OBDII value}]
*/

#define X 0;
#define Y 1;
#define Z 2;
#define LAT 0;
#define LONG 1;

double accelerometer[3];
double gyro[3];
double magnet;
int satellites;
time_t time_h;
time_t time_m;
time_t time_s;
time_t date_d;
time_t date_m;
time_t date_y;
double coordinates[2];
int pos_fix;
double speed;
char obd_com[10];
int obd_val;

int read_port();
double strict_str2double(char* str);
void print_data();
void copy_string(char *target, char *source);
void parse_data();

#endif
