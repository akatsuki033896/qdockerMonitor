#include "mainwindow.h"
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets>
#include "containerManager.h"
#include "container.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Container Monitor");
    setFixedSize(800, 600);
    
    QWidget* central = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout(central);
    
    // 左侧列表
    QListWidget* listWidget = new QListWidget;
    listWidget->setFixedWidth(150);
    
    // 右侧页面
    QStackedWidget* stackedWidget = new QStackedWidget;

    layout->addWidget(listWidget);
    layout->addWidget(stackedWidget);
    setCentralWidget(central);

}

void MainWindow::onContainersUpdated(const std::vector<Container>& list) {
    // 更新左侧列表 or 页面
}

MainWindow::~MainWindow() = default;
