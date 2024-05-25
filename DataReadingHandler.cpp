#include "DataReadingHandler.h"
#include <QDebug>
#include <iostream>

DataReadingHandler::DataReadingHandler() {}

void DataReadingHandler::accReading(double accX, double accY)
{
    if(state == Idle)
    {
        return;
    }
    else if(state == Calibration)
    {
        updateCalibrationInfo(accX, accXSum, accXCount);
        updateCalibrationInfo(accY, accYSum, accYCount);

    }
    else if(state == Initial && fabs(prevAccX - accX) > accThresh)
    {
        state = MoveX;
        setgyroActive(false);
        currentDirection = (accX > 0) ? Right : Left;
    }
    else if(state == Initial && fabs(prevAccY - accY) > accThresh)
    {
        state = MoveY;
        setgyroActive(false);
        currentDirection = (accY > 0) ? Up : Down;
    }
    else if(state == MoveX)
    {
        handleMovementX(accX);
    }
    else if(state == MoveY)
    {
        handleMovementY(accY);
    }
    else
    {
        return;
    }
}

void DataReadingHandler::gyroReading(double gyroV)
{
    if(state == Idle)
    {
        return;
    }
    else if(state == Calibration)
    {
        updateCalibrationInfo(gyroV, rotationSum, rotationCount);
    }
    if(state == Initial && fabs(prevRotation - gyroV) > rotationThresh)
    {
        state = Rotation;
        setaccActive(false);
    }
    else if(state == Rotation)
    {
        handleRotation(gyroV);
    }
    else
    {
        return;
    }
}

void DataReadingHandler::startPattern()
{
    state = Initial;
}

void DataReadingHandler::stopPattern()
{
    state = Idle;
    // TODO: save pattern
}

void DataReadingHandler::startCalibration()
{
    std::cout << "Calibration started" << std::endl;
    state = Calibration;
    accXSum = 0;
    accXCount = 0;
    accYSum = 0;
    accYCount = 0;
    rotationSum = 0;
    rotationCount = 0;
    rotationNoise = 0;
    accXnoise = 0;
    accYnoise = 0;
}

double DataReadingHandler::movement() const
{
    return m_movement;
}

void DataReadingHandler::setMovement(double newMovement)
{
    if (m_movement == newMovement)
        return;
    m_movement = newMovement;
    emit movementChanged();
}

double DataReadingHandler::velocityX() const
{
    return m_velocityX;
}

void DataReadingHandler::setvelocityX(double newVelocityX)
{
    if (m_velocityX == newVelocityX)
        return;
    m_velocityX = newVelocityX;
    emit velocityXChanged();
}

double DataReadingHandler::velocityY() const
{
    return m_velocityY;
}

void DataReadingHandler::setvelocityY(double newVelocityY)
{
    if (m_velocityY == newVelocityY)
        return;
    m_velocityY = newVelocityY;
    emit velocityYChanged();
}

double DataReadingHandler::rotationZ() const
{
    return m_rotationZ;
}

void DataReadingHandler::setRotationZ(double newRotationZ)
{
    if (m_rotationZ == newRotationZ)
        return;
    m_rotationZ = newRotationZ;
    emit rotationZChanged();
}

bool DataReadingHandler::gyroActive() const
{
    return m_gyroActive;
}

void DataReadingHandler::setgyroActive(bool newGyroActive)
{
    if (m_gyroActive == newGyroActive)
        return;
    m_gyroActive = newGyroActive;

    emit gyroActiveChanged();
}

bool DataReadingHandler::accActive() const
{
    return m_accActive;
}

void DataReadingHandler::setaccActive(bool newAccActive)
{
    if (m_accActive == newAccActive)
        return;
    m_accActive = newAccActive;
    emit accActiveChanged();
}

void DataReadingHandler::handleMovementX(double a)
{
    a -= accXnoise;
    double v = m_velocityX + ((a + prevAccX)/2)/datarate;
    double x = ((a + prevAccX)/4)/(datarate * datarate) + m_velocityX + m_movement;
    if(v <= stationaryAccXThresh)
    {
        // TODO: save action
        v = 0;
        x = 0;
        setgyroActive(true);
        state = Initial;
    }
    prevAccX = a;
    setvelocityX(v);
    setMovement(x);
}

void DataReadingHandler::handleMovementY(double a)
{
    a -= accYnoise;
    double v = m_velocityY + ((a + prevAccY)/2)/datarate;
    double x = ((a + prevAccY)/4)/(datarate * datarate) + m_velocityY + m_movement;
    if(v <= stationaryAccYThresh)
    {
        // TODO: save action
        v = 0;
        x = 0;
        setgyroActive(true);
        state = Initial;
    }
    prevAccY = a;
    setvelocityY(v);
    setMovement(x);
}

void DataReadingHandler::handleRotation(double gyroV)
{
    gyroV -= rotationNoise;
    double teta = m_rotationZ + ((gyroV + prevRotation)/2)/datarate;
    if(gyroV <= rotationThresh)
    {
        //TODO: save action
        teta = 0;
        setaccActive(true);
        state = Initial;
    }
    prevRotation = gyroV;
    setRotationZ(teta);}

void DataReadingHandler::updateCalibrationInfo(double newData, double &sum, double &count)
{
    sum += newData;
    count++;
    if(count > 100)
    {
        stopCalibration();
    }
}

void DataReadingHandler::stopCalibration()
{
    rotationNoise = rotationSum / rotationCount;
    accXnoise = accXSum / accXCount;
    accYnoise = accYSum / accYCount;
    QString cal = "Rotation noise: " + QString::number(rotationNoise) + "\nAccX noise: " + QString::number(accXnoise) + "\nAccY noise: " + QString::number(accYnoise);
    setCalibration(cal);
    state = Idle;

}


QString DataReadingHandler::calibration() const
{
    return m_calibration;
}

void DataReadingHandler::setCalibration(const QString &newCalibration)
{
    if (m_calibration == newCalibration)
        return;
    m_calibration = newCalibration;
    emit calibrationChanged();
}
