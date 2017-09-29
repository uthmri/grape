/* Develop.h
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

#ifndef _Develop_H
#define _Develop_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>

class Develop : public Node
{
    Q_OBJECT

protected:

    QVariant* vI1, *vI2, *vI3, *vI4, *vI5, *vI6, *vI7;
    QVariant* vO1, *vO2, *vO3, *vO4, *vO5, *vO6, *vO7;

    QString command_name;

    QString output1;
    QString output2;
    QString output3;
    QString output4;
    QString output5;
    QString output6;
    QString output7;

    //QString mask_out_fname;
    QString mask_image;


protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

public:

    Develop(QObject *parent = 0);
    ~Develop();

    virtual void setConnectedInPorts();  // set in pipeline::load

    virtual QString uniqueKey() const;

    virtual void init();
    virtual int execute();
    virtual int validateDesign();
    virtual int validateRun();
    virtual void clearNodeData();

    void set_input(int in_port_id, QVariant* varvalue);
    QVariant* get_output(int out_port_id);
    bool isAllInputsReady();

    virtual Develop* copy() const;

    void markDirty();
    bool isSuperfluous() const;
};

#endif







