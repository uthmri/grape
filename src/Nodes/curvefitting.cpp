/* CurveFitting.cpp
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

#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>
#include <Nodes/curvefitting.h>
#include <xmlreclib.h>
#include "mpfit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <QImage>
#include <image/image.hpp>
#include <xmlreclib.h>

#include "omp.h"
#define REQUESTED_NUM_THREADS 8

#define DBL_RAND_MAX (double)(RAND_MAX);
#define INIT_RANDOM(seed) srand(seed);

//------------------------------------------------------------------------------
// struct used in optimization
struct vars_struct {
    double *x;
    double *y;
    double *ey;
    double tau=6.2;
    double td=4.2;
    double tr=2578;
    double Etl=8;
    double FA=7.0;
    double min_1=0;
    double min_2=0;
    double max_1=10000; //clip_T1_max=10000
    double max_2=8000;  //clip_PD_max=8000

};

struct T1_IR_2P_vars_struct {
    double TR;
    double *TI;
    double *y;
    double *ey;
};

//--------------------------------------------------------------------------------------------------
CurveFitting::CurveFitting(QObject *parent)
{
    setParent(parent);
    node_type = "CurveFitting";
    im1 = NULL;
    im2 = NULL;
}
//--------------------------------------------------------------------------------------------------
CurveFitting::~CurveFitting() { }
//--------------------------------------------------------------------------------------------------
void CurveFitting::init()
{
}
//--------------------------------------------------------------------------------------------------
bool CurveFitting::isAllInputsReady()
{
    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void CurveFitting::set_input(int in_port_id, QVariant* varvalue)
{
    Node::set_input(in_port_id, varvalue);
}
//--------------------------------------------------------------------------------------------------
void CurveFitting::set_input_by_index(int in_id, QVariant* varvalue)
{
    if(in_id==0)
    {
        bool isImage = varvalue->canConvert<ImageC*>();
        if(!isImage)
        {
            src_assigned[in_id] = false;
            return;
        }
        im = (ImageC*)(varvalue->value<ImageC*>());
        src_assigned[in_id] = true;
    }
}
//--------------------------------------------------------------------------------------------------
QVariant* CurveFitting::get_output(int out_port_id)
{
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant* CurveFitting::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
/* Gaussian noise with mean m and variance s, uses the Box-Muller transformation */
double CurveFitting::gNoise(double m, double s)
{
    double r1, r2, val;

    r1=((double)rand())/DBL_RAND_MAX;
    r2=((double)rand())/DBL_RAND_MAX;

    val=sqrt(-2.0*log(r1))*cos(2.0*M_PI*r2);

    val=s*val+m;

    return val;
}
//-------------------------------------------------------------------------------
/* Simple routine to print the fit results */
void printresult(double *x, double *xact, mp_result *result)
{
    int i;

    if ((x == 0) || (result == 0)) return;
    printf("  CHI-SQUARE = %f    (%d DOF)\n",
           result->bestnorm, result->nfunc-result->nfree);
    printf("     NPAR = %d\n", result->npar);
    printf("     NFREE = %d\n", result->nfree);
    printf("     NPEGGED = %d\n", result->npegged);
    printf("     NITER = %d\n", result->niter);
    printf("     NFEV = %d\n", result->nfev);
    printf("\n");
    if (xact) {
        for (i=0; i<result->npar; i++)
            printf("  P[%d] = %f",i, x[i], xact[i]);
    } else {
        for (i=0; i<result->npar; i++)
            printf("  P[%d] = %f ", i, x[i] );
    }
}
//------------------------------------------------------------------------------
int CurveFitting::linfunc(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i;
    struct vars_struct *v = (struct vars_struct *) vars;
    double *x, *y, *ey, f;

    x = v->x;
    y = v->y;
    ey = v->ey;

    for (i=0; i<m; i++) {
        f = p[0] - p[1]*x[i];     /* Linear fit function; note f = a - b*x */
        dy[i] = (y[i] - f)/ey[i];
    }
    return 0;
}
//------------------------------------------------------------------------------
int CurveFitting::quadfunc(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i;
    struct vars_struct *v = (struct vars_struct *) vars;
    double *x, *y, *ey;
    x = v->x;
    y = v->y;
    ey = v->ey;

    for (i=0; i<m; i++) {
        dy[i] = (y[i] - p[0] - p[1]*x[i] - p[2]*x[i]*x[i])/ey[i];
    }
    return 0;
}
//------------------------------------------------------------------------------
/* model to be fitted to measurements: x_i = p[0]*exp(-p[1]*i) + p[2], i=0...n-1 */

