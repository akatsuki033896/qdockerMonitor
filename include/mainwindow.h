#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "container.h"
#include "containerpage.h"
#include <QMainWindow>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qstackedwidget.h>
#include <string>
#include <unordered_map>
#include "containerManager.h"
#include "containerListPanel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
    // QListWidget* listPanel;
    ContainerListPanel* listPanel;
    QStackedWidget* stackedWidget;
    ContainerManager* manager;
    std::unordered_map<std::string, ContainerPage*> pages;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void onContainersUpdated(const std::vector<Container>& list);
    ~MainWindow() override;
public slots:
    void onContainerListChanged(const std::vector<Container>& list);
    void onContainerStatsUpdated(const Container& con);
    void onContainerSelected(const std::string& id);
};
#endif // MAINWINDOW_H
