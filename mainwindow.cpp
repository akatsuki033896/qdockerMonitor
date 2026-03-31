#include "mainwindow.h"
#include <QtWidgets/qwidget.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Container Monitor");
    setFixedSize(800, 600);
    setCentralWidget(new QWidget);
}

MainWindow::~MainWindow() = default;
