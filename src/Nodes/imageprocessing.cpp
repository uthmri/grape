/* ImageProcessing.cpp
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
#include <Nodes/imageprocessing.h>
#include <image/image.hpp>
#include <xmlreclib.h>

//--------------------------------------------------------------------------------------------------
ImageProcessing::ImageProcessing(QObject *parent)
{
    setParent(parent);
    node_type = "ImageProcessing";
}
//--------------------------------------------------------------------------------------------------
ImageProcessing::~ImageProcessing() { }
//--------------------------------------------------------------------------------------------------
void ImageProcessing::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageProcessing::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageProcessing::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageProcessing::set_input_by_index(int in_id, QVariant* varvalue)
{
    QString ImageProcessingType = nodeItem->propertyValue("ImageProcessingType").toString();
    QString var = varvalue->typeName();
    int arg_type;

    if(in_id==0)
    {
        if(var == "ImageC*")
        {
            arg_type = 0;
            im1 = varvalue->value<ImageC*>();
        }
        else //if((var==QMetaType::Float) || (var==QMetaType::Int) || (var==QMetaType::Double) )
        {
            arg_type = 1;
            user_val1 = varvalue->toFloat();
        }
        arg1_type = arg_type;
        src_assigned[in_id] = true;
    }
    else if(in_id==1)
    {
        if(var == "ImageC*")
        {
            arg_type = 0;
            im2 = varvalue->value<ImageC*>();
        }
        else //if((var==QMetaType::Float) || (var==QMetaType::Int) || (var==QMetaType::Double) )
        {
            arg_type = 1;
            user_val2 = varvalue->toFloat();
        }
        arg2_type = arg_type;
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageProcessing::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageProcessing::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int ImageProcessing::execute()
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

    QString ImageProcessingType = nodeItem->propertyValue("ImageProcessingType").toString();
    // qDebug() << "ImageProcessingType =" << ImageProcessingType;


    // op_arg_type:
    //    = 0 ==> img to img
    //    = 1 ==> img and number
    //    = 2 ==> number and image
    //    = 3 ==> number and number
    op_arg_type =   0 * ((arg1_type==0)&&(arg2_type==0)) +
            1 * ((arg1_type==0)&&(arg2_type==1)) +
            2 * ((arg1_type==1)&&(arg2_type==0)) +
            3 * ((arg1_type==1)&&(arg2_type==1));

    QString imageArithmeticType =  nodeItem->propertyValue("ImageArithmeticType").toString();

    //qDebug()<<"$$$$$$$$$-2  arg1_type="<<arg1_type<<", arg2_type="<<arg2_type;
    int MaxNumDimensions, isz[3], risz[3];

    if(arg1_type == 0)
    {
        MaxNumDimensions = im1->NumDimensions;
        for(int i=0; i<MaxNumDimensions; i++)
            isz[i] = im1->array_size[i];//image size

        qDebug() << " Input image NumDimensions = " << QString::number(im1->NumDimensions);
        qDebug() << " Input image Dimensions = " << QString::number(isz[0]) << " x " << QString::number(isz[1]) << " x " << QString::number(isz[2]) << " x " << QString::number(isz[3]);

    }
    else if(arg2_type == 0)
    {
        if(src_assigned[1]){
            MaxNumDimensions = im2->NumDimensions;
            for(int i=0; i<MaxNumDimensions; i++)
                isz[i] = im2->array_size[i];//image size
        }
    }
    else
    {
        MaxNumDimensions = 1;
    }

    XMLRecReader X;
    float iMax = X.imageMaxValue(im1);
    float iMin = X.imageMinValue(im1);

    // FIRST COPY DATA TO IMAGE OBJECT
    image::basic_image<float,3>  I3(image::geometry<3>(isz[0],isz[1],isz[2]));
    image::basic_image<int,3>  L3(image::geometry<3>(isz[0],isz[1],isz[2]));
    image::basic_image<float,2>  I2(image::geometry<2>(isz[0],isz[1]));
    image::basic_image<int,2>  L2(image::geometry<2>(isz[0],isz[1]));

    if (im1->NumDimensions == 3)
    {
        for(int z=0; z<im1->array_size[2]; z++)
            for(int y=0; y<im1->array_size[1]; y++)
                for(int x=0; x<im1->array_size[0]; x++)
                    I3.at(x,y,z) = im1->getVoxel(x,y,z);

    }
    else if (im1->NumDimensions==2)
    {
        for(int y=0; y<im1->array_size[1]; y++)
            for(int x=0; x<im1->array_size[0]; x++)
                I2.at(x,y) = im1->getVoxel(x,y);
    }

    // SECOND CALL THE IMAGE LIBRARY TO DO PROCESSING
    QString op = ImageProcessingType.toLower();

    if (im1->NumDimensions == 3)
    {
        if(op == QString("pointwise: invert"))
        {
            I3 *= -1.0;
            I3 += iMax;
        }
        else if(op == QString("filter: mean blur"))
        {
            image::filter::mean(I3);
        }
        else if(op == QString("filter: gaussian blur"))
        {
            image::filter::gaussian(I3);
        }
        else if(op == QString("filter: anisotroipic diffusion"))
        {
            image::filter::anisotropic_diffusion(I3);
        }
        else if(op == QString("filter: canny edge"))
        {
            image::filter::canny_edge<float,3>(I3); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        }
        else if(op == QString("filter: sobel edge"))
        {
            image::filter::sobel(I3);
        }
        else if(op == QString("filter: gradient magnitude"))
        {
            image::filter::gradient_magnitude(I3);
        }
        else if(op == QString("filter: laplacian"))
        {
            image::filter::laplacian<float,3>(I3);
        }
        else if(op == QString("segment: graph cut"))
        {
            image::segmentation::graph_cut(I3,L3,500,50);
            I3 = L3; // output will be copied from I
        }
        else if(op == QString("segment: watershed"))
        {
            image::filter::mean(I3);
            image::filter::sobel(I3);
            image::filter::mean(I3);
            image::segmentation::watershed(I3,L3);
            I3 = L3; // output will be copied from I
        }
        else if(op == QString("segment: otsu"))
        {
            float th = image::segmentation::otsu_threshold(I3);
            image::threshold(I3,I3,th);
            //image::segmentation::otsu<image::basic_image<float,3,std::vector<float> >,image::basic_image<int,3,std::vector<int> > >(I3,L3);
//            I3 = L3; // output will be copied from I
        }
        else if(op == QString("segment: fast marching"))
        {
            image::filter::sobel(I3);
            image::segmentation::fast_marching(I3,L3,image::pixel_index<3>(I3.width()/2,I3.height()/2,I3.depth()/2,I3.geometry()));
            for(size_t index = 0;index < I3.size();++index)
                if(L3[index] > 10000)
                    I3[index] = 0;
        }
        else if(op == QString("resample: down_sample"))
        {
            for(int i=0; i<3; i++) risz[i] = isz[i]/2;//resampled image size
            for(int i=0; i<3; i++) rvsz[i] = im1->voxel_size[i]*2;//resampled image voxel size
        }
        else if(op == QString("resample: up_sample"))
        {
            for(int i=0; i<3; i++) risz[i] = isz[i]*2;//resampled image size
            for(int i=0; i<3; i++) rvsz[i] = im1->voxel_size[i]/2;//resampled image voxel size
        }
        else if(op == QString("resample: zero_pad"))
        {
            int Npts = 3;
            float* x = new float[Npts];
            readArgDataArray<float>(x, Npts, "Arg1");

            for(int i=0; i<3; i++) risz[i] = x[i]; //new image size
            for(int i=0; i<3; i++) rvsz[i] = im1->voxel_size[i];
        }
        else if(op == QString("segment: stochastic competition"))
        {
        }
    }
    else if (im1->NumDimensions==2)
    {
        if(op == QString("pointwise: invert"))
        {
            I2 *= -1.0;
            I2 += iMax;
        }
        else if(op == QString("filter: mean blur"))
        {
            image::filter::mean(I2);
        }
        else if(op == QString("filter: gaussian blur"))
        {
            image::filter::gaussian(I2);
        }
        else if(op == QString("filter: anisotroipic diffusion"))
        {
            image::filter::anisotropic_diffusion(I2);
        }
        else if(op == QString("filter: canny edge"))
        {
            image::filter::canny_edge<float,2>(I2); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        }
        else if(op == QString("filter: sobel edge"))
        {
            image::filter::sobel(I2);
        }
        else if(op == QString("filter: gradient magnitude"))
        {
            image::filter::gradient_magnitude(I2);
        }
        else if(op == QString("filter: laplacian"))
        {
            image::filter::laplacian<float,2>(I2); //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        }
        else if(op == QString("segment: graph cut"))
        {
            image::segmentation::graph_cut(I2,L2,500,50);
            I2 = L2; // output will be copied from I
        }
        else if(op == QString("segment: watershed"))
        {
            image::filter::mean(I2);
            image::filter::sobel(I2);
            image::filter::mean(I2);
            image::segmentation::watershed(I2,L2);
            I2 = L2; // output will be copied from I
        }
        else if(op == QString("segment: otsu"))
        {
            float th = image::segmentation::otsu_threshold(I2);
            image::threshold(I2,I2,th);
//            image::segmentation::otsu_threshold(I2);
//            image::segmentation::otsu<float,int>(I2,L2);
//            I2 = L2; // output will be copied from I
        }
        else if(op == QString("segment: fast marching"))
        {
            image::filter::mean(I2);
            image::filter::gradient_magnitude(I2);
            image::pixel_index<2> t0_point(I2.width()/2,I2.height()/2,I2.geometry());
            image::segmentation::fast_marching(I2,L2,t0_point);

            //image::segmentation::otsu<float,int>(L2,I2);

            float th = image::segmentation::otsu_threshold(L2);
            image::threshold(L2,I2,th);


            //image::segmentation::otsu_threshold(I2);
        }
        else if(op == QString("segment: stochastic competition"))
        {
            image::basic_image<unsigned int,2> initial_contour;
            image::segmentation::stochastic_competition(I2,initial_contour,30,5);
            I2 = initial_contour;
        }
        else if(op == QString("resample: down_sample"))
        {
            for(int i=0; i<2; i++) risz[i] = isz[i]/2;//resampled image size
            for(int i=0; i<2; i++) rvsz[i] = im1->voxel_size[i]*2;//resampled image voxel size
        }
        else if(op == QString("resample: up_sample"))
        {
            for(int i=0; i<2; i++) risz[i] = isz[i]*2;//resampled image size
            for(int i=0; i<2; i++) rvsz[i] = im1->voxel_size[i]/2;//resampled image voxel size
        }
        else if(op == QString("resample: zero_pad"))
        {
            int Npts = 2;
            float* x = new float[Npts];
            readArgDataArray<float>(x, Npts, "Arg1");

            for(int i=0; i<2; i++) risz[i] = x[i]; //new image size
            for(int i=0; i<2; i++) rvsz[i] = im1->voxel_size[i]; //resampled image voxel size
        }
    }

    // FINALLY COPY BACK TO ImageC
    if (im1->NumDimensions == 3)
    {
        qDebug() << "++++++++++++++++++++ 1";
        result = new ImageC(im1);
        qDebug() << "++++++++++++++++++++ 2";

        if ((op == QString("resample: up_sample"))||(op == QString("resample: down_sample")))
        {
            image::basic_image<float,3> resamp_im;
            resamp_im.resize(image::geometry<3>(risz[0],risz[1],risz[2]));
            resample_linear(I3,resamp_im);
            //image::upsample_with_padding(I3,resamp_im,image::geometry<3>(risz[0],risz[1],rsz22));
            //image::downsample_with_padding(I3,resamp_im);

            result->data.resize(risz[0]*risz[1]*risz[2]);
            for(int i=0; i<3; i++) result->array_size[i] = risz[i];
            for(int i=0; i<3; i++) result->voxel_size[i] = rvsz[i];
            std::copy(resamp_im.begin(), resamp_im.end(), result->data.begin());
        }
        else if (op == QString("resample: zero_pad"))
        {
            for(int i=0; i<3; i++) result->array_size[i] = risz[i];
            for(int i=0; i<3; i++) result->voxel_size[i] = rvsz[i];

            qDebug() << " x " << QString::number(risz[0]) << " x " << QString::number(risz[1]) << " x " << QString::number(risz[2]);

            result->data.resize(risz[0]*risz[1]*risz[2]);
            std::fill(result->data.begin(), result->data.end(), 0.0);

            for(int z=0; z<std::min(risz[2],im1->array_size[2]); z++)
                for(int y=0; y<std::min(risz[1],im1->array_size[1]); y++)
                    for(int x=0; x<std::min(risz[0],im1->array_size[0]); x++)
                        result->setVoxel(im1->getVoxel(x,y,z), x, y, z);
        }
        else
            std::copy(I3.begin(), I3.end(), result->data.begin());
    }
    else if (im1->NumDimensions == 2)
    {
        result = new ImageC(im1);
        if ((op == QString("resample: up_sample"))||(op == QString("resample: down_sample")))
        {
            image::basic_image<float,2> resamp_im;
            resamp_im.resize(image::geometry<2>(risz[0],risz[1]));
            resample_linear(I2,resamp_im);
            //image::upsample_with_padding(I3,resamp_im,image::geometry<2>(risz[0],risz[1]));
            //image::downsample_with_padding(I3,resamp_im);

            result->data.resize(risz[0]*risz[1]);
            for(int i=0; i<2; i++) result->array_size[i] = risz[i];
            for(int i=0; i<2; i++) result->voxel_size[i] = rvsz[i];
            std::copy(resamp_im.begin(), resamp_im.end(), result->data.begin());
        }
        else if (op == QString("resample: zero_pad"))
        {
            for(int i=0; i<2; i++) result->array_size[i] = risz[i];
            for(int i=0; i<2; i++) result->voxel_size[i] = rvsz[i];

            result->data.resize(risz[0]*risz[1]);
            std::fill(result->data.begin(), result->data.end(), 0.0);

            for(int y=0; y<std::min(risz[1],im1->array_size[1]); y++)
                for(int x=0; x<std::min(risz[0],im1->array_size[0]); x++)
                    result->setVoxel(im1->getVoxel(x,y), x, y);
        }
        else
            std::copy(I2.begin(), I2.end(), result->data.begin());
    }

    I3.clear();        L3.clear();
    I2.clear();        L3.clear();

    output_found = true;

    QVariant* qv = new QVariant();

    if(op_arg_type==3)
        qv->setValue(result_numeric);
    else
        qv->setValue(result);
    outData.push_back(qv);



    setReadyOutPorts(8 + getNi().NumInputs + 0, output_found);
    markExecuted();

    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageProcessing::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageProcessing::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageProcessing::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
ImageProcessing* ImageProcessing::copy() const
{
    return new ImageProcessing(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageProcessing::uniqueKey() const
{
    return "ImageProcessing";
}
//--------------------------------------------------------------------------------------------------
void ImageProcessing::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageProcessing::isSuperfluous() const
{
    return false;
}

//--------------------------------------------------------------------------------------------------
void ImageProcessing::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
template <class T>
void ImageProcessing::readArgDataArray(T x[], int Npts, QString arg)
{
    if (Npts > 1)
    {
        QString xDattaValues = nodeItem->propertyValue(arg).toString();
        QStringList xDattaList = xDattaValues.split(',');
        for(int ii=0; ii< Npts; ii++) x[ii] = (T)(xDattaList[ii].toDouble());
    }
    else
    {
        qDebug() << "Error: in readArgDataArray, Npst should > 1 for T1-IR.";
        return;
    }
}
//--------------------------------------------------------------------------------------------------
