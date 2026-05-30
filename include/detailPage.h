//
// Created by akatsuki on 2026/5/30.
//

#ifndef SYSTEMMONITOR_DETAILPAGE_H
#define SYSTEMMONITOR_DETAILPAGE_H
#include "resChart.h"
#include <QWidget>
#include <QProgressBar>
#include "container.h"

class detailPage : public QWidget
{
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
public:
    explicit detailPage(QWidget* parent);
    void updateData(const Container& con);
    void updateChart(double, resChart*, int);
    QString formatPortsInline(const std::map<std::string, std::string>& ports);
};
#endif //SYSTEMMONITOR_DETAILPAGE_H
