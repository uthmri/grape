/* ImageSegmentation.cpp
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
#include <Nodes/imagesegmentation.h>
#include <common.h>

//--------------------------------------------------------------------------------------------------
ImageSegmentation::ImageSegmentation(QObject *parent)
{
    setParent(parent);
    node_type = "ImageSegmentation";
}
//--------------------------------------------------------------------------------------------------
ImageSegmentation::~ImageSegmentation() { }
//--------------------------------------------------------------------------------------------------
void ImageSegmentation::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageSegmentation::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentation::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentation::set_input_by_index(int in_id, QVariant* varvalue)
{    
    QString imagesegmentationType = nodeItem->propertyValue("ImageSegmentationType").toString();

    if(imagesegmentationType=="Atropos")
    {
        if(in_id==0)
        {
            in_fname1 = varvalue->toString();
            src_assigned[in_id] = true;
        }
        if(in_id==1)
        {
            in_fname2 = varvalue->toString();
            src_assigned[in_id] = true;
        }
        if(in_id==2)
        {
            in_fname3 = varvalue->toString();
            src_assigned[in_id] = true;
        }
        if(in_id==3)
        {
            mask_image = varvalue->toString();
            src_assigned[in_id] = true;
        }
    }
    else if(imagesegmentationType=="Threshold")
    {
        if(in_id==3)
        {
            mask_image = varvalue->toString();
            mask_image = mask_image;
            src_assigned[in_id] = true;
        }
        if(in_id==0)
        {
            in_fname1 = varvalue->toString();
            in_fname1 = in_fname1 + ".hdr";
            src_assigned[in_id] = true;
        }
    }
    else
    {
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageSegmentation::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageSegmentation::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int ImageSegmentation::execute()
{
    clearOutputData();

    int numberconnected = Node::getConnectedInPorts().length();

    bool output_found = false;

    if(nodeItem->containsProperty("CommandPath"))
        command_name = nodeItem->propertyValue("CommandPath").toString();

    if(nodeItem->containsProperty("Arg1"))
        arg1 = nodeItem->propertyValue("Arg1").toString();

    if(nodeItem->containsProperty("Arg2"))
        arg2 = nodeItem->propertyValue("Arg2").toString();

    if(nodeItem->containsProperty("Output1"))
        out_fname1 = nodeItem->propertyValue("Output1").toString();

    if(nodeItem->containsProperty("Output2"))
        out_fname2 = nodeItem->propertyValue("Output2").toString();

    QString imagesegmentationType = nodeItem->propertyValue("ImageSegmentationType").toString();

    if(imagesegmentationType=="Atropos")
    {
        QString command = AtroposPath + ' ' + arg1 + ' ' + arg2;

        if(numberconnected==2)
        {
            command = command + " -a " + in_fname1 + " -o [" + out_fname1 + "," +out_fname2 +"]" + " -x "+ mask_image;
            system(command.toUtf8());
        }
        else if (numberconnected==3)
        {
            command = command + " -a "+ in_fname1 + + " -a "+ in_fname2  + "-o [" + out_fname1 + "," +out_fname2 +"]"+ + " -x " + mask_image;

            system(command.toUtf8());
        }
        else if(numberconnected==4)
        {
            command = command + " -a "+ in_fname1 + " -a " + in_fname2 + " -a " + in_fname3 + " -o " + out_fname1 +" -x " + mask_image;
            system(command.toUtf8());
        }
        output_found = true;
    }
    else if(imagesegmentationType=="Threshold")
    {
        QString command = ImageThresholdPath + " 3 " + mask_image + ' ' + in_fname1 +  " Otsu 4";
        system(command.toUtf8());
        output_found = true;
    }
    else
    {
    }

    outData.clear();
    QVariant* qv0 = new QVariant();
    QVariant* qv1 = new QVariant();
    qv0->setValue(out_fname1);
    qv1->setValue(out_fname2);
    outData.push_back(qv0);
    outData.push_back(qv1);

    setReadyOutPorts(8 + getNi().NumInputs + 0, output_found);
    setReadyOutPorts(8 + getNi().NumInputs + 1, output_found);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageSegmentation::validateDesign()
{
    // check the segmenttaion executables exist
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
    QString imagesegmentationType = nodeItem->propertyValue("ImageSegmentationType").toString();
    if(imagesegmentationType=="Atropos")
    {
        if (osType=="windows") AtroposPath = commandPath + "/Atropos.exe";
        else AtroposPath = commandPath + "/Atropos";
        QFileInfo checkFile(AtroposPath);
        if (checkFile.exists() && checkFile.isFile())
            FileFound = true;
        else
        {
            qDebug() << "Cannot find path to Atropos";
            return -1;
        }
    }
    else if(imagesegmentationType=="Threshold")
    {
        if (osType=="windows") ImageThresholdPath = commandPath + "/ImageThreshold.exe";
        else ImageThresholdPath = commandPath + "/ImageThreshold";
        QFileInfo checkFile(ImageThresholdPath);
        if (checkFile.exists() && checkFile.isFile())
            FileFound = true;
        else
        {
            qDebug() << "Cannot find path to ImageThreshold";
            return -1;
        }
    }
    if(!FileFound)
    {
        qDebug() << "imagesegmentation: Cannot find executable";
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageSegmentation::validateRun()
{
    // check the input file exist
    QFileInfo checkFile1(in_fname1);
    if (!(checkFile1.exists() && checkFile1.isFile()))
    {
        qDebug() << "Cannot find input file:" << in_fname1;
        return -1;
    }
    QFileInfo checkFile(mask_image);
    if (!(checkFile.exists() && checkFile.isFile()))
    {
        qDebug() << "Cannot find mask file:" << mask_image;
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentation::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
ImageSegmentation* ImageSegmentation::copy() const
{
    return new ImageSegmentation(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentation::uniqueKey() const
{
    return "ImageSegmentation";
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentation::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageSegmentation::isSuperfluous() const
{
    return false;
}

//--------------------------------------------------------------------------------------------------
void ImageSegmentation::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
