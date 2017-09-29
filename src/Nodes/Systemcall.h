/* Systemcall.h
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

#ifndef _Systemcall_H
#define _Systemcall_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>

class Systemcall : public Node
{
    Q_OBJECT

protected:

    QVariant* vI1;
    QVariant* vI2;
    QVariant* vI3;
    QVariant* vI4;
    QVariant* vO1;
    QVariant* vO2;
    QVariant* vO3;
    QVariant* vI5;
    QString command_name;

    QString output1;
    QString output2;
    QString output3;
    //QString mask_out_fname;
    QString mask_image;


protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

public:

    Systemcall(QObject *parent = 0);
    ~Systemcall();

    virtual void setConnectedInPorts();  // set in pipeline::load
   // virtual void setReadyInPorts();  // set in pipeline::run

    virtual QString uniqueKey() const;

    virtual void init();
    virtual int execute();
    virtual int validateDesign();
    virtual int validateRun();
    virtual void clearNodeData();

    void set_input(int in_port_id, QVariant* varvalue);
    QVariant* get_output(int out_port_id);

    bool isAllInputsReady();


    virtual Systemcall* copy() const;

    void markDirty();
    bool isSuperfluous() const;
};

#endif







