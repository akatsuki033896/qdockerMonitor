//
// Created by akatsuki on 2026/5/30.
//

#include "processPage.h"
#include <qboxlayout.h>
#include <qjsonarray.h>
#include <qnamespace.h>
#include <qstandarditemmodel.h>
#include <QStandardItemModel>
#include "container.h"
#include <QHeaderView>

processPage::processPage(QWidget *parent) : QWidget(parent) {
    tableView = new QTableView(this);
    // set model
    model = new QStandardItemModel(0, 8, this);
    // UID   PID  PPID  C  STIME  TTY   TIME      CMD
    model->setHorizontalHeaderLabels({
        "UID",
        "PID",
        "PPID",
        "C",
        "STIME",
        "TTY",
        "TIME",
        "CMD"
    });
    tableView->setModel(model);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->verticalHeader()->hide();
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setAlternatingRowColors(true);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(tableView);
    setLayout(layout);
}

void processPage::updateData(const Container& con) {
    model->removeRows(0, model->rowCount()); // 清除表头
    for (const auto& pro : con.processes) {
        QList<QStandardItem*> row;
        row << new QStandardItem(pro.uid);
        row << new QStandardItem(pro.pid);
        row << new QStandardItem(pro.ppid);
        row << new QStandardItem(pro.cpu);
        row << new QStandardItem(pro.stime);
        row << new QStandardItem(pro.tty);
        row << new QStandardItem(pro.time);
        row << new QStandardItem(pro.cmd);
        model->appendRow(row);
    }
}