/*ImageArithmetic.h
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

#ifndef _ImageArithmetic_H
#define _ImageArithmetic_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include <QImage>
#include "datatypes.h"

class ImageArithmetic : public Node
{
    Q_OBJECT

protected:

    QString in_fname;
    QString out_fname;

   ImageC *im1, *im2, *result;
   QVariant *qv;

   double result_numeric;
   double user_val1;
   double user_val2;
   bool op_image_to_image;

   int arg1_type; // 0=image, 1=number
   int arg2_type; // 0=image, 1=number

   int op_arg_type;  // 0 : arg1=image to arg2=image
                    // 1 : arg1=image to arg2=number
                    // 2  : arg1=number to arg2=image
                    // 3  : arg1=number to arg2=number

    QString mask_image;


    bool operatorvalue;
protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

public:

   ImageArithmetic(QObject *parent = 0);
    ~ImageArithmetic();

    virtual void setConnectedInPorts();  // set in pipeline::load
   // virtual void setReadyInPorts();  // set in pipeline::run

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

    virtual ImageArithmetic* copy() const;

    void markDirty();
    bool isSuperfluous() const;
};

#endif







