/* ImageReader.cpp
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
#include <Nodes/imagereader.h>
#include <image/image.hpp>
#include <xmlreclib.h>
#include <stdio.h>
#include <vector>


//--------------------------------------------------------------------------------------------------
ImageReader::ImageReader(QObject *parent)
{
    setParent(parent);
    node_type = "ImageReader";
    im = NULL;
}
//--------------------------------------------------------------------------------------------------
ImageReader::~ImageReader() { }
//--------------------------------------------------------------------------------------------------
void ImageReader::init()
{
    src_assigned.clear();
    src_assigned.push_back(false);
}
//--------------------------------------------------------------------------------------------------
bool ImageReader::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageReader::set_input(int in_port_id, QVariant* varvalue)
{
   Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageReader::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id == 0)
    {
        in_fname = varvalue->toString();
    }
    src_assigned[in_id] = true;
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageReader::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageReader::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}

//--------------------------------------------------------------------------------------------------
IMAGE_INFO get_image_info(const char *fn , IMAGE_INFO image_info)
{
    FILE *fp;
    int lab;
    fp = fopen(fn,"r");
    if( fp == NULL )
    {
        qDebug() << "Can not open: " << fn;
        fclose(fp);
        return image_info;
    }
    lab = fseek( fp, 40+28+2, SEEK_SET);
    fread(&(image_info.datatype), 2, 1,fp);
    lab = fseek( fp, 40+30+2, SEEK_SET);
    fread(&(image_info.bitpix), 2, 1,fp);
    lab = fseek( fp, 80, SEEK_SET);
    fread(&(image_info.vsx), 4, 1,fp);
    lab = fseek( fp, 84, SEEK_SET);
    fread(&(image_info.vsy), 4, 1,fp);
    lab = fseek( fp, 88, SEEK_SET);
    fread(&(image_info.vsz), 4, 1,fp);
    fclose(fp);

    return image_info;

}

//------------------------------------------------------------------------------
int ImageReader::execute()
{
    clearOutputData();

    QString InputImageType =  nodeItem->propertyValue("InputImageType").toString();
    QFile file(in_fname);
    QFileInfo checkFile(in_fname);
    if( !checkFile.exists() || !checkFile.isFile() || !checkFile.isReadable() )
    {
        qDebug() << "ERROR: ImageReader::execute()" << "cannot find file " << in_fname;
        return -1;
    }

    // read image info and initialize basic_image
    QByteArray a = in_fname.toLatin1();
    const char *fn = a.data();
    image_info = get_image_info( fn, image_info);
    image::basic_image<float,3> image_data;

    if((InputImageType.toUpper() =="ANALYZE")||(InputImageType.toUpper() =="NIFTI"))
    {
        if(!image_data.load_from_file<image::io::nifti>(in_fname.toStdString().c_str()))
        {
            std::cout << "load data from failed" << std::endl;
            return -1;
        }
    }
    else if(InputImageType.toUpper() =="DICOM")
    {
        if(!image_data.load_from_file<image::io::dicom>(in_fname.toStdString().c_str()))
        {
              std::cout << "load DICOM failed" << std::endl;
              return -1;
        }
    }
    if((InputImageType.toUpper() =="ANALYZE")||(InputImageType.toUpper() =="NIFTI")||(InputImageType.toUpper() =="DICOM"))
    {
        int TrueImageDimension = 0;
        if(image_data.width()>1) TrueImageDimension++;
        if(image_data.height()>1) TrueImageDimension++;
        if(image_data.depth()>1) TrueImageDimension++;

        im = new ImageC;
        im->voxel_size[0]= image_info.vsx;
        im->voxel_size[1]= image_info.vsy;
        im->voxel_size[2]= image_info.vsz;
        im->datatype = image_info.datatype;
        im->bitpix = image_info.bitpix;
        im->NumDimensions =  TrueImageDimension;
        im->array_size[0] =  image_data.width();
        im->array_size[1] =  image_data.height();

        int NumElements;
        if(TrueImageDimension == 3)
        {
            im->array_size[2] =  image_data.depth();
            NumElements = im->array_size[0] * im->array_size[1] * im->array_size[2];
        }
        else if(TrueImageDimension == 2)
        {
            NumElements = im->array_size[0] * im->array_size[1] ;
        }

        im->data.resize(NumElements);
        std::copy(image_data.begin(), image_data.end(), im->data.begin());
        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
        image_data.clear();
    }
    else if(InputImageType.toUpper() =="XMLREC")
    {
        if( !file.exists() )
        {
            qDebug() << "ERROR: ImageReader::execute()" << "cannot find file " << in_fname;
            return -1;
        }
        XMLRecReader X;
        im = new ImageC();
        X.loadXmlRec(in_fname, im);
        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    }
    else if(InputImageType.toUpper() =="MAT")
    {
        // The mat v4 file should contain two matrices
        // 1) one named "dimension", storing the image dimension
        // 2) one named "image", storing the image data.
        XMLRecReader X;
        image::basic_image<short,2> image_data2d;
        image::io::mat_read mat_file;
        if(mat_file.load_from_file(in_fname.toStdString().c_str()))
            mat_file >> image_data2d;
        im = new ImageC();
        im->setNumDimensions(2);
        im->array_size[0] = image_data2d.width();
        im->array_size[1] = image_data2d.height();
        if((im->array_size[0]*im->array_size[1])==0)
            return -1;

        im->data.resize(im->array_size[0]*im->array_size[1]);

        for(int y=0; y<im->array_size[1]; y++)
            for(int x=0; x<im->array_size[0]; x++)
                im->setVoxel(image_data2d.at(x,y),x,y);

        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
        image_data2d.clear();
    }
    else
    {
        qDebug() << getType() << "<" << this->getDiagramItem()->caption1() << ">" << "ERROR: Unrecognized file type.";
        return -1;
    }

    qDebug() << getType() << "<" << this->getDiagramItem()->caption1() << ">" << "Successfully loaded the image";
    //qDebug() << "Image NumDimensions = " << im->numDimensions();
    //qDebug() << "Image Dimensions = " << im->arraySize(0) << im->arraySize(1) << im->arraySize(2) << im->arraySize(3);

    outData.clear();
    QVariant *qv = new QVariant();
    qv->setValue(im);
    outData.push_back(qv);

    markExecuted(); // set execution_status = EXECUTED;
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageReader::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageReader::validateRun()
{
    QFileInfo checkFile(in_fname);
    if (!(checkFile.exists() && checkFile.isFile()))
    {
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageReader::clearNodeData()
{
    if(im)
    {
        im->clear();
    }
}
//--------------------------------------------------------------------------------------------------
ImageReader* ImageReader::copy() const
{
    return new ImageReader(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageReader::uniqueKey() const
{
    return "ImageReader";
}
//--------------------------------------------------------------------------------------------------
void ImageReader::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageReader::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void ImageReader::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
