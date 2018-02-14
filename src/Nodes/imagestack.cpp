/* ImageStack.cpp
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
#include <Nodes/imagestack.h>
#include <image/image.hpp>
#include <xmlreclib.h>


//--------------------------------------------------------------------------------------------------
ImageStack::ImageStack(QObject *parent)
{
    setParent(parent);
    node_type = "ImageStack";
    im_in.clear();
    im = NULL;
}
//--------------------------------------------------------------------------------------------------
ImageStack::~ImageStack() { }
//--------------------------------------------------------------------------------------------------
void ImageStack::init()
{
    im_in.clear();
    src_assigned.clear();
}
//--------------------------------------------------------------------------------------------------
bool ImageStack::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageStack::set_input(int in_port_id, QVariant* varvalue)
{
   Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageStack::set_input_by_index(int in_id, QVariant* varvalue)
{
    QString ParType = varvalue->typeName();
    if(ParType != "ImageC*")
        return;

    if((in_id >= 0) && (in_id < 7))
    {
        im_in.insert(in_id, varvalue->value<ImageC*>());
        src_assigned.insert(in_id, true);
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageStack::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageStack::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int ImageStack::execute()
{
    clearOutputData();
    im = new ImageC();
    float v;
    if(im_in.at(0)->NumDimensions == 3)
    {
        im->NumDimensions =  4;
        im->array_size[0] =  im_in.at(0)->array_size[0];
        im->array_size[1] =  im_in.at(0)->array_size[1];
        im->array_size[2] =  im_in.at(0)->array_size[2];
        im->array_size[3] = getConnectedInPorts().length();
        im->data.resize(im->array_size[0]*im->array_size[1]*im->array_size[2]*im->array_size[3]);

        for(int kk=0; kk<im->array_size[2]; kk++)
            for(int y=0; y<im->array_size[1]; y++)
                for(int x=0; x<im->array_size[0]; x++)
                    for(int dd=0; dd<im->array_size[3]; dd++)  {
                        v  =im_in.at(dd)->getVoxel(x,y,kk);
                        im->setVoxel(v, x, y, kk, dd);
                    }

        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    }
    else if(im_in.at(0)->NumDimensions == 2) // if(im1->NumDimensions ==2)
    {
        im->NumDimensions =  3;
        im->array_size[0] =  im_in.at(0)->array_size[0];
        im->array_size[1] =  im_in.at(0)->array_size[1];
        im->array_size[2] = getConnectedInPorts().length();
        im->data.resize(im->array_size[0]*im->array_size[1]*im->array_size[2]);

        for(int y=0; y<image_data.height(); y++)
            for(int x=0; x<image_data.width(); x++)
                for(int dd=0; dd<im->array_size[2]; dd++)
                {
                    v  =im_in.at(dd)->getVoxel(x,y);
                    im->setVoxel(v, x, y,dd);
                }

        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    }

    QVariant* qv = new QVariant();
    qv->setValue(im);

    outData.clear();
    outData.push_back(qv);
    markExecuted();
    return 0;
}
//------------------------------------------------------------------------------
int ImageStack::validateDesign()
{
    return 0;
}
//------------------------------------------------------------------------------
int ImageStack::validateRun()
{
    // check that all input images have the same dimenion, and that they are all 2D or 3D
    int N = getConnectedInPorts().length();

    for(int d1=0; d1<N; d1++)
    {
        if ( im_in.at(d1)->NumDimensions < 2 )
            return -1;

        if ( im_in.at(d1)->NumDimensions > 3 )
            return -1;

        for(int d2=d1+1; d2<N; d2++)
        {
            if ( im_in.at(d1)->NumDimensions != im_in.at(d2)->NumDimensions )
                return -1;

            if ( im_in.at(d1)->array_size[0] != im_in.at(d2)->array_size[0] ) return -1;
            if ( im_in.at(d1)->array_size[1] != im_in.at(d2)->array_size[1] ) return -1;

            if(im_in.at(d1)->NumDimensions == 3)
                if ( im_in.at(d1)->array_size[2] != im_in.at(d2)->array_size[2] ) return -1;
        }
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageStack::clearNodeData()
{
    if(im)
    {
        im->clear();
    }
}
//--------------------------------------------------------------------------------------------------
ImageStack* ImageStack::copy() const
{
    return new ImageStack(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageStack::uniqueKey() const
{
    return "ImageStack";
}
//--------------------------------------------------------------------------------------------------
void ImageStack::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageStack::isSuperfluous() const
{
    return false;
}

//--------------------------------------------------------------------------------------------------
void ImageStack::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------

