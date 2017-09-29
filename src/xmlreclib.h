/* xmlreclib.h
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

#ifndef XMLRECLIB_H
#define XMLRECLIB_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include <QDomDocument>
#include <vector>
#include "datatypes.h"

using std::vector;


class XMLRecReader
{
//    Q_OBJECT

protected:
    QString filename;
    QString data_value;
    //QString mask_out_fname;
    QString mask_image;

    bool operatorvalue;

public:
    ImageC* im;
    //ImageC im;
    vector <double> v1d;

public:
    XMLRecReader(QObject *parent = 0);
  ~XMLRecReader();

    virtual QString uniqueKey() const;
    // XMLRecReaderors
    virtual void init();

    virtual XMLRecReader* copy() const;

    void markDirty();
    bool isSuperfluous() const;

public:   // XML tools/functions
    void readSeriesInfo(QDomNodeList series_info_list, QVector< QVector<QString> > &series_attributes, SeriesClass &SeriesInfo);
    void readImageKey(QDomNodeList, ImKeyClass&);
    void readImageInfo(QDomNodeList,  ImInfoClass &);
    void setStructField(ImInfoClass &InInfo, QString var_str, QVariant var_value);
    QString makeKeyString(ImKeyClass imageKey);

    void Convert_vec1d_to_vec2d(vector<vector<float> > &v2d, const vector<float> &v1d, int sz1, int sz2);
    void Convert_vec3d_to_vec1d(const vector<vector<vector<float> > > &v3d, vector <float> &v1d, int sz1, int sz2, int sz3);
    void Convert_vec2d_to_vec1d(const vector<vector<float> > &v2d, vector <float> &v1d, int sz1, int sz2);

    void SetArrayDims2d(vector<vector<float> > &array2d, int sz1, int sz2);
    void SetArrayDims3d(vector<vector<vector<float> > > &array3d, int sz1, int sz2, int sz3);
    void SetArrayDims4d(vector<vector<vector<vector<float> > > > &array4d, int sz1, int sz2, int sz3, int sz4);

    void clearArray2d(vector<vector<float> > &array2d);
    void clearArray3d(vector<vector<vector<float> > > &array3d);
    void clearArray4d(vector<vector<vector<vector<float> > > > &array4d);

    void Insert_vec2d_in_vec3d(vector<vector<float> > v2d, vector <vector<vector<float> > > &v3d, int sz1, int sz2, int index);
    void Insert_vec2d_in_vec4d(vector<vector<float> > v2d, vector <vector<vector<vector<float> > > > &v4d, int sz1, int sz2, int index3, int index4);


    int loadXmlRec(QString filename, ImageC* ImageOut);
    int saveXmlRec(QString filename, ImageC* ImageToWrite);
    int saveXmlRec(QString filename, ImageC* ImageToWrite, ImageC* HeaderImage, bool anonymous);

    QDomElement SeriesInfoToNode( QDomDocument &d, const QString patient_name, const int max_ph, const int max_ec, const int max_slc, const int max_dyn, const int max_mix, const int max_bval, const int max_grd, const int max_lbl);
    QDomElement SeriesInfoToNode( QDomDocument &d, const SeriesClass S);
    QDomElement ImageToNode( QDomDocument &d, ImInfoClass IM_INFO, const int slc, const int ec, const int dyn, const int ph, const int bv, const int gr, const QString lty, const QString ty, const QString sq, const int idx);

    double vectorMaxValue(const vector<float> &v);
    double vectorMaxValue(const vector<vector<float> > &v);
    double vectorMaxValue(const vector<vector<vector<float> > > &v);
    double vectorMaxValue(const vector<vector<vector<vector<float> > > > &v);

    double vectorMinValue(const vector<float> &v);
    double vectorMinValue(const vector<vector<float> > &v);
    double vectorMinValue(const vector<vector<vector<float> > > &v);
    double vectorMinValue(const vector<vector<vector<vector<float> > > > &v);

    double imageMaxValue(ImageC* im);
    double imageMinValue(ImageC* im);

    QString get_xml_rec_series_element(SeriesClass S, int ii);
    QString get_xml_rec_image_element(ImInfoClass S, int ii);
    void get_xml_rec_series_defs(vector<QString> &Names, vector<QString> &Tags, vector<QString> &Levels, vector<QString> &Types);
    void get_xml_rec_image_defs(vector<QString> &Names, vector<QString> &Tags, vector<QString> &Types, vector<QString> &EnumTypes, vector<QString> &Calcs, vector<QString> &ArraySizes);
    int indexofSmallestElement(const vector<float> &v);
};

#endif // XMLRECLIB_H

