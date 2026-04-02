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
    
    QProgressBar* cpuBar;
    QProgressBar* memBar;
public:
    explicit ContainerPage(const Container& con, QWidget* parent = nullptr);
    void updateData(const Container& con);
};