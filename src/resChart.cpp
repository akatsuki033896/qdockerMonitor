#include "resChart.h"
#include <qwidget.h>
#include <QChartView>
#include <QChart>

resChart::resChart(QWidget* parent) : QWidget(parent) {
    line = new QLineSeries(this);
    line->append(0, 0);
    line->append(1, 0);

    axisX = new QValueAxis();
    axisX->setRange(0, 30);
    axisX->setTickCount(10);
    axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTickCount(5);
    
    chart = new QChart();
    chart->addSeries(line);
    chart->legend()->hide(); // hide point
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    
    line->attachAxis(axisX);
    line->attachAxis(axisY);
    
    view = new QChartView(chart, this);
    view->setMinimumHeight(150);
    // resLayout->addWidget(view);
}
