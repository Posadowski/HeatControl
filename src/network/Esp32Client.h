#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "DeviceStatus.h"

class Esp32Client : public QObject
{
    Q_OBJECT

public:
    explicit Esp32Client(QObject *parent = nullptr);

    void requestStatus();
signals:    
    void communicationError(const QString &message);
    void statusReceived(DeviceStatus status);
private:
    QNetworkAccessManager m_networkManager;
    QNetworkRequest m_request;
    DeviceStatus m_status;    
};