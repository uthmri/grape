/* sink.cpp
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
#include <Nodes/sink.h>
#include <vector>
#include <qthread.h>
#include <QImage>
#include <image/image.hpp>
#include <xmlreclib.h>
#include <common.h>

//--------------------------------------------------------------------------------------------------
sink::sink(QObject *parent)
{
    setParent(parent);
    node_type = "sink";
    im = NULL;
    sdialog = NULL;
}
//--------------------------------------------------------------------------------------------------
sink::~sink()
{
}
//--------------------------------------------------------------------------------------------------
void sink::init()
{
}
//--------------------------------------------------------------------------------------------------
bool sink::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void sink::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void sink::set_input_by_index(int in_id, QVariant* varvalue)
{
    if (in_id == 0)
    {
        QString outputType = nodeItem->propertyValue("OutputType").toString();

        out_fname = nodeItem->propertyValue("FilePath").toString();
        sink_input_type = varvalue->typeName();

        if (outputType == "ImageDisplay")
        {
            bool isImage = varvalue->canConvert<ImageC*>();
            if(!isImage)
            {
                src_assigned[in_id] = false;
                return;
            }
            im = varvalue->value<ImageC*>();
            //qDebug() << "SINK (" << getType() << ") input image size = " << im->array_size[0] << im->array_size[1] << im->array_size[2] << im->array_size[3];
        }
        else if(outputType == "PrintText")
        {
            result = varvalue->toString();
        }
        else if(outputType == "WritetoFile")
        {
             out_fname = nodeItem->propertyValue("FilePath").toString();
             result = varvalue->toString();
        }
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* sink::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* sink::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int sink::execute()
{   
    clearOutputData();
    QString outputType = nodeItem->propertyValue("OutputType").toString();
    QString outputFilename = nodeItem->propertyValue("FilePath").toString();

    //qDebug() << getType() << "<" << this->getDiagramItem()->caption1() << ">" << "Mode = " << outputType;

    if (outputType == "ImageDisplay" && isGraphical())
    {
        sdialog = new SinkDialog();
        sdialog->setModal(false);
        sdialog->setWindowTitle(this->getDiagramItem()->caption1());

        sdialog->setImage(im);
        sdialog->updateImage();
        sdialog->show();
    }
    else if(outputType == "PrintText")
    {
        QString msg = QString("SINK PRINTING: ") + result;
        writeStrToLog(msg);
        qDebug() << msg;
    }
    else if(outputType == "WritetoFile")
    {
        /* Try and open a file for output */
        QFile outputFile(outputFilename);
        outputFile.open(QIODevice::WriteOnly);
        /* Check it opened OK */
        if(!outputFile.isOpen())
        {
            qDebug()  << "- Error, unable to open" << outputFilename << "for output";
            return -1;
        }

        /* Point a QTextStream object at the file */
        QTextStream outStream(&outputFile);

        /* Write the line to the file */
        outStream << result;
        /* Close the file */
        outputFile.close();
    }


    QVariant* qv = new QVariant;
    qv->setValue(0);
    if(outputType == "PrintText")
    {
        qv->setValue(result);
    }
    else if(outputType == "WritetoFile")
    {
        qv->setValue(out_fname);
    }
    else if(outputType == "ImageDisplay")
    {
        qv->setValue(0);
    }
    else
    {
        qDebug() << getType() << "<" << this->getDiagramItem()->caption1() << ">" << outputType << "is unreognized operation.";
        return -1;
    }
    outData.clear();
    outData.push_back(qv);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;
}
//------------------------------------------------------------------------------
int sink::validateDesign()
{
    QString outputFilename = nodeItem->propertyValue("FilePath").toString();
    QString outputType = nodeItem->propertyValue("OutputType").toString();

    if( (outputType == "writetoFile") && outputFilename.isEmpty())
        return -1;
    return 0;
}
//------------------------------------------------------------------------------
int sink::validateRun()
{
    QString outputType = nodeItem->propertyValue("OutputType").toString();

    if(outputType == "PrintText" && !(sink_input_type == "int" || sink_input_type == "QString" || sink_input_type == "double" || sink_input_type == "float" || sink_input_type == "bool") )
    {
        qDebug() << "ERROR: SINK<" << this->uniqueKey() << ">" << "Incorrect data type.";
        return -1;
    }

    if(outputType == "ImageDisplay")
    {
        if(!src_assigned.at(0))
        {
            qDebug() << "SINK<" << this->uniqueKey() << ">" << "Input is not an image";
            return -1;
        }
    }

    if(outputType == "writetoFile" && sink_input_type != "QString")
    {
        qDebug() << "ERROR: SINK<" << this->uniqueKey() << ">" << "Input is not a QString";
        return -1;
    }
    return 0;
}
//--------------------------------------------------------------------------------------------------
void sink::clearNodeData()
{
    delete sdialog;
}
//--------------------------------------------------------------------------------------------------
sink* sink::copy() const
{
    return new sink(*this);
}
//--------------------------------------------------------------------------------------------------
QString sink::uniqueKey() const
{
    return "Sink";
}
//--------------------------------------------------------------------------------------------------
void sink::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool sink::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------


