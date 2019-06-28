#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QEventLoop>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QMenu>
#include <QProcess>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, QString &clientUrl, QString &configDir, void (*launch) ());
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString clientUrl;
    QString configDir;
    void (*_launch) ();

public slots:
    void setWindowTitleByEngine(QString title);
    void author(QString author);
    void save(QString username, QString password, QString requiredServer, bool fullScreen, QString lang);
    void launch() {
        _launch();
    }
    void close() {
        exit(0);
    }
};

#endif // MAINWINDOW_H
