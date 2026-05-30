#include "resChart.h"

#include <QChartView>
#include <QChart>
#include <QValueAxis>
#include <QLineSeries>
#include <QPen>

resChart::resChart(QWidget* parent)
    : QWidget(parent)
{
    // ===== 曲线 =====

    line = new QLineSeries(this);

    line->append(0, 0);
    line->append(1, 0);

    QPen pen(QColor("#3b82f6"));   // 蓝色
    pen.setWidth(2);

    line->setPen(pen);


    // ===== X轴 =====

    axisX = new QValueAxis();

    axisX->setRange(0, 30);
    axisX->setTickCount(10);

    axisX->setLabelsColor(QColor("#9aa4b2"));
    axisX->setGridLineColor(QColor("#1f2937"));
    axisX->setLinePenColor(QColor("#374151"));


    // ===== Y轴 =====

    axisY = new QValueAxis();

    axisY->setRange(0, 100);
    axisY->setTickCount(5);

    axisY->setLabelsColor(QColor("#9aa4b2"));
    axisY->setGridLineColor(QColor("#1f2937"));
    axisY->setLinePenColor(QColor("#374151"));


    // ===== Chart =====

    chart = new QChart();

    chart->addSeries(line);

    chart->legend()->hide();

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    line->attachAxis(axisX);
    line->attachAxis(axisY);


    // ===== Chart背景 =====

    chart->setBackgroundVisible(false);

    chart->setPlotAreaBackgroundVisible(true);

    chart->setPlotAreaBackgroundBrush(
        QColor("#0f141b")
    );

    chart->setMargins(QMargins(10, 10, 10, 10));


    // ===== View =====

    view = new QChartView(chart, this);

    view->setMinimumHeight(150);

    view->setRenderHint(QPainter::Antialiasing);

    view->setStyleSheet(R"(
        background: #0f141b;
        border-radius: 10px;
    )");
}