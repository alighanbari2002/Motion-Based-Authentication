#include "DiagnosticSend.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <qDebug>

DiagnosticSend::DiagnosticSend()
{
    _socket = new QTcpSocket();

    QObject::connect(_socket, &QTcpSocket::connected, this,
                     &DiagnosticSend::onConnected);

    QObject::connect(_socket, &QTcpSocket::errorOccurred, this,
                     &DiagnosticSend::onError);

    _socket->connectToHost(HOST, PORT);
    if(_socket->waitForConnected(-1))
    {
        qDebug("Connected!");
    }
}

DiagnosticSend::~DiagnosticSend()
{
    _socket->close();
    delete _socket;
}

void DiagnosticSend::clearLists()
{
    clearQJsonArray(accelerations);
    clearQJsonArray(velocities);
    clearQJsonArray(movements);
    clearQJsonArray(rotationVelocities);
    clearQJsonArray(rotations);
}

void DiagnosticSend::sendDiagInfo(DiagInfoMode mode)
{
    QJsonObject sendjson{
        {"action", mode}
    };
    if(mode == DiagInfoMode::Acceleration)
    {
        sendjson.insert("accelerations", accelerations);
        sendjson.insert("velocities", velocities);
        sendjson.insert("movements", movements);
    }
    else
    {
        sendjson.insert("rotationVelocities", rotationVelocities);
        sendjson.insert("rotations", rotations);
    }
    _socket->write(QJsonDocument(sendjson).toJson(QJsonDocument::JsonFormat::Compact));
    _socket->flush();
}

void DiagnosticSend::onConnected()
{
    qDebug() << "successfull";
}

void DiagnosticSend::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}

void DiagnosticSend::clearQJsonArray(QJsonArray &jsarr)
{
    while(jsarr.count())
        jsarr.pop_back();
}


