#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QTcpSocket>
#include "tcpsocketrec.h"

int main(int argc, char *argv[])
{
    // QCoreApplication a(argc, argv);
    QApplication a(argc, argv);

    QHostAddress addr;
    quint16 port;
    if(argc != 3)
    {
        addr = QHostAddress("192.168.100.98");
        port = 20080;
    }
    else
    {
        addr = QHostAddress(argv[1]);
        port = QString(argv[2]).toUInt();
    }
    tcpsocketrec tcpsockserver = tcpsocketrec(addr, port);
    tcpsockserver.serverWaitConnection();
    return a.exec();
}
