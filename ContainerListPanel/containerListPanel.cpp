#include "containerListPanel.h"
#include <QVBoxLayout>
#include <qlistwidget.h>

ContainerListPanel::ContainerListPanel(QWidget* parent) {
    listWidget = new QListWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(listWidget);
    setLayout(layout);
    connect(listWidget, &QListWidget::currentRowChanged,
        this, [this](int row) {
            if (row >= 0 && row < ids.size()) {
                emit containerSelected(ids[row]);
            }
        });
}

// 更新列表UI
void ContainerListPanel::updateList(const std::vector<Container>& list) {
    listWidget->clear();
    ids.clear();

    for (const auto& con : list) {
        listWidget->addItem(QString::fromStdString(con.name));
        ids.push_back(con.id);
    }
}
