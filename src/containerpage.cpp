#include "containerpage.h"
#include "container.h"
#include <QtCore/qobject.h>
#include <QtCore/qprocess.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets>

ContainerPage::ContainerPage(const Container& con, QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // ===== 1. Container Info =====
    QGroupBox* infoBox = new QGroupBox("Container Info");
    QVBoxLayout* infoLayout = new QVBoxLayout();

    nameLabel = new QLabel();
    nameLabel->setObjectName("titleLabel"); // 主标题

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
    portsLabel->setWordWrap(true);

    netLayout->addWidget(ipLabel);
    netLayout->addWidget(macLabel);
    netLayout->addWidget(portsLabel);

    netBox->setLayout(netLayout);

    // ===== 3. Storage =====
    QGroupBox* storageBox = new QGroupBox("Storage");
    QVBoxLayout* storageLayout = new QVBoxLayout();

    mountLabel = new QLabel();
    mountLabel->setWordWrap(true);

    storageLayout->addWidget(mountLabel);
    storageBox->setLayout(storageLayout);

    // ===== 4. Resources =====
    QGroupBox* resBox = new QGroupBox("Resources");
    QVBoxLayout* resLayout = new QVBoxLayout();

    cpuBar = new QProgressBar;
    cpuBar->setRange(0, 100);
    cpuBar->setFormat("%p%"); // 显示百分比

    memBar = new QProgressBar;
    memBar->setRange(0, 100);
    memBar->setFormat("%p%");

    resLayout->addWidget(new QLabel("CPU"));
    resLayout->addWidget(cpuBar);
    resLayout->addWidget(new QLabel("Memory"));
    resLayout->addWidget(memBar);

    resBox->setLayout(resLayout);

    // ===== 添加到主布局 =====
    mainLayout->addWidget(infoBox);
    mainLayout->addWidget(netBox);
    mainLayout->addWidget(storageBox);
    mainLayout->addWidget(resBox);
}


void ContainerPage::updateData(const Container& con) {
    nameLabel->setText(QString::fromStdString(con.name));
    idLabel->setText("ID: " + QString::fromStdString(con.id));
    imageLabel->setText("Image: " + QString::fromStdString(con.image));

    ipLabel->setText("IP: " + QString::fromStdString(con.ip));
    macLabel->setText("MAC: " + QString::fromStdString(con.mac));

    if(con.status == "running") {
        statusLabel->setText("Status: Running");
        statusLabel->setStyleSheet("color: green;");
    } else {
        statusLabel->setText("Status: Stopped");
        statusLabel->setStyleSheet("color: red;");
    }
    
    if (con.ports.empty())
        portsLabel->setText("Ports: none");
    else
        portsLabel->setText("Ports: " + formatPortsInline(con.ports));

    if (con.mounts.empty()) {
        mountLabel->setText("Mounts: none");
    } else {
        std::string mountsStr = std::accumulate(
            con.mounts.begin(), con.mounts.end(), std::string(),
            [](const std::string& acc, const std::pair<std::string, std::string>& m) {
                return acc + m.first + " → " + m.second + "\n";
            }
        );
        mountLabel->setText("Mounts:\n" + QString::fromStdString(mountsStr));
    }

    cpuBar->setValue(static_cast<int>(con.cpuPercent));
    memBar->setValue(static_cast<int>(con.memPercent));
}


QString ContainerPage::formatPortsInline(const std::map<std::string, std::string>& ports) {
    QStringList list;

    for (const auto& [containerPort, hostPort] : ports) {
        list << QString("%1:%2")
                .arg(QString::fromStdString(containerPort))
                .arg(QString::fromStdString(hostPort));
    }

    return list.join(", ");
}