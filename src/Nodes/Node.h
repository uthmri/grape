/* node.h
 *
 *
* Copyright (C) 2016 UTHealth MRI Research
*
* This file is part of the GRAPE Diagram Editor.
*
* GRAPE is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GRAPE is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GRAPE.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef _NODE_H
#define _NODE_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include "nodeinterface.h"
#include "NodeItem.h"

struct NodeInputAttr
{
    QString type;
    QString defaultvalue;
    bool mandatory;
    QString tooltip;
    QString helptext;
};

struct NodeOutputAttr
{
    QString type;
    QString defaultvaue;
    bool mandatory;
    QString tooltip;
    QString helptext;
};

typedef enum { NOT_EXECUTED, EXECUTING, EXECUTED, PAUSED } NODE_EXECUTION_STATUS_ENUM;
typedef enum { EXECUTABLE, REEXECUTABLE, NON_EXECUATABLE, HALTED } NODE_EXECUTION_MODE_ENUM ;

class Node : public QObject
{
    Q_OBJECT

protected:
    NODE_EXECUTION_STATUS_ENUM execution_status;
    NODE_EXECUTION_MODE_ENUM execution_mode;

    NodeInterface ni;

    bool NodeOutputUpdated;
    QColor NodeColorBackup;
    QTimer FlashingTimer;
    QTimer FileSearchTimer;

    QString node_type;
    NODE_EXECUTION_MODE_ENUM executable;
    QBrush nodeBrush;

    QList<bool> src_assigned;

    bool GraphicalFlag;

    int RunTimeError;

    NodeItem* nodeItem;
    Diagram* nodeDiagram;
    QList<QVariant> inData;
    QList<QVariant*> outData;

    QList<int> mandatoryInPorts;
    QList<int> mandatoryOutPorts;
    QList<int> connectedInputPorts;
    QList<int> inputReadyPorts;
    QList<int> inputFreshPorts;
    QList<int> outputReadyPorts;
    QList<int> connectedOutputPorts;

public:
    bool NowFlashing;

    QList <NodeInputAttr>InputAttribute;
    QList <NodeOutputAttr>OutputAttribute;

public:

    Node();
    Node(QString name);
    Node(const Node& node);
    ~Node();

    virtual QString uniqueKey() const;
    virtual QString getType() ;

    virtual void init();
    virtual int execute();
    virtual int validateDesign();
    virtual int validateRun();
    virtual void clearNodeData();

    virtual void set_input(int in_port_id, QVariant* varvalue);
    virtual void set_input_by_index(int in_port_id, QVariant* varvalue);

    virtual QVariant* get_output(int out_port_id);
    virtual QVariant* get_output_by_index(int out_id);

    virtual bool isAllInputsReady();

public:
    void setConnectedInPorts(int port);
    void setConnectedOutPorts(int port);

    void setReadyInPorts(int port);
    void setReadyInPorts();

    void setFreshInPorts(int port, bool fresh = true);
    void setFreshInPorts(bool fresh);
    bool AnyFreshInPorts();

    void setReadyOutPorts(int conn_port, bool ready);
    void setReadyOutPorts(int conn_port);

    bool hasOutputData(int port);
    void clearOutputData();

    virtual void setMandatoryInPorts(int port);
    virtual void setConnectedInPorts();

    virtual void setMandatoryOutPorts(int mand_port);
    virtual void setConnectedOutPorts();

    QList<int> getConnectedInPorts();
    QList<int> getConnectedOutPorts();

    QList<int> getReadyInPorts();
    QList<int> getFreshInPorts();
    QList<int> getReadyOutPorts();

    int getNumOutputPort();
    int getNumInputPort();

    virtual Node* copy() const;

    bool isExecutable() const;
    bool isReExecutable() const;
    bool isExecuted() const;
    bool isExecuting() const;

    bool isFlashing() const;

    bool isGraphical() const;
    void setGraphical(bool d);

    void setExecutionState(NODE_EXECUTION_STATUS_ENUM ex_state);
    void markExecuted();
    void markUnExecuted();
    void setExecutionMode(NODE_EXECUTION_MODE_ENUM  ex_mode);


    bool nodeOutputUpdated();
    void setNodeOutputUpdated(bool u);

    NodeItem* getDiagramItem();
    void setDiagramItem(NodeItem*);

    Diagram* getDiagram();
    void setDiagram(Diagram*);

    QColor getNodeColorBackup();
    void setNodeColorBackup(QColor);

    void flashNode(QColor c, int time_msec);

    void markDirty();
    bool isSuperfluous() const;

    void SetNodeInputAttr(int index, QString type, QString defaultvalue, QString mandatory, QString tooltip, QString helptext);
    void SetNodeOutputAttr(int index, QString type, QString defaultvalue, QString mandatory, QString tooltip, QString helptext);

public:
    NodeInterface getNi() const;
    void setNi(const NodeInterface &value);

    int getRunTimeError() const;
    void setRunTimeError(int value);
    void setInputMode(int index, QString type, QString defaultvalue,QString mandatory, QString tooltip,QString helptext);
    void setOutputMode(int index, QString type, QString defaultvalue,QString mandatory, QString tooltip,QString helptext);

public slots:
    void unflashNode();
};

#endif

