/* ImageNormalizer.cpp
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
#include <Nodes/imagenormalizer.h>
#include <image/image.hpp>
#include <xmlreclib.h>


ImageNormalizer::ImageNormalizer(QObject *parent)
{
    setParent(parent);
    node_type = "ImageNormalizer";
    result = NULL;
}
//--------------------------------------------------------------------------------------------------
ImageNormalizer::~ImageNormalizer() { }
//--------------------------------------------------------------------------------------------------
void ImageNormalizer::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageNormalizer::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizer::set_input(int in_port_id, QVariant* varvalue)
{
   Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizer::set_input_by_index(int in_id, QVariant* varvalue)
{
    QString var = varvalue->typeName();

    if(in_id==0)
    {
        bool isImage = varvalue->canConvert<ImageC*>();
        if(!isImage)
        {
            src_assigned[in_id] = true;
            return;
        }
        im = (ImageC*)(varvalue->value<ImageC*>());
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageNormalizer::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageNormalizer::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int ImageNormalizer::execute()
{
     clearOutputData();
    XMLRecReader X;

    // get user range
    double user_min =  nodeItem->propertyValue("Min").toDouble();
    double user_max =  nodeItem->propertyValue("Max").toDouble();
    double MaxValue, MinValue;

    MaxValue = X.imageMaxValue(im);
    MinValue = X.imageMinValue(im);

    if(MaxValue <= MinValue)
    {
        qDebug() << "Image MaxValue = " << MaxValue;
        qDebug() << "Image MinValue = " << MinValue;
        qDebug() << "Image size = " << im->array_size[0] << im->array_size[1] << im->array_size[2] << im->array_size[3];
    }

    if(MaxValue <= MinValue) // all voxels are the same value; still accept but normalization will do nothing
    {
        MaxValue = MinValue + 1;
    }

    result = new ImageC(im);
    result->data.resize(im->numElements());

    for(int i=0; i<(im->data.size()); i++)
        result->data.at(i) = (im->data.at(i)-MinValue)/(MaxValue-MinValue)*(user_max-user_min)+user_min;


    QVariant* qv = new QVariant();
    qv->setValue(result);
    outData.push_back(qv);


   setReadyOutPorts(8 + getNi().NumInputs + 0, true);
   markExecuted();
   return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageNormalizer::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageNormalizer::validateRun()
{
    if(im->NumDimensions < 2 || im->NumDimensions > 4)
        return -1;

    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizer::clearNodeData()
{
    if(result)
        result->clear();
}
//--------------------------------------------------------------------------------------------------
ImageNormalizer* ImageNormalizer::copy() const
{
    return new ImageNormalizer(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageNormalizer::uniqueKey() const
{
    return "ImageNormalizer";
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizer::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageNormalizer::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizer::setConnectedInPorts()
{
}

