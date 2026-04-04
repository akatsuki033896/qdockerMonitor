#pragma once
#include "container.h"
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qprogressbar.h>
#include <QtWidgets/qwidget.h>
#include <QLabel>

class ContainerPage : public QWidget {
    Q_OBJECT
    QLabel* idLabel;
    QLabel* nameLabel;
    QLabel* imageLabel;
    QLabel* statusLabel;

    QLabel* ipLabel;
    QLabel* macLabel;
    QLabel* portsLabel;
    QLabel* mountLabel;
    
    QProgressBar* cpuBar;
    QProgressBar* memBar;
    QLabel* memDetailLabel;
public:
    explicit ContainerPage(const Container& con, QWidget* parent = nullptr);
    QLabel* createLabel(const QString& text);
    void updateData(const Container& con);
    QString formatPortsInline(const std::map<std::string, std::string>& ports);
};