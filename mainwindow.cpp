#include "mainwindow.h"
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets>
#include "container.h"
#include "containerpage.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Container Monitor");
    setFixedSize(800, 600);
    
    QWidget* central = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(central);
    
    // 左侧列表
    listWidget = new QListWidget;
    listWidget->setFixedWidth(150);
    
    // 右侧页面
    stackedWidget = new QStackedWidget;

    layout->addWidget(listWidget);
    layout->addWidget(stackedWidget);
    setCentralWidget(central);

    connect(listWidget, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);
}

// 更新列表（左边）
void MainWindow::onContainerListChanged(const std::vector<Container>& list) {
    // 新建/删除 item
    listWidget->clear();
    for (const auto& con : list) {
        listWidget->addItem(QString::fromStdString(con.name));
        // 页面
        if (!pages.count(con.id)) {
            auto* page = new ContainerPage(con);
            stackedWidget->addWidget(page);
            pages[con.id] = page;
        }
    }
}

// 更新单个页面（右边）
void MainWindow::onContainerStatsUpdated(const Container& con) {
    qDebug() << "UI received update";
    if (pages.count(con.id)) {
        pages[con.id]->updateData(con);
    }
    // pages[con.id]->updateData(con);
    // onContainerStatsUpdated → page.updateData()
}

MainWindow::~MainWindow() = default;
