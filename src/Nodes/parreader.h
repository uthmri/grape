/*ParReader.h
 *
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

#ifndef _ParReader_H
#define _ParReader_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include "datatypes.h"
#include <image/image.hpp>

class ParReader : public Node
{
    Q_OBJECT

protected:

    QString in_fname;
    QString out_fname;
    QString xmlfilename;

    QDir dir;

    //QString mask_out_fname;
    QString mask_image;
    QList<QString> vars;

    bool operatorvalue;
protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

public:

   ParReader(QObject *parent = 0);
    ~ParReader();

    virtual void setConnectedInPorts();  // set in pipeline::load
   // virtual void setReadyInPorts();  // set in pipeline::run

    virtual QString uniqueKey() const;

    virtual void init();
    virtual int execute();
    virtual int validateDesign();
    virtual int validateRun();
    virtual void clearNodeData();

    virtual void set_input(int in_port_id, QVariant* varvalue);
    virtual QVariant* get_output(int out_port_id);

    bool isAllInputsReady();


    virtual ParReader* copy() const;

    void markDirty();
    bool isSuperfluous() const;
};

#endif







