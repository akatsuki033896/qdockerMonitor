#include "containerpage.h"
#include <QtWidgets>
#include <QVBoxLayout>
#include "container.h"

ContainerPage::ContainerPage(const Container& con, QWidget* parent)
    : QWidget(parent) {
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    tabWidget = new QTabWidget(this);
    detail_page = new detailPage(this);
    process_page = new processPage(this);
    // process page, file page, log page, terminal ...
    tabWidget->addTab(detail_page, "Detail");
    tabWidget->addTab(process_page, "Process");
    mainLayout->addWidget(tabWidget);
}

void ContainerPage::updateData(const Container& con) const
{
    detail_page->updateData(con);
}
