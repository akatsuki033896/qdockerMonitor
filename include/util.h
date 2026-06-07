#pragma once
#include "container.h"
#include <QString>
#include <QLabel>

bool isDockerRunning();
QString runDockerCLI(const QStringList& args);
bool getStat(Container& c);
bool getInspect(Container& c);
void printInfo(const Container& c);
bool getProcess(const Container& c);