#include "nodetreewidget.h"

NodeTreeWidget::NodeTreeWidget(QWidget* parent) : QTreeWidget(parent)
{
    setParent(parent);
    connect(this, SIGNAL(itemActivated(QTreeWidgetItem *, int )),
            this, SLOT(getNameItemActivated(QTreeWidgetItem *, int)));
}

void NodeTreeWidget::getNameItemActivated(QTreeWidgetItem *item, int column)
{
    QString ItemName = item->text(0);
    emit nameItemActivated( ItemName );
}

