#include "DataReadingHandler.h"

DataReadingHandler::DataReadingHandler() {}

void DataReadingHandler::accReading(double accX, double accY)
{
    if(state == Initial && fabs(prevAccX - accX) > accThresh)
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
    double v = m_velocityX + ((a + prevAccX)/2)/datarate;
    double x = ((a + prevAccX)/4)/(datarate * datarate) + m_velocityX + m_movement;
    if(v <= 0)
    {
        v = 0;
        x = 0;
        setgyroActive(true);
        state = Initial;
    }
    setvelocityX(v);
    setMovement(x);
}

void DataReadingHandler::handleMovementY(double a)
{
    double v = m_velocityY + ((a + prevAccY)/2)/datarate;
    double x = ((a + prevAccY)/4)/(datarate * datarate) + m_velocityY + m_movement;
    if(v <= 0)
    {
        v = 0;
        x = 0;
        setgyroActive(true);
        state = Initial;
    }
    setvelocityY(v);
    setMovement(x);
}

void DataReadingHandler::handleRotation(double gyroV)
{
    double teta = m_rotationZ + ((gyroV + prevRotation)/2)/datarate;
    if(teta <= 0)
    {
        teta = 0;
        setaccActive(true);
        state = Initial;
    }
    setRotationZ(teta);
}

