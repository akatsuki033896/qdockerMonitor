//
// Created by akatsuki on 2026/5/30.
//
#include "detailPage.h"

#include "util.h"

detailPage::detailPage(QWidget* parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);

    // ===== 1. Container Info =====
    QGroupBox* infoBox = new QGroupBox("Container Info");
    QVBoxLayout* infoLayout = new QVBoxLayout();

    nameLabel = new QLabel();
    nameLabel->setObjectName("titleLabel");  // 主标题

    idLabel = new QLabel();
    imageLabel = new QLabel();
    statusLabel = new QLabel();

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(idLabel);
    infoLayout->addWidget(imageLabel);

    infoBox->setLayout(infoLayout);

    // ===== 2. Network =====
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

    // ===== 3. Storage =====
    // QGroupBox* storageBox = new QGroupBox("Storage");
    // QVBoxLayout* storageLayout = new QVBoxLayout();

    // ===== 4. Resources =====
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
    // layout->addWidget(storageBox);
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