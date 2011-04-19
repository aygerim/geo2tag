#include "LoginWidget.h"
#include "Channel.h"
#include "defines.h"
#include <QString>
#include <syslog.h>
//#include "OpenStreetMap.h"
//#include "YandexMap.h"
#include <QObject>
#include <Wt/WLabel>
#include <Wt/WText>
#include <Wt/WBreak>

LoginWidget::LoginWidget(WContainerWidget *parent)
    : WContainerWidget(parent)
{
    this->setId("login_widget");
    this->setStyleClass("login_widget_style");
    WText *intro = new WText("Hello! Please, login into Geo2tag "
                             "or view marks from public channels",
                             this);
    intro->setId("intro");
//    WBreak *break1 = new WBreak(this);
    WLabel *usernameLabel = new WLabel("Username:", this);
    usernameEdit = new WLineEdit(this);
    usernameLabel->setBuddy(usernameEdit);
    WLabel *passwordLabel = new WLabel("Password:", this);
    //m_loginQuery=new LoginQuery();
    passwordEdit = new WLineEdit(this);
    passwordEdit->setEchoMode(WLineEdit::Password);
    passwordLabel->setBuddy(passwordEdit);
    loginButton = new WPushButton("Login", this);
//    WBreak *break2 = new WBreak(this);
    map = new WGoogleMap(this);
    map->setMinimumSize(WLength(300), WLength(400));
    map->setMaximumSize(WLength(500), WLength(400));
    map->setCenter(Wt::WGoogleMap::Coordinate(60, 30));
    map->enableScrollWheelZoom();
//    WBreak *break3 = new WBreak(this);
//    WOpenStreetMap *map2 = new WOpenStreetMap(this);
//    map2->setMinimumSize(WLength(300), WLength(400));
//    //map2->setMaximumSize(WLength(500), WLength(400));
//
//    WBreak *break4 = new WBreak(this);
//    WYandexMap *map3 = new WYandexMap(this);
//    map3->setMinimumSize(WLength(300), WLength(400));

  //  loginButton->clicked().connect(this, &LoginWidget::loginClicked);
    fillMap();
    initCons();
//    m_con=new Connector<LoginWidget>(&m_loginQuery,LoginQueryConnected,&LoginWidget::userRecieved,this);

    this->setStyleClass("login_wigdet");
}

void LoginWidget::initCons()
{
	loginButton->clicked().connect(this,&LoginWidget::loginClicked);
	syslog(LOG_INFO,"trying create LoginWidgetConnector object");
	m_connector=new LoginWidgetConnector(&m_loginQuery,this,&LoginWidget::userRecieved);
	syslog(LOG_INFO,"successfull creation");
}

void LoginWidget::fillMap()
{
/*    QSharedPointer<DataMarks> marks = common::DbSession::
                                          getInstance().getMarks();
    map->clearOverlays();
    for (int i = 0; i < marks->size(); i++)
    {
        map->addMarker(WGoogleMap::
                       Coordinate(marks->at(i)->getLatitude(),
                                  marks->at(i)->getLongitude()),
                       marks->at(i)->getDescription());
    }*/
}

void LoginWidget::loginClicked()
{
    QString name = QString(usernameEdit->text().toUTF8().c_str());
    QString pass = QString(passwordEdit->text().toUTF8().c_str());
    syslog(LOG_INFO,"LoginWidget::loginClicked(), %s - %s",usernameEdit->text().toUTF8().c_str(),passwordEdit->text().toUTF8().c_str());
    syslog(LOG_INFO,"Server url: %s, server port: %i",getServerUrl().toStdString().c_str(),getServerPort());
    m_loginQuery.setQuery(name,pass);
    m_loginQuery.doRequest();
	
}

void LoginWidget::userRecieved(){
	syslog(LOG_INFO,"LoginWidget::userRecieved()");
	QSharedPointer<User> us=this->m_loginQuery.getUser();
	loginSuccessful.emit(us);

//loginSuccessful.emit(std::string(DEFAULT_TOKEN));
}
