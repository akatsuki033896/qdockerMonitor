#pragma once
#include "container.h"
#include <QString>
#include <QLabel>

bool isDockerRunning();
QString runDockerCLI(const QStringList& args);
QString runDockerProcess(const QStringList& args);
void printInfo(const Container& c);