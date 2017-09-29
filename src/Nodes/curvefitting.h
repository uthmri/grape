/* CurveFitting.h
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



#ifndef _CurveFitting_H
#define _CurveFitting_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include <xmlreclib.h>
#include <image/image.hpp>

class CurveFitting : public Node
{
    Q_OBJECT

protected:

    QString xdata_fname;
    QString ydata_fname;
    QString out_fname;
    QVariant par;
    double p[10];

    ImageC *im, *im1, *im2, *im3;
    QVariant* qv;

    image::basic_image<short,3> image_data1;
    image::basic_image<short,3> image_data2;
    image::basic_image<short,3> image_data3;
    image::basic_image<short,3> image_data4;
    image::basic_image<short,3> image_data5;
    image::basic_image<short,3> image_data6;
    image::basic_image<short,3> image_data7;

    struct MP2RAGE_PARS
    {
        float a1 = 7;      // flip angle of 1st image
        float a2 = 5;      // flip angle of 2nd image
        int np = 240;                // number of pulses; turbo factor; ETL
        int n_startup = 20;         // TFE startup pulses
        float TR_PSIR = 4500;         // cycle time = time between two inversion pulses
        float TR = 8.13;              // readout repeatition time
        float TE = 3.75;              // readout echo time
        float TI1 = 500;              // first TI
        float TI2 = 2600;             // value discarded; second TI is automatically calculated
        float eff = 0.96;             // efficiency of inversion pulse
        float T1_clip_u = 10000;
        float T1_clip_l = 0;
    }
    mp2rage_pars;

protected:
    virtual QVariant* get_output_by_index(int out_id);
    virtual void set_input_by_index(int in_id, QVariant* varvalue);

    template <class T>
    void readXDataValue(T x[], int Npts, int &parvalue);
    template <class T>
    void readXDataValue_1(T x[], int Npts, int &parvalue);
    template <class T>
    void readArgDataArray(T x[], int Npts, QString arg);
    template <class T>
    void readArgDataValue(T &x, QString arg);

public:

    CurveFitting(QObject *parent = 0);
    ~CurveFitting();

    virtual void setConnectedInPorts();  // set in pipeline::load
   // virtual void setReadyInPorts();  // set in pipeline::run
   virtual int linfunc(int m, int n, double *p, double *dy, double **dvec, void *vars);
   virtual int quadfunc(int m, int n, double *p, double *dy, double **dvec, void *vars);
   virtual int expfunc(int m, int n, double *p, double *dy, double **dvec, void *vars);
   virtual int  gaussfunc(int m, int n, double *p, double *dy, double **dvec, void *vars);
   //int T1func(int m, int n, double *p, double *dy, double **dvec, void *vars);
   virtual double gNoise(double m, double s);
   void mp2rage_lut_calc(float LUT[],MP2RAGE_PARS p);


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


    virtual CurveFitting* copy() const;

    void markDirty();
    bool isSuperfluous() const;
};

#endif







