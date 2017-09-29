/* ImageRegistration.h
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

#ifndef _ImageRegistration_H
#define _ImageRegistration_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>

class ImageRegistration : public Node
{
    Q_OBJECT

protected:

    QString baseNamePath;
    QString in_fpath;
    QString out_fpath;
    QString param1;
    QString param2;

    QString Imageregistration_Type;
    QString Similarity_Fuc;
    QString Work_Model;
    QString Fixed_Image_Path ;
    QString Moving_Image_Path;
    QString Fixed_Image;
    QString Moving_Image;
    QString Fixed_Image_Port ;
    QString Moving_Image_Port;
    QString Out_Image_Path;
    QString Text_Transform_File;
    QString Nii_Transform_File;
    QString commandPath;
    QString rigidparam;
    QString affineparam;
    QVariant* qv1;
    QVariant* qv2;
    QVariant* qv3;
    QVariant* vI1;
    QVariant* vI2;
    QVariant* vI3;
    QVariant* vI4;
    QFileInfo info1;
    //QString mask_out_fname;
    QString mask_image;


protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

public:

    ImageRegistration(QObject *parent = 0);
    ~ImageRegistration();

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

    virtual ImageRegistration* copy() const;

    void markDirty();
    bool isSuperfluous() const;
};

#endif







