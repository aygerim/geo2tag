#ifndef REQUESTSENDER_H
#define REQUESTSENDER_H

#include <QTcpSocket>

class RequestSender : public QObject
{
    Q_OBJECT

    QTcpSocket *m_socket;

public:

    RequestSender(QTcpSocket *socket, QObject *parent = 0);

    QTcpSocket* getSocket();
    void setSocket(QTcpSocket *socket);

    bool login(QString name, QString password);
    bool setChannel(QString name, QString key);
    bool addChannel(QString name, QString key);
    bool start();
    bool stop();
    QString status();

};

#endif // REQUESTSENDER_H
