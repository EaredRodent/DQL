#include "fetcher.h"

Fetcher::Fetcher(QObject *parent, QString &baseAPIUrl) : QObject(parent)
{
    this->baseAPIUrl = baseAPIUrl;
}

QJsonDocument Fetcher::fetch(QNetworkRequest &request) {
    request.setUrl(QString("%1%2")
                   .arg(baseAPIUrl)
                   .arg(request.url().toString()));
    nam = new QNetworkAccessManager();
    reply = nam->get(request);
    QEventLoop el;
    QTimer timer;
    QObject::connect(reply, SIGNAL(finished()), &el, SLOT(quit()));
    QObject::connect(&timer, SIGNAL(timeout()), &el, SLOT(quit()));
    timer.start(30000);
    el.exec();
    QJsonDocument json;
    if(reply) {
        json = QJsonDocument::fromJson(reply->readAll());
    }
    return json;
}

Fetcher::~Fetcher() {
}
