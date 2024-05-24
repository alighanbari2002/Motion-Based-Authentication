#ifndef DATAREADINGHANDLER_H
#define DATAREADINGHANDLER_H

#include <QObject>

class DataReadingHandler
{
    Q_OBJECT

    Q_PROPERTY(real movement READ movement WRITE setMovement
                   NOTIFY movementChanged FINAL)

    Q_PROPERTY(real velocityX READ velocityX WRITE setvelocityX
                   NOTIFY velocityXChanged FINAL)

    Q_PROPERTY(real velocityY READ velocityY WRITE setvelocityY
                   NOTIFY velocityYChanged FINAL)

    Q_PROPERTY(real rotationZ READ rotationZ WRITE setRotationZ
                   NOTIFY rotationZChanged FINAL)

    Q_PROPERTY(bool gyroActive READ gyroActive WRITE setgyroActive
                   NOTIFY gyroActiveChanged FINAL)

    Q_PROPERTY(bool accActive READ accActive WRITE setaccActive
                   NOTIFY accActiveChanged FINAL)

public:
    DataReadingHandler();
    Q_INVOKABLE void accReading(double accX, double accY);
    Q_INVOKABLE void gyroReading(double gyroV);

    real movement() const;
    void setMovement(real newMovement);

    real velocityX() const;
    void setvelocityX(real newVelocityX);

    real velocityY() const;
    void setvelocityY(real newVelocityY);

    real rotationZ() const;
    void setRotationZ(real newRotationZ);

    bool gyroActive() const;
    void setgyroActive(bool newGyroActive);

    bool accActive() const;
    void setaccActive(bool newAccActive);

signals:
    void movementChanged();

    void velocityXChanged();

    void velocityYChanged();

    void rotationZChanged();

    void gyroActiveChanged();

    void accActiveChanged();


private:
    enum ReadingState { Initial, MoveX, MoveY, Rotation };
    Q_ENUM(ReadingState)
    ReadingState state = Initial;

    enum MoveDirection { Left, Right, Up, Down };
    Q_ENUM(MoveDirection)

    MoveDirection currentDirection = Up;
    double prevAccX = 0;
    double prevAccY = 0;
    double prevRotation = 0;

    const double accThresh = 0.3;
    const double rotationThresh = 0.3;
    const double datarate = 25;

    real m_movement = 0;
    real m_velocityX = 0;
    real m_velocityY = 0;
    real m_rotationZ = 0;
    bool m_gyroActive = 0;
    bool m_accActive = 0;

    void handleMovementX(double a);
    void handleMovementY(double a);
    void handleRotation(double gyroV);

};

#endif // DATAREADINGHANDLER_H
