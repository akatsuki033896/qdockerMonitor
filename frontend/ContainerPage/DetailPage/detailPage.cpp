//
// Created by akatsuki on 2026/5/30.
//
#include "detailPage.h"

#include <qlabel.h>
#include <qpushbutton.h>

#include <QtWidgets>


detailPage::detailPage(QWidget* parent) : QWidget(parent) {
    layout = new QVBoxLayout(this);

    QGroupBox* infoBox = new QGroupBox("Container Info");
    QVBoxLayout* infoLayout = new QVBoxLayout();

    nameLabel = new QLabel();
    nameLabel->setObjectName("titleLabel");  // 容器名字

    // button
    QHBoxLayout* b_layout = new QHBoxLayout(this);

    status_light = new QLabel(this);
    status_light->setFixedSize(12, 12);
    status_light->setStyleSheet(
        "background-color: green;"
        "border-radius: 6px;");

    stop = new QPushButton("Stop");
    start = new QPushButton("Start");
    pause = new QPushButton("Pause");

    stop->resize(50, 20);
    start->resize(50, 20);
    pause->resize(50, 20);

    b_layout->addWidget(status_light);
    b_layout->addWidget(nameLabel);
    b_layout->addStretch();
    b_layout->addWidget(stop);
    b_layout->addWidget(start);
    b_layout->addWidget(pause);
    infoLayout->addLayout(b_layout);

    idLabel = new QLabel();
    imageLabel = new QLabel();
    statusLabel = new QLabel();

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(idLabel);
    infoLayout->addWidget(imageLabel);
    infoBox->setLayout(infoLayout);

    // network
    QGroupBox* netBox = new QGroupBox("Network");
    QVBoxLayout* netLayout = new QVBoxLayout();

    ipLabel = new QLabel();
    macLabel = new QLabel();
    portsLabel = new QLabel();
    mountLabel = new QLabel();
    portsLabel->setWordWrap(true);
    mountLabel->setWordWrap(true);

    netLayout->addWidget(ipLabel);
    netLayout->addWidget(macLabel);
    netLayout->addWidget(portsLabel);
    netLayout->addWidget(mountLabel);
    netBox->setLayout(netLayout);

    QGroupBox* resBox = new QGroupBox("Resources");
    QVBoxLayout* resLayout = new QVBoxLayout();

    cpuBar = new QProgressBar;
    cpuBar->setRange(0, 100);
    cpuBar->setFormat("%p%");  // 显示百分比
    resLayout->addWidget(new QLabel("CPU"));
    resLayout->addWidget(cpuBar);
    cpuChart = new resChart();
    resLayout->addWidget(cpuChart->getView());

    memBar = new QProgressBar;
    memBar->setRange(0, 100);
    memBar->setFormat("%p%");
    resLayout->addWidget(new QLabel("Memory"));
    resLayout->addWidget(memBar);
    memChart = new resChart();
    resLayout->addWidget(memChart->getView());
    resBox->setLayout(resLayout);

    layout->addWidget(infoBox);
    layout->addWidget(netBox);
    layout->addWidget(resBox);
}

void detailPage::updateData(const Container& con) {
    nameLabel->setText(QString::fromStdString(con.name));
    idLabel->setText("ID: " + QString::fromStdString(con.id));
    imageLabel->setText("Image: " + QString::fromStdString(con.image));

    if (con.status == "running") {
        statusLabel->setText("Status: Running");
        statusLabel->setStyleSheet("color: green;");
    } else {
        statusLabel->setText("Status: Stopped");
        statusLabel->setStyleSheet("color: red;");
    }

    macLabel->setText("MAC: " + QString::fromStdString(con.mac));
    if (con.ip.empty())
        ipLabel->setText("IP: none");
    else
        ipLabel->setText("IP: " + QString::fromStdString(con.ip));

    if (con.mac.empty())
        macLabel->setText("MAC: none");
    else
        macLabel->setText("MAC: " + QString::fromStdString(con.mac));

    if (con.ports.empty())
        portsLabel->setText("Ports: none");
    else
        portsLabel->setText("Ports: " + formatPortsInline(con.ports));

    if (con.mounts.empty()) {
        mountLabel->setText("Mounts: none");
    } else {
        std::string mountsStr =
            std::accumulate(con.mounts.begin(), con.mounts.end(), std::string(),
                            [](const std::string& acc,
                               const std::pair<std::string, std::string>& m) {
                                return acc + m.first + " → " + m.second + "\n";
                            });
        mountLabel->setText("Mounts:\n" + QString::fromStdString(mountsStr));
    }

    cpuBar->setValue(static_cast<int>(con.cpuPercent));
    memBar->setValue(static_cast<int>(con.memPercent));

    static int t = 0;
    updateChart(con.cpuPercent, cpuChart, t);
    updateChart(con.memPercent, memChart, t);
    t++;
}

QString detailPage::formatPortsInline(
    const std::map<std::string, std::string>& ports) {
    QStringList list;
    for (const auto& [containerPort, hostPort] : ports) {
        list << QString("%1:%2")
                    .arg(QString::fromStdString(containerPort))
                    .arg(QString::fromStdString(hostPort));
    }
    return list.join(", ");
}

void detailPage::updateChart(double percent, resChart* chart, int t) {
    auto line = chart->getLine();
    auto axisX = chart->get_axisX();
    line->append(t, percent);
    if (line->count() > 30) line->remove(0);
    axisX->setRange(std::max(0, t - 29), t);
}

void detailPage::updateStatusLight(const QString& status) {
    QString color;

    if (status == "running")
        color = "#2ecc71";
    else if (status == "paused")
        color = "#f1c40f";
    else if (status == "restarting")
        color = "#3498db";
    else
        color = "#e74c3c";

    status_light->setStyleSheet(QString("background:%1;"
                                       "border-radius:6px;"
                                       "border:1px solid #666;")
                                   .arg(color));
}