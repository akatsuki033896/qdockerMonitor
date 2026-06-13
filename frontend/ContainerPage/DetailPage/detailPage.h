//
// Created by akatsuki on 2026/5/30.
//

#ifndef SYSTEMMONITOR_DETAILPAGE_H
#define SYSTEMMONITOR_DETAILPAGE_H
#include <qlabel.h>
#include <qpushbutton.h>
#include <QProgressBar>
#include <QWidget>

#include "container.h"
#include "resChart.h"


class detailPage : public QWidget {
    Q_OBJECT
    QVBoxLayout* layout;

    QLabel* idLabel;
    QLabel* nameLabel;
    QLabel* imageLabel;
    QLabel* statusLabel;

    QLabel* ipLabel;
    QLabel* macLabel;
    QLabel* portsLabel;
    QLabel* mountLabel;

    QProgressBar* cpuBar;
    QProgressBar* memBar;
    QLabel* memDetailLabel;

    resChart* cpuChart;
    resChart* memChart;

    QLabel* status_light;
    QPushButton* pause;
    QPushButton* stop;
    QPushButton* start;
public:
    explicit detailPage(QWidget* parent);
    void updateData(const Container& con);
    void updateChart(double, resChart*, int);
    void updateStatusLight(const QString &status);
    QString formatPortsInline(const std::map<std::string, std::string>& ports);
};
#endif  // SYSTEMMONITOR_DETAILPAGE_H
