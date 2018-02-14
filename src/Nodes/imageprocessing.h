/* imageprocessing.h
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

#ifndef _ImageProcessing_H
#define _ImageProcessing_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include <QImage>
#include "datatypes.h"

class ImageProcessing : public Node
{
    Q_OBJECT

protected:

    QString in_fname1, in_fname2, in_fname3;
    QString mask_image;
    QString command_name;

    QString out_fname1, out_fname2;
    QString arg1, arg2;

    ImageC* im1;
    ImageC* im2;
    ImageC* result;
    float result_numeric;
    float rvsz[4];


    int arg1_type; // 0=image, 1=number
    int arg2_type; // 0=image, 1=number

    double user_val1;
    double user_val2;

    int op_arg_type;  // 0 : arg1=image to arg2=image
                     // 1 : arg1=image to arg2=number
                     // 2  : arg1=number to arg2=image
                     // 3  : arg1=number to arg2=number

protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

    template <class T>
    void readArgDataArray(T x[], int Npts, QString arg);

public:

    ImageProcessing(QObject *parent = 0);
    ~ImageProcessing();

    virtual void setConnectedInPorts();  // set in pipeline::load

    virtual QString uniqueKey() const;

    // Selectors
    virtual void init();
    virtual int execute();
    virtual int validateDesign();
    virtual int validateRun();
    virtual void clearNodeData();

    void set_input(int in_port_id, QVariant* varvalue);
    QVariant* get_output(int out_port_id);
    bool isAllInputsReady();

    virtual ImageProcessing* copy() const;

    void markDirty();
    bool isSuperfluous() const;
};

#endif







