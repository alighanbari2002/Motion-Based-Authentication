#include "tcpsocketrec.h"
#include <QPointF>
#include <QVector>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QChartView>
#include <QChart>
#include <QMainWindow>
#include <fstream>


tcpsocketrec::tcpsocketrec(QHostAddress addr, quint16 port)
{

    _server = new QTcpServer();

    QObject::connect(_server, &QTcpServer::pendingConnectionAvailable,
                     this, &tcpsocketrec::onConnectionAvailable);

    if(_server->listen(addr, port))
    {
        qDebug() << "address: " << addr << "\nport: " << port;
    }
}

tcpsocketrec::~tcpsocketrec()
{
    qDebug() << "Server Closed!";
    _server->close();
    delete _server;
}

void tcpsocketrec::drawFromQJsonDocument(QJsonDocument recDoc)
{
    QJsonObject recObject = recDoc.object();

    qDebug() << recObject;
    if(recObject.value("action").toInt() == 0)
    {
        drawAccelerationChart(recObject.value("accelerations").toArray(),
                              recObject.value("velocities").toArray(),
                              recObject.value("movements").toArray(),
                              DATARATE);
    }
    else
    {
        // TODO: rotation
    }
}

void tcpsocketrec::drawAccelerationChart(QJsonArray accs,
                                         QJsonArray vels,
                                         QJsonArray movs,
                                         double datarate)
{
    double counter = 0;
    QSplineSeries* a_series = new QSplineSeries();
    for(const auto &acc : accs.toVariantList())
    {
        a_series->append(counter / datarate, acc.toDouble());
    }
    QChart* a_chart = new QChart();
    a_chart->legend()->hide();
    a_chart->addSeries(a_series);
    a_chart->createDefaultAxes();
    a_chart->setTitle("Acceleration");
    QMainWindow window;
    QChartView* a_chart_view = new QChartView(a_chart);
    window.setCentralWidget(a_chart_view);
    window.show();
}

// Intended only for one client use
void tcpsocketrec::onConnectionAvailable()
{
    _socket = _server->nextPendingConnection();
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &tcpsocketrec::onNewMessage);
}

void tcpsocketrec::onNewMessage()
{
    QJsonDocument jsdocrec;
    jsdocrec = QJsonDocument::fromJson(_socket->readAll());
    std::ofstream file;
    file.open("./sensor_diag_info.txt", std::ios_base::app | std::ios_base::out);
    if(file.is_open())
    {
        file << jsdocrec.toJson(QJsonDocument::JsonFormat::Indented).toStdString();
    }
    else
    {
        qDebug () << "File not open!";
    }
    // drawFromQJsonDocument(jsdocrec);
    return;
}

void tcpsocketrec::serverWaitConnection()
{
    if(_server->waitForNewConnection())
    {
        qDebug() << "New Connection";
    }
    else
    {
        qDebug() << "No New Connection";
    }
}
