/* line.cpp
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
#include <Nodes/line.h>


//--------------------------------------------------------------------------------------------------
QVariant* line::get_output_by_index(int out_id)
{
    return NULL;
}
//--------------------------------------------------------------------------------------------------
void line::set_input_by_index(int in_id, QVariant* varvalue)
{
}
//--------------------------------------------------------------------------------------------------
line::line(QObject *parent)
{
    setParent(parent);
    node_type = "line";
    setExecutionMode(NON_EXECUATABLE);
}
//--------------------------------------------------------------------------------------------------
line::~line() { }
//--------------------------------------------------------------------------------------------------
void line::init()
{
}
//--------------------------------------------------------------------------------------------------
void line::set_input(int in_port_id, QVariant* varvalue)
{
}
//--------------------------------------------------------------------------------------------------
bool line::isAllInputsReady()
{
    return false;
}
//--------------------------------------------------------------------------------------------------
QVariant* line::get_output(int out_port_id)
{
    return NULL;
}
//--------------------------------------------------------------------------------------------------
int line::execute()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int line::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int line::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void line::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
line* line::copy() const
{
    return new line(*this);
}
//--------------------------------------------------------------------------------------------------
QString line::uniqueKey() const
{
    return "line";
}
//--------------------------------------------------------------------------------------------------
void line::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool line::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------

