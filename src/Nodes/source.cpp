/* source.cpp
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
#include <Nodes/source.h>
#include <image/image.hpp>
#include <xmlreclib.h>
#include <stdlib.h>
#include <random>
#include <QTimer>

//--------------------------------------------------------------------------------------------------
source::source(QObject *parent)
{
    setParent(parent);
    node_type = "Source";

    //    set_input(0+8, 0);        // Don't use, there are no inputs to Source; only need fresh ports
    setFreshInPorts(0+8, true);

    im = NULL;
    SearchReady = true;

    FileSearchTimer.stop();
    FileSearchTimer.setSingleShot(false);
    FileSearchTimer.setInterval(5000); // 5 sec
    connect(&FileSearchTimer, SIGNAL(timeout()), this, SLOT(setSearchReady()));
}
//--------------------------------------------------------------------------------------------------
source::~source() { }
//--------------------------------------------------------------------------------------------------
void source::init()
{
}
//--------------------------------------------------------------------------------------------------
bool source::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void source::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void source::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id == 0)
        src_assigned[in_id] = true;
}
//--------------------------------------------------------------------------------------------------
QVariant* source::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* source::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
bool source::fileExists(QString path)
{
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    return (checkFile.exists() && checkFile.isFile());
}
//--------------------------------------------------------------------------------------------------
int source::execute()
{    
    clearOutputData();

    QString data_type = nodeItem->propertyValue("DataType").toString();
    QVariant dataValue = nodeItem->propertyValue("DataValue");
    bool array = nodeItem->propertyValue("Array").toBool();

    XMLRecReader X;

    float rand_seed = dataValue.toInt();
    float randn_seed = dataValue.toInt();

    std::default_random_engine ugenerator;
    ugenerator.seed(rand_seed);
    std::uniform_real_distribution<float> udistribution(0.0, 1.0);

    std::default_random_engine ngenerator;
    ngenerator.seed(randn_seed);
    std::normal_distribution<float> ndistribution(0.0, 1.0);

    if(array)
    {
        im = new ImageC;

        int dim1=0, dim2=0, dim3=0, dim4=0;

        if(!(nodeItem->propertyValue("Dimension1").toString().isEmpty()))
            dim1 = nodeItem->propertyValue("Dimension1").toString().toInt();

        if(!(nodeItem->propertyValue("Dimension2").toString().isEmpty()))
            dim2 = nodeItem->propertyValue("Dimension2").toString().toInt();

        if(!(nodeItem->propertyValue("Dimension3").toString().isEmpty()))
            dim3 = nodeItem->propertyValue("Dimension3").toString().toInt();

        if(!(nodeItem->propertyValue("Dimension4").toString().isEmpty()))
            dim4 = nodeItem->propertyValue("Dimension4").toString().toInt();

        im->array_size[0] = dim1;
        im->array_size[1] = dim2;
        im->array_size[2] = dim3;
        im->array_size[3] = dim4;

        im->NumDimensions = 0;
        int NumElements = 0;

        if(dim4)
        {
            im->NumDimensions = 4;
            NumElements = im->array_size[0] * im->array_size[1] * im->array_size[2] * im->array_size[3];
        }
        else if(dim3)
        {
            im->NumDimensions = 3;
            NumElements = im->array_size[0] * im->array_size[1] * im->array_size[2];
        }
        else if(dim2)
        {
            im->NumDimensions = 2;
            NumElements = im->array_size[0] * im->array_size[1];
        }
        else if (dim1)
        {
            im->NumDimensions = 1;
            NumElements = im->array_size[0];
        }

        im->data.resize(NumElements);

        for(int x=0; x<im->data.size(); x++)
            if(data_type == "rand")
                im->data.at(x) = udistribution(ugenerator);
            else if(data_type == "randn")
                im->data.at(x) = ndistribution(ngenerator);
            else
                im->data.at(x) = dataValue.toDouble();

        output = new QVariant;
        output->setValue(im);
        outData.push_back(output);
    }
    else    // not array
    {
        output = new QVariant();
        if (data_type.toLower() == "string")
        {            
            output->setValue(dataValue.toString());
        }
        else if(data_type.toLower() == "int")
        {
            output->setValue(dataValue.toInt());
        }
        else if(data_type.toLower() == "double")
        {
            output->setValue(dataValue.toDouble());
        }
        else if(data_type.toLower() == "rand")
        {
            output->setValue(QVariant(udistribution(ugenerator)));
        }
        else if(data_type.toLower() == "randn")
        {
            output->setValue(QVariant(ndistribution(ngenerator)));
        }
        else if(data_type.toLower() == "float")
        {
            output->setValue(dataValue.toFloat());
        }
        else if(data_type.toLower() == "boolean")
        {
            output->setValue(dataValue.toBool());
        }
        else if(data_type == "File")
        {
            // if validateRun() succeeds, then the file already exists!
            output->setValue(dataValue.toString());
            FileSearchTimer.stop();
        }
        else if(data_type == "rand")
        {
            output->setValue(udistribution(ugenerator));
        }
        else if(data_type == "randn")
        {
            output->setValue(ndistribution(ngenerator));
        }

        outData.push_back(output);
    }

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int source::validateDesign()
{
    QString data_type = nodeItem->propertyValue("DataType").toString();

    if(data_type == "File")
    {
        FileSearchTimer.start();
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------
int source::validateRun()
{
    QString data_type = nodeItem->propertyValue("DataType").toString();
    QVariant dataValue = nodeItem->propertyValue("DataValue");

    if(data_type == "File")
    {
        if(SearchReady)
        {
            SearchReady = false;  // do not search again until the FileSearchTimer fires
            if(!fileExists(dataValue.toString()))
            {
                qDebug() << "Cannot find file:" << dataValue.toString();
                return -2;  // node not ready
            }

            QFile outputFile;
            outputFile.setFileName(dataValue.toString());
            outputFile.open(QIODevice::ReadOnly);
            if(!outputFile.isOpen())
            {
//                qDebug() << "Cannot open file:" << dataValue.toString();
                outputFile.close();
                return -2;  // node not ready
            }
            outputFile.close();
//            qDebug() << "File exists:" << dataValue.toString();
            return 0;
        }
        else
        {
            return -2;
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------
void source::clearNodeData()
{
    if(im)
    {
        im->clear();
        im->init();
    }
}
//--------------------------------------------------------------------------------------------------
source* source::copy() const
{
    return new source(*this);
}
//--------------------------------------------------------------------------------------------------
QString source::uniqueKey() const
{
    return "Source";
}
//--------------------------------------------------------------------------------------------------
void source::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool source::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void source::setSearchReady()
{
     SearchReady = true;
}
//--------------------------------------------------------------------------------------------------
vector<float> source::getNumV()
{
    y.resize(8000000,1);
    return y;
}
//--------------------------------------------------------------------------------------------------
vector<float>& source::getNum()
{
    y.resize(8000000,1);
    return y;
}
//--------------------------------------------------------------------------------------------------
void source::myNum()
{
    xp = &(getNum());
    qDebug() << xp->size();
}
//--------------------------------------------------------------------------------------------------
