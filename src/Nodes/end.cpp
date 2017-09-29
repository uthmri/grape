/* End.cpp
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
#include <Items/EndItem.h>
#include <Nodes/end.h>
#include <xmlreclib.h>

//--------------------------------------------------------------------------------------------------

End::End(QObject *parent)
{
    setParent(parent);
    node_type = "End";
}
//--------------------------------------------------------------------------------------------------
End::~End() { }
//--------------------------------------------------------------------------------------------------
void End::init()
{
}
//--------------------------------------------------------------------------------------------------
bool End::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void End::set_input(int in_port_id, QVariant* varvalue)
{
     Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void End::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id == 0)
        src_assigned[in_id] = true;
}
//--------------------------------------------------------------------------------------------------
QVariant* End::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QString End::getEndType() const
{
    return EndType;
}
//--------------------------------------------------------------------------------------------------
void End::setEndType(const QString &value)
{
    EndType = value;
}
//--------------------------------------------------------------------------------------------------
QVariant* End::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int End::execute()
{
    clearOutputData();

    QVariant* qv = new QVariant();
    qv->setValue(0);
    outData.push_back(qv);

    EndType = nodeItem->propertyValue("EndType").toString();
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int End::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int End::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void End::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
End* End::copy() const
{
    return new End(*this);
}
//--------------------------------------------------------------------------------------------------
QString End::uniqueKey() const
{
    return "End";
}
//--------------------------------------------------------------------------------------------------
void End::markDirty()
{
//	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
bool End::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void End::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------


