#ifndef DATAREADINGHANDLER_H
#define DATAREADINGHANDLER_H

#include <QObject>

class DataReadingHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double movement READ movement WRITE setMovement
                   NOTIFY movementChanged FINAL)

    Q_PROPERTY(double velocityX READ velocityX WRITE setvelocityX
                   NOTIFY velocityXChanged FINAL)

    Q_PROPERTY(double velocityY READ velocityY WRITE setvelocityY
                   NOTIFY velocityYChanged FINAL)

    Q_PROPERTY(double rotationZ READ rotationZ WRITE setRotationZ
                   NOTIFY rotationZChanged FINAL)

    Q_PROPERTY(bool gyroActive READ gyroActive WRITE setgyroActive
                   NOTIFY gyroActiveChanged FINAL)

    Q_PROPERTY(bool accActive READ accActive WRITE setaccActive
                   NOTIFY accActiveChanged FINAL)

    Q_PROPERTY(QString calibration READ calibration WRITE setCalibration
                   NOTIFY calibrationChanged FINAL)

public:
    DataReadingHandler();
    Q_INVOKABLE void accReading(double accX, double accY);
    Q_INVOKABLE void gyroReading(double gyroV);
    Q_INVOKABLE void startPattern();
    Q_INVOKABLE void stopPattern();
    Q_INVOKABLE void startCalibration();

    enum ReadingState { Idle, Calibration, Initial, MoveX, MoveY, Rotation };
    Q_ENUM(ReadingState)
    ReadingState state = Idle;

    enum MoveDirection { Left, Right, Up, Down };
    Q_ENUM(MoveDirection)

    double movement() const;
    void setMovement(double newMovement);

    double velocityX() const;
    void setvelocityX(double newVelocityX);

    double velocityY() const;
    void setvelocityY(double newVelocityY);

    double rotationZ() const;
    void setRotationZ(double newRotationZ);

    bool gyroActive() const;
    void setgyroActive(bool newGyroActive);

    bool accActive() const;
    void setaccActive(bool newAccActive);

    QString calibration() const;
    void setCalibration(const QString &newCalibration);

signals:
    void movementChanged();

    void velocityXChanged();

    void velocityYChanged();

    void rotationZChanged();

    void gyroActiveChanged();

    void accActiveChanged();


    void calibrationChanged();

private:
    MoveDirection currentDirection = Up;
    double prevAccX = 0;
    double prevAccY = 0;
    double prevRotation = 0;

    // Constants
    const double accThresh = 0.3;
    const double rotationThresh = 5;
    const double datarate = 25;

    // Thresholds
    const int calibrationLimit = 100;
    const double stationaryAccXThresh = 0.15;
    const double stationaryAccYThresh = 0.15;
    const double stationaryRotationThresh = 0.1;

    // Q_PROPERTY values
    double m_movement = 0;
    double m_velocityX = 0;
    double m_velocityY = 0;
    double m_rotationZ = 0;
    bool m_gyroActive = 0;
    bool m_accActive = 0;

    // Calibration variables
    double accXSum = 0;
    double accXCount = 0;
    double accYSum = 0;
    double accYCount = 0;
    double rotationSum = 0;
    double rotationCount = 0;
    double rotationNoise = 0;
    double accXnoise = 0;
    double accYnoise = 0;

    void handleMovementX(double a);
    void handleMovementY(double a);
    void handleRotation(double gyroV);
    void updateCalibrationInfo(double newData, double &sum, double &count);
    void stopCalibration();

    QString m_calibration;
};

#endif // DATAREADINGHANDLER_H
