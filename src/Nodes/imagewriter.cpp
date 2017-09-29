/* ImageWriter.cpp
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
#include <Nodes/imagewriter.h>
#include <image/image.hpp>
#include <xmlreclib.h>

//--------------------------------------------------------------------------------------------------
ImageWriter::ImageWriter(QObject *parent)
{
   setParent(parent);
   node_type = "ImageWriter";
}
//--------------------------------------------------------------------------------------------------
ImageWriter::~ImageWriter() { }
//--------------------------------------------------------------------------------------------------
void ImageWriter::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageWriter::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageWriter::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageWriter::set_input_by_index(int in_id, QVariant* varvalue)
{
    if (in_id == 0)
    {
        bool isImage = varvalue->canConvert<ImageC*>();
        if(!isImage)
        {
            src_assigned[in_id] = false;
            return;
        }
        im = varvalue->value<ImageC*>();
        src_assigned[in_id] = true;
    }
    else if (in_id == 1)
    {
        bool isImage = varvalue->canConvert<ImageC*>();
        if(!isImage)
        {
            src_assigned[in_id] = false;
            return;
        }
        hdr_im = varvalue->value<ImageC*>();
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageWriter::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageWriter::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int ImageWriter::execute()
{
    clearOutputData();
    QString OutputImageType =  nodeItem->propertyValue("OutputImageType").toString();
    out_fname =  nodeItem->propertyValue("OutputfilePath").toString();
    XMLRecReader X;

    //qDebug() << "ImageWriter: Saving image";
    //qDebug() << "Image NumDimensions = " << im->numDimensions();
    //qDebug() << "Image Dimensions = " << im->arraySize(0) << im->arraySize(1) << im->arraySize(2) << im->arraySize(3);

    // NOTE: Need to add code to create the output directory if it does not exist

    if(OutputImageType =="Analyze")
    {
        QFileInfo info(out_fname);
        QString out_fname_hdr = info.path()+"/"+info.baseName()+".hdr";
        QString out_fname_img = info.path()+"/"+info.baseName()+".img";
//        qDebug() << "$$$$$$$$$$$$$$$$ :"<<out_fname_hdr;
//        qDebug() << "$$$$$$$$$$$$$$$$ :"<<out_fname_img;
        if (im->NumDimensions == 2)
        {

            image::basic_image<short,2> image_data;
            image_data.resize(image::geometry<2>(im->array_size[0],im->array_size[1]));

            double iMax = X.imageMaxValue(im);
            double iMin = X.imageMinValue(im);
            for(int y=0; y<im->array_size[1]; y++)
                for(int x=0; x<im->array_size[0]; x++)
                    image_data.at(x,y) = 4095*(im->getVoxel(x,y)-iMin)/(iMax-iMin);

            QString out_fname_img = out_fname;
            QString out_fname_hdr = out_fname;
            out_fname_hdr.replace(".img", ".hdr");

            float slope = (iMax-iMin) / 4095;
            float intercept = iMin;

            image::io::nifti NI;
            NI.init_header();
            NI.load_from_image(image_data);
            qDebug() << "image_data.dimension = " << image_data.dimension;
            NI.set_slope_intercept((float)slope, (float)intercept);
            float vsize[2] = {1.0};
            vsize[0] = im->voxel_size[0];
            vsize[1] = im->voxel_size[1];

            NI.set_voxel_size(vsize);
            NI.save_to_ana_file(out_fname_img.toStdString().c_str(), out_fname_hdr.toStdString().c_str());
        }
        else if(im->NumDimensions == 3)
        {
            image::basic_image<short,3> image_data;
            image_data.resize(image::geometry<3>(im->array_size[0],im->array_size[1],im->array_size[2]));

            double iMax = X.imageMaxValue(im);
            double iMin = X.imageMinValue(im);

            for(int z=0; z<im->array_size[2]; z++)
                for(int y=0; y<im->array_size[1]; y++)
                    for(int x=0; x<im->array_size[0]; x++)
                        image_data.at(x,y,z) = 4095*(im->getVoxel(x,y,z)-iMin)/(iMax-iMin);

            float slope = (iMax-iMin) / 4095;
            float intercept = iMin;

//            QString out_fname_img = out_fname;
//            QString out_fname_hdr = out_fname;
//            out_fname_hdr.replace(".img", ".hdr");

            image::io::nifti NI;
            NI.init_header();
            NI.load_from_image(image_data);
            NI.set_slope_intercept((float)slope, (float)intercept);

            float vsize[3] = {1.0};
            vsize[0] = im->voxel_size[0];
            vsize[1] = im->voxel_size[1];
            vsize[2] = im->voxel_size[2];
            NI.set_voxel_size(vsize);

            NI.save_to_ana_file(out_fname_img.toStdString().c_str(), out_fname_hdr.toStdString().c_str());

        }
        else if(im->NumDimensions == 4)
        {
            qDebug() << "ImageWriter: 4D images are not supported for Analyze";
        }
        else
        {
            qDebug() << "ImageWriter: images are not supported for Analyze";
            im->NumDimensions = 0;
        }
    } // if(OutputImageType =="Analyze")
    else if(OutputImageType =="DICOM")
    {
        qDebug() << "ImageWriter: images are not supported for DICOM";
    }
    else if(OutputImageType =="NIfTI")
    {
        if (im->NumDimensions == 2)
        {
            image::basic_image<short,2> image_data;
            image_data.resize(image::geometry<2>(im->array_size[0],im->array_size[1]));

            double iMax = X.imageMaxValue(im);
            double iMin = X.imageMinValue(im);

            for(int y=0; y<im->array_size[1]; y++)
                for(int x=0; x<im->array_size[0]; x++)
                    image_data.at(x,y) = 4095*(im->getVoxel(x,y)-iMin)/(iMax-iMin);

            float slope = (iMax-iMin) / 4095;
            float intercept = iMin;

            image::io::nifti NI;
            NI.init_header();
            NI.load_from_image(image_data);
            NI.set_slope_intercept((float)slope, (float)intercept);
            float vsize[2] = {1.0};
            vsize[0] = im->voxel_size[0];
            vsize[1] = im->voxel_size[1];

            NI.set_voxel_size(vsize);

            NI.save_to_file(out_fname.toStdString().c_str());

        }
        else if(im->NumDimensions == 3)
        {
            image::basic_image<short,3> image_data;
            image_data.resize(image::geometry<3>(im->array_size[0],im->array_size[1], im->array_size[2]));

            // calculate image max and min
            double iMax = X.imageMaxValue(im);
            double iMin = X.imageMinValue(im);

            // scaling data into 16-bit integer
            for(int z=0; z<im->array_size[2]; z++)
                for(int y=0; y<im->array_size[1]; y++)
                    for(int x=0; x<im->array_size[0]; x++)
                        image_data.at(x,y,z) = 4095*(im->getVoxel(x,y,z)-iMin)/(iMax-iMin);

            float slope = (iMax-iMin) / 4095;
            float intercept = iMin;

            image::io::nifti NI;
            NI.init_header();
            NI.load_from_image(image_data);
            NI.set_slope_intercept((float)slope, (float)intercept);
            float vsize[3] = {1.0};
            vsize[0] = im->voxel_size[0];
            vsize[1] = im->voxel_size[1];
            vsize[2] = im->voxel_size[2];
            NI.set_voxel_size(vsize);
            NI.save_to_file(out_fname.toStdString().c_str());
        }
        else if(im->NumDimensions == 4)
        {
            qDebug() << "ImageWriter: 4D images are not supported for NIfTI";
        }
        else
        {
            qDebug() << "ImageWriter: images are not supported for NIfTI";
        }
    }  // if(OutputImageType =="NIfTI")
    else if(OutputImageType =="XMLREC")
    {
        XMLRecReader X;
        bool anonymize = nodeItem->propertyValue("Anonymous").toBool();

        if(getConnectedInPorts().length()==2)
            X.saveXmlRec(out_fname, im, hdr_im, anonymize);
        else
            X.saveXmlRec(out_fname, im);
    }  // if(OutputImageType =="XMLREC")
    else
    {
        qDebug() << "Unrecognized file type";
        return -1;
    }

    //    QString OutputImageType =  nodeItem->propertyValue("OutputImageType").toString();
    if(OutputImageType.toUpper() =="ANALYZE")
        out_fname.replace(".img", ".hdr");

    QVariant* qv = new QVariant();
    qv->setValue(out_fname);
    outData.push_back(qv);


   setReadyOutPorts(8 + getNi().NumInputs + 0, true);
   markExecuted();
   return 0;
}

//--------------------------------------------------------------------------------------------------
int ImageWriter::validateDesign()
{
    QString OutputImageType =  nodeItem->propertyValue("OutputImageType").toString();
    if(!((OutputImageType=="Analyze") ||(OutputImageType=="XMLREC")||(OutputImageType=="NIfTI")))
    {
        qDebug() << "ImageWriter: The output image type must be either Analyze/XMLREC/NIfTI" ;
        return -1;
    }
    out_fname =  nodeItem->propertyValue("OutputfilePath").toString();
    QFileInfo info(out_fname);
    QString output_dir = info.absoluteDir().absolutePath();
    if( !info.absoluteDir().exists() )
    {
        //qDebug() << "ERROR: ImageWriter: output folder ("<<output_dir<<") doesn't exist.";
        info.absoluteDir().mkdir(output_dir);
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageWriter::validateRun()
{
    if( im->NumDimensions < 2  || im->NumDimensions > 4)
    {
        qDebug() << "The image dimension is not 2 or 3" ;
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageWriter::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
ImageWriter* ImageWriter::copy() const
{
    return new ImageWriter(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageWriter::uniqueKey() const
{
    return "ImageWriter";
}
//--------------------------------------------------------------------------------------------------
void ImageWriter::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageWriter::isSuperfluous() const
{
    return false;
}

//--------------------------------------------------------------------------------------------------
void ImageWriter::setConnectedInPorts()
{
}

