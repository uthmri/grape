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

#include <datatypes.h>
#include <QDebug>
//------------------------------------------------------------------------------------
SeriesClass::SeriesClass() // Here you DEFINE your constructor
{
    int i;

    Patient_Name = "-";
    Examination_Name = "-";
    Protocol_Name = "-";
    Examination_Date = "-";
    Examination_Time = "-";
    Series_Data_Type = "-";
    Aquisition_Number = 0;
    Reconstruction_Number = 0;
    Scan_Duration = 0.0;

    Max_No_Phases = 1;
    Max_No_Echoes = 1;
    Max_No_Slices = 1;
    Max_No_Dynamics = 1;
    Max_No_Mixes = 1;
    Max_No_B_Values = 1;
    Max_No_Gradient_Orients = 1;
    No_Label_Types = 1;
    Max_No_Types = 1;

    Patient_Position = "HFS";
    Preparation_Direction = "AP";
    Technique = "TSE";
    Scan_Resolution_X = 1;
    Scan_Resolution_Y = 1;
    Scan_Mode = "3D";
    for(i=0; i<10; i++) Repetition_Times[i] = -1.0;
    FOV_AP = 0.0;
    FOV_FH = 0.0;
    FOV_RL = 0.0;
    Water_Fat_Shift = 0.0;
    Angulation_AP = 0.0;
    Angulation_FH = 0.0;
    Angulation_RL = 0.0;
    Off_Center_AP = 0.0;
    Off_Center_FH = 0.0;
    Off_Center_RL = 0.0;
    Flow_Compensation = "N";
    Presaturation = "N";
    for(i=0; i<3; i++)  Phase_Encoding_Velocity[i] = 0.0;
    MTC = "N";
    SPIR = "N";
    EPI_factor = 1;
    Dynamic_Scan = "N";
    Diffusion = "N";
    Diffusion_Echo_Time = 0.0;
    PhotometricInterpretation = "N";
}
//------------------------------------------------------------------------------------
ImKeyClass::ImKeyClass() // Here you DEFINE your constructor
{
    Slice = 0;
    Echo = 0;
    Dynamic = 0;
    Phase = 0;
    BValue = 0;
    Grad_Orient = 0;
    Label_Type = "-";
    Type = "M";
    Sequence = "-";
    Index = 0;
}
//------------------------------------------------------------------------------------
ImInfoClass::ImInfoClass() // Here you DEFINE your constructor
{
    Pixel_Size = 16;
    Scan_Percentage = 100;
    Resolution_X = 1;
    Resolution_Y = 1;
    Rescale_Intercept = 0;
    Rescale_Slope = 1;
    Scale_Slope = 1;
    Window_Center = 0;
    Window_Width = 0;
    Slice_Thickness = 1;
    Slice_Gap = 0;
    Display_Orientation = "NONE";
    fMRI_Status_Indication = 0;
    Image_Type_Ed_Es = "U";
    Pixel_Spacing[0] = 1;   Pixel_Spacing[1] = 1;
    Echo_Time = 0;
    Dyn_Scan_Begin_Time = 0;
    Trigger_Time = 0;
    Diffusion_B_Factor = 0;
    No_Averages = 1;
    Image_Flip_Angle = 0;
    Cardiac_Frequency = 1;
    Min_RR_Interval = 1;
    Max_RR_Interval = 1;
    TURBO_Factor = 1;
    Inversion_Delay = 0;
    Contrast_Type = "T2";
    Diffusion_Anisotropy_Type = "-";
    Diffusion_AP = 0;
    Diffusion_FH = 0;
    Diffusion_RL = 0;
    Angulation_AP = 0;
    Angulation_FH = 0;
    Angulation_RL = 0;
    Offcenter_AP = 0;
    Offcenter_FH = 0;
    Offcenter_RL = 0;
    Slice_Orientation = "Transversal";
    Image_Planar_Configuration = 0;
    Samples_Per_Pixel = 1;
}
//------------------------------------------------------------------------------------
ImageC::ImageC()
{
    init();
}
//------------------------------------------------------------------------------------
ImageC::~ImageC()
{
    init();
}
//------------------------------------------------------------------------------------
ImageC::ImageC(ImageC* im)
{
    init();
    copyAttributes(im);
    data.resize(im->data.size());
}
//------------------------------------------------------------------------------------
ImageC::ImageC(ImageC* im,  int nd)
{
    init();
    copyAttributes(im);
    NumDimensions = nd;
    for(int dd = nd; dd < 8; dd++)
        array_size[dd] = 1;

    int NumElements = 1;
    for(int dd = 0; dd < nd; dd++)
        NumElements *= array_size[dd];

    data.resize(NumElements);
}
//------------------------------------------------------------------------------------
ImageC::ImageC(ImageC* im, bool CopyDataFlag)
{
    init();
    copyAttributes(im);
    data.resize(im->data.size());
    if(CopyDataFlag)
        std::copy(im->data.begin(),im->data.end(),data.begin());
}
//------------------------------------------------------------------------------------
void ImageC::init()
{
    int i;
    NumDimensions = 0;
    BitDepth = "16";
    for(i=0; i<8; i++)            array_size[i] = 1;
    for(i=0; i<4; i++)            voxel_size[i] = 1;
    data.clear();
}
//------------------------------------------------------------------------------------
void ImageC::copyAttributes(const ImageC* im)
{
    int i;
    NumDimensions = im->NumDimensions;
    BitDepth = im->BitDepth;
    for(i=0; i<8; i++) array_size[i] = im->array_size[i];
    for(i=0; i<4; i++) voxel_size[i] = im->voxel_size[i];

    int NumElements = 1;
    for(int dd = 0; dd < im->NumDimensions; dd++)
    {
        array_size[dd] = im->array_size[dd];
        NumElements *= array_size[dd];
    }

    series_info = im->series_info;
    image_info = im->image_info;
    series_info_list = im->series_info_list;
    ImageArray = im->ImageArray;
    ImageIndex = im->ImageIndex;
}
//------------------------------------------------------------------------------------
void ImageC::clear()
{
    if(data.size())
        data.clear();

    init();
}
//------------------------------------------------------------------------------------
float ImageC::getVoxel(int x) { return data.at(x); }
//------------------------------------------------------------------------------------
float ImageC::getVoxel(int x, int y) { return data.at(x+y*array_size[0]); }
//------------------------------------------------------------------------------------
float ImageC::getVoxel(int x, int y, int z) { return data.at(x+y*array_size[0]+z*array_size[0]*array_size[1]); }
//------------------------------------------------------------------------------------
float ImageC::getVoxel(int x,int y, int z, int w) { return data.at(x+y*array_size[0]+z*array_size[0]*array_size[1]+w*array_size[0]*array_size[1]*array_size[2]); }
//------------------------------------------------------------------------------------
void ImageC::setVoxel(float v, int x) { data.at(x) = v; }
//------------------------------------------------------------------------------------
void ImageC::setVoxel(float v, int x, int y) { data.at(x+y*array_size[0]) = v; }
//------------------------------------------------------------------------------------
void ImageC::setVoxel(float v, int x, int y, int z) { data.at(x+y*array_size[0]+z*array_size[0]*array_size[1]) = v; }
//------------------------------------------------------------------------------------
void ImageC::setVoxel(float v, int x, int y, int z, int w){ data.at(x+y*array_size[0]+z*array_size[0]*array_size[1]+w*array_size[0]*array_size[1]*array_size[2]) = v; }
//------------------------------------------------------------------------------------
int ImageC::numDimensions() const {return NumDimensions;}
//------------------------------------------------------------------------------------
void ImageC::setNumDimensions(int value) {NumDimensions = value;}
//------------------------------------------------------------------------------------
int ImageC::arraySize(unsigned int dim) const
{
    return array_size[dim];
}
//------------------------------------------------------------------------------------
void ImageC::setArraySize(unsigned int dim, unsigned int sz)
{
    array_size[dim] = sz;
}
//------------------------------------------------------------------------------------
float ImageC::voxelSize(unsigned int dim) const
{
    return voxel_size[dim];
}
//------------------------------------------------------------------------------------
void ImageC::setVoxelSize(unsigned int dim, float sz)
{
    voxel_size[dim] = sz;
}
//------------------------------------------------------------------------------------
unsigned int ImageC::numElements() const
{
    if(NumDimensions < 1)
        return 0;

    unsigned int n = 1;
    for(int d = 0; d<NumDimensions; d++)
        n *= array_size[d];

    return n;
}
//------------------------------------------------------------------------------------

