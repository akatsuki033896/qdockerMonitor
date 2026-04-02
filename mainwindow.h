#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "container.h"
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void onContainersUpdated(const std::vector<Container>& list);
    ~MainWindow() override;
};
#endif // MAINWINDOW_H
