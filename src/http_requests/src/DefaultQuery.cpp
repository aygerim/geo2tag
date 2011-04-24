#include <QDebug>
#include <QNetworkConfiguration>
#include <syslog.h>
#include "DefaultQuery.h"
#include "defines.h"

DefaultQuery::DefaultQuery(QObject *parent): QObject(parent),
    m_manager(NULL)
{
}


void DefaultQuery::doRequest()
{

    if(m_manager == NULL)
    {
        qDebug() << "initializeng DefaultQuery::doRequest()";
        m_manager = new QNetworkAccessManager(this);
        connect(m_manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(process(QNetworkReply*)));
        connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(handleError()));
    }


    QNetworkRequest request;

    QUrl url(getUrl());

    //NOTE: for testing
    url="http://tracklife.ru/service";

    // url.setPort(getServerPort());
    request.setUrl(url);

    qDebug() << "doing post to" << url << " with body: " << getRequestBody();
    syslog(LOG_INFO,"posting http request to %s with body %s",
           url.toString().toStdString().c_str(),
           QString(getRequestBody()).toStdString().c_str());

    /*QNetworkReply *reply = */m_manager->post(request, getRequestBody());
    //qDebug() << reply;
    //connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(handleError()));
}

void DefaultQuery::process(QNetworkReply *reply)
{
    processReply(reply);
}

void DefaultQuery::handleError()
{
    syslog(LOG_INFO,"Network error occured while sending request");
    Q_EMIT errorOccured("network error occcured");
}
