/* Node.cpp
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

#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>
#include <Nodes/Node.h>
#include <QList>


//--------------------------------------------------------------------------------------------------
Node :: Node()
{
    execution_status = NOT_EXECUTED;
    executable = EXECUTABLE;

    inputFreshPorts.clear();
    mandatoryInPorts.clear();
    mandatoryOutPorts.clear();
    connectedInputPorts.clear();
    inputReadyPorts.clear();
    outputReadyPorts.clear();
    connectedOutputPorts.clear();

    NodeOutputUpdated = false;
    NowFlashing = false;
    GraphicalFlag = false;
    RunTimeError = 0;

    src_assigned.clear();
//    for(int ii=0; ii<getNi().NumInputs; ii++)
//        src_assigned.push_back(false);
    for(int ii=0; ii<20; ii++)
        src_assigned.push_back(false);

    inData.clear();
    outData.clear();
}
//--------------------------------------------------------------------------------------------------
Node::Node(QString type)
{
    node_type = type;
    Node();
}
//--------------------------------------------------------------------------------------------------
Node::Node(const Node& node) { }
//--------------------------------------------------------------------------------------------------
Node::~Node() { }
//--------------------------------------------------------------------------------------------------
void Node::init()
{
    src_assigned.clear();
    for(int ii=0; ii<getNi().NumInputs; ii++)
        src_assigned.push_back(false);
}
//--------------------------------------------------------------------------------------------------
void Node::set_input(int in_port_id, QVariant* varvalue)
{
    setReadyInPorts(in_port_id);
    int in_id = in_port_id - 8;
    set_input_by_index(in_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Node::set_input_by_index(int in_id, QVariant* varvalue)
{
}
//--------------------------------------------------------------------------------------------------
QVariant* Node::get_output(int out_port_id)
{
     int out_id = out_port_id - getNi().NumInputs - 8;
     return get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Node::get_output_by_index(int out_id)
{
    if(out_id < getNi().NumOutputs)
        return outData.at(out_id);

    return NULL;
}
//--------------------------------------------------------------------------------------------------
int  Node::getNumInputPort()
{
    return getNi().NumInputs;
}
//--------------------------------------------------------------------------------------------------
int  Node::getNumOutputPort()
{
    return getNi().NumOutputs;
}
//--------------------------------------------------------------------------------------------------
int Node::execute()
{
    execution_status = EXECUTED;
    return 0;
}
//--------------------------------------------------------------------------------------------------
Node* Node::copy() const
{
    return new Node(*this);
}
//--------------------------------------------------------------------------------------------------
QString Node::uniqueKey() const
{
    return "node";
}
//--------------------------------------------------------------------------------------------------
void Node::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Node::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Node::SetNodeInputAttr(int index, QString ty, QString defaultval, QString mand, QString tipt, QString helpt)
{
    NodeInputAttr s;
    if((InputAttribute.length()==0) || (InputAttribute.length() < (index+1)))
    {
        s.type = ty;
        s.defaultvalue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        InputAttribute.append(s);
    }
    else // the input port attributes exist
    {
        s.type = ty;
        s.defaultvalue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        InputAttribute.replace(index, s);
    }
}
//--------------------------------------------------------------------------------------------------
void Node::SetNodeOutputAttr(int index, QString ty, QString defaultval, QString mand, QString tipt, QString helpt)
{
    NodeOutputAttr s;
    if((OutputAttribute.length()==0) || (OutputAttribute.length() < (index+1)))
    {
        s.type = ty;
        s.defaultvaue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        OutputAttribute.append(s);
    }
    else // the input port attributes exist
    {
        s.type = ty;
        s.defaultvaue = defaultval;
        s.mandatory = (mand.toLower()=="true");
        s.tooltip = tipt;
        s.helptext = helpt;
        OutputAttribute.replace(index, s);
    }
}
//--------------------------------------------------------------------------------------------------
void Node::setMandatoryInPorts(int conn_port)
{
    if(!mandatoryInPorts.contains(conn_port))
         mandatoryInPorts.append(conn_port);
}
//--------------------------------------------------------------------------------------------------
void Node::setConnectedInPorts(int conn_port)
{
    if(!connectedInputPorts.contains(conn_port))
        connectedInputPorts.append(conn_port);
}
//--------------------------------------------------------------------------------------------------
void Node::setConnectedOutPorts(int conn_port)
{
    if(!connectedOutputPorts.contains(conn_port))
         connectedOutputPorts.append(conn_port);
}
//--------------------------------------------------------------------------------------------------
void Node::setReadyInPorts(int conn_port)
{
    if(!inputReadyPorts.contains(conn_port))
         inputReadyPorts.append(conn_port);
}
//--------------------------------------------------------------------------------------------------
void Node::setFreshInPorts(int conn_port, bool fresh)
{
    if(fresh) // add it to inputFreshPorts (if not already there)
    {
        if(!inputFreshPorts.contains(conn_port))
            inputFreshPorts.push_back(conn_port);
    }
    else // remove it from inputFreshPorts (if found)
    {
        if(inputFreshPorts.contains(conn_port))
            inputFreshPorts.removeAt(inputFreshPorts.indexOf(conn_port));
    }
}
//--------------------------------------------------------------------------------------------------
void Node::setFreshInPorts(bool fresh)
{    
    int Nin = getNi().NumInputs;
    int i, conn_port;

    if(fresh)
    {
        for(i=0; i<Nin; i++)
        {
            conn_port = i + 8;
            if( !inputFreshPorts.contains(conn_port) )
                inputFreshPorts.push_back(conn_port);
        }
    }
    else
        inputFreshPorts.clear();
}
//--------------------------------------------------------------------------------------------------
void Node::setReadyOutPorts(int conn_port, bool ready)
{
    if(ready)
    {
        if(!outputReadyPorts.contains(conn_port))
            outputReadyPorts.append(conn_port);
    }
    else
    {
        if(outputReadyPorts.contains(conn_port))
            outputReadyPorts.removeAt(outputReadyPorts.indexOf(conn_port));
    }
}
//--------------------------------------------------------------------------------------------------
void Node::setReadyOutPorts(int conn_port)
{
    setReadyOutPorts(conn_port, true);
}
//--------------------------------------------------------------------------------------------------
void Node::setMandatoryOutPorts(int conn_port)
{
    if(!mandatoryOutPorts.contains(conn_port))
        mandatoryOutPorts.append(conn_port);
}
//--------------------------------------------------------------------------------------------------
void Node::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
void Node::setConnectedOutPorts()
{
}
//--------------------------------------------------------------------------------------------------
void Node::setReadyInPorts()
{
}
//--------------------------------------------------------------------------------------------------
bool Node::isExecutable() const
{
    return ((executable == EXECUTABLE) || (executable == REEXECUTABLE));
}
//--------------------------------------------------------------------------------------------------
bool Node::isReExecutable() const
{
    return (executable == REEXECUTABLE);
}
//--------------------------------------------------------------------------------------------------
bool Node::isExecuted() const
{
    return (execution_status == EXECUTED);
}
//--------------------------------------------------------------------------------------------------
bool Node::isExecuting() const
{
    return (execution_status == EXECUTING);
}
//--------------------------------------------------------------------------------------------------
bool Node::isFlashing() const
{
    return NowFlashing;
}
//--------------------------------------------------------------------------------------------------
bool Node::isGraphical() const
{
    return GraphicalFlag;
}
//--------------------------------------------------------------------------------------------------
void Node::setGraphical(bool d)
{
    GraphicalFlag = d;
}
//--------------------------------------------------------------------------------------------------
QString Node::getType()
{
    return node_type;
}
//--------------------------------------------------------------------------------------------------
bool Node::isAllInputsReady()
{
    int Nin = getNi().NumInputs;

    int i, conn_port;
    for(i=0; i<Nin; i++)
    {
        conn_port = i + 8;
        // all mandatory inputs should be ready
        if( ( !inputReadyPorts.contains(conn_port)) && getNi().Inputs.at(i).mandatory )
            return false;

        // all connected inputs should be ready
        if( ( !inputReadyPorts.contains(conn_port)) && connectedInputPorts.contains(conn_port) )
            return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
int Node::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Node::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void Node::clearNodeData()
{}
//--------------------------------------------------------------------------------------------------
void Node::setExecutionState(NODE_EXECUTION_STATUS_ENUM ex_state)
{
    execution_status = ex_state;
}
//--------------------------------------------------------------------------------------------------
void Node::markExecuted()
{
    execution_status = EXECUTED;
}
//--------------------------------------------------------------------------------------------------
void Node::markUnExecuted()
{
    execution_status = NOT_EXECUTED;
}
//--------------------------------------------------------------------------------------------------

void Node::setExecutionMode(NODE_EXECUTION_MODE_ENUM ex_mode)
{
    executable = ex_mode;
}
//--------------------------------------------------------------------------------------------------
void Node::setDiagram(Diagram* diagram)
{
    nodeDiagram = diagram;
}
//--------------------------------------------------------------------------------------------------
QColor Node::getNodeColorBackup()
{
    return NodeColorBackup;
}
//--------------------------------------------------------------------------------------------------
void Node::setNodeColorBackup(QColor c)
{
    NodeColorBackup = c;
}
//--------------------------------------------------------------------------------------------------
void Node::flashNode(QColor FlashColor, int time_msec)
{
    DiagramBoxResizeItem* d;
    d = (DiagramBoxResizeItem*) getDiagramItem();
    setNodeColorBackup(d->brush().color());
    d->setBrush(QBrush(FlashColor));

    NowFlashing = true;

    FlashingTimer.setSingleShot(false);
    FlashingTimer.setInterval(time_msec);
    connect(&FlashingTimer, SIGNAL(timeout()), this, SLOT(unflashNode()));
    FlashingTimer.start();
}
//--------------------------------------------------------------------------------------------------
void Node::unflashNode()
{
    FlashingTimer.stop();

    NowFlashing = false;

    DiagramBoxResizeItem* d;
    d = (DiagramBoxResizeItem*) getDiagramItem();
    d->setBrush(QBrush(getNodeColorBackup()));
}
//--------------------------------------------------------------------------------------------------
void Node::setDiagramItem(NodeItem * n)
{
    nodeItem = n;
}
//--------------------------------------------------------------------------------------------------
NodeItem* Node::getDiagramItem()
{
    return nodeItem;
}
//--------------------------------------------------------------------------------------------------
Diagram* Node::getDiagram()
{
    return nodeDiagram;
}
//--------------------------------------------------------------------------------------------------
QList<int> Node::getConnectedInPorts()
{
    return connectedInputPorts;
}
//--------------------------------------------------------------------------------------------------
QList<int> Node::getConnectedOutPorts()
{
    return connectedOutputPorts;
}
//--------------------------------------------------------------------------------------------------
QList<int> Node::getReadyInPorts()
{
    return inputReadyPorts;
}
//--------------------------------------------------------------------------------------------------
QList<int> Node::getFreshInPorts()
{
    return inputFreshPorts;
}
//--------------------------------------------------------------------------------------------------
QList<int> Node::getReadyOutPorts()
{
    return outputReadyPorts;
}
//--------------------------------------------------------------------------------------------------
bool Node::AnyFreshInPorts()
{
    return !inputFreshPorts.isEmpty();
}
//--------------------------------------------------------------------------------------------------
bool Node::hasOutputData(int port)
{
    return (outputReadyPorts.indexOf(port) != -1);
}
//--------------------------------------------------------------------------------------------------
void Node::clearOutputData()
{
    outputReadyPorts.clear();
}
//--------------------------------------------------------------------------------------------------
bool Node::nodeOutputUpdated()
{
    return NodeOutputUpdated;
}
//--------------------------------------------------------------------------------------------------
void Node::setNodeOutputUpdated(bool u)
{
    NodeOutputUpdated = u;
}
//--------------------------------------------------------------------------------------------------
NodeInterface Node::getNi() const
{
    return ni;
}
//--------------------------------------------------------------------------------------------------
void Node::setNi(const NodeInterface &value)
{
    ni = value;
}
//--------------------------------------------------------------------------------------------------
int Node::getRunTimeError() const
{
    return RunTimeError;
}
//--------------------------------------------------------------------------------------------------
void Node::setRunTimeError(int value)
{
    RunTimeError = value;
}
//--------------------------------------------------------------------------------------------------
void Node::setInputMode(int index, QString type, QString defaultvalue,QString mandatory, QString tooltip,QString helptext)
{
    //    getNi().Inputs.at(index).mandatory = mand;
    getNi().SetNodeInputAttr(index, type, defaultvalue, mandatory, tooltip, helptext);
}
//--------------------------------------------------------------------------------------------------
void Node::setOutputMode(int index, QString type, QString defaultvalue,QString mandatory, QString tooltip,QString helptext)
{
    getNi().SetNodeOutputAttr(index, type, defaultvalue, mandatory, tooltip, helptext);
}
//--------------------------------------------------------------------------------------------------





