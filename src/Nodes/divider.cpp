/* Divider.cpp
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
#include <DividerItem.h>
#include <Nodes/divider.h>
#include "xmlreclib.h"

//--------------------------------------------------------------------------------------------------
Divider::Divider(QObject *parent)
{
    setParent(parent);
    node_type = "Divider";
}
//--------------------------------------------------------------------------------------------------
Divider::~Divider() { }
//--------------------------------------------------------------------------------------------------
void Divider::init()
{
}
//--------------------------------------------------------------------------------------------------
bool Divider::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void Divider::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Divider::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        src_assigned[in_id] = true;
        input1_value = varvalue;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* Divider::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Divider::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int Divider::execute()
{
    outData.append(input1_value);
    outData.append(input1_value);
    outData.append(input1_value);
    markExecuted();
    clearOutputData();
    setReadyOutPorts(8 + getNi().NumInputs + 0);
    setReadyOutPorts(8 + getNi().NumInputs + 1);
    setReadyOutPorts(8 + getNi().NumInputs + 2);
    return 0;
}
//------------------------------------------------------------------------------
int Divider::validateDesign()
{
    return 0;
}
//------------------------------------------------------------------------------
int Divider::validateRun()
{
    return 0;
}
//------------------------------------------------------------------------------
void Divider::clearNodeData()
{
}
//------------------------------------------------------------------------------
Divider* Divider::copy() const
{
    return new Divider(*this);
}
//--------------------------------------------------------------------------------------------------
QString Divider::uniqueKey() const
{
    return "Divider";
}
//--------------------------------------------------------------------------------------------------
void Divider::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Divider::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Divider::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------

