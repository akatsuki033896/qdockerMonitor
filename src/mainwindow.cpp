#include "mainwindow.h"
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets>
#include "container.h"
#include "containerManager.h"
#include "containerpage.h"
#include "containerListPanel.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Container Monitor");
    setMinimumSize(800, 650);
    
    QWidget* central = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(central);
    
    // 左侧列表
    listPanel = new ContainerListPanel;
    listPanel->setFixedWidth(150);
    
    // 右侧页面
    stackedWidget = new QStackedWidget;

    layout->addWidget(listPanel);
    layout->addWidget(stackedWidget);
    setCentralWidget(central);

    manager = new ContainerManager();

    connect(manager, &ContainerManager::ContainerListChanged, this, &MainWindow::onContainerListChanged);
    connect(manager, &ContainerManager::ContainerStatsUpdated, this, &MainWindow::onContainerStatsUpdated);
    connect(listPanel, &ContainerListPanel::containerSelected, this, &MainWindow::onContainerSelected);
    
}

// 更新显示的列表内容
void MainWindow::onContainerListChanged(const std::vector<Container>& list) {
    // 新建/删除 item
    listPanel->updateList(list); // 更新UI

    for (const auto& con : list) {
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
    // qDebug() << "UI received update";
    if (pages.count(con.id)) {
        pages[con.id]->updateData(con);
    }
}

MainWindow::~MainWindow() = default;

void MainWindow::onContainerSelected(const std::string& id) {
    if (pages.count(id)) {
        stackedWidget->setCurrentWidget(pages[id]);
    }
}