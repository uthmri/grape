/* datatypes.h
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

#ifndef DATATYPES
#define DATATYPES

#include <QMetaType>
#include <QDomDocument>
#include <qmap.h>
//#include "xmlreclib.h"
#include <QStringList>

using std::vector;

//------------------------------------------------------------------------------------
class SeriesClass
{
public:
    SeriesClass();

    QString Patient_Name;
    QString Examination_Name;
    QString Protocol_Name;
    QString Examination_Date;
    QString Examination_Time;
    QString Series_Data_Type;
    int Aquisition_Number;
    int Reconstruction_Number;
    double Scan_Duration;

    int Max_No_Phases;
    int Max_No_Echoes;
    int Max_No_Slices;
    int Max_No_Dynamics;
    int Max_No_Mixes;
    int Max_No_B_Values;
    int Max_No_Gradient_Orients;
    int No_Label_Types;
    int Max_No_Types;

    QStringList EnumSequence;
    QStringList EnumImageType;
    QStringList EnumLabelType;

    QString Patient_Position;
    QString Preparation_Direction;
    QString Technique;
    int Scan_Resolution_X;
    int Scan_Resolution_Y;
    QString Scan_Mode;
    double Repetition_Times[10];
    double FOV_AP;
    double FOV_FH;
    double FOV_RL;
    double Water_Fat_Shift;
    double Angulation_AP;
    double Angulation_FH;
    double Angulation_RL;
    double Off_Center_AP;
    double Off_Center_FH;
    double Off_Center_RL;
    QString Flow_Compensation;
    QString Presaturation;
    double Phase_Encoding_Velocity[3];
    QString MTC;
    QString SPIR;
    int EPI_factor;
    QString Dynamic_Scan;
    QString Diffusion;
    double Diffusion_Echo_Time;
    QString PhotometricInterpretation;
};
//------------------------------------------------------------------------------------
class ImKeyClass
{
public:
    ImKeyClass();

    int Slice;
    int Echo;
    int Dynamic;
    int Phase;
    int BValue;
    int Grad_Orient;
    QString Label_Type;
    QString Type;
    QString Sequence;
    int Index;
};
//------------------------------------------------------------------------------------
class ImInfoClass
{
public:
    ImInfoClass();

    int SliceIndex;  // how slices are sorted in XML/REC files
    ImKeyClass Key;

    int Pixel_Size;
    double Scan_Percentage;
    int Resolution_X;
    int Resolution_Y;
    double Rescale_Intercept;
    double Rescale_Slope;
    double Scale_Slope;
    double Window_Center;
    double Window_Width;
    double Slice_Thickness;
    double Slice_Gap;
    QString Display_Orientation;
    int fMRI_Status_Indication;
    QString Image_Type_Ed_Es;
    double Pixel_Spacing[2];
    double Echo_Time;
    double Dyn_Scan_Begin_Time;
    double Trigger_Time;
    double  Diffusion_B_Factor;
    double  No_Averages;
    double  Image_Flip_Angle;
    int Cardiac_Frequency;
    int Min_RR_Interval;
    int Max_RR_Interval;
    int TURBO_Factor;
    double Inversion_Delay;
    QString Contrast_Type;
    QString Diffusion_Anisotropy_Type;
    double Diffusion_AP;
    double  Diffusion_FH;
    double  Diffusion_RL;
    double  Angulation_AP;
    double  Angulation_FH;
    double  Angulation_RL;
    double  Offcenter_AP;
    double  Offcenter_FH;
    double  Offcenter_RL;
    QString Slice_Orientation;
    int Image_Planar_Configuration;
    int Samples_Per_Pixel;
};
//------------------------------------------------------------------------------------
class ImageC
{
public:
    int NumDimensions;
    int array_size[8];
    float voxel_size[4]; // [pixel_spacing_1,pixel_spacing_2,slice_thickness,slice_gap]
    QString BitDepth;
    short int datatype=0;
    short int bitpix=0;

    vector<float> data;

    SeriesClass series_info;
    vector<ImInfoClass> image_info;
    QDomNodeList series_info_list;
    QDomNodeList ImageArray;
    QMap<QString, int> ImageIndex;

public:
    ImageC();
    ~ImageC();
    ImageC(const ImageC &im) ;
    ImageC(ImageC* im);
    ImageC(ImageC* im,  int nd);
    ImageC(ImageC* im, bool CopyDataFlag);

    void init();
    void copyAttributes(const ImageC* im);

public:
    float getVoxel(int x);
    float getVoxel(int x, int y);
    float getVoxel(int x, int y, int z);
    float getVoxel(int x,int y, int z, int w);

    void setVoxel(float v, int x);
    void setVoxel(float v, int x, int y);
    void setVoxel(float v, int x, int y, int z);
    void setVoxel(float v, int x, int y, int z, int w);

    int numDimensions() const;
    void setNumDimensions(int value);

    int arraySize(unsigned int dim) const;
    void setArraySize(unsigned int dim, unsigned int sz);

    float voxelSize(unsigned int dim) const;
    void setVoxelSize(unsigned int dim, float sz);

    unsigned int numElements() const;

    void clear();
};

//------------------------------------------------------------------------------------

Q_DECLARE_METATYPE(ImageC);
Q_DECLARE_METATYPE(ImageC*);


#endif // DATATYPES
