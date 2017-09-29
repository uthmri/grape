/* Loop.cpp
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
#include <LoopItem.h>
#include <Nodes/loop.h>


//--------------------------------------------------------------------------------------------------
Loop::Loop(QObject *parent)
{
    setParent(parent);
    node_type = "Loop";

    counter_start_value = 0;
    counter_end_value = 0;
    counter_step_value = 0;
    counter_value = 0;

    next_input = 0;
    reset_input = 0;
    stop_input = 0;
    end_value;

    first_iter = true;
}
//--------------------------------------------------------------------------------------------------
Loop::~Loop() { }
//--------------------------------------------------------------------------------------------------
void Loop::init()
{
    first_iter = true;
}
//--------------------------------------------------------------------------------------------------
bool Loop::isAllInputsReady()
{
    // This is a special node; we do not need all inputs to be ready, one is enough to execute LOOP
    int Nin = getNi().NumInputs;
    int i, conn_port;
    for(i=0; i<Nin; i++)
    {
        conn_port = i + 8;
        // all mandatory inputs should be ready
        if( ( !inputReadyPorts.contains(conn_port)) && getNi().Inputs.at(i).mandatory )
            return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
void Loop::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Loop::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        // Start/NEXT
        src_assigned[in_id] = true;
        next_input = varvalue->toInt();
        setExecutionMode(EXECUTABLE);
    }
    if(in_id==1)
    {
        // start/RESET
        src_assigned[in_id] = true;
        first_iter = true;
        reset_input = varvalue->toInt();
        setExecutionMode(EXECUTABLE);
    }
    if(in_id==2)
    {
        // STOP
        src_assigned[in_id] = true;
        stop_input = varvalue->toInt();
        setExecutionMode(HALTED);
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* Loop::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* Loop::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int Loop::execute()
{
    clearOutputData();
    bool loop_output = false;
    bool end_flag_output = false;

    // increment counter only after the first iteration
    if(first_iter)
    {
        counter_value = counter_start_value;
        first_iter = false;
        loop_output = true;
    }
    else
    {
        if(counter_start_value < counter_end_value) // is incrementing counter
        {
            if((counter_value + counter_step_value) <= counter_end_value)
            {
                counter_value += counter_step_value;
                loop_output = true;
            }
        }
        else // decrementing counter; step must be negative
        {
            if((counter_value + counter_step_value) >= counter_end_value)
            {
                counter_value += counter_step_value;
                loop_output = true;
            }
        }
    }

    // check that this is the last iteration (after incr/decr will be out of range)
    if(counter_start_value < counter_end_value) // is incrementing counter
    {
        // send an end signal only if there is a count output
        if( loop_output && ((counter_value + counter_step_value) > counter_end_value) )
        {
            end_flag_output = true;
            end_value = 0;
        }
    }
    else // decrementing counter; step must be negative
    {
        if( loop_output &&  ((counter_value + counter_step_value) < counter_end_value) )
        {
            end_flag_output = true;
            end_value = 0;
        }
    }


    QVariant* qv0 = new QVariant();
    QVariant* qv1 = new QVariant();
    qv0->setValue(counter_value);
    qv1->setValue(counter_end_value);
    outData.clear();
    outData.push_back(qv0);
    outData.push_back(qv1);

    setReadyOutPorts(8 + getNi().NumInputs + 0, loop_output);
    setReadyOutPorts(8 + getNi().NumInputs + 1, end_flag_output);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Loop::validateDesign()
{
    if(nodeItem->containsProperty("StartValue"))
    {
        if(!(nodeItem->propertyValue("StartValue").toString().isEmpty()))
            counter_start_value = nodeItem->propertyValue("StartValue").toString().toFloat();
    }

    if(nodeItem->containsProperty("Step"))
    {
        if(!(nodeItem->propertyValue("Step").toString().isEmpty()))
            counter_step_value = nodeItem->propertyValue("Step").toString().toFloat();
    }

    if(nodeItem->containsProperty("EndValue"))
    {
        if(!(nodeItem->propertyValue("EndValue").toString().isEmpty()))
            counter_end_value = nodeItem->propertyValue("EndValue").toString().toFloat();
    }

    // Test that the user has the correct combination of values
    if( (counter_start_value < counter_end_value) && (counter_step_value<=0) )
    {
        qDebug() << "Loop: wrong loop bound settings" ;
        return -1;
    }
    if( (counter_start_value > counter_end_value) && (counter_step_value>=0) )
    {
        qDebug() << "Loop: wrong loop bound settings" ;
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Loop::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void Loop::clearNodeData()
{
}
//------------------------------------------------------------------------------
bool Loop::isLoopDone()
{
    QString startValue, step, endValue;

    if(nodeItem->containsProperty("StartValue"))
       startValue = nodeItem->propertyValue("StartValue").toString();

    if(nodeItem->containsProperty("Step"))
        step = nodeItem->propertyValue("Step").toString();

    if(nodeItem->containsProperty("EndValue"))
        endValue = nodeItem->propertyValue("EndValue").toString();

   // check that this is the last iteration
    if(counter_start_value < counter_end_value)
    {
       if((counter_value+counter_step_value) > counter_end_value)
            return true;
    }
    else
    {
        if(counter_value < counter_end_value)
            return true;
    }
    return false;
}
//--------------------------------------------------------------------------------------------------
Loop* Loop::copy() const
{
    return new Loop(*this);
}
//--------------------------------------------------------------------------------------------------
QString Loop::uniqueKey() const
{
    return "Loop";
}
//--------------------------------------------------------------------------------------------------
void Loop::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Loop::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Loop::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------


