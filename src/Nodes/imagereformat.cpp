/* ImageReformat.cpp
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
#include <Nodes/imagereformat.h>
#include <image/image.hpp>
#include <xmlreclib.h>

#define MAX_DIMENTION_NUM 3

//--------------------------------------------------------------------------------------------------
ImageReformat::ImageReformat(QObject *parent)
{
   setParent(parent);
   node_type = "ImageReformat";
   ImageOut = NULL;
}
//--------------------------------------------------------------------------------------------------
ImageReformat::~ImageReformat() { }
//--------------------------------------------------------------------------------------------------
void ImageReformat::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageReformat::isAllInputsReady()
{
    bool ready =  Node::isAllInputsReady();
    return ready;
}
//--------------------------------------------------------------------------------------------------
void ImageReformat::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageReformat::set_input_by_index(int in_id, QVariant* varvalue)
{
    QString var = varvalue->typeName();
    //qDebug() << "var="<<var<<"in_id="<<in_id;

    if(in_id==0)
    {
        if(var == "ImageC*")
        {
            im = varvalue->value<ImageC*>();
            src_assigned[in_id] = true;
        }
        else
        {
            src_assigned[in_id] = true;
        }
    }

}
//--------------------------------------------------------------------------------------------------
QVariant* ImageReformat::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageReformat::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int ImageReformat::execute()
{
    //ArgValueEdit[3]->setEnabled(false);
    //QString xDattaValues = nodeItem->propertyValue("Arg3").toString();
    //qDebug()<<"im->NumDimensions="<<im->NumDimensions;

    float in_vs [4] = {0};
    in_vs[0] = im->voxel_size[0];
    in_vs[1] = im->voxel_size[1];
    in_vs[2] = im->voxel_size[2];
    in_vs[3] = im->voxel_size[3];

    clearOutputData();
    d[1] = im->array_size[0];
    d[2] = im->array_size[1];
    if (im->NumDimensions==3) d[3] = im->array_size[2];

    // accept negative values for the dimension for reflection
    dd[0] = im->array_size[abs(arg[1])-1];
    dd[1] = im->array_size[abs(arg[2])-1];
    if (im->NumDimensions==3) dd[2] = im->array_size[abs(arg[3])-1];

    // set sign values for the dimension
    int dim_sign[3];
    dim_sign[0] = (arg[1] > 0) * 1 - (arg[1] < 0);
    dim_sign[1] = (arg[2] > 0) * 1 - (arg[2] < 0);
    if (im->NumDimensions==3) dim_sign[2] = (arg[3] > 0) * 1 - (arg[3] < 0);


    if (im->NumDimensions==3)
    {
        ImageOut = new ImageC(im,3);
        ImageOut->array_size[0] = dd[0];
        ImageOut->array_size[1] = dd[1];
        ImageOut->array_size[2] = dd[2];

        ImageOut->voxel_size[0] = in_vs[abs(arg[1])-1];
        ImageOut->voxel_size[1] = in_vs[abs(arg[1])-2];
        ImageOut->voxel_size[2] = in_vs[abs(arg[1])-3];
        ImageOut->voxel_size[3] = in_vs[3];
        float v;
        int xn, yn, zn;
        d[0] = im->array_size[0];
        d[1] = im->array_size[1];
        d[2] = im->array_size[2];

        for(int z=0; z<d[2]; z++)
            for(int y=0; y<d[1]; y++)
                for(int x=0; x<d[0]; x++)
                {
                    xn = (abs(arg[1])==1)? x : (abs(arg[1])==2)? y : z ;
                    yn = (abs(arg[2])==1)? x : (abs(arg[2])==2)? y : z ;
                    zn = (abs(arg[3])==1)? x : (abs(arg[3])==2)? y : z ;

                    if(dim_sign[0] < 0) xn = dd[0] - 1 - xn;
                    if(dim_sign[1] < 0) yn = dd[1] - 1 - yn;
                    if(dim_sign[2] < 0) zn = dd[2] - 1 - zn;

                    v = im->getVoxel(x,y,z);
                    ImageOut->setVoxel(v, xn,yn,zn);
                }
    } // end of if (im->NumDimensions==3)
    else if (im->NumDimensions==2)
    {
        ImageOut = new ImageC(im,2);
        ImageOut->array_size[0] = dd[0];
        ImageOut->array_size[1] = dd[1];

        ImageOut->voxel_size[0] = in_vs[abs(arg[1])-1];
        ImageOut->voxel_size[1] = in_vs[abs(arg[1])-2];

        float v;
        int xn, yn, zn;
        d[0] = im->array_size[0];
        d[1] = im->array_size[1];
        for(int y=0; y<d[1]; y++)
            for(int x=0; x<d[0]; x++)
            {
                if(abs(arg[1])==1) xn = x;
                if(abs(arg[1])==2) xn = y;
                if(abs(arg[2])==1) yn = x;
                if(abs(arg[2])==2) yn = y;

//                xn = (abs(arg[1])==1)? x : (abs(arg[1])==2)? y;
//                yn = (abs(arg[2])==1)? x : (abs(arg[2])==2)? y;

                if(dim_sign[0] < 0) xn = dd[0] - 1 - xn;
                if(dim_sign[1] < 0) yn = dd[1] - 1 - yn;

                v = im->getVoxel(x,y);
                ImageOut->setVoxel(v, xn,yn);
            }
    } // end of if (im->NumDimensions==2)
    else
    {
        qDebug() << "Error: wrong permutation setting in ImageReformat.";
        return -1;
    }

    QVariant* qv = new QVariant();
    qv->setValue(ImageOut);
    outData.push_back(qv);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;

}
//--------------------------------------------------------------------------------------------------
int ImageReformat::validateDesign()
{
    for (int j=0; j<MAX_DIMENTION_NUM; j++)
    {
        readArgDataValue<int>(arg[j+1], "Arg"+QString::number(j));
        //qDebug() << "arg["<<j+1<<"]= " << arg[j+1];
        if ((abs(arg[j+1]) <= 0)||abs((arg[j+1])>MAX_DIMENTION_NUM))
        {
            qDebug() << "Error: arg["<<j+1<<"] not correct in ImageReformat.";
            return -1;
        }
    }
    if((arg[1]==arg[2])||(arg[1]==arg[3])||(arg[2]==arg[3]))
    {
        qDebug() << "Error: wrong input-setting in ImageReformat.";
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageReformat::validateRun()
{    
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageReformat::clearNodeData()
{
    if(ImageOut)
    {
        ImageOut->clear();
        ImageOut->init();
    }
}
//--------------------------------------------------------------------------------------------------
ImageReformat* ImageReformat::copy() const
{
    return new ImageReformat(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageReformat::uniqueKey() const
{
    return "ImageReformat";
}
//--------------------------------------------------------------------------------------------------
void ImageReformat::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageReformat::isSuperfluous() const
{
    return false;
}

//--------------------------------------------------------------------------------------------------
void ImageReformat::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
template <class T>
void ImageReformat::readArgDataValue(T &x, QString arg)
{
    x = nodeItem->propertyValue(arg).toDouble();
}
