/* XMLWiter.cpp
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
#include <Nodes/parwriter.h>
#include <image/image.hpp>
#include <xmlreclib.h>
#include <fstream>


//--------------------------------------------------------------------------------------------------
ParWriter::ParWriter(QObject *parent)
{
    setParent(parent);
    node_type = "ParWriter";
}
//--------------------------------------------------------------------------------------------------
ParWriter::~ParWriter() { }
//--------------------------------------------------------------------------------------------------
void ParWriter::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ParWriter::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ParWriter::set_input(int in_port_id, QVariant *varvalue)
{
   Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ParWriter::set_input_by_index(int in_id, QVariant *varvalue)
{
    int N =  getNi().NumInputs;
    //qDebug()<<"N="<<N;
    if((in_id >= 0) && (in_id < N))
    {
        src_assigned[in_id] = true;
        vars.insert(in_id, varvalue->toString());
    }
}
//--------------------------------------------------------------------------------------------------
QVariant *ParWriter::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant *ParWriter::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int ParWriter::execute()
{   
    clearOutputData();
    out_fname =  nodeItem->propertyValue("OutputfilePath").toString();
    qDebug() << " in ParWriter::execute(), out_fname="<<out_fname;
    //    // get  filename from Item Dialog
    QList<QString> tags;

    //  get variable tags from Item Dialog
    if(nodeItem->containsProperty("DataList"))
    {
        QString s1;
        QString text = nodeItem->propertyValue("DataList").toString();
        QRegExp rx("\\{*\\}");    // find the string between braces
        int count = 0;

        int pos = 0, newpos = 0;
        while ((newpos = rx.indexIn(text, pos)) != -1)
        {
            s1 = text.mid(pos+1, newpos-pos-1);
            tags.append(s1);

            pos = newpos + 1;
            //            tags =
            count++;
        }
    }

    if(0)//(OUTPUT_TYPE_XLM)
    {
        QDomDocument document;

        if (!(out_fname.contains(".xml"))&&!(out_fname.contains(".XML")))
        {
            qDebug( "File name does not contain .xml or .XML." );
            return -1;
        }

        QFile xmlfile(out_fname);

        if( !xmlfile.open( QIODevice::WriteOnly | QIODevice::Text ) )
        {
            qDebug( "Failed to open XML file for writing." );
            return -1;
        }

        QDomElement root = document.createElement("PARAMETERS");

        // write the values and tage
        for(int i=0; i<tags.length(); i++)
        {
            QDomElement e = document.createElement(tags.at(i));;
            QDomText  node_value = document.createTextNode(vars[i]);
            e.appendChild(node_value);
            root.appendChild(e);
        }

        document.appendChild(root);
        QTextStream ts( &xmlfile );
        ts << document.toString();
        ts.flush();
        xmlfile.close();
    }
    else if(1)//(OUTPUT_TYPE_PLAIN)
    {
        std::ofstream outStream;
        outStream.open(out_fname.toStdString().c_str());

        if(!outStream.is_open())
        {
            qDebug()  << "- Error, unable to open" << out_fname << "for output";
            return -1;
        }

        /* Write the line to the file */
        for(int i=0; i<vars.size(); i++)
        {
            outStream << vars[i].toStdString() << std::endl;
        }

        outStream.close();
    }

    QVariant* qv = new QVariant();
    qv->setValue(out_fname);
    outData.clear();
    outData.push_back(qv);

    setReadyOutPorts(8 + getNi().NumInputs + 0, true);
    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ParWriter::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ParWriter::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ParWriter::clearNodeData()
{

}
//--------------------------------------------------------------------------------------------------
ParWriter* ParWriter::copy() const
{
    return new ParWriter(*this);
}
//--------------------------------------------------------------------------------------------------
QString ParWriter::uniqueKey() const
{
    return "ParWriter";
}
//--------------------------------------------------------------------------------------------------
void ParWriter::markDirty()
{
    //	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
bool ParWriter::isSuperfluous() const
{
    return false;
    //return (DiagramOvalItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ParWriter::setConnectedInPorts()
{
}

