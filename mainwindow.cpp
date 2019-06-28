#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, QString &clientUrl, QString &configDir, void (*launch) ()) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->clientUrl = clientUrl;
    this->configDir = configDir;
    this->_launch = launch;
    ui->setupUi(this);
    QWebEngineView *webView = new QWebEngineView(this);
    webView->hide();
    ui->verticalLayout->addWidget(webView);
    QWebChannel *webChannel = new QWebChannel(this);
    QObject::connect(webView, SIGNAL(titleChanged(QString)), SLOT(setWindowTitleByEngine(QString)));
    webChannel->registerObject("cpp", this);
    webView->page()->setWebChannel(webChannel);
    webView->page()->profile()->setHttpUserAgent("DSO_QuickLauncher");
    QEventLoop el;
    QObject::connect(webView, SIGNAL(loadFinished(bool)), &el, SLOT(quit()));
    webView->load(QUrl(this->clientUrl));
    el.exec();
    qDebug() << webView->page()->title();
    if(!webView->page()->title().contains("DSO_QuickLauncher")) {
        QMessageBox::critical(0, "Network problems.", webView->page()->title());
        exit(0);
    }
    setWindowTitle(webView->page()->title());
    webView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save(QString username, QString password, QString requiredServer, bool fullScreen, QString lang) {
    qDebug() << "FS: " << fullScreen;
    QFile file(configDir);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ts << username << endl << password << endl << requiredServer << endl
           << QString(fullScreen ? "fullscreen=true" : "fullscreen=false") << endl
           << lang << endl;
        file.close();
    } else {
        QMessageBox::critical(0, "Save error.",
                              "Something is blocking file creation.\n"
                              "QFile (QIODevice::WriteOnly | QIODevice::Truncate) failed.");
        exit(0);
    }
}

void MainWindow::setWindowTitleByEngine(QString title) {
    setWindowTitle(title);
}

void MainWindow::author(QString author) {
    QProcess process;
    QStringList stringList;
    stringList.append("/c");
    stringList.append(QString("start %1").arg(author));
    process.startDetached("cmd.exe", stringList);
}

