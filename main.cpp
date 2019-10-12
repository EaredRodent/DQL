#include "mainwindow.h"
#include "fetcher.h"
#include <QApplication>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QStringList>

// Project constants

QString softwareVersion("0.3.2");
QString configureArgName("--configure");
QString configDir(QString("%1/Wn8JWmP2X9").arg(QDir::tempPath()));

#ifdef QT_DEBUG
QString baseAPIUrl("http://dso-client/");
#endif
#ifndef QT_DEBUG
QString baseAPIUrl("https://dso.earedrodent.com/");
#endif
QString clientUrl(baseAPIUrl + "private/dso-quicklauncher");

// =================

Fetcher fetcher(0, baseAPIUrl);

bool isCurrentVersion() {
    QNetworkRequest request(QUrl("v1/start-up"));
    QJsonObject json(fetcher.fetch(request).object());
    QString currentVersion = json.value("version").toString();

    if (currentVersion.isEmpty()) {
        QMessageBox::critical(0, "Network problems.", "Please configure network.");
        return false;
    }

    if(softwareVersion != currentVersion)
    {
        QMessageBox::critical(0, "Please upgrade.", QString("Software version is out of date.\n\n"
                                                            "This version:    %1\n"
                                                            "Current version: %2")
                              .arg(softwareVersion)
                              .arg(currentVersion));
        return false;
    }

    return true;
}

bool isConfigure(int argc, char *argv[]) {
    for(int i = 0; i < argc; i++){
        if(QString(argv[i]) == configureArgName) {
            return true;
        }
    }
    return false;
}

void launch() {
    QFile file(configDir);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Open error.",
                              "Something is blocking file opening.\n"
                              "QFile (QIODevice::ReadOnly) failed.");
        exit(0);
    }
    QTextStream ts(&file);
    ts.setCodec("UTF-8");
    QString username(ts.readLine());
    QString password(ts.readLine());
    QString requiredServer(ts.readLine());
    QString fullScreen(ts.readLine());
    QString lang(ts.readLine());
    file.close();
    QUrl url("v1/dso-user-manager");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("username", username);
    urlQuery.addQueryItem("password", password);
    urlQuery.addQueryItem("requiredServer", requiredServer);
    urlQuery.addQueryItem("lang", lang);
//    urlQuery.addQueryItem("XDEBUG_SESSION_START", "PHPSTORM");
    url.setQuery(urlQuery);
    QNetworkRequest request(url);
    QJsonObject json = fetcher.fetch(request).object();
    QString cmdRun = json.value("scripts").toObject().value("cmdRun").toString();
    if(cmdRun.isEmpty()) {
        QString error;
        QStringList errorHeaders = json.keys();
        for(int i = 0; i < errorHeaders.length(); i++) {
            error.append(errorHeaders[i] + ": " + json.value(errorHeaders[i]).toVariant().toString() + '\n');
        }
        QMessageBox::critical(0, "API error.",
                              QString("Does not contain the required script.\n\n%1")
                              .arg(error));
        exit(0);
    }

    if (fullScreen == "fullscreen=true") {
        cmdRun += " -fullscreen";
    }

    QProcess process;
    QStringList stringList;
    stringList.append("/c");
    stringList.append(QString("start ") + cmdRun);
    process.startDetached("cmd.exe", stringList);
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseOpenGLES);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication::setAttribute(Qt::AA_UseOpenGLES);
    QApplication a(argc, argv);

    if(!isCurrentVersion()) {
        exit(0);
    }

    if(!isConfigure(argc, argv)) {
        launch();
        exit(0);
    }

    MainWindow w(0, clientUrl, configDir, launch);
    w.show();
    return a.exec();
}
