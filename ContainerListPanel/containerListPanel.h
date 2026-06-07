#pragma once
#include <QWidget>
#include <qtmetamacros.h>
#include "container.h"
#include <QListWidget>
#include <vector>

class ContainerListPanel : public QWidget {
    Q_OBJECT
    QListWidget* listWidget;
    std::vector<std::string> ids;
public:
    explicit ContainerListPanel(QWidget* parent = nullptr);
    void updateList(const std::vector<Container>& list);
signals:
    void containerSelected(const std::string& id);
};