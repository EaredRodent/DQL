#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>

class Launcher : public QObject
{
    Q_OBJECT
public:
    explicit Launcher(QObject *parent, QString &configDir);
    void Launch();

private:
    QString configDir;

signals:

public slots:
};

#endif // LAUNCHER_H
