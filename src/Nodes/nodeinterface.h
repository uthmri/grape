#ifndef NODEINTERFACE_H
#define NODEINTERFACE_H

#include <QtCore>

struct InputPort
{
    QString type;
    QString defaultvalue;
    bool mandatory;
    QString tooltip;
    QString helptext;
};

struct OutputPort
{
    QString type;
    QString defaultvalue;
    bool mandatory;
    QString tooltip;
    QString helptext;
};


class NodeInterface
{
public:
    int NumInputs, NumOutputs;
    QList<InputPort> Inputs;
    QList<OutputPort> Outputs;
    QString cap;

public:
    NodeInterface();

    void SetNodeInputAttr(int index, QString type, QString defaultvalue, QString mandatory, QString tooltip, QString helptext);
    void SetNodeOutputAttr(int index, QString type, QString defaultvalue, QString mandatory, QString tooltip, QString helptext);
};

#endif // NODEINTERFACE_H
