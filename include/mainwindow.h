#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "container.h"
#include "containerpage.h"
#include <QMainWindow>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qstackedwidget.h>
#include <string>
#include <unordered_map>

class MainWindow : public QMainWindow {
    Q_OBJECT
    QListWidget* listWidget;
    QStackedWidget* stackedWidget;
    std::unordered_map<std::string, ContainerPage*> pages;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void onContainersUpdated(const std::vector<Container>& list);
    ~MainWindow() override;
public slots:
    void onContainerListChanged(const std::vector<Container>& list);
    void onContainerStatsUpdated(const Container& con);
};
#endif // MAINWINDOW_H
