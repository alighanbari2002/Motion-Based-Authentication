#ifndef DIAGNOSTICSEND_H
#define DIAGNOSTICSEND_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QJsonArray>

#define SENSOR_DIAG

class DiagnosticSend : public QObject
{
    Q_OBJECT
public:
    DiagnosticSend();
    ~DiagnosticSend();
    enum DiagInfoMode { Acceleration, Rotation };

    void clearLists();
    void sendDiagInfo(DiagInfoMode mode);


    QJsonArray accelerations;
    QJsonArray velocities;
    QJsonArray movements;
    QJsonArray rotationVelocities;
    QJsonArray rotations;

private slots:
    void onConnected();
    void onError(QTcpSocket::SocketError socketError);

private:
    const QHostAddress HOST{"192.168.100.98"};
    const quint16 PORT = 20080;

    QTcpSocket* _socket;

    void clearQJsonArray(QJsonArray &jsarr);
};

#endif // DIAGNOSTICSEND_H