int CurveFitting::expfunc(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i;
    struct vars_struct *v = (struct vars_struct *) vars;
    double *x, *y, *ey, f;
    x = v->x;
    y = v->y;
    ey = v->ey;

    for (i=0; i<m; i++) {
        f = p[0]*exp(-p[1]*i) + p[2];     /* Exponential fit function; note f = p[0]*exp(-p[1]*i)+p[2] */
        dy[i] = (y[i] - f)/ey[i];
    }
    return 0;
}
//------------------------------------------------------------------------------
/*
 * gaussian fit function
 *
 * m - number of data points
 * n - number of parameters (4)
 * p - array of fit parameters
 *     p[0] = constant offset
 *     p[1] = peak y value
 *     p[2] = x centroid position
 *     p[3] = gaussian sigma width
 * dy - array of residuals to be returned
 * vars - private data (struct vars_struct *)
 *
 * RETURNS: error code (0 = success)
 */
int CurveFitting::gaussfunc(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i;
    struct vars_struct *v = (struct vars_struct *) vars;
    double *x, *y, *ey;
    double xc, sig2;

    x = v->x;
    y = v->y;
    ey = v->ey;
    sig2 = p[3]*p[3];

    for (i=0; i<m; i++) {
        xc = x[i]-p[2];
        dy[i] = (y[i] - p[1]*exp(-0.5*xc*xc/sig2) - p[0])/ey[i];
    }
    return 0;
}
//------------------------------------------------------------------------------
/* model to be fitted to measurements: x_i = p[0]*(1-exp(-TR/p[1]))-2*exp(-x[i]/p[1]), i=0...n-1 */
int T1func_2P(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i;
    struct T1_IR_2P_vars_struct *v = (struct T1_IR_2P_vars_struct *) vars;
    double *x, *y, *ey, f;
    x = v->TI;
    y = v->y;
    ey = v->ey;
    double  TR = v->TR;

    for (i=0; i<m; i++) {
        f = p[0]* ((1 + exp(-TR/p[1]) - 2 * exp(-x[i]/p[1])));
        if(f<0)
            f = -f;
        dy[i] = (y[i] - f)/ey[i];
    }
    return 0;
}
//------------------------------------------------------------------------------
/* model to be fitted to measurements: x_i = p[0]*(1-exp(-TR/p[1]))-2*exp(-x[i]/p[1]), i=0...n-1 */
int T1func_3P(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i;
    struct vars_struct *v = (struct vars_struct *) vars;
    double *x, *y, *ey, f;

    x = v->x;
    y = v->y;
    ey = v->ey;

    double  TR = 900;
    for (i=0; i<m; i++) {
        f = p[0]*(1-p[2]*exp(-x[i]/p[1]));
        if(f<0)
            f = -f;
        dy[i] = (y[i] - f)/ey[i];
    }
    return 0;
}
//------------------------------------------------------------------------------
int T2func(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i;
    struct vars_struct *v = (struct vars_struct *) vars;
    double *x, *y, *ey, f;

    x = v->x;
    y = v->y;
    ey = v->ey;

    for (i=0; i<m; i++) {
        f = p[0]*exp(-x[i]/p[1]);     /* Exponential fit function; note yi = p[0]*exp(-x[i]/p[1]) */
        dy[i] = (y[i] - f)/ey[i];
    }
    return 0;
}
//------------------------------------------------------------------------------
int LookLocker(int m, int n, double *p, double *dy, double **dvec, void *vars)
{
    int i, N;
    struct vars_struct *v = (struct vars_struct *) vars;
    double *x, *y,*ey,alfa,Etau,Er,Ed,tau,tr,td,Q,F,Meq,Etl, FA;
    double S[200];

    x = v->x;
    y = v->y;
    ey = v->ey;

//    tau = 6.2; td = 4.2; tr = 2578;
//    Etl = 8; N = m * Etl;
//    double PI = 3.141592653589793;
//    FA=7.0;

    tau = v->tau; td = v->td; tr = v->tr;
    Etl = v->Etl; N = m * Etl;
    double PI = 3.141592653589793;
    FA=v->FA;

    alfa = FA*PI/180.0;
    // (n = 1,2, . . . N)
    // printf("     %f %f\n", p[0], p[1]);
    Etau = exp(-tau/p[1]);
    Er = exp(-tr/p[1]);
    Ed = exp(-td/p[1]);
    F = (1-Etau)/(1-cos(alfa)*Etau);
    Q = (-F*cos(alfa)*Er*Ed*(1-pow(cos(alfa)*Etau,(N-1)))-2*Ed+Er+1)/(1+cos(alfa)*Er*Ed*pow(cos(alfa)*Etau,(N-1)));
    for (n=0; n<m; n++)
    {
        S[n] = p[0]*(F+pow(cos(alfa)*Etau,n*Etl-1)*(Q-F));
        if (S[n]<0.0)
            S[n] = -S[n];
        dy[n] = (y[n] - S[n]);
    }
    return 0;
}
//------------------------------------------------------------------------------
int CurveFitting::execute()
{    
    clearOutputData();
    outData.clear();

    QString arg1, arg2;
    if(nodeItem->containsProperty("xData"))
    {
        arg1 = nodeItem->propertyValue("xData").toString();
    }
    if(nodeItem->containsProperty("parValue"))
    {
        arg2 = nodeItem->propertyValue("parValue").toString();
    }

    if(0)   // load the xData from a text file
    {
        QFile file(arg1);
        file.open(QIODevice::ReadOnly);

        QTextStream in(&file);
        QString line1;
        QString line, s1;
        int count1=0;
        while(!in.atEnd())
        {
            line1 = in.readLine();
            if(count1==0)
                line.append(line1);
            else
                line.append(","+line1);
            count1++;
        }
        int arr[200]; //int arr[line.length()];
        QRegExp rx("\\,");    // find the string between comma
        int count = 0;
        int pos = 0, newpos = 0;
        while(newpos = rx.indexIn(line, pos)!=-1)
        {
            newpos = rx.indexIn(line, pos);
            s1 = line.mid(pos, newpos-pos);
            arr[count] =s1.toInt() ;
            pos = newpos + 1;
            count++;
        }
        s1 = line.mid(pos, line.length()-1);
        arr[count] = s1.toInt() ;
    }


    QString CurveFittingtype = nodeItem->propertyValue("CurveFittingType").toString();

    if(CurveFittingtype=="T1-IR")     // Inversion recovery of 3D volumes (like in DGEMRIC)
    {
        int i;
        int Npts = im->array_size[3];

        if(Npts <=0)
        {
            qDebug() << "ERROR: Incorrect number of dimensions";
            return -1;
        }

        int parvalue;
        double* x = new double[Npts];
        double* y = new double[Npts];
        double* ey = new double[Npts];

        double TR;

        readArgDataArray<double>(x, Npts, "Arg0");
        readArgDataValue<double>(TR, "Arg1");
        readArgDataValue<int>(parvalue, "Arg2");
//        for(i=0; i<Npts; i++)qDebug() << "in execute, Arg0=x, i="<<i<<",  x="<<x[i];
//        qDebug() << "in execute, Arg1=TR="<<TR;
//        qDebug() << "in execute, Arg2=parvalue="<<parvalue;

        for(i=0; i<Npts; i++)
        {
            y[i] = 0.0;
            ey[i] = 0.7;   /* Data errors */
        }

        struct vars_struct v;
        int status;
        v.x = x;
        v.y = y;
        v.ey = ey;
        double perror[2];                   /* Returned parameter errors */

        mp_result result;

        if(parvalue==2)
            double p[2] = {0.0};
        else if(parvalue==3)
            double p[3] = {0.0};

        memset(&result,0,sizeof(result));       /* Zero results structure */
        result.xerror = perror;

        if(im->NumDimensions == 4)
        {
            im1 = new ImageC(im, 3);
            im2 = new ImageC(im, 3);
            im3 = new ImageC(im, 3);

            float clip_T1 = 10000;
            float clip_PD = 8000;
            double meanTI =  0.0;

            for(int tt=0; tt<Npts; tt++)  meanTI += v.x[tt];
            if (meanTI)  meanTI /= Npts;

#pragma omp parallel for schedule(static),shared(x,clip_PD,clip_T1) num_threads(REQUESTED_NUM_THREADS)
            for(int kk=0; kk<im->array_size[2]; kk++) //kk<data50.depth()
            {
                //This is the top level for loop to be parallelized by OpenMP
                double y[7] = {0.0};
                double ey[7];
                struct vars_struct v;
                struct T1_IR_2P_vars_struct v1;
                int status;

                v.x = x;
                v.y = y;
                v.ey = ey;
                v1.TR = TR; //1700;
                v1.TI = x;
                v1.y = y;
                v1.ey = ey;
                double perror[2];                   /* Returned parameter errors */
                mp_result result;
                double p[3] = {0.0};
                memset(&result,0,sizeof(result));       /* Zero results structure */
                result.xerror = perror;

                for (int i=0; i<Npts; i++)
                    ey[i] = 0.7;   /* Data errors */

                for(int jj=0; jj<im->array_size[1]; jj++) //im->array_size[1]
                    for(int ii=0; ii<im->array_size[0]; ii++)//im->array_size[0]
                        if(im->getVoxel(ii,jj,kk,0)>=50.0)
                        {
                            for(int tt=0; tt<Npts; tt++) y[tt] = im->getVoxel(ii,jj,kk,tt);

                            double pnew[3];
                            if(parvalue == 3)
                            {
                                pnew[0] = y[0]; pnew[1] = meanTI; pnew[2] = 2.0;
                                status = mpfit(T1func_3P, Npts, 3 , pnew, 0, 0, (void *) &v, &result);
                            }
                            else if(parvalue == 2)
                            {
                                pnew[0] = y[0]; pnew[1] = meanTI;
                                status = mpfit(T1func_2P, Npts, 2, pnew, 0, 0, (void *) &v1, &result);
                            }

                            if((pnew[0]<clip_PD) && (pnew[0]>0))
                                im1->setVoxel(pnew[0], ii,jj,kk);
                            else if(pnew[0]>clip_PD)
                                im1->setVoxel(clip_PD, ii,jj,kk);
                            if((pnew[1]<clip_T1) && (pnew[1]>0))
                                im2->setVoxel(pnew[1], ii,jj,kk);
                            else if(pnew[1]>clip_T1)
                                im2->setVoxel(clip_T1, ii,jj,kk);
                        }
            }

        }       // if(im->NumDimensions == 4)
        outData.clear();
        QVariant* qv1 = new QVariant(); qv1->setValue(im1); outData.push_back(qv1);
        QVariant* qv2 = new QVariant(); qv2->setValue(im2); outData.push_back(qv2);
        QVariant* qv3 = new QVariant(); qv3->setValue(im3); outData.push_back(qv3);

        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
        setReadyOutPorts(8 + getNi().NumInputs + 1, true);
        setReadyOutPorts(8 + getNi().NumInputs + 2, true);

    }
    else if(CurveFittingtype=="T2")
    {
        float clip_T2 = 500;
        if(im->NumDimensions == 4)
        {
            int i;
            int Npts = im->array_size[3];

            if(Npts <=0)
            {
                qDebug() << "ERROR: Incorrect number of dimensions";
                return -1;
            }

            int parvalue;
            double* x = new double[Npts];
            double* y = new double[Npts];
            double* ey = new double[Npts];

            readArgDataArray<double>(x, Npts, "Arg0");
//            readArgDataValue<int>(parvalue, "Arg1");
//            for(i=0; i<Npts; i++) qDebug() << "in execute, i="<<i<<",  x="<<x[i];
//            qDebug() << "in execute, Arg1=parvalue="<<parvalue;

            struct vars_struct v;
            int status;
            mp_result result;

            for(i=0; i<Npts; i++)
            {
                y[i] = 0.0;
                ey[i] = 0.7;   /* Data errors */
            }

            /* Parameter initial conditions */
            memset(&result,0,sizeof(result));       /* Zero results structure */

            v.x = x;
            v.y = y;
            v.ey = ey;
            im1 = new ImageC(im, 3);
            im2 = new ImageC(im, 3);
            for(int k=0; k<im->array_size[2]; k++)
            {
                for(int j=0; j<im->array_size[1]; j++)
                    for(int i=0; i<im->array_size[0]; i++)
                    {
                        if (im->getVoxel(i,j,k,0)!=0.0)
                        {
                            double p[2] = {1500.0, 50.0};
                            y[0] = im->getVoxel(i,j,k,0);
                            y[1] = im->getVoxel(i,j,k,1);
                            status = mpfit(T2func, 2, 2, p, 0, 0, (void *) &v, &result);
                            im1->setVoxel(p[0], i,j,k);
                            im2->setVoxel(p[1], i,j,k);
                        }
                    }
            }
        } // multi-echo 3D
        else if(im->NumDimensions == 3)   // multi-echo 2D
        {
            int i;
            int Npts = im->array_size[2];

            if(Npts <=0)
            {
                qDebug() << "ERROR: Incorrect number of dimensions";
                return -1;
            }

            int parvalue;
            double* x = new double[Npts];
            double* y = new double[Npts];
            double* ey = new double[Npts];

            readXDataValue<double>(x, Npts, parvalue);

            struct vars_struct v;
            int status;
            mp_result result;

            for(i=0; i<Npts; i++)
            {
                y[i] = 0.0;
                ey[i] = 0.7;   /* Data errors */
            }

            /* Parameter initial conditions */
            memset(&result,0,sizeof(result));       /* Zero results structure */

            v.x = x;
            v.y = y;
            v.ey = ey;
            im1 = new ImageC(im, 2);
            im2 = new ImageC(im, 2);

            for(int j=0; j<im->array_size[1]; j++)
            {
                for(int i=0; i<im->array_size[0]; i++)
                {
                    if (im->getVoxel(i,j,0)!=0.0)
                    {
                        double p[2] = {1500.0, 50.0};
                        y[0] = im->getVoxel(i,j,0);
                        y[1] = im->getVoxel(i,j,1);
                        status = mpfit(T2func, 2, 2, p, 0, 0, (void *) &v, &result);
                        im1->setVoxel(p[0], i,j);
                        im2->setVoxel(p[1], i,j);
                    }
                }
            }
        }

        // clip T2 values
        for(int ii = 0; ii<im1->data.size(); ii++)
        {
            if(im1->data.at(ii) < 0)
            {
                im1->data.at(ii) = 0.0;
                im2->data.at(ii) = 0.0;
            }
            else
            {
                if(im2->data.at(ii) < 0)
                    im2->data.at(ii) = 0.0;

                if( im2->data.at(ii) > clip_T2 )
                    im2->data.at(ii) = clip_T2;
            }
        }

        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
        setReadyOutPorts(8 + getNi().NumInputs + 1, true);

        outData.clear();
        QVariant* qv1 = new QVariant(); qv1->setValue(im1); outData.push_back(qv1);
        QVariant* qv2 = new QVariant(); qv2->setValue(im2); outData.push_back(qv2);

    }
    else if(CurveFittingtype=="T1-LookLocker")
    {

        int Npts = im->array_size[2];
        double* x = new double[Npts];
        double* y = new double[Npts];
        double* ey = new double[Npts];
        int i;
        struct vars_struct v;
        int status;
        mp_result result;

        readArgDataValue<double>(v.tau, "Arg0");
        readArgDataValue<double>(v.td,  "Arg1");
        readArgDataValue<double>(v.tr,  "Arg2");
        readArgDataValue<double>(v.Etl, "Arg3");
        readArgDataValue<double>(v.FA,  "Arg4");
        readArgDataValue<double>(v.min_1,  "Arg6");
        readArgDataValue<double>(v.max_1,  "Arg7");
        readArgDataValue<double>(v.min_2,  "Arg8");
        readArgDataValue<double>(v.max_2,  "Arg9");

        for (i=0; i<Npts; i++) ey[i] = 0.07;   /* Data errors */

        v.x = x;
        v.y = y;
        v.ey = ey;
        im1 = new ImageC(im, 2);
        im2 = new ImageC(im, 2);

        float clip_T1_min = v.min_1;
        float clip_T1_max = v.max_1;//10000.0;
        float clip_PD_min = v.min_2;
        float clip_PD_max = v.max_1;//8000.0;;  // very large number, no max clip

        for(int j=0; j<im1->array_size[1]; j++) //data50.height()
        {
            for(int i=0; i<im1->array_size[0]; i++)//data50.width()
            {
                if(im->getVoxel(i,j,0)!=0)
                {
                    for(int ii=0; ii<Npts; ii++)
                        y[ii] = im->getVoxel(i,j,ii);
                    double p[2] = {y[0], 1000};

                    status = mpfit(LookLocker, Npts, 2, p, 0, 0, (void *) &v, &result);

                    // clip T1
                    if(p[1]>clip_T1_max)
                        im2->setVoxel(clip_T1_max, i,j);
                    else if(p[1]<clip_T1_min)
                        im2->setVoxel(clip_T1_min, i,j);
                    else
                        im2->setVoxel(p[1], i,j);

                    // clip PD
                    if(p[0]>clip_PD_max)
                        im1->setVoxel(clip_PD_max, i,j);
                    else if(p[1]<clip_PD_min)
                        im1->setVoxel(clip_PD_min, i,j);
                    else
                        im1->setVoxel(p[0], i,j);
                }
            }
        }
        setReadyOutPorts(8 + getNi().NumInputs + 0, true);
        setReadyOutPorts(8 + getNi().NumInputs + 1, true);

        delete [] x;
        delete [] y;
        delete [] ey;

        outData.clear();
        QVariant* qv1 = new QVariant(); qv1->setValue(im1); outData.push_back(qv1);
        QVariant* qv2 = new QVariant(); qv2->setValue(im2); outData.push_back(qv2);
    }
    else
    {
    }

    markExecuted();
    return 0;
}
//--------------------------------------------------------------------------------------------------
int CurveFitting::validateDesign()
{
    QString CurveFittingtype = nodeItem->propertyValue("CurveFittingType").toString();
    QString xData = nodeItem->propertyValue("xData").toString();
    //    if (CurveFittingtype=="T1-IR" & CurveFittingtype=="T2" & xData=="")
    if (CurveFittingtype=="T1-IR" && xData=="")
    {
        qDebug() << "CurveFitting:: T1 fitting requires nonempyu xData";
        return -1;
    }

    if (CurveFittingtype=="T2" && xData=="")
    {
        qDebug() << "CurveFitting:: T2 fitting requires nonempyu xData";
        return -1;
    }

    int parValue = nodeItem->propertyValue("parValue").toInt();
    if (parValue < 2 || parValue > 3)
    {
        qDebug() << "CurveFitting:: parValue must have the value 2 or 3";
        return -1;
    }

    // case of maximum 2 outputs
    if(CurveFittingtype=="T1-IR" || CurveFittingtype=="T2"|| CurveFittingtype=="T1-LookLocker")
        if( (getConnectedOutPorts().contains(2+8+getNi().NumInputs)) || (getConnectedOutPorts().length()>2))
        {
            qDebug() << "CurveFitting:: expecting at most 2 output connections at ports 1 and 2";
            return -1;
        }

    return 0;
}
//--------------------------------------------------------------------------------------------------
int CurveFitting::validateRun()
{
    if (im->NumDimensions > 4 || im->NumDimensions  < 2)
        return -1;

    return 0;
}
//--------------------------------------------------------------------------------------------------
void CurveFitting::clearNodeData()
{
    QString CurveFittingtype = nodeItem->propertyValue("CurveFittingType").toString();

    if(CurveFittingtype=="T1-IR" || CurveFittingtype=="T2"|| CurveFittingtype=="T1-LookLocker" )
        if(im1)
        {
            im1->clear();
        }

    if(CurveFittingtype=="T1-IR" || CurveFittingtype=="T2"|| CurveFittingtype=="T1-LookLocker")
        if(im2)
        {
            im2->clear();
        }
}
//--------------------------------------------------------------------------------------------------
CurveFitting* CurveFitting::copy() const
{
    return new CurveFitting(*this);
}
//--------------------------------------------------------------------------------------------------
QString CurveFitting::uniqueKey() const
{
    return "CurveFitting";
}
//--------------------------------------------------------------------------------------------------
void CurveFitting::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool CurveFitting::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void CurveFitting::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------
template <class T>
void CurveFitting::readXDataValue(T x[], int Npts, int &parvalue)
{
    QStringList xDattaList;
    QString xDattaValues;

    parvalue =  nodeItem->propertyValue("parValue").toString().toInt();

    xDattaValues = nodeItem->propertyValue("xData").toString();
    xDattaList = xDattaValues.split(',');

    for(int ii=0; ii< Npts; ii++)
    {
        x[ii] = xDattaList[ii].toDouble();
    }
}
//--------------------------------------------------------------------------------------------------
template <class T>
void CurveFitting::readXDataValue_1(T x[], int Npts, int &parvalue)
{
    QString xDattaValues = nodeItem->propertyValue("Arg0").toString();
    parvalue =  nodeItem->propertyValue("Arg2").toString().toInt();
    QStringList xDattaList = xDattaValues.split(',');
    for(int ii=0; ii< Npts; ii++) x[ii] = xDattaList[ii].toDouble();
}
//--------------------------------------------------------------------------------------------------
template <class T>
void CurveFitting::readArgDataArray(T x[], int Npts, QString arg)
{
//    qDebug() << "in readArgDataArray, Arg0="<<nodeItem->propertyValue("Arg0").toString();
//    qDebug() << "in readArgDataArray, Arg1="<<nodeItem->propertyValue("Arg1").toString();
//    qDebug() << "in readArgDataArray, Arg2="<<nodeItem->propertyValue("Arg2").toString();
//    qDebug() << "in readArgDataArray, Npts="<<Npts;
//    qDebug() << "in readArgDataArray, arg="<<arg;

    if (Npts > 1)
    {
        QString xDattaValues = nodeItem->propertyValue(arg).toString();
        QStringList xDattaList = xDattaValues.split(',');
        for(int ii=0; ii< Npts; ii++) x[ii] = (T)(xDattaList[ii].toDouble());
    }
    else
    {
        qDebug() << "Error: in readArgDataArray, Npst should > 1 for T1-IR.";
        return;
    }

//    for(int i=0; i<Npts; i++)
//        qDebug() << "in readArgDataArray, Arg0=x, i="<<i<<",  x="<<x[i];
}
//--------------------------------------------------------------------------------------------------
template <class T>
void CurveFitting::readArgDataValue(T &x, QString arg)
{
    x = nodeItem->propertyValue(arg).toDouble();
}
//--------------------------------------------------------------------------------------------------

