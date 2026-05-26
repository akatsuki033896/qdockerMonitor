#pragma once
#include <qlineseries.h>

#include <QLabel>
#include <QLineSeries>
#include <QVBoxLayout>
#include <QValueAxis>
#include <QtCore>
#include <QtWidgets>

#include "container.h"
#include "resChart.h"

class ContainerPage : public QWidget {
    Q_OBJECT
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
    explicit ContainerPage(const Container& con, QWidget* parent = nullptr);
    QLabel* createLabel(const QString& text);
    void updateData(const Container& con);
    void updateChart(double, resChart*, int);
    QString formatPortsInline(const std::map<std::string, std::string>& ports);
};