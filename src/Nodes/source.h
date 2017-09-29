/* source.h
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

#ifndef _source_H
#define _source_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include <xmlreclib.h>

class source : public Node
{
    Q_OBJECT

    vector<float> x,y;
    vector<float> *xp;
    vector<float>  & getNum();
    void myNum();
    vector<float>  getNumV();

protected:
    QString in_fname;
    QString out_fname;
    QVariant* output;
    //QVector3DArray array;

protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

public:
    ImageC* im;
    bool SearchReady;

public:
    source(QObject *parent = 0);
    ~source();

    virtual QString uniqueKey() const;

    virtual void init();
    virtual int execute();
    virtual int validateDesign();
    virtual int validateRun();
    virtual void clearNodeData();

    virtual void set_input(int in_port_id, QVariant* varvalue);
    virtual QVariant* get_output(int out_port_id);
    bool fileExists(QString path);

    bool isAllInputsReady();

    virtual source* copy() const;
    void markDirty();
    bool isSuperfluous() const;

public slots:
    void setSearchReady();
};

#endif







