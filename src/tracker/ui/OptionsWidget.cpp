#include "OptionsWidget.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QToolBox>
#include <QLabel>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <DaemonManager.h>
#include <QtNetwork/QNetworkProxy>
#include "tracker.h"
#include "defines.h"

void OptionsWidget::applyProxySettings()
{
    QNetworkProxy proxy;
    QNetworkProxy::ProxyType proxy_type;
    proxy_type = (QNetworkProxy::ProxyType) m_proxyType->itemData(m_proxyType->currentIndex()).value<int>();
    proxy.setType(proxy_type);
    proxy.setHostName(m_proxyHostEdit->text());
    proxy.setPort(m_proxyPortEdit->value());
    QNetworkProxy::setApplicationProxy(proxy);
}

OptionsWidget::OptionsWidget(QString productName,QWidget *parent) :
        QScrollArea(parent), m_productName(productName), m_settings(QSettings::SystemScope,"osll",m_productName)
{
    m_widg = new QWidget(this);
    QVBoxLayout * layout = new QVBoxLayout(m_widg);
    QToolBox * tb = new QToolBox(m_widg);

    QWidget * w_login = new QWidget(tb);

    QFormLayout * layout_login = new QFormLayout(w_login);

    layout_login->addRow("Login", m_nameEdit = new QLineEdit(w_login));
    layout_login->addRow("Password", m_passwordEdit = new QLineEdit(w_login));
    layout_login->addWidget(m_passwordCheckBox = new QCheckBox("Show password", w_login));
    layout_login->addRow("Channel name", m_channelEdit = new QLineEdit(w_login));

    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordCheckBox->setChecked(false);

    w_login->setLayout(layout_login);

    QWidget * w_server = new QWidget(tb);

    QFormLayout * layout_server = new QFormLayout(w_server);

    layout_server->addRow("Server", m_serverUrlEdit = new QLineEdit(w_server));
    layout_server->addRow("Port", m_serverPortEdit = new QSpinBox(w_server));

    m_serverPortEdit->setMinimum(0);
    m_serverPortEdit->setMaximum(65535);

    w_server->setLayout(layout_server);

    QWidget * w_proxy = new QWidget(tb);

    QFormLayout * layout_proxy = new QFormLayout(w_proxy);

    layout_proxy->addRow("Proxy type", m_proxyType = new QComboBox(w_proxy));
    layout_proxy->addRow("Proxy host", m_proxyHostEdit = new QLineEdit(w_proxy));
    layout_proxy->addRow("Proxy port", m_proxyPortEdit = new QSpinBox(w_proxy));

    m_proxyType->addItem("DefaultProxy", QNetworkProxy::DefaultProxy);
    m_proxyType->addItem("Socks5Proxy", QNetworkProxy::Socks5Proxy);
    m_proxyType->addItem("NoProxy", QNetworkProxy::NoProxy);
    m_proxyType->addItem("HttpProxy", QNetworkProxy::HttpProxy);
    m_proxyType->addItem("HttpCachingProxy", QNetworkProxy::HttpCachingProxy);
    m_proxyType->addItem("FtpCachingProxy", QNetworkProxy::FtpCachingProxy);
    m_proxyPortEdit->setMinimum(0);
    m_proxyPortEdit->setMaximum(65535);

    w_proxy->setLayout(layout_proxy);

    QWidget * w_cache = new QWidget(tb);

    QFormLayout * layout_cache = new QFormLayout(w_cache);

    QHBoxLayout * cache_path = new QHBoxLayout();
    cache_path->addWidget(m_cachePath = new QLineEdit(w_cache));
    cache_path->addWidget(m_cachePathButton = new QPushButton("Choose dir...", w_cache));

    layout_cache->addRow("Cache type", m_cacheType = new QComboBox(w_cache));
    layout_cache->addWidget(m_cacheOn = new QCheckBox("Enable cache", w_cache));
    layout_cache->addRow("Cache path", cache_path);

    m_cacheType->addItem("None", "None");
    m_cacheType->addItem("Network cache", "Network");
    m_cacheType->addItem("Disk cache", "Disk");

    w_cache->setLayout(layout_cache);

    QHBoxLayout * buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_doneButton = new QPushButton("Done", m_widg));
    buttonLayout->addWidget(m_defaultButton = new QPushButton("Default settings", m_widg));
    buttonLayout->addWidget(m_cancelButton = new QPushButton("Cancel", m_widg));

    tb->addItem(w_login, "Login");
    tb->addItem(w_server, "Server");
    tb->addItem(w_proxy, "Proxy server");
    tb->addItem(w_cache, "Cache");

    layout->addWidget(tb);
    layout->addLayout(buttonLayout);

    m_widg->setLayout( layout);
    this->setWidget(m_widg);
    this->setWidgetResizable(true);
    this->adjustSize();

    connect(m_doneButton, SIGNAL(clicked()), this, SLOT(onDoneClicked()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
    connect(m_defaultButton,SIGNAL(clicked()), this, SLOT(setDefaultSettings()));
    connect(m_proxyType, SIGNAL(currentIndexChanged(int)), this, SLOT(onProxyTypeChanged(int)));
    connect(m_passwordCheckBox, SIGNAL(clicked(bool)), this, SLOT(onShowPasswordChecked(bool)));

    onProxyTypeChanged(m_proxyType->currentIndex());

    if( m_settings.value("magic").toString() != APP_MAGIC )
    {
	setDefaultSettings();
	createSettings();
    }

    initSettings();
    applyProxySettings();
}

QString OptionsWidget::name()
{
    return m_settings.value("user").toString();
}

QString OptionsWidget::password()
{
    return m_settings.value("password").toString();
}

QString OptionsWidget::channel()
{
    return m_settings.value("channel").toString();
}

void OptionsWidget::onDoneClicked()
{
    createSettings();

    applyProxySettings();

    emit done();
}

void OptionsWidget::onCancelClicked()
{
    initSettings();
    emit cancel();
}

void OptionsWidget::onProxyTypeChanged(int index)
{
    bool enabled_flag = index != 0 && index != 2;
    m_proxyHostEdit->setEnabled(enabled_flag);
    m_proxyPortEdit->setEnabled(enabled_flag);
}

void OptionsWidget::onShowPasswordChecked(bool checked)
{
    if(checked)
        m_passwordEdit->setEchoMode(QLineEdit::Normal);
    else
        m_passwordEdit->setEchoMode(QLineEdit::Password);
}

void OptionsWidget::initSettings()
{
    if( m_settings.value("magic").toString() == APP_MAGIC )
    {
        qDebug() << "magic = " << m_settings.value("magic").toString();
        readSettings();
    }
    else
    {
        createSettings();
    }
}

void OptionsWidget::readSettings()
{
    QSettings settings(QSettings::SystemScope,"osll",m_productName);
    m_nameEdit->setText(m_settings.value("user").toString());
    m_passwordEdit->setText(m_settings.value("password").toString());
    m_channelEdit->setText(m_settings.value("channel").toString());
    m_proxyType->setCurrentIndex(m_proxyType->findData(m_settings.value("proxy_type").toInt()));
    m_proxyHostEdit->setText(m_settings.value("proxy_host").toString());
    m_proxyPortEdit->setValue(m_settings.value("proxy_port").toInt());
    m_serverUrlEdit->setText(getServerUrl());
    m_serverPortEdit->setValue(getServerPort());
}

void OptionsWidget::createSettings()
{
    m_settings.setValue("channel", m_channelEdit->text());
    m_settings.setValue("user", m_nameEdit->text());
    m_settings.setValue("password", m_passwordEdit->text());
    m_settings.setValue("proxy_type", m_proxyType->itemData(m_proxyType->currentIndex()).value<int>());
    m_settings.setValue("proxy_host", m_proxyHostEdit->text());
    setServerUrl(m_serverUrlEdit->text());
    setServerPort(m_serverPortEdit->value());
    m_settings.setValue("proxy_port", m_proxyPortEdit->value());
    m_settings.setValue("magic", APP_MAGIC);
}

void OptionsWidget::setDefaultSettings()
{
    m_nameEdit->setText("tracker");
    m_passwordEdit->setText("test");
    m_channelEdit->setText("default");
    m_proxyType->setCurrentIndex(0);
    m_proxyHostEdit->setText("");
    m_proxyPortEdit->setValue(0);
    m_serverUrlEdit->setText("http://tracklife.ru/");
    m_serverPortEdit->setValue(80);
}
