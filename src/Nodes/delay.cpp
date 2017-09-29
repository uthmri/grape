/* Delay.cpp
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
#include <Nodes/delay.h>
#include <xmlreclib.h>


//--------------------------------------------------------------------------------------------------
Delay::Delay(QObject *parent)
{
    setParent(parent);
    node_type = "Delay";
    timer_started = false;
    outData.clear();
}
//--------------------------------------------------------------------------------------------------
Delay::~Delay() { }
//--------------------------------------------------------------------------------------------------
void Delay::init()
{
    timer_started = false;
    outData.clear();
}
//--------------------------------------------------------------------------------------------------
bool Delay::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void Delay::set_input(int in_port_id, QVariant* varvalue)
{
     Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Delay::set_input_by_index(int in_id, QVariant* varvalue)
{
    if (in_id == 0)
    {
        output = new QVariant();
        //output->setValue(varvalue); // will output the input
        output = varvalue;

        src_assigned[in_id] = true;

        // start the timer now !
        delaytime = nodeItem->propertyValue("DelayTime").toDouble();
        starttime = QTime::currentTime().minute()*60*1000 + QTime::currentTime().second()*1000 + QTime::currentTime().msec();
        timer_started = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* Delay::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Delay::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int Delay::execute()
{
    clearOutputData();

    outData.append(output);

    markExecuted();
    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Delay::validateDesign()
{
    int requested_delaytime = nodeItem->propertyValue("DelayTime").toDouble();
    if(requested_delaytime < 0)
    {
        qDebug() << "Delay: Requested delay time < 0";
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Delay::validateRun()
{

    bool input_ready = Node::isAllInputsReady();
    bool input_ready_and_time_up;// = Node::isAllInputsReady();

    if(input_ready && timer_started)
    {
        double currtime = QTime::currentTime().minute()*60*1000 + QTime::currentTime().second()*1000 + QTime::currentTime().msec();
        if(currtime > (starttime+delaytime))
        {
            timer_started = false;
            input_ready_and_time_up = true;
        }
    }
    else
    {
        input_ready_and_time_up = false;
    }

    return input_ready_and_time_up;


    //return 0;
}
//--------------------------------------------------------------------------------------------------
void Delay::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
Delay* Delay::copy() const
{
    return new Delay(*this);
}
//--------------------------------------------------------------------------------------------------
QString Delay::uniqueKey() const
{
    return "Delay";
}
//--------------------------------------------------------------------------------------------------
void Delay::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Delay::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Delay::setConnectedInPorts()
{
}

