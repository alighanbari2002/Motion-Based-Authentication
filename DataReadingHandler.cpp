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

real DataReadingHandler::movement() const
{
    return m_movement;
}

void DataReadingHandler::setMovement(real newMovement)
{
    if (m_movement == newMovement)
        return;
    m_movement = newMovement;
    emit movementChanged();
}

real DataReadingHandler::velocityX() const
{
    return m_velocityX;
}

void DataReadingHandler::setvelocityX(real newVelocityX)
{
    if (m_velocityX == newVelocityX)
        return;
    m_velocityX = newVelocityX;
    emit velocityXChanged();
}

real DataReadingHandler::velocityY() const
{
    return m_velocityY;
}

void DataReadingHandler::setvelocityY(real newVelocityY)
{
    if (m_velocityY == newVelocityY)
        return;
    m_velocityY = newVelocityY;
    emit velocityYChanged();
}

real DataReadingHandler::rotationZ() const
{
    return m_rotationZ;
}

void DataReadingHandler::setRotationZ(real newRotationZ)
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
    double v = (a + prevAccX)/2;
    setvelocityX(v);
}

void DataReadingHandler::handleMovementY(double a)
{
    setvelocityY((a + prevAccY)/2);
}

void DataReadingHandler::handleRotation(double gyroV)
{
    double v =
}
