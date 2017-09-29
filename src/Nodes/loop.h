/*Loop.h
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

#ifndef _Loop_H
#define _Loop_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>

class Loop : public Node
{
    Q_OBJECT

protected:

    float counter_start_value;
    float counter_end_value;
    float counter_step_value;
    float counter_value;
    int next_input, reset_input, stop_input;
    int end_value;
    bool first_iter;

    //QString mask_out_fname;
    QString mask_image;


    bool operatorvalue;
protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

public:
    Loop(QObject *parent = 0);

    ~Loop();

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


    virtual Loop* copy() const;

    void markDirty();
    bool isSuperfluous() const;

    bool isLoopDone();
};

#endif







