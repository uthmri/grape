/* BiasCorrection.cpp
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
#include <Nodes/biascorrection.h>
#include <common.h>
#include <xmlreclib.h>

//--------------------------------------------------------------------------------------------------
BiasCorrection::BiasCorrection(QObject *parent)
{
    setParent(parent);
    node_type = "BiasCorrection";
}
//--------------------------------------------------------------------------------------------------
BiasCorrection::~BiasCorrection() { }
//--------------------------------------------------------------------------------------------------
void BiasCorrection::init()
{
}
//--------------------------------------------------------------------------------------------------
bool BiasCorrection::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrection::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void BiasCorrection::set_input_by_index(int in_id, QVariant* varvalue)
{
    if (in_id == 0)
    {
        in_fname = varvalue->toString();
        src_assigned[in_id] = true;
    }
    else if (in_id == 1)
    {
        mask_fname = varvalue->toString();
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* BiasCorrection::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* BiasCorrection::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int BiasCorrection::execute()
{
    clearOutputData();

    QString commandpath;
    QString outputPrefix;
    QString dimension;
    QString shrinkFactor;
    QString convergence;
    QString bSplineFitting;
    QString command;

    commandpath = nodeItem->propertyValue("CommandPath").toString();
    outputPrefix = nodeItem->propertyValue("OutputPrefix").toString();
    dimension = nodeItem->propertyValue("Dimension").toString();
    shrinkFactor = nodeItem->propertyValue("ShrinkFactor").toString();
    convergence = nodeItem->propertyValue("Convergence").toString();
    bSplineFitting = nodeItem->propertyValue("BSplineFitting").toString();

    QFileInfo info1(in_fname);

    QString directory = info1.path();
    out_fname = directory+"/" + nodeItem->propertyValue("OutputPrefix").toString() + info1.fileName();
    QString biascorrectiontype = nodeItem->propertyValue("BiasCorrectionType").toString();
    if(biascorrectiontype=="N3")
    {
        //QString command = ANTSN3Path +" " + dimension + " " + in_fname + " " + out_fname + " "+shrinkFactor+ " "+mask_fname+" "+convergence;
        QString command = ANTSN3Path +" " + dimension + " " + in_fname + " " + out_fname + " "+shrinkFactor+ " "+convergence;
        qDebug()<<"N3_command="<<command.toUtf8();
        system(command.toUtf8());
    }
    else if(biascorrectiontype=="N4")
    {
        if(src_assigned[1])  // mask file passed
            //command = commandpath + "N4BiasFieldCorrection" +" -d " + dimension + " -i " + in_fname + ' ' + "-x "+ mask_fname+ ' '+ "-o "+ out_fname+ ' '+ "-s "+shrinkFactor+ ' '+ "-c "+convergence+ ' '+ "-b "+"["+bSplineFitting+"]";
            command = ANTSN4Path +" -d " + dimension + " -i " + in_fname + ' ' + "-x "+ mask_fname+ ' '+ "-o "+ out_fname+ ' '+ "-s "+shrinkFactor+ ' '+ "-c "+convergence+ ' '+ "-b "+"["+bSplineFitting+"]";
        else
            //command = commandpath + "N4BiasFieldCorrection" +" -d " + dimension + " -i " + in_fname + ' ' + "-o "                        + out_fname+ ' '+ "-s "+shrinkFactor+ ' '+ "-c "+convergence+ ' '+ "-b "+"["+bSplineFitting+"]";
            command = ANTSN4Path +" -d " + dimension + " -i " + in_fname + ' ' + "-o " + out_fname+ ' '+ "-s "+shrinkFactor+ ' '+ "-c "+convergence+ ' '+ "-b "+"["+bSplineFitting+"]";
        qDebug()<<"N4_command="<<command.toUtf8();
        system(command.toUtf8());
    }
    else
    {
    }

    qv = new QVariant(out_fname);
    outData.clear();
    outData.push_back(qv);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int BiasCorrection::validateDesign()
{
    // checking that the executable exist
    bool FileFound = false;
    QSysInfo sysInfo;
    QString osType = sysInfo.productType();

    // check the user path, if any ............................
    commandPath = nodeItem->propertyValue("CommandPath").toString();
    // if no user path, check the config file
    if(!commandPath.length())
    {
        commandPath = this->getDiagram()->mainPipeline.getAppPath("ANTS");
        if(commandPath.length()==0)
            return -1;
    }

    // check OS type ............................................
    QString biascorrectiontype = nodeItem->propertyValue("BiasCorrectionType").toString();
    if(biascorrectiontype=="N4")
    {
        if (osType=="windows") ANTSN4Path = commandPath + "/N4BiasFieldCorrection.exe";
        else ANTSN4Path = commandPath + "/N4BiasFieldCorrection";
        QFileInfo checkFile(ANTSN4Path);
        if (checkFile.exists() && checkFile.isFile())
            FileFound = true;
        else
        {
            qDebug() << "Cannot find path to N4";
            return -1;
        }
    }
    else if(biascorrectiontype=="N3")
    {
        if (osType=="windows") ANTSN3Path = commandPath + "/N3BiasFieldCorrection.exe";
        else ANTSN3Path = commandPath + "/N3BiasFieldCorrection";
        QFileInfo checkFile(ANTSN3Path);
        if (checkFile.exists() && checkFile.isFile())
            FileFound = true;
        else
        {
            qDebug() << "Cannot find path to N3";
            return -1;
        }
    }

    if(!FileFound)
    {
        qDebug() << "BiasCorrection: Cannot find executable";
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------
int BiasCorrection::validateRun()
{
    QFileInfo checkFile1(in_fname);
    if (!(checkFile1.exists() && checkFile1.isFile()))
    {
        return -1;
    }
    if(src_assigned[1])  // mask file passed
    {
        QFileInfo checkFile1(mask_fname);
        if (!(checkFile1.exists() && checkFile1.isFile()))
        {
            return -1;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void BiasCorrection::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
BiasCorrection* BiasCorrection::copy() const
{
    return new BiasCorrection(*this);
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrection::uniqueKey() const
{
    return "BiasCorrection";
}
//--------------------------------------------------------------------------------------------------
void BiasCorrection::markDirty()
{
    //	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
bool BiasCorrection::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void BiasCorrection::setConnectedInPorts()
{
}

