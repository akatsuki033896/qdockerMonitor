#pragma once
#include <qvalueaxis.h>
#include <qwidget.h>

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QtWidgets>


// resource chart for cpu and ram
class resChart : public QWidget {
    Q_OBJECT
    QLineSeries* line;
    QValueAxis* axisX;
    QValueAxis* axisY;
    QChart* chart;
    QChartView* view;

   public:
    explicit resChart(QWidget* parent = nullptr);
    QLineSeries* getLine() const { return line; };
    QValueAxis* get_axisX() const { return axisX; };
    QChartView* getView() const { return view; };
    ~resChart() = default;
};