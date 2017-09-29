#ifndef NODETREEWIDGET_H
#define NODETREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidgetItem>

class NodeTreeWidget : public QTreeWidget
{    
    Q_OBJECT

public:
    NodeTreeWidget(QWidget* parent = NULL);

public slots:
    void getNameItemActivated(QTreeWidgetItem *item, int column);

signals:
    void nameItemActivated(const QString&);

};

#endif // NODETREEWIDGET_H
