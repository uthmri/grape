/* Select.cpp
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
#include <SelectItem.h>
#include <Nodes/select.h>

//--------------------------------------------------------------------------------------------------
Select::Select(QObject *parent)
{
    setParent(parent);
    node_type = "Select";
}
//--------------------------------------------------------------------------------------------------
Select::~Select() { }
//--------------------------------------------------------------------------------------------------
void Select::init()
{
}
//--------------------------------------------------------------------------------------------------
bool Select::isAllInputsReady()
{
   return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void Select::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Select::set_input_by_index(int in_id, QVariant* varvalue)
{
   if(in_id==0)
   {
       src_assigned[in_id] = true;
       index_value = varvalue->toInt();
   }
}
//--------------------------------------------------------------------------------------------------
QVariant* Select::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Select::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int Select::execute()
{
    clearOutputData();
    bool string_found = false;
    if(nodeItem->containsProperty("DataList"))
    {
        QString s1;
        QString text = nodeItem->propertyValue("DataList").toString();

        QRegExp rx("\\{*\\}");    // find the string between braces
        int count = 1;
        int pos = 0, newpos = 0;
        while ((newpos = rx.indexIn(text, pos)) != -1)
        {
            s1 = text.mid(pos+1, newpos-pos-1);
            pos = newpos + 1;
            if(count == index_value)
            {
                string_found = true;
                QVariant* qv = new QVariant();
                qv->setValue(s1);
                outData.clear();
                outData.push_back(qv);
                //data_value = s1;
                break;
            }
            count++;
        }
    }

    setReadyOutPorts(8 + getNi().NumInputs + 0, string_found);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Select::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Select::validateRun()
{
    if(!nodeItem->containsProperty("DataList"))
        return -1;
    return 0;
}
//--------------------------------------------------------------------------------------------------
void Select::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
Select* Select::copy() const
{
    return new Select(*this);
}
//--------------------------------------------------------------------------------------------------
QString Select::uniqueKey() const
{
    return "Select";
}
//--------------------------------------------------------------------------------------------------
void Select::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Select::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Select::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
