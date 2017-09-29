/* Systemcall.cpp
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
#include <Nodes/Systemcall.h>


//--------------------------------------------------------------------------------------------------
Systemcall::Systemcall(QObject *parent)
{
    setParent(parent);
    node_type = "Systemcall";
}
//--------------------------------------------------------------------------------------------------
Systemcall::~Systemcall() { }
//--------------------------------------------------------------------------------------------------
void Systemcall::init()
{
}
//--------------------------------------------------------------------------------------------------
void Systemcall::set_input(int in_port_id, QVariant* varvalue)
{
   Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void Systemcall::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        vI1 = varvalue;  
        src_assigned[in_id] = true;
    }
    if(in_id==1)
    {
        vI2 = varvalue;  
        src_assigned[in_id] = true;
    }
    if(in_id==2)
    {
        vI3 = varvalue;  
        src_assigned[in_id] = true;
    }
    if(in_id==3)
    {
        vI4 = varvalue;
        src_assigned[in_id] = true;
    }
    if(in_id==4)
    {
        vI5 = varvalue;
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
bool Systemcall::isAllInputsReady()
{
    QString arg_src;

    arg_src =  nodeItem->propertyValue("Ar1Port").toString();
    if(!((arg_src=="fixed") ||(arg_src=="none")))
    {
        if( (arg_src=="I1") && (!src_assigned[0]) )            return false;
        if( (arg_src=="I2") && (!src_assigned[1]) )            return false;
        if( (arg_src=="I3") && (!src_assigned[2]) )            return false;
        if( (arg_src=="I4") && (!src_assigned[3]) )            return false;
        if( (arg_src=="I5") && (!src_assigned[4]) )            return false;
    }

    arg_src =  nodeItem->propertyValue("Arg2Port").toString();
    if(!((arg_src=="fixed") ||(arg_src=="none")))
    {
        if( (arg_src=="I1") && (!src_assigned[0]) )            return false;
        if( (arg_src=="I2") && (!src_assigned[1]) )            return false;
        if( (arg_src=="I3") && (!src_assigned[2]) )            return false;
        if( (arg_src=="I4") && (!src_assigned[3]) )            return false;
        if( (arg_src=="I5") && (!src_assigned[4]) )            return false;
    }

    arg_src =  nodeItem->propertyValue("Arg3Port").toString();
    if(!((arg_src=="fixed") ||(arg_src=="none")))
    {
        if( (arg_src=="I1") && (!src_assigned[0]) )            return false;
        if( (arg_src=="I2") && (!src_assigned[1]) )            return false;
        if( (arg_src=="I3") && (!src_assigned[2]) )            return false;
        if( (arg_src=="I4") && (!src_assigned[3]) )            return false;
        if( (arg_src=="I5") && (!src_assigned[4]) )            return false;
    }

    arg_src =  nodeItem->propertyValue("Arg4Port").toString();
    if(!((arg_src=="fixed") ||(arg_src=="none")))
    {
        if( (arg_src=="I1") && (!src_assigned[0]) )            return false;
        if( (arg_src=="I2") && (!src_assigned[1]) )            return false;
        if( (arg_src=="I3") && (!src_assigned[2]) )            return false;
        if( (arg_src=="I4") && (!src_assigned[3]) )            return false;
        if( (arg_src=="I5") && (!src_assigned[4]) )            return false;
    }

    arg_src =  nodeItem->propertyValue("Arg5Port").toString();
    if(!((arg_src=="fixed") ||(arg_src=="none")))
    {
        if( (arg_src=="I1") && (!src_assigned[0]) )            return false;
        if( (arg_src=="I2") && (!src_assigned[1]) )            return false;
        if( (arg_src=="I3") && (!src_assigned[2]) )            return false;
        if( (arg_src=="I4") && (!src_assigned[3]) )            return false;
        if( (arg_src=="I5") && (!src_assigned[4]) )            return false;
    }
    return true;
}
//--------------------------------------------------------------------------------------------------
QVariant* Systemcall::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant *Systemcall::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int Systemcall::execute()
{
    clearOutputData();
    QString pr = "Command";
    if(nodeItem->containsProperty(pr))
       command_name = nodeItem->propertyValue(pr).toString();
    else
    {
       qDebug() << "Systemcall: Cannot find command";
       return -1;
    }

    qDebug()  << "Systemcall: command_name =" << command_name;
    QString fixed_arg1 = "", fixed_arg2 = "", fixed_arg3 = "", fixed_arg4 = "", fixed_arg5 = "";

    if(nodeItem->containsProperty("Arg1"))        fixed_arg1 = nodeItem->propertyValue("Arg1").toString();
    if(nodeItem->containsProperty("Arg2"))        fixed_arg2 = nodeItem->propertyValue("Arg2").toString();
    if(nodeItem->containsProperty("Arg3"))        fixed_arg3 = nodeItem->propertyValue("Arg3").toString();
    if(nodeItem->containsProperty("Arg4"))        fixed_arg4 = nodeItem->propertyValue("Arg4").toString();
    if(nodeItem->containsProperty("Arg5"))        fixed_arg5 = nodeItem->propertyValue("Arg5").toString();

    QString command = command_name;
    QString argPort1 =  nodeItem->propertyValue("Arg1Port").toString();
    QString argPort2  =  nodeItem->propertyValue("Arg2Port").toString();
    QString argPort3 =  nodeItem->propertyValue("Arg3Port").toString();
    QString argPort4 =  nodeItem->propertyValue("Arg4Port").toString();
    QString argPort5 =  nodeItem->propertyValue("Arg5Port").toString();

    if (argPort1=="none")           command = command;
    else if(argPort1=="I1")        command = command + ' ' + vI1->toString()  ;
    else if(argPort1=="I2")         command = command + ' ' + vI2->toString()  ;
    else if(argPort1=="I3")         command = command + ' ' + vI3->toString()  ;
    else if(argPort1=="I4")         command = command + ' ' + vI4->toString()  ;
    else if(argPort1=="I5")         command = command + ' ' + vI5->toString()  ;
    else if(argPort1=="fixed")      command = command + ' ' + fixed_arg1;

    if (argPort2=="none")        ;//command = command;
    else if(argPort2=="I1")        command = command + ' ' + vI1->toString()  ;
    else if(argPort2=="I2")        command = command + ' ' + vI2->toString()  ;
    else if(argPort2=="I3")        command = command + ' ' + vI3->toString()  ;
    else if(argPort2=="I4")        command = command + ' ' + vI4->toString()  ;
    else if(argPort2=="I5")        command = command + ' ' + vI5->toString()  ;
    else if(argPort2=="fixed")     command = command + ' ' + fixed_arg2;

    if (argPort3=="none")        ;//command = command;
    else if(argPort3=="I1")        command = command + ' ' + vI1->toString()  ;
    else if(argPort3=="I2")        command = command + ' ' + vI2->toString()  ;
    else if(argPort3=="I3")        command = command + ' ' + vI3->toString()  ;
    else if(argPort3=="I4")        command = command + ' ' + vI4->toString()  ;
    else if(argPort3=="I5")        command = command + ' ' + vI5->toString()  ;
    else if(argPort3=="fixed")     command = command + ' ' + fixed_arg3;

    if (argPort4=="none")        ;//command = command;
    else if(argPort4=="I1")        command = command + ' ' + vI1->toString();
    else if(argPort4=="I2")        command = command + ' ' + vI2->toString();
    else if(argPort4=="I3")        command = command + ' ' + vI3->toString();
    else if(argPort4=="I4")        command = command + ' ' + vI4->toString();
    else if(argPort4=="I5")        command = command + ' ' + vI5->toString();
    else if(argPort4=="fixed")     command = command + ' ' + fixed_arg4;

    if (argPort5=="none")        ;//command = command;
    else if(argPort5=="I1")        command = command + ' ' + vI1->toString();
    else if(argPort5=="I2")        command = command + ' ' + vI2->toString();
    else if(argPort5=="I3")        command = command + ' ' + vI3->toString();
    else if(argPort5=="I4")        command = command + ' ' + vI4->toString();
    else if(argPort5=="I5")        command = command + ' ' + vI5->toString();
    else if(argPort5=="fixed")     command = command + ' ' +  fixed_arg5;

    command.remove(QRegExp("[\\n\\r]"));
    qDebug()  << "Systemcall: command_name =" << command;
    system(command.toUtf8());


// assign outouts
    QString userString;
    QString v11 ;
    QStringList ports;
    ports.push_back("Output1Port");
    ports.push_back("Output2Port");
    ports.push_back("Output3Port");

    QVariant* qv;
    outData.clear();

    for(int ii=0; ii<ports.length(); ii++)
    {
        qv = new QVariant();
        v11 =  nodeItem->propertyValue(ports.at(ii)).toString();

        if (v11=="none")            qv->setValue((int)0);
        else if (v11=="I1")         qv->setValue(vI1->toString());
        else if(v11=="I2")          qv->setValue(vI2->toString());
        else if(v11=="I3")          qv->setValue(vI3->toString());
        else if(v11=="I4")          qv->setValue(vI4->toString());
        else if(v11=="I5")          qv->setValue(vI5->toString());
        else if(v11=="fixed")
        {
            if(ii==0) userString = nodeItem->propertyValue("Output1").toString();
            if(ii==1) userString = nodeItem->propertyValue("Output2").toString();
            if(ii==2) userString = nodeItem->propertyValue("Output3").toString();

            qv->setValue(userString);
        }
        outData.push_back(qv);
    }

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    setReadyOutPorts(8 + getNi().NumInputs + 1, true);
    setReadyOutPorts(8 + getNi().NumInputs + 2, true);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Systemcall::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int Systemcall::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void Systemcall::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
Systemcall* Systemcall::copy() const
{
    return new Systemcall(*this);
}
//--------------------------------------------------------------------------------------------------
QString Systemcall::uniqueKey() const
{
    return "Systemcall";
}
//--------------------------------------------------------------------------------------------------
void Systemcall::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool Systemcall::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void Systemcall::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------


