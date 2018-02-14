/*ImageReader.h
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

#ifndef _ImageReader_H
#define _ImageReader_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include "datatypes.h"
#include <image/image.hpp>

struct IMAGE_INFO{
    float vsx=0;  //voxel size -x
    float vsy=0;  //voxel size -y
    float vsz=0;  //voxel size -z
    short int datatype=0; /* 30 + 2 */
    short int bitpix=0; /* 32 + 2 */

};

class ImageReader : public Node
{
    Q_OBJECT

protected:

    QString in_fname;
    QString out_fname;
    ImageC* im;
    QVariant vI1;
    QVariant vI2;
    QVariant vI3;
    QVariant* outdata;
    QDir dir;
    IMAGE_INFO image_info;
    image::basic_image<float,3> F3_IM;

    //QString mask_out_fname;
    QString mask_image;

    bool operatorvalue;
protected:
    virtual QVariant* get_output_by_index(int out_id);
    //virtual QVariant get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);
public:

   ImageReader(QObject *parent = 0);
    ~ImageReader();

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

    virtual ImageReader* copy() const;

    void markDirty();
    bool isSuperfluous() const;

};

#endif







