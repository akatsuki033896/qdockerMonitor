#include "resChart.h"

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QPen>
#include <QValueAxis>


resChart::resChart(QWidget* parent) : QWidget(parent) {

    line = new QLineSeries(this);

    line->append(0, 0);
    line->append(1, 0);

    QPen pen(QColor("#3b82f6"));
    pen.setWidth(2);
    line->setPen(pen);

    axisX = new QValueAxis();
    axisX->setRange(0, 30);
    axisX->setTickCount(10);
    axisX->setLabelsColor(QColor("#9aa4b2"));
    axisX->setGridLineColor(QColor("#1f2937"));
    axisX->setLinePenColor(QColor("#374151"));

    axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTickCount(5);
    axisY->setLabelsColor(QColor("#9aa4b2"));
    axisY->setGridLineColor(QColor("#1f2937"));
    axisY->setLinePenColor(QColor("#374151"));

    chart = new QChart();
    chart->addSeries(line);
    chart->legend()->hide();
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    line->attachAxis(axisX);
    line->attachAxis(axisY);

    chart->setBackgroundVisible(false);
    chart->setPlotAreaBackgroundVisible(true);
    chart->setPlotAreaBackgroundBrush(QColor("#0f141b"));

    chart->setMargins(QMargins(10, 10, 10, 10));

    view = new QChartView(chart, this);
    view->setMinimumHeight(150);
    view->setRenderHint(QPainter::Antialiasing);
    view->setStyleSheet(R"(
        background: #0f141b;
        border-radius: 10px;
    )");
}