#include "containerpage.h"
#include "container.h"
#include <QtCore/qobject.h>
#include <QtCore/qprocess.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets>

ContainerPage::ContainerPage(const Container& con, QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    nameLabel = new QLabel("name: " + QString::fromStdString(con.name));
    idLabel = new QLabel("id: " + QString::fromStdString(con.id));
    imageLabel = new QLabel("image: " + QString::fromStdString(con.image));

    cpuBar = new QProgressBar;
    cpuBar->setRange(0, 100);

    memBar = new QProgressBar;
    memBar->setRange(0, 100);

    layout->addWidget(nameLabel);
    layout->addWidget(idLabel);
    layout->addWidget(imageLabel);

    layout->addWidget(new QLabel("CPU"));
    layout->addWidget(cpuBar);
    layout->addWidget(new QLabel("Memory"));
    layout->addWidget(memBar);
}

void ContainerPage::updateData(const Container& con) {
    // qDebug() << "emit statsUpdated";
    nameLabel->setText("name: " + QString::fromStdString(con.name));
    idLabel->setText("id: " + QString::fromStdString(con.id));
    imageLabel->setText("image: " + QString::fromStdString(con.image));
    // qDebug() << "image =" << QString::fromStdString(con.image);
    cpuBar->setValue(static_cast<int>(con.cpuPercent));
    memBar->setValue(static_cast<int>(con.memPercent));
}