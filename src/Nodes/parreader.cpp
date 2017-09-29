/* ParReader.cpp
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
#include <Nodes/parreader.h>
#include <image/image.hpp>
#include <xmlreclib.h>


ParReader::ParReader(QObject *parent)
{
    setParent(parent);
    node_type = "ParReader";
}
//--------------------------------------------------------------------------------------------------
ParReader::~ParReader() { }
//--------------------------------------------------------------------------------------------------
void ParReader::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ParReader::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ParReader::set_input(int in_port_id, QVariant *varvalue)
{
   Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void ParReader::set_input_by_index(int in_id, QVariant *varvalue)
{
    if(in_id == 0)
    {
        src_assigned[in_id] = true;
        xmlfilename = varvalue->toString();
    }
}
//--------------------------------------------------------------------------------------------------
QVariant *ParReader::get_output(int out_port_id)
{
   return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant *ParReader::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//--------------------------------------------------------------------------------------------------
int ParReader::execute()
{
    clearOutputData();
    in_fname =  nodeItem->propertyValue("InputfilePath").toString();

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
            count++;
        }
    }

    if(0)       // XML File
    {
        QDomDocument document;
        QFile file(xmlfilename);

        if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            qDebug( "Failed to open file for reading." );
            return -1;
        }

        if( !document.setContent( &file ) )
        {
            qDebug( "Failed to parse the file into a DOM tree." );
            file.close();
            return -1;
        }

        file.close();

        QDomElement root = document.firstChildElement();
        QDomNodeList tag_list;
        for(int i=0; i<tags.length(); i++)
        {
            tag_list = root.elementsByTagName(tags.at(i));
            if(tag_list.length()>0)
            {
                QVariant* qv = new QVariant(tag_list.at(0).firstChild().nodeValue());
                outData.insert(i, qv);
                setReadyOutPorts(8 + getNi().NumInputs + i, true);
            }
        }
    }
    else if(1)//(OUTPUT_TYPE_PLAIN)
    {
        std::ifstream inStream;
        std::string line;
        inStream.open(in_fname.toStdString().c_str());

        if(!inStream.is_open())
        {
            qDebug()  << "Error, unable to open" << in_fname << "for input";
            return -1;
        }

        vars.clear();
        outData.clear();

        while ( std::getline(inStream,line) )
        {
            vars.push_back(QString::fromStdString(line));
            QVariant* qv = new QVariant(QString::fromStdString(line));
            outData.push_back(qv);
        }
        inStream.close();
    }

    for(int i = 0; i<outData.length(); i++)
        setReadyOutPorts(8 + getNi().NumInputs + i, true);

    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ParReader::validateDesign()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ParReader::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ParReader::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
ParReader* ParReader::copy() const
{
    return new ParReader(*this);
}
//--------------------------------------------------------------------------------------------------
QString ParReader::uniqueKey() const
{
    return "ParReader";
}
//--------------------------------------------------------------------------------------------------
void ParReader::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ParReader::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void ParReader::setConnectedInPorts()
{
}

