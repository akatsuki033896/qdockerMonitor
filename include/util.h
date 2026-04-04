#pragma once
#include "container.h"
#include <QString>

QString runDocker(const QStringList& args);
bool getStat(Container& c);
bool getInspect(Container& c);