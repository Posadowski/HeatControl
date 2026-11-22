#include "Esp32Client.h"
#include <cmath>
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

const QString tmpEspUrl = "http://192.168.1.100/api/status";

Esp32Client::Esp32Client(QObject *parent) : QObject(parent){

}

void Esp32Client::requestStatus(){
    m_request.setUrl(QUrl(tmpEspUrl));
    QNetworkReply *reply = m_networkManager.get(m_request);
    QObject::connect(reply, &QNetworkReply::finished, this, [this, reply](){
            if (reply->error()) {
                const QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
                qWarning() << "Response error code: " << statusCode;
                emit communicationError(QStringLiteral("Response error code:") + statusCode));
            } else {
                QByteArray responseData = reply->readAll();
                QJsonParseError parseError;
                QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);
                if (parseError.error != QJsonParseError::NoError) {
                    qWarning() << "JSON parser error:" << parseError.errorString();
                    emit communicationError(QStringLiteral("JSON parser error: ") + parseError.errorString());
                } else if (!doc.isObject()) {
                    qWarning() << "JSON response is not an object";
                    emit communicationError("JSON response is not an object");
                } else {
                    QJsonObject rootObj;
                    qDebug() << responseData;
                    rootObj = doc.object();
                    struct DeviceStatus tmpStatus;
                    bool allFieldsValid  = true;

                    const QJsonValue temperatureValue = rootObj.value(QStringLiteral("temperature"));
                    const QJsonValue targetTemperatureValue = rootObj.value(QStringLiteral("target_temperature"));
                    const QJsonValue heaterEnabledValue = rootObj.value(QStringLiteral("heater_enabled"));
                    const QJsonValue sensorOkValue = rootObj.value(QStringLiteral("sensor_ok"));

                    if(temperatureValue.isDouble()){
                        tmpStatus.temperature = temperatureValue.toDouble();
                        if(std::isfinite(tmpStatus.temperature) == false) {
                            allFieldsValid  = false;
                        }
                    } else {
                        allFieldsValid  = false;
                    }

                    if(targetTemperatureValue.isDouble()){
                        tmpStatus.target_temperature = targetTemperatureValue.toDouble();
                        if(std::isfinite(tmpStatus.target_temperature) == false) {
                            allFieldsValid  = false;
                        }
                    } else {
                        allFieldsValid  = false;
                    }

                    if(heaterEnabledValue.isBool()){
                        tmpStatus.heater_enabled = heaterEnabledValue.toBool();
                    } else {
                        allFieldsValid  = false;
                    }

                    if(sensorOkValue.isBool()){
                        tmpStatus.sensor_ok = sensorOkValue.toBool();
                    } else {
                        allFieldsValid  = false;
                    }
                    if(allFieldsValid  == true) {
                        m_status = tmpStatus;
                        emit statusReceived(m_status);
                    } else {
                        emit communicationError(QStringLiteral("Invalid or missing fields in JSON response"));
                    }
                }
            }
            reply->deleteLater();
        }
    );
}