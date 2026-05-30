#pragma once
#include <QtCore>
#include <QtWidgets>
#include "container.h"
#include "detailPage.h"
#include "processPage.h"

class ContainerPage : public QWidget {
    Q_OBJECT
    QTabWidget* tabWidget;
    QVBoxLayout* mainLayout;

   public:
    explicit ContainerPage(const Container& con, QWidget* parent = nullptr);
    void updateData(const Container& con) const;
    detailPage* detail_page;
    processPage* process_page;
};