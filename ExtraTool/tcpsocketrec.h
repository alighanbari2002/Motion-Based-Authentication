#ifndef TCPSOCKETREC_H
#define TCPSOCKETREC_H

#include <QObject>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>


class tcpsocketrec : public QObject
{
    Q_OBJECT
public:
    tcpsocketrec(QHostAddress addr, quint16 port);
    ~tcpsocketrec();

private:
    QTcpServer* _server;
    QTcpSocket* _socket;

    const double DATARATE = 25;

    void drawFromQJsonDocument(QJsonDocument recDoc);
    void drawAccelerationChart(QJsonArray accs, QJsonArray vels,
                               QJsonArray movs, double datarate);
    void drawRotation(QJsonArray rots, QList<double> rotVels,
                      double datarate);

private slots:
    void onConnectionAvailable();
    void onNewMessage();

public:
    void serverWaitConnection();
};

#endif // TCPSOCKETREC_H
