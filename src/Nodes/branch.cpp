/* Branch.cpp
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
#include <BranchItem.h>
#include <Nodes/branch.h>
#include "xmlreclib.h"

//--------------------------------------------------------------------------------------------------
Branch::Branch(QObject *parent)
{
    setParent(parent);
    node_type = "Branch";
}
//--------------------------------------------------------------------------------------------------
Branch::~Branch() { }
//--------------------------------------------------------------------------------------------------
void Branch::init()
{
}
//--------------------------------------------------------------------------------------------------
bool Branch::isAllInputsReady()
{
     return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void Branch::set_input(int in_port_id, QVariant* varvalue)
{
     Node::set_input(in_port_id, varvalue);
 }
 //--------------------------------------------------------------------------------------------------
void Branch::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        input1_value = new QVariant();
        input1_value = varvalue;
        src_assigned[in_id] = true;
    }
    if (in_id==1)
    {        
        if(varvalue->canConvert(QMetaType::Int))
        {
            condition1 = varvalue->toInt();
            src_assigned[in_id] = true;
        }
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* Branch::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Branch::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int Branch::execute()
{
    clearOutputData();
    outData.clear();

    if(condition1)
    {
        outData.append(input1_value);
        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    }
    else
    {
        outData.append(input1_value);
        setReadyOutPorts(8 + getNi().NumInputs + 1, true);
    }

    markExecuted();

    return 0;
}
//--------------------------------------------------------------------------------------------------
int Branch::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Branch::validateRun()
{   
    if(!src_assigned.at(1))
        return -1;
    return 0;
}
//--------------------------------------------------------------------------------------------------
void Branch::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
Branch* Branch::copy() const
{
    return new Branch(*this);
}
//--------------------------------------------------------------------------------------------------
QString Branch::uniqueKey() const
{
    return "Branch";
}
//--------------------------------------------------------------------------------------------------
void Branch::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Branch::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Branch::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------

