#include "DataReadingHandler.h"
#include "defs.h"
#include <QDebug>
#include <numeric>

DataReadingHandler::DataReadingHandler()
{
    #ifdef SENSOR_DIAG
        _diagsend = new DiagnosticSend();
    #endif
}

DataReadingHandler::~DataReadingHandler()
{
    #ifdef SENSOR_DIAG
        delete _diagsend;
    #endif
}

void DataReadingHandler::accReading(double accX, double accY)
{
    double inputx;
    double inputy;

    if(accXList.count() < System::SAMPLE_COUNT || accYList.count() < System::SAMPLE_COUNT)
    {
        accXList.append(accX);
        accYList.append(accY);
        return;
    }
    else
    {
        accX = std::accumulate(accXList.begin(), accXList.end(), 0.0) / System::SAMPLE_COUNT;
        accY = std::accumulate(accYList.begin(), accYList.end(), 0.0) / System::SAMPLE_COUNT;
        accXList.clear();
        accYList.clear();
    }

    if(accX <= accXmax && accX >= accXmin)
    {
        inputx = 0;
    }
    else
    {
        inputx = accX - accXnoise;
    }
    if(accY <= accYmax && accY >= accYmin)
    {
        inputy = 0;
    }
    else
    {
        inputy = accY - accYnoise;
    }

    if(state != Calibration && state != Idle)
    {
        setfilteredX(inputx);
        setfilteredY(inputy);
    }

    if(state == Idle)
    {
        setaccActive(false);
        setgyroActive(false);
        return;
    }
    else if(state == Calibration)
    {
        updateCalibrationInfo(accX, accXSum, accXCount, accXmax, accXmin);
        updateCalibrationInfo(accY, accYSum, accYCount, accYmax, accYmin);
    }
    else if(state == Initial)
    {
        if((fabs(inputx) > Thresh::START_ACC) || (fabs(inputy) > Thresh::START_ACC))
        {
            if(fabs(inputx) > fabs(inputy))
            {
                state = MoveX;
                // setgyroActive(false);
                currentDirection = calculateDirection(inputx);
                handleMovement(inputx, m_velocityX, prevAccX, m_movement,
                               Thresh::STOP_ACC_X, countzeroX, true);
            }
            else
            {
                state = MoveY;
                // setgyroActive(false);
                currentDirection = calculateDirection(inputy);
                handleMovement(inputy, m_velocityY, prevAccY, m_movement, Thresh::STOP_ACC_Y,
                               countzeroY, false);
            }
        }
    }
    else if(state == MoveX)
    {
        handleMovement(inputx, m_velocityX, prevAccX, m_movement,
                       Thresh::STOP_ACC_X, countzeroX, true);
    }
    else if(state == MoveY)
    {
        handleMovement(inputy, m_velocityY, prevAccY, m_movement, Thresh::STOP_ACC_Y,
                       countzeroY, false);
    }
    else
    {
        return;
    }
}

DataReadingHandler::MoveDirection DataReadingHandler::calculateDirection(double input)
{
    if (state == MoveX) {
        if (input > 0) {
            switch (currentrotation) {
            case 0:
                return Right;
            case 90:
                return Up;
            case 180:
                return Left;
            case -90:
                return Down;
            default:
                return Right;
            }
        } else {
            switch (currentrotation) {
            case 0:
                return Left;
            case 90:
                return Down;
            case 180:
                return Right;
            case -90:
                return Up;
            default:
                return Left;
            }
        }
    } else {
        if (input > 0) {
            switch (currentrotation) {
            case 0:
                return Up;
            case 90:
                return Left;
            case 180:
                return Down;
            case -90:
                return Right;
            default:
                return Up;
            }
        } else {
            switch (currentrotation) {
            case 0:
                return Down;
            case 90:
                return Right;
            case 180:
                return Up;
            case -90:
                return Left;
            default:
                return Down;
            }
        }
    }

}

