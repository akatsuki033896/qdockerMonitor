#pragma once
#include <Qstring>

struct ProcessInfo {
    QString uid;
    QString pid;
    QString ppid;
    QString cpu;
    QString stime;
    QString tty;
    QString time;
    QString cmd;
};