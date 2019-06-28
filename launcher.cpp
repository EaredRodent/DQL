#include "launcher.h"

Launcher::Launcher(QObject *parent, QString &configDir) : QObject(parent)
{
    this->configDir = configDir;
}

void Launcher::Launch() {

}
