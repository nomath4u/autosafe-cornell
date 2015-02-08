#include "uidatacontroller.h"

/* UI Data functions */

UIDataController::UIDataController(QObject *parent)
{

}

void UIDataController::setValues(
        double acc[3],
        double gy[3],
        double mag,
        int sats,
        time_t hour,
        time_t min,
        time_t sec,
        time_t month,
        time_t day,
        time_t year,
        double coords[2],
        int fix,
        double spd,
        char com[10],
        int val)
{
    _AccelerometerX = acc[0];
    _AccelerometerY = acc[1];
    _AccelerometerZ = acc[2];

    _GyroX = gy[0];
    _GyroY = gy[1];
    _GyroZ = gy[2];

    _Magnetometer = mag;
    _Satellites = sats;
    _Hour = hour;
    _Minute = min;
    _Second = sec;
    _Month = month;
    _Day = day;
    _Year = year;

    _CoordinatesLat = coords[0];
    _CoordinatesLong = coords[1];

    _PositionFix = fix;
    _Speed = spd; //convert from knots to to mph here?

    _OBDIICommand = com;
    _OBDIIValue = val;
}


double UIDataController::strict_str2double(char* str)
{
    char* endptr;
    double value = strtod(str, &endptr);
    if (*endptr) return 0;
    return value;
}

void UIDataController::parse_data(){

    qDebug() << "Made it to parse_data()";

    char *token = (char *) malloc(10 * sizeof(char));
    int i;

    char buf[] = "0.26,-0.17,1.17,-1.15,-5.50,0.31,0,0,0,0,0,0,0,0,0,0,0,RPM,694";

    token = strtok(buf, ",");
    for (i=0; token != NULL; i++) {

        qDebug() << "Parsing item " << i << " Token = " << token;

        if (i == 0) {
            _AccelerometerX = strict_str2double(token);
        }
        else if (i == 1) {
            _AccelerometerY = strict_str2double(token);
        }
        else if (i == 2) {
            _AccelerometerZ = strict_str2double(token);
        }
        else if (i == 3) {
            _GyroX = strict_str2double(token);
        }
        else if (i == 4) {
            _GyroY = strict_str2double(token);
        }
        else if (i == 5) {
            _GyroZ = strict_str2double(token);
        }
        else if (i == 6) {
            _PositionFix = atoi(token);
        }
        else if (i == 7) {
            _Satellites = atoi(token);
        }
        else if (i == 8) {
            _Hour = (time_t)atoi(token);
        }
        else if (i == 9) {
            _Minute = (time_t)atoi(token);
        }
        else if (i == 10) {
            _Second = (time_t)atoi(token);
        }
        else if (i == 11) {
            _Month = (time_t)atoi(token);
        }
        else if (i == 12) {
            _Day = (time_t)atoi(token);
        }
        else if (i == 13) {
            _Year = (time_t)atoi(token);
        }
        else if (i == 14) {
            _CoordinatesLat = strict_str2double(token);
        }
        else if (i == 15) {
            _CoordinatesLong = strict_str2double(token);
        }
        else if (i == 16) {
            _Speed = atoi(token);
        }
        else if (i == 17) {
            _OBDIICommand = "Get OBDII Command!";

            qDebug() << "_OBDII Command = " << _OBDIICommand;
        }
        else if (i == 18) {
            _OBDIIValue = atoi(token);
        }

        token = strtok(NULL, ",");
    }
}

void UIDataController::dump_data(){

    qDebug() << "Dumping data!";

    qDebug() << "Accelerometer X: "  << _AccelerometerX;
    qDebug() << "Accelerometer Y: "  << _AccelerometerY;
    qDebug() << "Accelerometer Z: "  << _AccelerometerZ;
    qDebug() << "Gyro X: "           << _GyroX;
    qDebug() << "Gyro Y: "           << _GyroY;
    qDebug() << "Gyro Z: "           << _GyroZ;
    qDebug() << "Magentometer: "     << _Magnetometer;
    qDebug() << "Position Fix: "     << _PositionFix;
    qDebug() << "Satellites: "       << _Satellites;
    qDebug() << "Time Hour: "        << _Hour;
    qDebug() << "Time Minutes: "     << _Minute;
    qDebug() << "Time Seconds: "     << _Second;
    qDebug() << "Date Month: "       << _Month;
    qDebug() << "Date Day: "         << _Day;
    qDebug() << "Date Year: "        << _Year;
    qDebug() << "Lattitude: "        << _CoordinatesLat;
    qDebug() << "Longitude: "        << _CoordinatesLong;
    qDebug() << "Speed: "            << _Speed;
    qDebug() << "OBDII Command: "    << _OBDIICommand;
    qDebug() << "OBDII Value: "      << _OBDIIValue;
}


void UIDataController::updateData()
{
    qDebug() << "Updating data!";
    parse_data();
    dump_data();
}

QStringList UIDataController::getList()
{
    QStringList list;
    list.append("Accelerometer X: " + QString::number(_AccelerometerX));
    return list;
}

void UIDataController::handleResults(const QString &result)
{
    qDebug() << "Handling results!" << result;
}

void UIDataController::runSensorThread()
{
    SensorThread *sensorThread = new SensorThread(this);
    connect(sensorThread, &SensorThread::resultReady, this, &UIDataController::handleResults);
    connect(sensorThread, &SensorThread::finished, sensorThread, &UIDataController::deleteLater); //automatically handled
    sensorThread->start();
}

UIDataController::~UIDataController()
{

}
