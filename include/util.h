#pragma once
#include "container.h"
#include <QString>

bool isDockerRunning();
QString runDocker(const QStringList& args);
bool getStat(Container& c);
bool getInspect(Container& c);