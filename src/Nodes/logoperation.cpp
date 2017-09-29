/* LogOperation.cpp
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
#include <Nodes/logoperation.h>


//--------------------------------------------------------------------------------------------------
LogOperation::LogOperation(QObject *parent)
{
    setParent(parent);
    node_type = "LogOperation";
}
//--------------------------------------------------------------------------------------------------
LogOperation::~LogOperation() { }
//--------------------------------------------------------------------------------------------------
void LogOperation::init()
{
}
//--------------------------------------------------------------------------------------------------
bool LogOperation::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void LogOperation::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void LogOperation::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        if(varvalue->toString().toLower()=="true")
            vI1 = true;
        else
            vI1 = false;
        src_assigned[in_id] = true;
    }
    if(in_id==1)
    {
        if(varvalue->toString().toLower()=="true")
            vI3 = true;
        else
            vI3 = false;
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* LogOperation::get_output(int out_port_id)
{    
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* LogOperation::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int LogOperation::execute()
{
    clearOutputData();
    bool output_exist = false;
    QString connector =  nodeItem->propertyValue("LogOperationType").toString();

    if (connector == "&&")
    {
        output_exist = true;
        operatorvalue = vI1 && vI3;
    }
    else if (connector == "||")
    {
       output_exist = true;
       operatorvalue = vI1 || vI3;
    }
    else if(connector == "~")
    {
        output_exist = true;
        if (src_assigned.at(0))
            operatorvalue = !vI1;
        else if(src_assigned.at(0))
            operatorvalue = !vI3;
        else
            operatorvalue = 0;
    }
    else
    {
        qDebug() << "LogOperation: Unrecoginized operation";
        return -1;
    }

    QVariant* qv = new QVariant();
    qv->setValue(operatorvalue);
    outData.push_back(qv);

   setReadyOutPorts(8 + getNi().NumInputs + 0, output_exist);
   markExecuted();
   return 0;
}
//--------------------------------------------------------------------------------------------------
int LogOperation::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int LogOperation::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void LogOperation::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
LogOperation* LogOperation::copy() const
{
    return new LogOperation(*this);
}
//--------------------------------------------------------------------------------------------------
QString LogOperation::uniqueKey() const
{
    return "node";
}
//--------------------------------------------------------------------------------------------------
void LogOperation::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool LogOperation::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void LogOperation::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------

