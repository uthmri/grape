/* Synch.cpp
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
#include <SynchItem.h>
#include <Nodes/synch.h>
#include "xmlreclib.h"

//--------------------------------------------------------------------------------------------------
Synch::Synch(QObject *parent)
{
    setParent(parent);
    node_type = "Synch";
}
//--------------------------------------------------------------------------------------------------
Synch::~Synch() { }
//--------------------------------------------------------------------------------------------------
void Synch::init()
{
}
//--------------------------------------------------------------------------------------------------
bool Synch::isAllInputsReady()
{
    return ( Node::isAllInputsReady());
}
//--------------------------------------------------------------------------------------------------
void Synch::set_input(int in_port_id, QVariant* varvalue)
{
     Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Synch::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        src_assigned[in_id] = true;
        input1_value = varvalue;
    }
    if (in_id==1)
    {
        src_assigned[in_id] = true;
        input2_value = varvalue;
    }
    if (in_id==2)
    {
        src_assigned[in_id] = true;
        input3_value = varvalue;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* Synch::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Synch::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int Synch::execute()
{
    clearOutputData();

    outData.clear();
    outData.push_back(input1_value);

    if(src_assigned.at(1))
        outData.push_back(input2_value);

    if(src_assigned.at(2))
        outData.push_back(input3_value);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    setReadyOutPorts(8 + getNi().NumInputs + 1, true);
    setReadyOutPorts(8 + getNi().NumInputs + 2, true);

    markExecuted();
    return 0;
}
//------------------------------------------------------------------------------
int Synch::validateDesign()
{
    return 0;
}
//------------------------------------------------------------------------------
int Synch::validateRun()
{
    return 0;
}
//------------------------------------------------------------------------------
void Synch::clearNodeData()
{
}
//------------------------------------------------------------------------------
Synch* Synch::copy() const
{
    return new Synch(*this);
}
//--------------------------------------------------------------------------------------------------
QString Synch::uniqueKey() const
{
    return "Synch";
}
//--------------------------------------------------------------------------------------------------
void Synch::markDirty()
{
//	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
bool Synch::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Synch::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------

