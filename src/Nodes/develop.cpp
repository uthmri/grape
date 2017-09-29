/* Develop.cpp
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
#include <Nodes/develop.h>


//--------------------------------------------------------------------------------------------------
Develop::Develop(QObject *parent)
{
    setParent(parent);
    node_type = "Develop";
}
//--------------------------------------------------------------------------------------------------
Develop::~Develop() { }
//--------------------------------------------------------------------------------------------------
void Develop::init()
{
}
//--------------------------------------------------------------------------------------------------
void Develop::set_input(int in_port_id, QVariant* varvalue)
{
   Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Develop::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        vI1 = varvalue;
        src_assigned[in_id] = true;
    }
    else if(in_id==1)
    {
        vI2 = varvalue;
        src_assigned[in_id] = true;
    }
   else  if(in_id==2)
    {
        vI3 = varvalue;
        src_assigned[in_id] = true;
    }
   else  if(in_id==3)
    {
        vI4 = varvalue;
        src_assigned[in_id] = true;
    }
  else   if(in_id==4)
    {
        vI5 = varvalue;
        src_assigned[in_id] = true;
    }
    if(in_id==5)
    {
        vI6 = varvalue;
        src_assigned[in_id] = true;
    }
  else   if(in_id==6)
    {
        vI7 = varvalue;
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
bool Develop::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
QVariant* Develop::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Develop::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int Develop::execute()
{
     clearOutputData();
    QString userFcn =  nodeItem->propertyValue("Arg1Port").toString();

    if(userFcn == "f0") // example add two numbers
    {
        vO1 = new QVariant();
        vO1->setValue( vI1->toDouble() + vI2->toDouble());
        outData.insert(0,vO1);
        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    }
    else if(userFcn == "f1") // aexample add and subtract two numbers
    {
        vO1 = new QVariant();
        vO2 = new QVariant();
        vO1->setValue(vI1->toDouble() + vI2->toDouble());
        vO2->setValue(vI1->toDouble() - vI2->toDouble());
        outData.insert(0,vO1);
        outData.insert(1,vO2);

        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
        setReadyOutPorts(8 + getNi().NumInputs + 1, true);
    }
    else if(userFcn == "f2")
    {
    }
    else if(userFcn == "f3")
    {
    }
    else if(userFcn == "f4")
    {
    }
    else if(userFcn == "f5")
    {
    }
    else if(userFcn == "f6")
    {
    }
    else
    {
    }

    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Develop::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Develop::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void Develop::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
Develop* Develop::copy() const
{
    return new Develop(*this);
}
//--------------------------------------------------------------------------------------------------
QString Develop::uniqueKey() const
{
    return "Develop";
}
//--------------------------------------------------------------------------------------------------
void Develop::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Develop::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Develop::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------