void DataReadingHandler::gyroReading(double gyroV)
{
    if(gyroList.count() < System::SAMPLE_COUNT)
    {
        gyroList.append(gyroV);
        return;
    }
    else
    {
        gyroV = std::accumulate(gyroList.begin(), gyroList.end(), 0.0) / System::SAMPLE_COUNT;
        gyroList.clear();
    }

    if(gyroV <= rotationMax && gyroV >= rotationMin)
    {
        gyroV = 0;
    }
    else
    {
        gyroV = gyroV - rotationNoise;
    }

    if(state != Calibration)
    {
        setfilteredZ(gyroV);
    }

    if(state == Idle)
    {
        setaccActive(false);
        setgyroActive(false);
        return;
    }
    else if(state == Calibration)
    {
        updateCalibrationInfo(gyroV, rotationSum, rotationCount, rotationMax, rotationMin);
    }
    else if(state == Initial || state == MoveX || state == MoveY)
    {
        if(fabs(gyroV) > Thresh::START_ROTATION)
        {
            if(state == MoveX || state == MoveY)
            {
                setvelocityY(0.0);
                setMovement(0.0);
                setvelocityX(0.0);
            }
            state = Rotation;
            DataReadingHandler::handleRotation(gyroV);
        }
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
    if(state == Idle)
    {
        setaccActive(true);
        setgyroActive(true);
        state = Initial;
        authpattern = false;
        authSource.startNewPattern();
    }
}

void DataReadingHandler::stopPattern()
{
    setaccActive(false);
    setgyroActive(false);
    state = Idle;

    setMovement(0);
    setvelocityX(0);
    setvelocityY(0);
    setRotationZ(0);
    currentrotation = 0;
}

void DataReadingHandler::startAuthentication()
{
    if(state ==  Idle)
    {
        setaccActive(true);
        setgyroActive(true);
        state = Initial;
        authpattern = true;
        toBeAuthed.startNewPattern();
    }
}

void DataReadingHandler::stopAuthentication()
{
    setaccActive(false);
    setgyroActive(false);
    state = Idle;
    setAuthresult(Pattern::isPatternMatch(authSource, toBeAuthed));
    setMovement(0);
    setvelocityX(0);
    setvelocityY(0);
    setRotationZ(0);
    currentrotation = 0;

}


void DataReadingHandler::startCalibration()
{
    setaccActive(true);
    setgyroActive(true);
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
    accXmax = -100;
    accXmin = 100;
    accYmin = 100;
    accYmax = -100;
    rotationMax = -100;
    rotationMin = 100;
}

void DataReadingHandler::handleMovement(double a, double &velocity, double &prevAcc, double &movement,
                                        const double STOP_ACC, int &countZero, bool isX)
{
    double v = velocity + ((a + prevAcc)/2) / System::SAMPLE_DATARATE;
    double x = ((a + prevAcc)/4) / (System::SAMPLE_DATARATE * System::SAMPLE_DATARATE) +
                velocity/System::SAMPLE_DATARATE + movement;

    #ifdef SENSOR_DIAG
        _diagsend->accelerations.append(QJsonValue(a));
        _diagsend->velocities.append(QJsonValue(v));
        _diagsend->movements.append(QJsonValue(x));
    #endif

    if(fabs(a) < STOP_ACC)
    {
        countZero += 1;
        endmovement += ((a + prevAcc)/4) / (System::SAMPLE_DATARATE * System::SAMPLE_DATARATE) +
                        velocity/System::SAMPLE_DATARATE;
    }
    else
    {
        countZero = 0;
        endmovement = 0;
    }

    prevAcc = a;
    if(countZero >= 3)
    {
        #ifdef SENSOR_DIAG
                _diagsend->sendDiagInfo(DiagnosticSend::DiagInfoMode::Acceleration);
                _diagsend->clearLists();
        #endif

        x -= endmovement;
        auto it = DirectionMap.find(currentDirection);

        if(authpattern)
            toBeAuthed.addNewSequence(x, it->second , currentrotation);
        else
            authSource.addNewSequence(x, it->second , currentrotation);

        QString newmove = "Movement: " + QString::number(x) + " Direction: " + it->second +
                          " angle: " +
                          QString::number(currentrotation);
        setNewpattern(newmove);
        v = 0;
        x = 0;
        state = Initial;
        prevAcc = 0;
        countZero = 0;
        endmovement = 0.0;
    }
    if(isX)
    {
        setvelocityX(v);
    }
    else
    {
        setvelocityY(v);
    }
    setMovement(x);
}

void DataReadingHandler::handleRotation(double gyroV)
{
    double teta = m_rotationZ + ((gyroV + prevRotation)/2) / System::SAMPLE_DATARATE;

    #ifdef SENSOR_DIAG
        _diagsend->rotations.append(QJsonValue(teta));
        _diagsend->rotationVelocities.append(QJsonValue(gyroV));
    #endif

    prevRotation = gyroV;
    // countz += 1;
    if(fabs(gyroV) < Thresh::STOP_ROTATION)
        countzeroZ += 1;
    else
        countzeroZ = 0;
    if(countzeroZ >= 3)
    {
        #ifdef SENSOR_DIAG
            _diagsend->sendDiagInfo(DiagnosticSend::DiagInfoMode::Rotation);
            _diagsend->clearLists();
        #endif

        int actual = settotalrotation(teta);

        QString newmove = "Rotation: " + QString::number(teta) + "actual: " + QString::number(actual);
        setNewpattern(newmove);
        teta = 0;
        // setaccActive(true);
        state = Initial;
        prevRotation = 0;
        // countz = 0;
        countzeroZ = 0;

        setMidMoveCal(true);
        DataReadingHandler::startCalibration();
    }
    setRotationZ(teta);
}

int DataReadingHandler::settotalrotation(double teta)
{
    int roundup = 0;
    while(teta > 360)
        teta -= 360;

    while(teta < 0)
        teta += 360;

    if(teta < 45 || teta >= 315)
        roundup = 0;
    else if (teta >= 45 && teta < 135)
        roundup = 90;
    else if(teta >= 135 && teta < 225)
        roundup = 180;
    else if(teta >= 225 && teta < 315)
        roundup = -90;

    currentrotation += roundup;
    if(currentrotation >= 360)
        currentrotation -= 360;
    if(currentrotation < -90)
        currentrotation += 360;
    return roundup;
}

void DataReadingHandler::updateCalibrationInfo(double newData, double &sum, double &count, double &max, double &min)
{
    sum += newData;
    count++;
    if(newData > max)
    {
        max = newData;
    }
    if(newData < min)
    {
        min = newData;
    }
    if(count > Thresh::CALIBRATION_LIMIT)
    {
        DataReadingHandler::stopCalibration();
    }
}

void DataReadingHandler::stopCalibration()
{
    rotationNoise = rotationSum / rotationCount;
    accXnoise = accXSum / accXCount;
    accYnoise = accYSum / accYCount;

    accXmax = accXmax * rangecoef;
    accXmin = accXmin * rangecoef;
    accYmax = accYmax * rangecoef;
    accYmin = accYmin * rangecoef;
    rotationMax = rotationMax * rangecoef;
    rotationMin = rotationMin * rangecoef;

    QString cal = "Rotation noise: " + QString::number(rotationNoise) + "\nAccX noise: " +
                  QString::number(accXnoise) + "\nAccY noise: " + QString::number(accYnoise);
    setCalibration(cal);
    calDoneCount += 1;
    if(calDoneCount % 3 == 0)
    {
        if(midMoveCal())
        {
            state = Initial;
            setMidMoveCal(false);
        }
        else
        {
            state = Idle;
        }
    }
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


double DataReadingHandler::filteredX() const
{
    return m_filteredX;
}

void DataReadingHandler::setfilteredX(double newFilteredX)
{
    if (qFuzzyCompare(m_filteredX, newFilteredX))
        return;
    m_filteredX = newFilteredX;
    emit filteredXChanged();
}

double DataReadingHandler::filteredY() const
{
    return m_filteredY;
}

void DataReadingHandler::setfilteredY(double newFilteredY)
{
    if (qFuzzyCompare(m_filteredY, newFilteredY))
        return;
    m_filteredY = newFilteredY;
    emit filteredYChanged();
}

double DataReadingHandler::filteredZ() const
{
    return m_filteredZ;
}

void DataReadingHandler::setfilteredZ(double newFilteredZ)
{
    if (qFuzzyCompare(m_filteredZ, newFilteredZ))
        return;
    m_filteredZ = newFilteredZ;
    emit filteredZChanged();
}

QString DataReadingHandler::newpattern() const
{
    return m_newpattern;
}

void DataReadingHandler::setNewpattern(const QString &newNewpattern)
{
    m_newpattern = newNewpattern;
    emit newpatternChanged();
}

bool DataReadingHandler::authresult() const
{
    return m_authresult;
}

void DataReadingHandler::setAuthresult(const bool &newAuthresult)
{
    m_authresult = newAuthresult;
    emit authresultChanged();
}

bool DataReadingHandler::midMoveCal() const
{
    return m_midMoveCal;
}

void DataReadingHandler::setMidMoveCal(bool newMidMoveCal)
{
    if (m_midMoveCal == newMidMoveCal)
        return;
    m_midMoveCal = newMidMoveCal;
    emit midMoveCalChanged();
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
