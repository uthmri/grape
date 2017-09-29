/* BET.cpp
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
#include <BETItem.h>
#include <Nodes/bet.h>
#include <xmlreclib.h>
#include <QSysInfo>

//--------------------------------------------------------------------------------------------------
BET::BET(QObject *parent)
{
    setParent(parent);
    node_type = "BET";
}
//--------------------------------------------------------------------------------------------------
BET::~BET() { }
//--------------------------------------------------------------------------------------------------
void BET::init()
{
}
//--------------------------------------------------------------------------------------------------
bool BET::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void BET::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void BET::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id == 0)
    {
        in_fname = varvalue->toString();
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* BET::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* BET::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int BET::execute()
{
    QFileInfo info1(in_fname);
    QString directory = info1.path()+"/";
    out_fname = directory + nodeItem->propertyValue("OutputFile").toString() + info1.baseName();// + QString(".hdr");
    clearOutputData();

    in_fname = info1.path()+"/"+info1.baseName();

    QString commandPath = nodeItem->propertyValue("CommandPath").toString();
    if(!commandPath.length())
    {
        QString config_ants_path = this->getDiagram()->mainPipeline.getAppPath("BET");
        if(config_ants_path.length())
            commandPath = config_ants_path;
        else
        {
            qDebug() << "Cannot find path to BET";
            return -1;
        }
    }
    QString fit;
    QString command = commandPath + "/bet " + in_fname + " " + out_fname;

    fit =  " -f " + nodeItem->propertyValue("FractionalIntensityThreshold").toString();
    command = command + " " + fit ;

    if(nodeItem->propertyValue("GenerateBrainSurfaceOutline").toBool())
        command = command + " -o";

    if(nodeItem->propertyValue("BinaryBrainMask").toBool())
        command = command + " -m";

    if(nodeItem->propertyValue("SegmentedBrainImage").toBool())
        command = command + " -n";

    if(nodeItem->propertyValue("ApproximateSkullImage").toBool())
        command = command + " -s";

    if(nodeItem->propertyValue("ApplyThresholdingToSegmentedBrainImageAndMask").toBool())
        command = command + " -t";

    QSysInfo sysInfo;//
    QString sysMod = sysInfo.productVersion();
    qDebug() << "sysMod ="<<sysMod;

    QString vgifit = " -g " + nodeItem->propertyValue("VerticaGradientInFractionalIintensityThreshold").toString();
    command = command + " " + vgifit;

    // Call BET
    qDebug() << "command  = " << command ;
    system(command.toUtf8());

    QFileInfo info2(out_fname);
    QString osType = sysInfo.productType();
    if (osType=="windows")
    {
        out_fname = out_fname + QString(".hdr");
        mask_fname = directory + info2.completeBaseName() + QString("_mask.hdr");
    }
    else
    {
        out_fname = out_fname + QString(".nii");
        mask_fname = directory + info2.completeBaseName() + QString("_mask.nii");

    }
    qDebug()<<" stripped file = "<<mask_fname;
    qDebug()<<" mask file = "<<mask_fname;

    qv1 = new QVariant(out_fname);
    qv2 = new QVariant(mask_fname);

    outData.clear();
    outData.push_back(qv1);
    outData.push_back(qv2);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    setReadyOutPorts(8 + getNi().NumInputs + 1, true);

    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int BET::validateDesign()
{
    // check the BET executable exists
    bool bet_found = false;
    QSysInfo sysInfo;
    QString osType = sysInfo.productType();

    // check the user path, if any ............................
    commandPath = nodeItem->propertyValue("CommandPath").toString();
    // if no user path, check the config filr
    if(!commandPath.length())
    {
        commandPath = this->getDiagram()->mainPipeline.getAppPath("BET");
        if(commandPath.length()==0)
            return -1;
    }

    // check OS type ............................................
    QFileInfo checkFile(commandPath);
    commandPath = checkFile.path() + "/" + checkFile.baseName();
    QString bet_file;
    if (osType=="windows") bet_file = commandPath + "/bet.exe";
    else bet_file = commandPath + "/bet";
    //qDebug() << " $$$$$$$$$$ commandPath="<<commandPath;
    //qDebug() << " $$$$$$$$$$ bet_file="<<bet_file;
    QFileInfo checkFile1(bet_file);
    if (checkFile1.exists() && checkFile1.isFile())
        bet_found = true;
    else
    {
        qDebug() << "Cannot find path to BET";
        return -1;
    }

    // Check BET parameters ......................................
    float fit = nodeItem->propertyValue("FractionalIntensityThreshold").toString().toFloat();
    float vgifit = nodeItem->propertyValue("VerticaGradientInFractionalIintensityThreshold").toString().toFloat();

    if((fit < 0) || (fit > 1))
    {
        qDebug() << "BET: Incorrect value for parameter FractionalIntensityThreshold";
        return -1;
    }

    if((vgifit < 0) || (vgifit > 1))
    {
        qDebug() << "BET: Incorrect value for parameter VerticaGradientInFractionalIintensityThreshold";
        return -1;
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------
int BET::validateRun()
{
    QFileInfo checkFile(in_fname);
    if (!(checkFile.exists() && checkFile.isFile()))
    {
        qDebug() << "BET:: Could not find image file.";
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void BET::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
BET* BET::copy() const
{
    return new BET(*this);
}
//--------------------------------------------------------------------------------------------------
QString BET::uniqueKey() const
{
    return "BET";
}
//--------------------------------------------------------------------------------------------------
void BET::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool BET::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void BET::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------

