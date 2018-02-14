/* ImageArithmetic.cpp
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
#include <Nodes/imagearithmetic.h>
#include <image/image.hpp>
#include <xmlreclib.h>

//--------------------------------------------------------------------------------------------------
ImageArithmetic::ImageArithmetic(QObject *parent)
{
    setParent(parent);
    node_type = "ImageArithmetic";
    src_assigned.push_back(false);
    src_assigned.push_back(false);
    result  = NULL;
}
//--------------------------------------------------------------------------------------------------
ImageArithmetic::~ImageArithmetic() { }
//--------------------------------------------------------------------------------------------------
void ImageArithmetic::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageArithmetic::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageArithmetic::set_input(int in_port_id, QVariant* varvalue)
{
   Node::set_input( in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ImageArithmetic::set_input_by_index(int in_id, QVariant* varvalue)
{
    int arg_type;

    if(in_id==0)
    {
        bool isImage = varvalue->canConvert<ImageC*>();
        if(isImage)
        {
            arg_type = 0;
            im1 = (ImageC*)(varvalue->value<ImageC*>());
        }
        else
        {
            arg_type = 1;
            user_val1 = varvalue->toDouble();
        }
        arg1_type = arg_type;
        src_assigned[in_id]= true;
    }

    if(in_id==1)
    {
        bool isImage = varvalue->canConvert<ImageC*>();
        if(isImage)
        {
            arg_type = 0;
            im2 = (ImageC*)(varvalue->value<ImageC*>());
        }
        else
        {
            arg_type = 1;
            user_val2 = varvalue->toDouble();
        }
        arg2_type = arg_type;
        src_assigned[in_id]= true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageArithmetic::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* ImageArithmetic::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int ImageArithmetic::execute()
{
    unsigned int ii;
    clearOutputData();

    // op_arg_type:
    //    = 0 ==> img to img
    //    = 1 ==> img and number
    //    = 2 ==> number and image
    //    = 3 ==> number and number
    op_arg_type =   0 * ((arg1_type==0)&&(arg2_type==0)) +
            1 * ((arg1_type==0)&&(arg2_type==1)) +
            2 * ((arg1_type==1)&&(arg2_type==0)) +
            3 * ((arg1_type==1)&&(arg2_type==1));

    if(op_arg_type == 0) // img to img
    {
        bool MatchingSize = true;
        if( im1->numDimensions() != im2->numDimensions() )
            MatchingSize = false;
        else
        {
            for(int dd=0; dd<im1->numDimensions(); dd++)
                if ( im1->arraySize(dd) != im2->arraySize(dd) )
                {
                    MatchingSize = false;
                    break;
                }
        }

        if(!MatchingSize)
        {
            qDebug() << "Operation cannot be completed. Incompatible image dimensions";
            return -1;
        }
    }

    if((op_arg_type==0) || (op_arg_type==1)) // result is an image
        result = new ImageC(im1);
    else if(op_arg_type==2) // result is an image
        result = new ImageC(im2);

    QString imageArithmeticType =  nodeItem->propertyValue("ImageArithmeticType").toString();

    if (imageArithmeticType == "+")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) + im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) + user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 + im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 + user_val2;
        }
    }
    else if (imageArithmeticType == "-")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) - im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) - user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 - im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 - user_val2;
        }
    }
    else if (imageArithmeticType == "*")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) * im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) * user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 * im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 * user_val2;
        }
    }
    else if (imageArithmeticType == "/")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++){
                if(im2->data.at(ii) !=0.0) result->data.at(ii) = im1->data.at(ii) / im2->data.at(ii);
                else if(im2->data.at(ii) ==0.0) result->data.at(ii) = 0;
            }
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++){
                if(user_val2 !=0.0) result->data.at(ii) = im1->data.at(ii) / user_val2;
                else if(user_val2 ==0.0) result->data.at(ii) = 0;
            }
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++){
                if(im2->data.at(ii) !=0.0) result->data.at(ii) = user_val1 / im2->data.at(ii);
                else if(im2->data.at(ii) ==0.0) result->data.at(ii) = 0;
            }
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 + user_val2;
        }
    }
    else if (imageArithmeticType == "pow")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = qPow(im1->data.at(ii), im2->data.at(ii));
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = qPow(im1->data.at(ii), user_val2);
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = qPow(user_val1, im2->data.at(ii));
        }
        else if(op_arg_type==3)
        {
            result_numeric = qPow(user_val1 ,user_val2);
        }
    }
    else if (imageArithmeticType == "==")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) == im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) == user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 == im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 == user_val2;
        }
    }
    else if (imageArithmeticType == ">=")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) >= im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) >= user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 >= im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 >= user_val2;
        }
    }
    else if (imageArithmeticType == "<=")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) <= im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) <= user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 <= im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 <= user_val2;
        }
    }
    else if (imageArithmeticType == ">")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) > im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) > user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 > im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 > user_val2;
        }
    }
    else if (imageArithmeticType == "<")
    {
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) < im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) < user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(int ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 < im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 < user_val2;
        }
    }
    else if (imageArithmeticType == "&&"){
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) && im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) && user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(int ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 && im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 && user_val2;
        }
    }
    else if (imageArithmeticType == "||"){
        if((op_arg_type==0)) // image to image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) || im2->data.at(ii);
        }
        if((op_arg_type == 1)) // image to num
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = im1->data.at(ii) || user_val2;
        }
        if((op_arg_type==2)) // num to image
        {
            result = new ImageC(im2);
            for(int ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = user_val1 || im2->data.at(ii);
        }
        else if(op_arg_type==3)
        {
            result_numeric = user_val1 || user_val2;
        }
    }
    else if (imageArithmeticType == "~"){
        if((arg1_type==0)) // image
        {
            result = new ImageC(im1);
            for(ii = 0; ii<result->data.size(); ii++)
                result->data.at(ii) = !im1->data.at(ii);
        }
        else // scalar
        {
            result_numeric = !user_val1;
        }
    }


    qv = new QVariant;
    if(op_arg_type==3)
        qv->setValue(result_numeric);
    else
        qv->setValue(result);
    outData.clear();
    outData.push_back(qv);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageArithmetic::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageArithmetic::validateRun()
{
    // op_arg_type:
    //    = 0 ==> img to img
    //    = 1 ==> img and number
    //    = 2 ==> number and image
    //    = 3 ==> number and number
    op_arg_type =   0 * ((arg1_type==0)&&(arg2_type==0)) +
            1 * ((arg1_type==0)&&(arg2_type==1)) +
            2 * ((arg1_type==1)&&(arg2_type==0)) +
            3 * ((arg1_type==1)&&(arg2_type==1));

    if((op_arg_type==0)||(op_arg_type==1))
        if (im1->NumDimensions < 2 || im1->NumDimensions > 4)
            return -1;

    if((op_arg_type==0)||(op_arg_type==2))
        if (im2->NumDimensions < 2 || im2->NumDimensions > 4)
            return -1;

    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageArithmetic::clearNodeData()
{
//    if((src_assigned.at(0) && (arg1_type==0)) || (src_assigned.at(1) && (arg2_type==0)))
//        result->clear();

    if(result)
        result->clear();
}
//--------------------------------------------------------------------------------------------------
ImageArithmetic* ImageArithmetic::copy() const
{
    return new ImageArithmetic(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageArithmetic::uniqueKey() const
{
    return "ImageArithmetic";
}
//--------------------------------------------------------------------------------------------------
void ImageArithmetic::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageArithmetic::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void ImageArithmetic::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
