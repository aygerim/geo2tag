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
#include <QDir>
#include <QFileDialog>

#include "tracker.h"
#include "inc/ByteSpinBox.h"
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
    layout_cache->addRow("Cache path", cache_path);
    layout_cache->addRow("Max cache size", m_cacheMaxSize = new QSpinBox(w_cache));

    m_cacheType->addItem("None", 0);
    m_cacheType->addItem("Network cache", 1);
    m_cacheType->addItem("Disk cache", 2);
    m_cacheType->setCurrentIndex(-1);

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
    connect(m_cacheType, SIGNAL(currentIndexChanged(int)), this, SLOT(onCacheTypeChanged(int)));
    connect(m_cachePathButton, SIGNAL(clicked()), this, SLOT(onCachePathButtonClick()));
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
    QNetworkProxy::ProxyType proxy_type = (QNetworkProxy::ProxyType)m_proxyType->itemData(index).toInt();
    bool enabled_flag = (proxy_type != QNetworkProxy::DefaultProxy && proxy_type != QNetworkProxy::NoProxy);
    m_proxyHostEdit->setEnabled(enabled_flag);
    m_proxyPortEdit->setEnabled(enabled_flag);
}

void OptionsWidget::onCacheTypeChanged(int index)
{
    m_cachePath->setEnabled( m_cacheType->itemData(index).toInt() != 0 );
    m_cachePathButton->setEnabled( m_cachePath->isEnabled() );
}

void OptionsWidget::onShowPasswordChecked(bool checked)
{
    if(checked)
        m_passwordEdit->setEchoMode(QLineEdit::Normal);
    else
        m_passwordEdit->setEchoMode(QLineEdit::Password);
}

void OptionsWidget::onCachePathButtonClick()
{
    qDebug() <<"click";
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::DirectoryOnly);
    fd.setOptions(QFileDialog::ShowDirsOnly);
    connect(&fd, SIGNAL(fileSelected(QString)), this, SLOT(onCachePathSelected(QString)));
    fd.exec();
}

void OptionsWidget::onCachePathSelected(QString path)
{
    m_cachePath->setText(path + "/");
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
    m_nameEdit->setText(m_settings.value("user").toString());
    m_passwordEdit->setText(m_settings.value("password").toString());
    m_channelEdit->setText(m_settings.value("channel").toString());

    m_proxyType->setCurrentIndex(m_proxyType->findData(m_settings.value("proxy_type").toInt()));
    m_proxyHostEdit->setText(m_settings.value("proxy_host").toString());
    m_proxyPortEdit->setValue(m_settings.value("proxy_port").toInt());

    m_serverUrlEdit->setText(getServerUrl());
    m_serverPortEdit->setValue(getServerPort());

    m_cacheType->setCurrentIndex(m_cacheType->findData(m_settings.value("cache_type", 0).toInt()));
    m_cachePath->setText(m_settings.value("cache_path", QDir::homePath() + "/.geo2tag/uploaded_maps/").toString());
}

void OptionsWidget::createSettings()
{
    m_settings.setValue("channel", m_channelEdit->text());
    m_settings.setValue("user", m_nameEdit->text());
    m_settings.setValue("password", m_passwordEdit->text());

    QNetworkProxy::ProxyType proxy_type = (QNetworkProxy::ProxyType)m_proxyType->itemData(m_proxyType->currentIndex()).toInt();
    m_settings.setValue("proxy_type", proxy_type);
    if(proxy_type != QNetworkProxy::DefaultProxy && proxy_type != QNetworkProxy::NoProxy)
    {
        m_settings.setValue("proxy_host", m_proxyHostEdit->text());
        m_settings.setValue("proxy_port", m_proxyPortEdit->value());
    }
    else
    {
        m_settings.remove("proxy_host");
        m_settings.remove("proxy_port");
    }

    setServerUrl(m_serverUrlEdit->text());
    setServerPort(m_serverPortEdit->value());

    m_settings.setValue("cache_type", m_cacheType->itemData(m_cacheType->currentIndex()).toInt());
    if(m_cacheType->itemData(m_cacheType->currentIndex()).toInt() > 0)
        m_settings.setValue("cache_path", m_cachePath->text());
    else
        m_settings.remove("cache_path");
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
    m_cacheType->setCurrentIndex(m_cacheType->findData(0));
    m_cachePath->setText(QDir::homePath() + "/.geo2tag/uploaded_maps/");
}
