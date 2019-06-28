#ifndef FETCHER_H
#define FETCHER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class Fetcher : public QObject
{
    Q_OBJECT
public:
    explicit Fetcher(QObject *parent, QString &baseAPIUrl);
    ~Fetcher();
    QJsonDocument fetch(QNetworkRequest &request);

private:
    QString baseAPIUrl;
    QNetworkAccessManager *nam;
    QNetworkReply *reply;

signals:

public slots:
};

#endif // FETCHER_H
