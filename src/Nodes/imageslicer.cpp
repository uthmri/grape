/* ImageSlicer.cpp
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
#include <Nodes/imageslicer.h>
#include <image/image.hpp>
#include <xmlreclib.h>


//--------------------------------------------------------------------------------------------------
ImageSlicer::ImageSlicer(QObject *parent)
{
   setParent(parent);
   node_type = "ImageSlicer";
    ImageOut = NULL;
}
//--------------------------------------------------------------------------------------------------
ImageSlicer::~ImageSlicer() { }
//--------------------------------------------------------------------------------------------------
void ImageSlicer::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageSlicer::isAllInputsReady()
{
    bool ready =  Node::isAllInputsReady();

    if(!ready)
        return false;

    QString SlicerType =  nodeItem->propertyValue("SlicerType").toString();

    if( (im->NumDimensions == 4) && (SlicerType =="4Dto2D") )
        ready = ready && src_assigned.at(0) && src_assigned.at(2);

    if( (im->NumDimensions == 4) && (SlicerType =="4Dto3D") )
        ready = ready && src_assigned.at(0);

    if( (im->NumDimensions == 4) && (SlicerType =="3Dto2D") )
        ready = ready && src_assigned.at(2);
    return ready;
}
//--------------------------------------------------------------------------------------------------
void ImageSlicer::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageSlicer::set_input_by_index(int in_id, QVariant* varvalue)
{
    QString var = varvalue->typeName();
    //qDebug() << "var="<<var<<"in_id="<<in_id;

    if(in_id==0)
    {
        stack = varvalue->toInt();
        src_assigned[in_id] = true;
    }
    else if(in_id==1)
    {        
        if(var == "ImageC*")
        {
            im = varvalue->value<ImageC*>();
            src_assigned[in_id] = true;
        }
        else
        {
            qDebug() << "ERROR: input to ImageSlicer(" << uniqueKey() << ") must be an image";
            src_assigned[in_id] = false;
        }
    }
    else if(in_id==2)
    {
        slice = varvalue->toInt();
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageSlicer::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageSlicer::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int ImageSlicer::execute()
{
    clearOutputData();
    QString SlicerType =  nodeItem->propertyValue("SlicerType").toString();

    if(SlicerType =="4Dto2D")
    {
        ImageOut = new ImageC(im,2);
        for(int y=0; y<im->array_size[1]; y++)
            for(int x=0; x<im->array_size[0]; x++)
            {
                ImageOut->setVoxel(im->getVoxel(x,y,slice,stack), x,y);
            }
    }
    else if(SlicerType =="3Dto2D")
    {
        ImageOut = new ImageC(im,2);
        for(int y=0; y<im->array_size[1]; y++)
            for(int x=0; x<im->array_size[0]; x++)
            {
                ImageOut->setVoxel(im->getVoxel(x,y,slice), x,y);
            }
    }
    else if(SlicerType =="4Dto3D")
    {
        ImageOut = new ImageC(im,3);
        for(int z=0; z<im->array_size[2]; z++)
            for(int y=0; y<im->array_size[1]; y++)
                for(int x=0; x<im->array_size[0]; x++)
                {
                    ImageOut->setVoxel(im->getVoxel(x,y,z,stack), x,y,z);
                }
    }

    QVariant* qv = new QVariant();
    qv->setValue(ImageOut);
    outData.push_back(qv);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageSlicer::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageSlicer::validateRun()
{    
    QString SlicerType =  nodeItem->propertyValue("SlicerType").toString();

    if(SlicerType == "4Dto2D")
    {
        if( getConnectedInPorts().length() < 3)
            return -1;

        if( (slice < 0) || (stack < 0) )
            return -1;
    }
    else if(SlicerType == "3Dto2D")
    {       
        if(slice < 0)
            return -1;
    }
    else if(SlicerType == "4Dto3D")
    {
        if(stack < 0)
            return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageSlicer::clearNodeData()
{
    if(ImageOut)
    {
        ImageOut->clear();
        ImageOut->init();
    }
}
//--------------------------------------------------------------------------------------------------
ImageSlicer* ImageSlicer::copy() const
{
    return new ImageSlicer(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageSlicer::uniqueKey() const
{
    return "ImageSlicer";
}
//--------------------------------------------------------------------------------------------------
void ImageSlicer::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageSlicer::isSuperfluous() const
{
    return false;
}

//--------------------------------------------------------------------------------------------------
void ImageSlicer::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
