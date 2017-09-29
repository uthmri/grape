/* RelOperator.cpp
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
#include <Nodes/reloperator.h>


//--------------------------------------------------------------------------------------------------
RelOperator::RelOperator(QObject *parent)
{
    setParent(parent);
    node_type = "RelOperator";
}
//--------------------------------------------------------------------------------------------------
RelOperator::~RelOperator() { }
//--------------------------------------------------------------------------------------------------
void RelOperator::init()
{
}
//--------------------------------------------------------------------------------------------------
void RelOperator::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void RelOperator::set_input_by_index(int in_id, QVariant* varvalue)
{
    QString v13 = nodeItem->propertyValue("RelOperatorType").toString();

    if(v13=="=")
    {
        if(in_id==0)
        {
            vI1 = varvalue;
            src_assigned[in_id] = true;
        }
        if(in_id==1)
        {
            vI3 = varvalue;
            src_assigned[in_id] = true;
        }
    }
    else if(v13=="<=")
    {
        if(in_id==0)
        {
            vI1 = varvalue;
            src_assigned[in_id] = true;
        }
        if(in_id==1)
        {
            vI3 = varvalue;
            src_assigned[in_id] = true;
        }
   }
    else if(v13==">=")
    {
        if(in_id==0)
        {
            vI1 = varvalue;
            src_assigned[in_id] = true;
        }
        if(in_id==1)
        {
            vI3 = varvalue;
            src_assigned[in_id] = true;
        }
    }
   else if(v13=="<>")
   {
        if(in_id==0)
        {
            vI1 = varvalue;
            src_assigned[in_id] = true;
        }
        if(in_id==1)
        {
            vI3 = varvalue;
            src_assigned[in_id] = true;
        }
   }
   else if(v13=="<")
   {
        if(in_id==0)
        {
            vI1 = varvalue;
            src_assigned[in_id] = true;
        }
        if(in_id==1)
        {
            vI3 = varvalue;
            src_assigned[in_id] = true;
        }
   }
    else if(v13==">")
    {
        if(in_id==0)
        {
            vI1 = varvalue;
            src_assigned[in_id] = true;
        }
        if(in_id==1)
        {
            vI3 = varvalue;
            src_assigned[in_id] = true;
        }
    }
   else
   {
   }
}
//--------------------------------------------------------------------------------------------------
bool RelOperator::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant* RelOperator::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* RelOperator::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------

int RelOperator::execute()
{
    clearOutputData();
    QString ppr11 = "Arg1Port";
    QString v11 =  nodeItem->propertyValue(ppr11).toString();
    QString ppr12 = "Arg2Port";
    QString v12 =  nodeItem->propertyValue(ppr12).toString();
    QString v1;
    QString v2;

    if (v11=="I1" && v12=="I1" )
    {
        v1 = vI1->toString();
        v2 = v1;
    }
    else if(v11=="I3" && v12=="I1" )
    {
        v1 = vI3->toString();
        v2 = vI1->toString();
    }
    else if(v11=="I1" && v12=="I3" )
    {
        v2 = vI3->toString();
        v1 = vI1->toString();
    }
    else if(v11=="I3" && v12=="I3" )
    {
        v1 = vI1->toString();
        v2 = v1;
    }

    QString ppr13 = "RelOperatorType";
    QString v13 =  nodeItem->propertyValue(ppr13).toString();

    if (v13=="=")
    {
        if (v1 == v2)
            operatorvalue=true;
        else
            operatorvalue=false;
    }

    else if (v13=="<>")
    {
        if (v1 == v2)
            operatorvalue=false;
        else
            operatorvalue=true;
    }

    else if(v13==">")
    {
        if (v1 > v2)
            operatorvalue=true;
        else
            operatorvalue=false;
    }

    else if(v13=="<")
    {
        if (v1 < v2)
            operatorvalue=true;
        else
            operatorvalue=false;
    }
    else if(v13==">=")
    {
        if (v1 >= v2)
            operatorvalue=true;
        else
            operatorvalue=false;
    }

    else if(v13=="<=")
    {
        if (v1 <= v2)
            operatorvalue=true;
        else
            operatorvalue=false;
    }
    else
    {
        qDebug() << "RelOperator: Unrecognized operation";
        return -1;
    }

    QVariant* qv = new QVariant();
    qv->setValue(operatorvalue);
    outData.clear();
    outData.push_back(qv);


    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int RelOperator::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int RelOperator::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void RelOperator::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
RelOperator* RelOperator::copy() const
{
    return new RelOperator(*this);
}
//--------------------------------------------------------------------------------------------------
QString RelOperator::uniqueKey() const
{
    return "RelOperator";
}
//--------------------------------------------------------------------------------------------------
void RelOperator::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool RelOperator::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void RelOperator::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------


