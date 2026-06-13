//
// Created by akatsuki on 2026/5/30.
//

#ifndef SYSTEMMONITOR_PROCESSPAGE_H
#define SYSTEMMONITOR_PROCESSPAGE_H
#include <qtableview.h>
#include <qstandarditemmodel.h>
#include <QWidget>
#include "container.h"

class processPage : public QWidget {
    Q_OBJECT
    QTableView* tableView;
    QStandardItemModel* model;
public:
    explicit processPage(QWidget* parent);
    void updateData(const Container& con);
};
#endif  // SYSTEMMONITOR_PROCESSPAGE_H
