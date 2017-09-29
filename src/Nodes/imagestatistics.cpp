/* ImageStatistics.cpp
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
#include <Nodes/imagestatistics.h>
#include <image/image.hpp>
#include <xmlreclib.h>

//--------------------------------------------------------------------------------------------------
ImageStatistics::ImageStatistics(QObject *parent)
{
    setParent(parent);
    node_type = "ImageStatistics";
    Source_im = NULL;
    Mask_im = NULL;
}
//--------------------------------------------------------------------------------------------------
ImageStatistics::~ImageStatistics() { }
//--------------------------------------------------------------------------------------------------
void ImageStatistics::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageStatistics::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageStatistics::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageStatistics::set_input_by_index(int in_id, QVariant* varvalue)
{
    QString ParType = varvalue->typeName();
    if(in_id==0)
    {
        Source_im = (ImageC*)(varvalue->value<ImageC*>());
        src_assigned[in_id] = true;
    }

    if(in_id==1)
    {
        Mask_im = (ImageC*)(varvalue->value<ImageC*>());
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageStatistics::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageStatistics::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
double set_decimal(double a, int num_of_kept_decimal)
{
    QString str_a, str_a2;
    str_a.setNum(a,'f',num_of_kept_decimal); // maximun precision 6
    double b = str_a.toDouble();
    return b;
}
//------------------------------------------------------------------------------
int ImageStatistics::execute()
{
    clearOutputData();
    bool set_roumd = nodeItem->propertyValue("SetRound").toBool();
    float s=0;
    QString imageStatistics = nodeItem->propertyValue("ImageStatistics").toString();

    double result = 0.0;
    double sum = 0.0;
    double mask_sum = 0.0;
    double mean = 0.0;
    QList<float> dataList;
    vector<float> v_1d;
    int decimal_num=6;
//    long L=Source_im->array_size[0]*Source_im->array_size[1]*Source_im->array_size[2];
//    if((Source_im->datatype==2)&&(Source_im->bitpix==8))
//        vector<unsigned char> v_1d;
//    v_1d.resize(L);

    if(src_assigned.at(1)) // mask input
    {
        std::copy(Source_im->data.begin(),Source_im->data.end(),std::back_inserter(v_1d));
        std::copy(Mask_im->data.begin(),Mask_im->data.end(),std::back_inserter(mask_1d));

        if(! (Source_im->array_size[0] == Mask_im->array_size[0] & Source_im->array_size[1] == Mask_im->array_size[1]) )
        {
            qDebug() << "ERROR: image and mask dimensions must be the same";
            return -1;
        }

        if(Source_im->NumDimensions == 2)
        {
            img_1d.clear();
            for(int i = 0; i<mask_1d.size(); i++)
                if(mask_1d[i])
                    img_1d.push_back(v_1d[i]);

            if(img_1d.size() == 0)
            {
                output = 0.0;
                return -1;
            }

            if (imageStatistics == "Min")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s = img_1d.at(0);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            if (imageStatistics == "Max")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= img_1d.at(img_1d.size()-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Median")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= set_decimal(img_1d.at(img_1d.size()/2-1),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Mean")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                s = set_decimal(sum / img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Variance")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "stdDev")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(sqrt(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size()),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Percentile")
            {
                std::sort(img_1d.begin(), img_1d.end());
                QVariant percentile_value = nodeItem->propertyValue("Percentile");
                s = img_1d.at(img_1d.size()*percentile_value.toInt()/100-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
        }
        else if(Source_im->NumDimensions == 3)
        {
            img_1d.clear();
            for(int i = 0; i<mask_1d.size(); i++)
                if(mask_1d[i])
                    img_1d.push_back(v_1d[i]);

            if(img_1d.size() == 0)
            {
                output = 0.0;
                return -1;
            }

            if (imageStatistics == "Min")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s = img_1d.at(0);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            if (imageStatistics == "Max")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= img_1d.at(img_1d.size()-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Median")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= img_1d.at(img_1d.size()/2-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Mean")
            {
                sum = (std::accumulate(img_1d.begin(), img_1d.end(), 0.0));
                s = set_decimal(sum/img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Variance")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "stdDev")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(sqrt(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size()),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Percentile")
            {
                std::sort(img_1d.begin(), img_1d.end());
                QVariant percentile_value = nodeItem->propertyValue("Percentile");
                s = img_1d.at(img_1d.size()*percentile_value.toInt()/100-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
        }
    }
    else    // Mask not available
    {
        XMLRecReader X;
        std::vector<float> img_1d;
        std::copy(Source_im->data.begin(),Source_im->data.end(),std::back_inserter(img_1d));

        if(Source_im->NumDimensions == 2)
        {
            if(img_1d.size() == 0)
            {
                output = 0.0;
                return -1;
            }
            if (imageStatistics == "Min")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s = img_1d.at(0);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            if (imageStatistics == "Max")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= img_1d.at(img_1d.size()-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Median")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= set_decimal(img_1d.at(img_1d.size()/2-1),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Mean")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                s = set_decimal(sum / img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Variance")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "stdDev")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(sqrt(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size()),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Percentile")
            {
                std::sort(img_1d.begin(), img_1d.end());
                QVariant percentile_value = nodeItem->propertyValue("Percentile");
                s = img_1d.at(img_1d.size()*percentile_value.toInt()/100-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }

        }
        else if(Source_im->NumDimensions == 3)
        {
            if(img_1d.size() == 0)
            {
                output = 0.0;
                return -1;
            }

            if (imageStatistics == "Min")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s = img_1d.at(0);
                if ( set_roumd==true ) s = round(s);
                qDebug()<<"$$$$$$$$$$$$ s="<<s;
                output = s;
            }
            if (imageStatistics == "Max")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= img_1d.at(img_1d.size()-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Median")
            {
                std::sort(img_1d.begin(), img_1d.end());
                s= set_decimal(img_1d.at(img_1d.size()/2-1),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Mean")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                s = set_decimal(sum / img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Variance")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size(),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "stdDev")
            {
                sum = std::accumulate(img_1d.begin(), img_1d.end(), 0.0);
                mean = sum / img_1d.size();

                for(int i = 0; i<img_1d.size(); i++)
                    img_1d[i] = (img_1d[i]-mean) * (img_1d[i]-mean);

                s = set_decimal(sqrt(std::accumulate(img_1d.begin(), img_1d.end(), 0.0) / img_1d.size()),decimal_num);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
            else if (imageStatistics == "Percentile")
            {
                std::sort(img_1d.begin(), img_1d.end());
                QVariant percentile_value = nodeItem->propertyValue("Percentile");
                s = img_1d.at(img_1d.size()*percentile_value.toInt()/100-1);
                if ( set_roumd==true ) s = round(s);
                output = s;
            }
        }
    }// if mask present

    QVariant* qv = new QVariant();
    qv->setValue(output);
    outData.push_back(qv);

   setReadyOutPorts(8 + getNi().NumInputs + 0, true);
   markExecuted();
   return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageStatistics::validateDesign()
{
    QString imageStatistics = nodeItem->propertyValue("ImageStatistics").toString();
     if (imageStatistics == "Percentile")
     {
        int percentile_value = nodeItem->propertyValue("Percentile").toInt();
        if(percentile_value < 0 || percentile_value >100)
        {
            qDebug() << "ImageStatistics: incorrect percentile value";
            return -1;
        }
     }
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageStatistics::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageStatistics::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
ImageStatistics* ImageStatistics::copy() const
{
    return new ImageStatistics(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageStatistics::uniqueKey() const
{
    return "ImageStatistics";
}
//--------------------------------------------------------------------------------------------------
void ImageStatistics::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageStatistics::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------


