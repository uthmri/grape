/* arc.cpp
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
#include <Nodes/arc.h>


arc::arc(QObject *parent)
{
    setParent(parent);
    node_type = "arc";
    setExecutionMode(NON_EXECUATABLE);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
arc::~arc() { }
//--------------------------------------------------------------------------------------------------
void arc::init()
{
}
//--------------------------------------------------------------------------------------------------
bool arc::isAllInputsReady()
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void arc::set_input(int in_port_id, QVariant* varvalue)
{
}
//--------------------------------------------------------------------------------------------------
void arc::set_input_by_index(int in_id, QVariant* varvalue)
{
}
//--------------------------------------------------------------------------------------------------
QVariant* arc::get_output(int out_port_id)
{
    return NULL;
}
//--------------------------------------------------------------------------------------------------
QVariant* arc::get_output_by_index(int out_id)
{
    return NULL;
}
//------------------------------------------------------------------------------
int arc::execute()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int arc::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int arc::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void arc::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
arc* arc::copy() const
{
    return new arc(*this);
}
//--------------------------------------------------------------------------------------------------
QString arc::uniqueKey() const
{
    return "node";
}
//--------------------------------------------------------------------------------------------------
void arc::markDirty()
{
//	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
bool arc::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
