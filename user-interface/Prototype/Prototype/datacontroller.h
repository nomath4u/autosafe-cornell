#ifndef DATACONTROLLER_H
#define DATACONTROLLER_H

#include <QObject>
#include <QString>
#include <QQmlApplicationEngine>
#include <QQmlListProperty>

struct SensorData {
    double accelerometerX;
    double accelerometerY;
    double accelerometerZ;
    double gyroX;
    double gyroY;
    double gyroZ;
    double magX;
    double magY;
    double magZ;
    time_t hour;
    time_t minute;
    time_t second;
    time_t day;
    time_t month;
    time_t year;
    double speed;
    double longitude;
    double lattitude;
    int fix;
    int satellites;
    char *command;
    int value;
    int knobTurn;
    int knobPress;
    double roll;
    double pitch;
};

struct OBDIIData {
    QString OBDIICommand;
    int OBDIIValue;
};

class DataController : public QObject
{
    Q_OBJECT

public:
    DataController(QObject * parent = 0);
    ~DataController();

    void runSensorThread();
    void runCrashDetectionThread();
    void runNetworkThread();

    double strToDouble(char* str);
    int openPort();
    void dumpData();

    SensorData getDataPacket();

    Q_INVOKABLE QStringList getVehicleInfoList();
    Q_INVOKABLE QStringList getVehicleAlertsList();
    Q_INVOKABLE QStringList getMessageList();

public slots:
    void handleData();
    void parseData(char* buffer);
    void getMessage(const QString &msg);
    void handleCrash(const QString &msg);

    //for testing UI without sensor data/crash detection or network connection
    void handleTestCrashFromQML();
    void handleTestNetworkMessageFromQML();
    void handleTabLeftFromQML();
    void handleTabRightFromQML();

signals:
    void updateDiagnosticInfo();
    void updateMessages();
    void sendToCrashDetection(const SensorData &data);
    void sendMessage(const QString &msg, const int flag);

    //for testing UI without other threads
    void confirmLocalIncident();
    void alertDriverToIncidentAhead();
    void tabRight();
    void tabLeft();

protected:
    QQmlApplicationEngine _Engine;

    double _AccelerometerX;
    double _AccelerometerY;
    double _AccelerometerZ;
    double _GyroX;
    double _GyroY;
    double _GyroZ;
    double _MagX;
    double _MagY;
    double _MagZ;
    int    _Satellites;
    time_t _Hour;
    time_t _Minute;
    time_t _Second;
    time_t _Day;
    time_t _Month;
    time_t _Year;
    double _CoordinatesLat;
    double _CoordinatesLong;
    int _PositionFix;
    double _Speed;
    QString _OBDIICommand;
    int _OBDIIValue;
    int _KnobTurn;
    int _KnobPress;

    long _OBDIITRBL;
    int _OBDIIRPMS;
    int _OBDIISpeed;
    int _OBDIIFuelPressure;
    int _OBDIIOilTemp;
    int _OBDIIBattery;
    int _OBDIIFuelInput;
    int _OBDIIBoost;

    //UI Data Lists
    QList<QString> _DiagnosticDataList;
    QList<QString> _VehicleAlertList;
    QList<QString> _MessageList;
};

#endif // DATA_H
