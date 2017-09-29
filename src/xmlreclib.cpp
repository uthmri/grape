/* xmlreclib.cpp
 *
 Copyright (C) 2016 UTHealth MRI Research
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

#include <xmlreclib.h>

#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>
#include <QImage>
#include <QDomDocument>
#include "stdint.h"
#include <ImageWriterItem.h>
#include <NodeItem.h>
#include <QHash>

//--------------------------------------------------------------------------------------------------
XMLRecReader::XMLRecReader(QObject *parent)
{
}
//--------------------------------------------------------------------------------------------------
XMLRecReader::~XMLRecReader() { }
//--------------------------------------------------------------------------------------------------
void XMLRecReader::init()
{
}
//--------------------------------------------------------------------------------------------------
XMLRecReader* XMLRecReader::copy() const
{
    return new XMLRecReader(*this);
}
//--------------------------------------------------------------------------------------------------
QString XMLRecReader::uniqueKey() const
{
    return "XMLRecReader";
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::markDirty()
{
    //	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
bool XMLRecReader::isSuperfluous() const
{
    return false;
    //return (DiagramOvalItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::readImageKey(QDomNodeList ImKey_attr_list, ImKeyClass &imageKey)
{
    for(int kk = 0; kk < ImKey_attr_list.length(); kk++)
    {
        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Slice")
            imageKey.Slice =  ImKey_attr_list.at(kk).firstChild().nodeValue().toInt();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Echo")
            imageKey.Echo =  ImKey_attr_list.at(kk).firstChild().nodeValue().toInt();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Dynamic")
            imageKey.Dynamic =  ImKey_attr_list.at(kk).firstChild().nodeValue().toInt();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Phase")
            imageKey.Phase =  ImKey_attr_list.at(kk).firstChild().nodeValue().toInt();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "BValue")
            imageKey.BValue =  ImKey_attr_list.at(kk).firstChild().nodeValue().toInt();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Grad Orient")
            imageKey.Grad_Orient =  ImKey_attr_list.at(kk).firstChild().nodeValue().toInt();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Label Type")
            imageKey.Label_Type =  ImKey_attr_list.at(kk).firstChild().nodeValue();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Type")
            imageKey.Type =  ImKey_attr_list.at(kk).firstChild().nodeValue();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Sequence")
            imageKey.Sequence =  ImKey_attr_list.at(kk).firstChild().nodeValue();

        if( ImKey_attr_list.at(kk).toElement().attribute("Name") == "Index")
            imageKey.Index =  ImKey_attr_list.at(kk).firstChild().nodeValue().toInt();
    }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::readSeriesInfo(QDomNodeList series_info_fields_list, QVector<QVector<QString> > &series_attributes, SeriesClass &SeriesInfo)
{
    //QDomNodeList series_info_fields_list  = series_info_list.at(0).toElement().elementsByTagName("Attribute");
    //    QDomNodeList series_info_fields_list  = series_info_list.at(0).toElement().childNodes();

    double a=0.0, b=0.0, c=0.0;

    if(series_info_fields_list.length()== 0)
    {
        qDebug() << "Could not find Series_Info";
        return;
    }

    //    qDebug() << "XML/REC Series found " << series_info_fields_list.length() << " items";

    int ArrSize;

    for(int k =0; k < series_info_fields_list.count(); k++)
    {
        ArrSize = 1;
        series_attributes[k][0] = series_info_fields_list.at(k).toElement().attribute("Name");
        series_attributes[k][1] = series_info_fields_list.at(k).toElement().attribute("Tag");
        series_attributes[k][2] = series_info_fields_list.at(k).toElement().attribute("Level");
        series_attributes[k][3] = series_info_fields_list.at(k).toElement().attribute("Type");
        series_attributes[k][4] = series_info_fields_list.at(k).firstChild().nodeValue();

        if(series_info_fields_list.at(k).toElement().hasAttribute("ArraySize"))
            ArrSize = series_info_fields_list.at(k).toElement().attribute("ArraySize").toInt();

        if( series_attributes[k][0] == "Max No Phases")    SeriesInfo.Max_No_Phases = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Max No Echoes")    SeriesInfo.Max_No_Echoes = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Max No Slices")    SeriesInfo.Max_No_Slices = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Max No Dynamics")    SeriesInfo.Max_No_Dynamics = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Max No Mixes")    SeriesInfo.Max_No_Mixes = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Max No B Values")    SeriesInfo.Max_No_B_Values = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Max No Gradient Orients")    SeriesInfo.Max_No_Gradient_Orients = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "No Label Types")     SeriesInfo.No_Label_Types = series_attributes[k][4].toInt();

        if( series_attributes[k][0] == "Patient Name")       SeriesInfo.Patient_Name = series_attributes[k][4];
        if( series_attributes[k][0] == "Examination Name")     SeriesInfo.Examination_Name = series_attributes[k][4];
        if( series_attributes[k][0] == "Protocol Name")     SeriesInfo.Protocol_Name = series_attributes[k][4];
        if( series_attributes[k][0] == "Examination Date")     SeriesInfo.Examination_Date = series_attributes[k][4];
        if( series_attributes[k][0] == "Examination Time")     SeriesInfo.Examination_Time = series_attributes[k][4];
        if( series_attributes[k][0] == "Series Data Type")     SeriesInfo.Series_Data_Type = series_attributes[k][4];
        if( series_attributes[k][0] == "Aquisition Number")     SeriesInfo.Aquisition_Number = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Reconstruction Number")     SeriesInfo.Reconstruction_Number = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Scan Duration")     SeriesInfo.Scan_Duration = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Patient Position")     SeriesInfo.Patient_Position = series_attributes[k][4];
        if( series_attributes[k][0] == "Preparation Direction")     SeriesInfo.Preparation_Direction = series_attributes[k][4];
        if( series_attributes[k][0] == "Technique")     SeriesInfo.Technique = series_attributes[k][4];
        if( series_attributes[k][0] == "Scan Resolution X")     SeriesInfo.Scan_Resolution_X = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Scan Resolution Y")     SeriesInfo.Scan_Resolution_Y = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Scan Mode")     SeriesInfo.Scan_Mode = series_attributes[k][4];

        if( series_attributes[k][0] == "Repetition Times")
        {
            if(ArrSize > 1)
            {
                a=0.0; b=0.0; c=0.0;
                QTextStream myteststream(&series_attributes[k][4]);
                if(ArrSize == 2)
                {
                    myteststream >> a >> b;
                    SeriesInfo.Repetition_Times[0] = a;
                    SeriesInfo.Repetition_Times[1] = b;
                }
                else if(ArrSize == 3)
                {
                    myteststream >> a >> b >> c;
                    SeriesInfo.Repetition_Times[0] = a;
                    SeriesInfo.Repetition_Times[1] = b;
                    SeriesInfo.Repetition_Times[2] = c;
                }
                else
                    qDebug() << "Error reading file: unsupported array size for Repetition_Times";
            }
            else
                SeriesInfo.Repetition_Times[0] = series_attributes[k][4].toDouble();
        }

        if( series_attributes[k][0] == "FOV AP")     SeriesInfo.FOV_AP = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "FOV FH")     SeriesInfo.FOV_FH = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "FOV RL")     SeriesInfo.FOV_RL = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Water Fat Shift")     SeriesInfo.Water_Fat_Shift = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Angulation AP")     SeriesInfo.Angulation_AP = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Angulation FH")     SeriesInfo.Angulation_FH = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Angulation RL")     SeriesInfo.Angulation_RL = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Off Center AP")     SeriesInfo.Off_Center_AP = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Off Center FH")     SeriesInfo.Off_Center_FH = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Off Center RL")     SeriesInfo.Off_Center_RL = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "Flow Compensation")     SeriesInfo.Flow_Compensation = series_attributes[k][4];
        if( series_attributes[k][0] == "Presaturation")     SeriesInfo.Presaturation = series_attributes[k][4];

        if( series_attributes[k][0] == "Phase Encoding Velocity")
        {
            if(ArrSize > 1)
            {
                a=0.0; b=0.0; c=0.0;
                QTextStream myteststream2(&series_attributes[k][4]);
                if(ArrSize == 2)
                {
                    myteststream2 >> a >> b;
                    SeriesInfo.Phase_Encoding_Velocity[0] = a;
                    SeriesInfo.Phase_Encoding_Velocity[1] = b;
                }
                else if(ArrSize == 3)
                {
                    myteststream2 >> a >> b >> c;
                    SeriesInfo.Phase_Encoding_Velocity[0] = a;
                    SeriesInfo.Phase_Encoding_Velocity[1] = b;
                    SeriesInfo.Phase_Encoding_Velocity[2] = c;
                }
            }
            else
                SeriesInfo.Phase_Encoding_Velocity[0] = series_attributes[k][4].toDouble();
        }

        if( series_attributes[k][0] == "MTC")     SeriesInfo.MTC = series_attributes[k][4];
        if( series_attributes[k][0] == "SPIR")     SeriesInfo.SPIR = series_attributes[k][4];
        if( series_attributes[k][0] == "EPI factor")     SeriesInfo.EPI_factor = series_attributes[k][4].toInt();
        if( series_attributes[k][0] == "Dynamic Scan")     SeriesInfo.Dynamic_Scan = series_attributes[k][4];
        if( series_attributes[k][0] == "Diffusion")     SeriesInfo.Diffusion = series_attributes[k][4];
        if( series_attributes[k][0] == "Diffusion Echo Time")     SeriesInfo.Diffusion_Echo_Time = series_attributes[k][4].toDouble();
        if( series_attributes[k][0] == "PhotometricInterpretation")     SeriesInfo.PhotometricInterpretation = series_attributes[k][4];
    }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::readImageInfo(QDomNodeList ImInfo_attr_list,  ImInfoClass &imageInfo)
{
    //QDomNodeList ImInfo_attr_list  = ImInfo.elementsByTagName("Attribute");
    QString field_name;
    QString field_name_despaced;

    QStringList ParList;
    ParList << "Pixel Size" << "Scan Percentage" << "Resolution X" << "Resolution Y"
            << "Rescale Intercept" << "Rescale Slope" << "Scale Slope" << "Window Center" << "Window Width"
            << "Slice Thickness" << "Slice Gap" << "Display Orientation"<< "fMRI Status Indication"
            << "Image Type Ed Es" << "Pixel Spacing" << "Echo Time" << "Dyn Scan Begin Time"
            << "Trigger Time" << "Diffusion B Factor" << "No Averages" << "Image Flip Angle" << "Cardiac Frequency"
            << "Min RR Interval" << "Max RR Interval"<< "TURBO Factor" << "Inversion Delay"<< "Contrast Type"
            << "Diffusion Anisotropy Type" << "Diffusion AP" << "Diffusion FH"<< "Diffusion RL" << "Angulation AP"
            << "Angulation FH" << "Angulation RL"<< "Offcenter AP"<< "Offcenter FH"<< "Offcenter RL"
            << "Slice Orientation" << "Image Planar Configuration" << "Samples Per Pixel";

    //    qDebug() << "ImInfo.Rescale_Intercept  = " << imageInfo.Rescale_Intercept ;
    //    qDebug() << "ImInfo.Rescale_Rescale Slope  = " << imageInfo.Rescale_Slope ;
    // qDebug() << "InInfo.Scale Slope  = " << InInfo.Scale Slope ;
    for(int ii = 0; ii < ImInfo_attr_list.length(); ii++)    {
        for(int ss = 0; ss < ParList.length(); ss++)        {
            field_name = ParList.at(ss);
            field_name_despaced = field_name;
            field_name_despaced.replace(" ", "_");

            if( ImInfo_attr_list.at(ii).toElement().attribute("Name") == field_name)            {
                setStructField(imageInfo, field_name_despaced, QVariant(ImInfo_attr_list.at(ii).firstChild().nodeValue()));
                break;
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::setStructField(ImInfoClass &ImInfo, QString str, QVariant var_value)
{    
    if(str == "Pixel_Size") ImInfo.Pixel_Size = var_value.toInt();
    else if(str == "Scan_Percentage") ImInfo.Scan_Percentage = var_value.toDouble();
    else if(str == "Resolution_X") ImInfo.Resolution_X = var_value.toInt();
    else if(str == "Resolution_Y") ImInfo.Resolution_Y = var_value.toInt();
    else if(str == "Rescale_Intercept") ImInfo.Rescale_Intercept = var_value.toDouble();
    else if(str == "Rescale_Slope") ImInfo.Rescale_Slope = var_value.toDouble();
    else if(str == "Scale_Slope") ImInfo.Scale_Slope = var_value.toDouble();
    else if(str == "Window_Center") ImInfo.Window_Center = var_value.toDouble();
    else if(str == "Window_Width") ImInfo.Window_Width = var_value.toDouble();
    else if(str == "Slice_Thickness") ImInfo.Slice_Thickness = var_value.toDouble();
    else if(str == "Slice_Gap") ImInfo.Slice_Gap = var_value.toDouble();
    else if(str == "Display_Orientation") ImInfo.Display_Orientation = var_value.toString();
    else if(str == "fMRI_Status_Indication") ImInfo.fMRI_Status_Indication = var_value.toInt();
    else if(str == "Image_Type_Ed_Es") ImInfo.Image_Type_Ed_Es = var_value.toString();
    else if(str == "Pixel_Spacing")
    {
        QString mystring = var_value.toString();
        QTextStream myteststream(&mystring);
        float a = 0, b = 0;
        myteststream >> a >> b;
        ImInfo.Pixel_Spacing[0] = a;
        ImInfo.Pixel_Spacing[1] = b;
    }
    else if(str == "Echo_Time") ImInfo.Echo_Time = var_value.toDouble();
    else if(str == "Dyn_Scan_Begin_Time") ImInfo.Dyn_Scan_Begin_Time = var_value.toDouble();
    else if(str == "Trigger_Time") ImInfo.Trigger_Time = var_value.toDouble();
    else if(str == "Diffusion_B_Factor") ImInfo.Diffusion_B_Factor = var_value.toDouble();
    else if(str == "No_Averages") ImInfo.No_Averages = var_value.toDouble();
    else if(str == "Image_Flip_Angle") ImInfo.Image_Flip_Angle = var_value.toDouble();
    else if(str == "Cardiac_Frequency") ImInfo.Cardiac_Frequency = var_value.toInt();
    else if(str == "Min_RR_Interval") ImInfo.Min_RR_Interval = var_value.toInt();
    else if(str == "Max_RR_Interval") ImInfo.Max_RR_Interval = var_value.toInt();
    else if(str == "TURBO_Factor") ImInfo.TURBO_Factor = var_value.toInt();
    else if(str == "Inversion_Delay") ImInfo.Inversion_Delay = var_value.toDouble();
    else if(str == "Contrast_Type") ImInfo.Contrast_Type = var_value.toString();
    else if(str == "Diffusion_Anisotropy_Type") ImInfo.Diffusion_Anisotropy_Type = var_value.toString();
    else if(str == "Diffusion_AP") ImInfo.Diffusion_AP = var_value.toDouble();
    else if(str == "Diffusion_FH") ImInfo.Diffusion_FH = var_value.toDouble();
    else if(str == "Diffusion_RL") ImInfo.Diffusion_RL = var_value.toDouble();
    else if(str == "Angulation_AP") ImInfo.Angulation_AP = var_value.toDouble();
    else if(str == "Angulation_FH") ImInfo.Angulation_FH = var_value.toDouble();
    else if(str == "Angulation_RL") ImInfo.Angulation_RL = var_value.toDouble();
    else if(str == "Offcenter_AP") ImInfo.Offcenter_AP = var_value.toDouble();
    else if(str == "Offcenter_FH") ImInfo.Offcenter_FH = var_value.toDouble();
    else if(str == "Offcenter_RL") ImInfo.Offcenter_RL = var_value.toDouble();
    else if(str == "Slice_Orientation") ImInfo.Slice_Orientation = var_value.toString();
    else if(str == "Image_Planar_Configuration") ImInfo.Image_Planar_Configuration = var_value.toInt();
    else if(str == "Samples_Per_Pixel") ImInfo.Samples_Per_Pixel = var_value.toInt();
}

//--------------------------------------------------------------------------------------------------
QString XMLRecReader::makeKeyString(ImKeyClass imageKey)
{
    QString KeyString = QString("key_") + QString::number(imageKey.Slice) + QString("_") + QString::number(imageKey.Echo) +
            QString("_")  + QString::number(imageKey.Dynamic) + QString("_") + QString::number(imageKey.Phase) +
            QString("_")  + QString::number(imageKey.BValue) + QString("_") + QString::number(imageKey.Grad_Orient) +
            "_" + imageKey.Label_Type + "_" + imageKey.Type + "_"  + imageKey.Sequence;

    return KeyString;
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::Convert_vec1d_to_vec2d(vector<vector<float> > &v2d, const vector<float> &v1d, int sz1, int sz2)
{
    v2d.resize(sz1);
    for (int i = 0; i < sz1; ++i)    {
        v2d[i].resize(sz2);
    }

    int cnt = 0;
    for (int y = 0; y < sz2; ++y)
        for (int x = 0; x < sz1; ++x)        {
            v2d[x][y] = v1d[cnt];
            cnt++;
        }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::Convert_vec2d_to_vec1d(const vector<vector<float> > &v2d, vector<float> &v1d, int sz1, int sz2)
{
    v1d.resize(sz1*sz2);

    int cnt = 0;
    for (int i = 0; i < sz1; ++i)
        for (int j = 0; j < sz2; ++j)        {
            v1d[cnt] = v2d[i][j];
            cnt++;
        }
}
//--------------------------------------------------------------------------------------------------

void XMLRecReader::Convert_vec3d_to_vec1d(const vector<vector<vector<float> > > &v3d, vector<float> &v1d, int sz1, int sz2, int sz3)
{
    v1d.resize(sz1*sz2*sz3);

    int cnt = 0;
    for (int i = 0; i < sz1; ++i)
        for (int j = 0; j < sz2; ++j)
            for (int k = 0; k < sz3; ++k)            {
                v1d[cnt] = v3d[i][j][k];
                cnt++;
            }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::Insert_vec2d_in_vec3d(vector<vector<float> > v2d, vector<vector<vector<float> > > &v3d, int sz1, int sz2, int index)
{
    for (int i = 0; i < sz1; ++i)
        for (int j = 0; j < sz2; ++j)
            v3d[i][j][index] = v2d[i][j];
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::Insert_vec2d_in_vec4d(vector<vector<float> > v2d, vector<vector<vector<vector<float> > > > &v4d, int sz1, int sz2, int index3, int index4)
{
    for (int i = 0; i < sz1; ++i)
        for (int j = 0; j < sz2; ++j)
            v4d[i][j][index3][index4] = v2d[i][j];
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::SetArrayDims2d(vector <vector <float> > &array2d, int sz1, int sz2)
{  
    // Set up sizes. (HEIGHT x WIDTH)
    array2d.resize(sz1);
    for (int i = 0; i < sz1; ++i)
    {
        array2d[i].resize(sz2);
    }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::SetArrayDims3d(vector <vector <vector <float> > > &array3d, int sz1, int sz2, int sz3)
{
    array3d.resize(sz1);
    for (int i = 0; i < sz1; ++i)    {
        array3d[i].resize(sz2);

        for (int j = 0; j < sz2; ++j)
            array3d[i][j].resize(sz3);
    }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::SetArrayDims4d(vector <vector <vector <vector <float> > > > &array4d, int sz1, int sz2, int sz3, int sz4)
{
    int i,j,k;

    array4d.resize(sz1);

    for (i = 0; i < sz1; ++i)    {
        array4d[i].resize(sz2);
        for (j = 0; j < sz2; ++j)        {
            array4d[i][j].resize(sz3);
            for (k = 0; k < sz3; ++k)
                array4d[i][j][k].resize(sz4);
        }
    }
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::clearArray2d(vector <vector <float> > &array2d)
{
    for (int i = 0; i < array2d.size(); ++i)
    {
        array2d[i].clear();
    }
    array2d.clear();
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::clearArray3d(vector <vector <vector <float> > > &array3d)
{
    for (int i = 0; i < array3d.size(); ++i)    {
        for (int j = 0; j < array3d[i].size(); ++j)
            array3d[i][j].clear();
        array3d[i].clear();
    }
    array3d.clear();
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::clearArray4d(vector <vector <vector <vector <float> > > > &array4d)
{
    int i,j,k;

    for (i = 0; i < array4d.size(); ++i)    {
        for (j = 0; j < array4d[i].size(); ++j)        {
            for (k = 0; k < array4d[i][j].size(); ++k)
                array4d[i][j][k].clear();
            array4d[i][j].clear();
        }
        array4d[i].clear();
    }
    array4d.clear();
}
//--------------------------------------------------------------------------------------------------
int XMLRecReader::loadXmlRec(QString xmlfilename, ImageC* ImageOut)
{
    QDomDocument document;
    QString recfilename(xmlfilename);

    bool xmlExtFound = xmlfilename.contains(".xml") || xmlfilename.contains(".XML");
    if (!xmlExtFound)
    {
        qDebug("Could not find XML file.");
        return -1;
    }

    if(xmlfilename.contains(".xml"))
        recfilename.replace(".xml",".rec");
    else
        recfilename.replace(".XML",".rec");

    QFileInfo checkFile(recfilename);
    if (!(checkFile.exists() && checkFile.isFile()))
    {
        recfilename.replace(".rec",".REC");
        checkFile.setFile(recfilename);
        if (!(checkFile.exists() && checkFile.isFile()))
        {
            qDebug("Could not find REC file.");
            return -1;
        }
    }

    QFile file(xmlfilename);
    QFile recfile(recfilename);

    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )    {
        qDebug("Failed to open file for reading.");
        return -1;
    }

    if( !recfile.open( QIODevice::ReadOnly ) )    {
        qDebug("Failed to open REC file for reading.");
        return -1;
    }

    if( !document.setContent( &file ) )    {
        qDebug("Failed to parse the file into a DOM tree.");
        file.close();
        return -1;
    }

    file.close();

    QDomElement root = document.firstChildElement();

    QDomNodeList series_info_list = root.elementsByTagName("Series_Info");

    if(series_info_list.count()== 0)    {
        qDebug() << "Could not find Series_Info";
        return -1;
    }

    // get series attributes
    QDomNodeList series_info_fields_list  = series_info_list.at(0).toElement().elementsByTagName("Attribute");
    SeriesClass SeriesInfo;
    QVector< QVector<QString> > series_attributes(series_info_fields_list.length());
    for (int i=0; i<series_info_fields_list.length(); i++)
        series_attributes[i].fill("", 5);    // for Name, Tag, Level, Type, and Value

    ImageOut->series_info_list = series_info_list;

    //    if(SeriesInfo.No_Label_Types == 0)
    //        SeriesInfo.No_Label_Types = 1;

    // Parse the series info here
    readSeriesInfo(series_info_fields_list,  series_attributes, SeriesInfo);


    // get the images
    QDomNodeList image_array_root_list = root.elementsByTagName("Image_Array");
    if(!image_array_root_list.at(0).isElement())
    {
        qDebug() << "ERROR parsing the document";
        return -1;
    }
    QDomNodeList ImageArray = image_array_root_list.at(0).toElement().elementsByTagName("Image_Info");

    ImageOut->ImageArray = ImageArray;

    int NumImages = ImageArray.length();

    //    qDebug() << "Total numer of images = " <<  NumImages;

    vector< ImKeyClass > imageKey(NumImages);
    vector< ImInfoClass > imageInfo(NumImages);

    // Philips forgot to report the number of types!!!
    // Now I have to do it manually by counting the different types in the image key

    QList<QString> UniqueTypeSeqList;

    QMap<QString, int> ImageIndex;
    for(int i = 0; i < NumImages; i++)
    {
        QDomElement ImInfo = ImageArray.at(i).toElement();

        // READ THE IMAGE KEY
        QDomElement ImKey = ImInfo.elementsByTagName("Key").at(0).toElement();
        QDomNodeList ImKey_attr_list  = ImKey.elementsByTagName("Attribute");

        readImageKey(ImKey_attr_list, imageKey[i]);

        if(!SeriesInfo.EnumImageType.contains(imageKey[i].Type, Qt::CaseInsensitive))
        {
            SeriesInfo.EnumImageType.append(imageKey[i].Type);
        }

        if(!SeriesInfo.EnumLabelType.contains(imageKey[i].Label_Type, Qt::CaseInsensitive))
        {
            SeriesInfo.EnumLabelType.append(imageKey[i].Label_Type);
        }

        if(!SeriesInfo.EnumSequence.contains(imageKey[i].Sequence, Qt::CaseInsensitive))
        {
            SeriesInfo.EnumSequence.append(imageKey[i].Sequence);
        }


        // Setup the image index
        //        QString KeyString = QString("key_") + QString::number(imageKey[i].Slice) + QString("_") + QString::number(imageKey[i].Echo) +
        //                QString("_")  + QString::number(imageKey[i].Dynamic) + QString("_") + QString::number(imageKey[i].Phase) +
        //                QString("_")  + QString::number(imageKey[i].BValue) + QString("_") + QString::number(imageKey[i].Grad_Orient) +
        //                "_" + imageKey[i].Label_Type + "_" + imageKey[i].Type + "_"  + imageKey[i].Sequence;

        QString KeyString = makeKeyString(imageKey[i]);

        ImageIndex.insert(KeyString, imageKey[i].Index);

        if(!UniqueTypeSeqList.contains(imageKey[i].Type + "_"  + imageKey[i].Sequence))
            UniqueTypeSeqList.append(imageKey[i].Type + "_"  + imageKey[i].Sequence);

        // READ THE IMAGE ATTRIBUTES
        QDomNodeList ImInfo_attr_list  = ImInfo.elementsByTagName("Attribute");
        readImageInfo(ImInfo_attr_list,  imageInfo[i]);
        imageInfo[i].Key = imageKey[i];
        //readImageInfo(ImInfo_attr_list,  imageInfo[imageKey[i].Index]);
    }

    int NumUniqueTypeSeq = UniqueTypeSeqList.length();
    int Max_No_Sqs = SeriesInfo.EnumSequence.length();

    SeriesInfo.Max_No_Types = SeriesInfo.EnumImageType.length();


    //    if((SeriesInfo.EnumLabelType.length()==1) && (SeriesInfo.EnumLabelType.at(0)=="-"))
    //    {
    //        SeriesInfo.No_Label_Types = 1;
    //        if(SeriesInfo.No_Label_Types != 0)
    //        {
    //            qDebug() << "SeriesInfo.EnumLabelType.length()" << SeriesInfo.EnumLabelType.length();
    //            qDebug() << "SeriesInfo.No_Label_Types" << SeriesInfo.No_Label_Types;
    //            qDebug() << "ERROR: label types count does not match";
    //            return -1;
    //        }
    //    }

    //    if(SeriesInfo.EnumLabelType.length() != SeriesInfo.No_Label_Types)
    //    {
    //        qDebug() << "SeriesInfo.EnumLabelType.length()" << SeriesInfo.EnumLabelType.length();
    //        qDebug() << "SeriesInfo.No_Label_Types" << SeriesInfo.No_Label_Types;
    //        qDebug() << "ERROR: label types count does not match";
    //        return -1;
    //    }

    // read the REC file
    int imageSize_bytes = imageInfo[0].Resolution_X * imageInfo[0].Resolution_Y * imageInfo[0].Pixel_Size / 8;

    QString read_type;
    switch (imageInfo[0].Pixel_Size)
    {
    case 8: read_type = "int8"; break;
    case 16: read_type = "short"; break;
    default: read_type = "uchar";
    }

    int Slice, Echo, Dynamic, Phase, BValue, Grad_Orient, LType, ty;

    if(0)    {
        qDebug() << "Start reading REC file...";
        qDebug() << "SeriesInfo.Max_No_Slices" << SeriesInfo.Max_No_Slices;
        qDebug() << "SeriesInfo.Max_No_Echoes" << SeriesInfo.Max_No_Echoes;
        qDebug() << "SeriesInfo.Max_No_Dynamics" << SeriesInfo.Max_No_Dynamics;
        qDebug() << "SeriesInfo.Max_No_Phases" << SeriesInfo.Max_No_Phases;
        qDebug() << "SeriesInfo.Max_No_B_Values" << SeriesInfo.Max_No_B_Values;
        qDebug() << "SeriesInfo.Max_No_Gradient_Orients" << SeriesInfo.Max_No_Gradient_Orients;
        qDebug() << "SeriesInfo.No_Label_Types" << SeriesInfo.No_Label_Types;
        qDebug() << "SeriesInfo.Max_No_Mixes" << SeriesInfo.Max_No_Mixes;
        qDebug() << "SeriesInfo.Max_No_Types" << SeriesInfo.Max_No_Types;
    }

    // determine the data dimensions (squeezing singelton dimensions out)
    int NonSigeltonDim[8];
    NonSigeltonDim[0] = (SeriesInfo.Max_No_Slices > 1);
    NonSigeltonDim[1] =(SeriesInfo.Max_No_Echoes > 1);
    NonSigeltonDim[2] =(SeriesInfo.Max_No_Dynamics > 1);
    NonSigeltonDim[3] = (SeriesInfo.Max_No_Phases > 1);
    NonSigeltonDim[4] = (SeriesInfo.Max_No_B_Values > 1);
    NonSigeltonDim[5] = (SeriesInfo.Max_No_Gradient_Orients > 1);
    NonSigeltonDim[6] = (SeriesInfo.No_Label_Types > 1);
    //    NonSigeltonDim[7] = (SeriesInfo.Max_No_Types > 1);
    NonSigeltonDim[7] = (NumUniqueTypeSeq > 1);

    int DimSize[8];
    DimSize[0] = SeriesInfo.Max_No_Slices;
    DimSize[1] = SeriesInfo.Max_No_Echoes;
    DimSize[2] = SeriesInfo.Max_No_Dynamics;
    DimSize[3] = SeriesInfo.Max_No_Phases;
    DimSize[4] = SeriesInfo.Max_No_B_Values;
    DimSize[5] = SeriesInfo.Max_No_Gradient_Orients;
    DimSize[6] = SeriesInfo.No_Label_Types;
    //    DimSize[7] = SeriesInfo.Max_No_Types;
    DimSize[7] = NumUniqueTypeSeq;


    int ndims = 2, dd = 0;
    for(dd = 0; dd < 8; dd++)
        ndims += NonSigeltonDim[dd];

    //    qDebug() << "Number of dimensions = " << ndims;

    int sz1 = imageInfo.at(0).Resolution_X;
    int sz2 = imageInfo.at(0).Resolution_Y;

    //    int NumElements = sz1 * sz2 *
    //            SeriesInfo.Max_No_Slices  * SeriesInfo.Max_No_Echoes * SeriesInfo.Max_No_Dynamics *
    //            SeriesInfo.Max_No_Phases * SeriesInfo.Max_No_B_Values * SeriesInfo.Max_No_Gradient_Orients *
    //            SeriesInfo.No_Label_Types;

    ImageOut->NumDimensions = ndims;
    ImageOut->BitDepth = imageInfo.at(0).Pixel_Size;

    int dim3 = 1, dim4 = 1;
    bool dim3Found = false;
    bool dim4Found = false;

    double max_memory_available;
    double required_memory;


    switch(ndims)
    {
    case 2:
        ImageOut->data.resize(sz1*sz2);
        break;
    case 3:
        dim3Found = false;
        for(dd=0; dd<8; dd++)
        {
            if(NonSigeltonDim[dd])
            {
                if(!dim3Found)
                {
                    dim3 = DimSize[dd];
                    dim3Found = true;
                }
            }
        }
        if(0)
        {
            qDebug() << "dim3 = " << dim3;
        }
        //        SetArrayDims3d(ImageOut->data3d, sz1, sz2, dim3);
        ImageOut->data.resize(sz1*sz2*dim3);
        break;
    case 4:
        dim3Found = false;
        dim4Found = false;
        for(dd=0; dd<8; dd++)
        {
            if(NonSigeltonDim[dd])
            {
                if(!dim3Found)
                {
                    dim3 = DimSize[dd];
                    dim3Found = true;
                }
                else if(!dim4Found)
                {
                    dim4 = DimSize[dd];
                    dim4Found = true;
                }
            }
        }

        if(1)
        {
            if(NumUniqueTypeSeq > dim4)
                dim4 = NumUniqueTypeSeq;
        }

        if(0)
        {
            qDebug() << "dim4 = " << dim4;
        }
        //qDebug() << "resizing";
        //        max_memory_available = ImageOut->data4d.max_size();
        max_memory_available = ImageOut->data.max_size();
        required_memory = sizeof(float) * sz1 * sz2 * dim3 * dim4;
        if(required_memory > max_memory_available)
        {
            qDebug() << "required memory = " << required_memory  << " > maximum available = " << ImageOut->data.max_size();
            return -1;
        }
        //        SetArrayDims4d(ImageOut->data4d, sz1, sz2, dim3, dim4);
        ImageOut->data.resize(sz1*sz2*dim3*dim4);
        //qDebug() << "finished resizing";
        break;
    default: qDebug() << "ERROR: unsupported image size"; return -1;
    }

    ImageOut->array_size[0] = sz1;
    ImageOut->array_size[1] = sz2;
    ImageOut->array_size[2] = dim3;
    ImageOut->array_size[3] = dim4;

    ImageOut->voxel_size[0] = imageInfo[0].Pixel_Spacing[0];
    ImageOut->voxel_size[1] = imageInfo[0].Pixel_Spacing[1];
    ImageOut->voxel_size[2] = imageInfo[0].Slice_Thickness;
    ImageOut->voxel_size[3] = imageInfo[0].Slice_Gap;

    ImageOut->series_info = SeriesInfo;
    ImageOut->image_info = imageInfo;
    ImageOut->ImageIndex = ImageIndex;

    int mm;
    int index3  = 0, index4 = 0;
    int index3Position, index4Position, qq;

    //    char data[imageSize_bytes];
    //    qDebug() << "imageSize_bytes = " << imageSize_bytes;

    char* data = new char[imageSize_bytes];
    uint16_t* data_int = (uint16_t*)(data);

    if(SeriesInfo.No_Label_Types == 0)
        SeriesInfo.No_Label_Types = 1;

    int imageLoc, num_read_bytes;

    if( 0 )
    {
        recfile.seek(0);
        num_read_bytes = recfile.read(data, imageSize_bytes*2*SeriesInfo.Max_No_Slices);
        qDebug() << "num_read_bytes : " << num_read_bytes;
        qDebug() << "number of images read : " << num_read_bytes/2.0/256/256;
    }

    QString Sequence;
    int sq;
    vector <float> vdImage;
    vector <vector <float> > v2dImage;

    int count=0;
    for(Slice = 0; Slice < SeriesInfo.Max_No_Slices; Slice++)
    {
        for(Echo = 0; Echo < SeriesInfo.Max_No_Echoes; Echo++)
        {
            for(Dynamic = 0; Dynamic < SeriesInfo.Max_No_Dynamics; Dynamic++)
            {
                for(Phase =0; Phase < SeriesInfo.Max_No_Phases; Phase++)
                {
                    for(BValue = 0; BValue < SeriesInfo.Max_No_B_Values; BValue++)
                    {
                        for(Grad_Orient = 0; Grad_Orient < SeriesInfo.Max_No_Gradient_Orients; Grad_Orient++)
                        {
                            for(LType = 0; LType < (SeriesInfo.No_Label_Types); LType++)
                            {
                                //                                QString Label_Type = "-";
                                QString Label_Type = SeriesInfo.EnumLabelType.at(LType);
                                for(ty = 0; ty < SeriesInfo.Max_No_Types; ty++)
                                {
                                    for(sq = 0; sq < SeriesInfo.Max_No_Mixes; sq++)
                                    {
                                        //                                    QString Type = "M";
                                        QString Type = SeriesInfo.EnumImageType.at(ty);

                                        //QString Sequence = imageKey[0].Sequence;
                                        //                                        QString Sequence = UniqueTypeSeqList.at(sq);
                                        QString Sequence = SeriesInfo.EnumSequence.at(sq);


                                        // test that type-seq is actually in the list
                                        if(!UniqueTypeSeqList.contains(Type + '_' + Sequence))
                                            continue;

                                        // make the image key string
                                        QString KeyString ;
                                        ImKeyClass imageKey;
                                        imageKey.Slice = Slice+1;
                                        imageKey.Echo = Echo+1;
                                        imageKey.Dynamic = Dynamic+1;
                                        imageKey.Phase = Phase+1;
                                        imageKey.BValue = BValue+1;
                                        imageKey.Grad_Orient = Grad_Orient+1;
                                        imageKey.Type = Type;
                                        imageKey.Label_Type = Label_Type;
                                        imageKey.Sequence = Sequence;

                                        KeyString = makeKeyString(imageKey);

                                        //qDebug() << "Reading KeyString = " << KeyString;
                                        imageLoc = ImageIndex[KeyString];

                                        recfile.seek(imageLoc*imageSize_bytes);
                                        //recfile.seek(count*imageSize_bytes);

                                        //qDebug() << "start reading from disk";
                                        num_read_bytes = recfile.read(data, imageSize_bytes);
                                        if(num_read_bytes < imageSize_bytes)
                                        {
                                            qDebug() << "num_read_bytes : " << num_read_bytes;
                                            qDebug() << "imageSize_bytes : " << imageSize_bytes;
                                            qDebug() << "ERROR reading the raw data at image " << imageLoc;
                                            return -1;
                                        }

                                        //   qDebug() << "start convert to int then double";
                                        vdImage.resize(sz1*sz2);

                                        for(mm = 0; mm < sz1*sz2;  mm++)
                                            vdImage[mm] = (float)(data_int[mm]);

                                        // rescale data back
                                        if(1)
                                        {
                                            for(mm = 0; mm < sz1*sz2;  mm++)
                                                vdImage[mm] = (vdImage[mm] * imageInfo.at(imageLoc).Rescale_Slope + imageInfo.at(imageLoc).Rescale_Intercept) /
                                                        (imageInfo.at(imageLoc).Rescale_Slope * imageInfo.at(imageLoc).Scale_Slope);
                                        }

                                        Convert_vec1d_to_vec2d(v2dImage, vdImage, sz1, sz2);

                                        int ind_typ_seq = UniqueTypeSeqList.indexOf((Type + '_' + Sequence));

                                        switch(ndims)
                                        {
                                        case 2:
                                            for (int i = 0; i < sz1; ++i)
                                                for (int j = 0; j < sz2; ++j)
                                                    ImageOut->setVoxel(v2dImage[i][j],i,j);
                                            break;
                                        case 3:
                                            index3 = 0;
                                            for(int qq = 0; qq < 8; qq++)
                                            {
                                                if (NonSigeltonDim[qq])
                                                {
                                                    index3Position = qq;
                                                    break;
                                                }
                                            }
                                            index3 = (index3Position==0) * Slice +
                                                    (index3Position==1) * Echo +
                                                    (index3Position==2) * Dynamic +
                                                    (index3Position==3) * Phase +
                                                    (index3Position==4) * BValue +
                                                    (index3Position==5) * Grad_Orient +
                                                    (index3Position==6) * LType +
                                                    (index3Position==7) * NumUniqueTypeSeq;

                                            for (int i = 0; i < sz1; ++i)
                                                for (int j = 0; j < sz2; ++j)
                                                    ImageOut->setVoxel(v2dImage[i][j],i,j,index3);
                                            break;
                                        case 4:
                                            // find which dimension is third dimension
                                            for(int qq = 0; qq < 8; qq++)
                                            {
                                                if (NonSigeltonDim[qq])
                                                {
                                                    index3Position = qq;
                                                    break;
                                                }
                                            }
                                            // find which dimension is the 4th
                                            for(qq = 0; qq < 8; qq++)
                                            {
                                                if((NonSigeltonDim[qq]) && (qq!=index3Position))
                                                {
                                                    index4Position = qq;
                                                    break;
                                                }
                                            }

                                            // find the dim3/dim4 index of this image
                                            index3 = (index3Position==0) * Slice +
                                                    (index3Position==1) * Echo +
                                                    (index3Position==2) * Dynamic +
                                                    (index3Position==3) * Phase +
                                                    (index3Position==4) * BValue +
                                                    (index3Position==5) * Grad_Orient +
                                                    (index3Position==6) * LType +
                                                    (index3Position==7) * ind_typ_seq;

                                            index4 = (index4Position==0) * Slice +
                                                    (index4Position==1) * Echo +
                                                    (index4Position==2) * Dynamic +
                                                    (index4Position==3) * Phase +
                                                    (index4Position==4) * BValue +
                                                    (index4Position==5) * Grad_Orient +
                                                    (index4Position==6) * LType +
                                                    (index4Position==7) * ind_typ_seq;


                                            for (int i = 0; i < sz1; ++i)
                                                for (int j = 0; j < sz2; ++j)
                                                    ImageOut->setVoxel(v2dImage[i][j],i,j,index3,index4);

                                            break;
                                        default: qDebug() << "ERROR: unsupported image size"; return -1;
                                        }
                                        count++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    vdImage.clear();
    clearArray2d(v2dImage);

    //delete [] data;
    //delete [] data_int;

    recfile.close();

    //    qDebug() << "Done reading REC file.";

    return 0;
}
//--------------------------------------------------------------------------------------------------
QDomElement XMLRecReader::SeriesInfoToNode( QDomDocument &d, const QString patient_name, const int max_ph, const int max_ec, const int max_slc, const int max_dyn, const int max_mix, const int max_bval, const int max_grd, const int max_lbl)
{
    QDomText  name_value = d.createTextNode("");
    QDomText  ph_value = d.createTextNode("");
    QDomText  ec_value = d.createTextNode("");
    QDomText  slc_value = d.createTextNode("");
    QDomText  dyn_value = d.createTextNode("");
    QDomText  mix_value = d.createTextNode("");
    QDomText  bval_value = d.createTextNode("");
    QDomText  grd_value = d.createTextNode("");
    QDomText  lbl_value = d.createTextNode("");

    QDomElement im_series = d.createElement("Series_Info");

    QDomElement name = d.createElement("Attribute");
    QDomElement ph = d.createElement("Attribute");
    QDomElement ec = d.createElement("Attribute");
    QDomElement slc = d.createElement("Attribute");
    QDomElement dyn = d.createElement("Attribute");
    QDomElement mix = d.createElement("Attribute");
    QDomElement bval = d.createElement("Attribute");
    QDomElement grd = d.createElement("Attribute");
    QDomElement lbl = d.createElement("Attribute");

    name.setAttribute("Name", "Patient Name"); name.setAttribute("Tag", "0x00100010"); name.setAttribute("Level", "Patient");    name.setAttribute("Type", "String"); name_value.setNodeValue(patient_name);     name.appendChild(name_value);
    ph.setAttribute("Name", "Max No Phases"); ph.setAttribute("Tag", "0x20011017"); ph.setAttribute("Level", "MRSeries");    ph.setAttribute("Type", "Int32");    ph_value.setNodeValue(QString::number(max_ph));     ph.appendChild(ph_value);
    ec.setAttribute("Name", "Max No Echoes"); ec.setAttribute("Tag", "0x20011014"); ec.setAttribute("Level", "MRSeries");    ec.setAttribute("Type", "Int32");    ec_value.setNodeValue(QString::number(max_ec));     ec.appendChild(ec_value);
    slc.setAttribute("Name", "Max No Slices"); slc.setAttribute("Tag", "0x20011018"); slc.setAttribute("Level", "MRSeries");    slc.setAttribute("Type", "Int32");    slc_value.setNodeValue(QString::number(max_slc));     slc.appendChild(slc_value);
    dyn.setAttribute("Name", "Max No Dynamics"); dyn.setAttribute("Tag", "0x20011081"); dyn.setAttribute("Level", "MRSeries");    dyn.setAttribute("Type", "Int32");    dyn_value.setNodeValue(QString::number(max_dyn));     dyn.appendChild(dyn_value);
    mix.setAttribute("Name", "Max No Mixes"); mix.setAttribute("Tag", "0x20051021"); mix.setAttribute("Level", "MRSeries");    mix.setAttribute("Type", "Int16");    mix_value.setNodeValue(QString::number(max_mix));     mix.appendChild(mix_value);
    bval.setAttribute("Name", "Max No B Values"); bval.setAttribute("Tag", "0x20051414"); bval.setAttribute("Level", "MRSeries");    bval.setAttribute("Type", "Int32");    bval_value.setNodeValue(QString::number(max_bval));     bval.appendChild(bval_value);
    grd.setAttribute("Name", "Max No Gradient Orients"); grd.setAttribute("Tag", "0x20051415"); grd.setAttribute("Level", "MRSeries");    grd.setAttribute("Type", "Int32");    grd_value.setNodeValue(QString::number(max_grd));     grd.appendChild(grd_value);
    lbl.setAttribute("Name", "No Label Types"); lbl.setAttribute("Tag", "0x20051428"); lbl.setAttribute("Level", "MRSeries");    lbl.setAttribute("Type", "Int32");    lbl_value.setNodeValue(QString::number(max_lbl));     lbl.appendChild(lbl_value);

    im_series.appendChild(name);
    im_series.appendChild(ph);
    im_series.appendChild(ec);
    im_series.appendChild(slc);
    im_series.appendChild(dyn);
    im_series.appendChild(mix);
    im_series.appendChild(bval);
    im_series.appendChild(grd);

    im_series.appendChild(lbl);


    return im_series;
}
//--------------------------------------------------------------------------------------------------
QDomElement XMLRecReader::SeriesInfoToNode( QDomDocument &d, const SeriesClass S)
{    
    vector<QString> Names;
    vector<QString> Tags;
    vector<QString> Types;
    vector<QString> Levels;

    // set a fixed hash value for a consistent order of attributes in the XML file
    qputenv("QT_HASH_SEED", "7");

    int NumSeriesInfoAttributes = 44;

    get_xml_rec_series_defs(Names, Tags, Levels, Types);

    QDomElement im_series = d.createElement("Series_Info");

    vector<QDomElement> XX;
    QDomElement xx;
    QDomText  xx_value;
    for(int ii = 0; ii< NumSeriesInfoAttributes; ii++)
    {
        xx = d.createElement("Attribute");
        xx_value = d.createTextNode("");
        //        xx.setAttribute("Name", "Patient Name"); name.setAttribute("Tag", "0x00100010"); name.setAttribute("Level", "Patient");    name.setAttribute("Type", "String"); name_value.setNodeValue(patient_name);     name.appendChild(name_value);
        xx.setAttribute("Level", Levels[ii]);
        xx.setAttribute("Type", Types[ii]);
        xx.setAttribute("Name", Names[ii]);
        xx.setAttribute("Tag", Tags[ii] );
        xx_value.setNodeValue(get_xml_rec_series_element(S, ii));
        if(ii==23)// repetion times
        {
            int arr_sz = 1;
            if(S.Repetition_Times[1] > -1.0) arr_sz++;
            if(S.Repetition_Times[2] > -1.0) arr_sz++;
            if(arr_sz>1)
                xx.setAttribute("ArraySize", arr_sz) ;
        }
        if(ii==36)// flow enc vel
        {
            int arr_sz = 1;
            if(S.Phase_Encoding_Velocity[1] > -1.0) arr_sz++;
            if(S.Phase_Encoding_Velocity[2] > -1.0) arr_sz++;
            if(arr_sz>1)
                xx.setAttribute("ArraySize", arr_sz) ;
        }
        xx.appendChild(xx_value);
        XX.push_back(xx);
        //        im_series.appendChild(xx);
        im_series.appendChild(XX[ii]);
    }


    // reset hash seed with new random value.
    qputenv("QT_HASH_SEED", "-1");

    return im_series;
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::get_xml_rec_series_defs(vector<QString> &Names, vector<QString> &Tags, vector<QString> &Levels, vector<QString> &Types)
{
    Names.clear();
    Tags.clear();
    Levels.clear();
    Types.clear();

    int ii = -1;
    ii++; Names.push_back("Patient Name");
    ii++; Names.push_back("Examination Name");
    ii++; Names.push_back("Protocol Name");
    ii++; Names.push_back("Examination Date");
    ii++; Names.push_back("Examination Time");
    ii++; Names.push_back("Series Data Type");
    ii++; Names.push_back("Aquisition Number");
    ii++; Names.push_back("Reconstruction Number");
    ii++; Names.push_back("Scan Duration");
    ii++; Names.push_back("Max No Phases");
    ii++; Names.push_back("Max No Echoes");
    ii++; Names.push_back("Max No Slices");
    ii++; Names.push_back("Max No Dynamics");
    ii++; Names.push_back("Max No Mixes");
    ii++; Names.push_back("Max No B Values");
    ii++; Names.push_back("Max No Gradient Orients");
    ii++; Names.push_back("No Label Types");
    ii++; Names.push_back("Patient Position");
    ii++; Names.push_back("Preparation Direction");
    ii++; Names.push_back("Technique");
    ii++; Names.push_back("Scan Resolution X");
    ii++; Names.push_back("Scan Resolution Y");
    ii++; Names.push_back("Scan Mode");
    ii++; Names.push_back("Repetition Times");
    ii++; Names.push_back("FOV AP");
    ii++; Names.push_back("FOV FH");
    ii++; Names.push_back("FOV RL");
    ii++; Names.push_back("Water Fat Shift");
    ii++; Names.push_back("Angulation AP");
    ii++; Names.push_back("Angulation FH");
    ii++; Names.push_back("Angulation RL");
    ii++; Names.push_back("Off Center AP");
    ii++; Names.push_back("Off Center FH");
    ii++; Names.push_back("Off Center RL");
    ii++; Names.push_back("Flow Compensation");
    ii++; Names.push_back("Presaturation");
    ii++; Names.push_back("Phase Encoding Velocity");
    ii++; Names.push_back("MTC");
    ii++; Names.push_back("SPIR");
    ii++; Names.push_back("EPI factor");
    ii++; Names.push_back("Dynamic Scan");
    ii++; Names.push_back("Diffusion");
    ii++; Names.push_back("Diffusion Echo Time");
    ii++; Names.push_back("PhotometricInterpretation");

    //    ..


    ii = -1;
    ii++; Tags.push_back("0x00100010");
    ii++; Tags.push_back("0x00400254");
    ii++; Tags.push_back("0x00181030");
    ii++; Tags.push_back("0x00400244");
    ii++; Tags.push_back("0x00400245");
    ii++; Tags.push_back("0x20051035");
    ii++; Tags.push_back("0x2001107B");
    ii++; Tags.push_back("0x2001101D");
    ii++; Tags.push_back("0x20051033");
    ii++; Tags.push_back("0x20011017");
    ii++; Tags.push_back("0x20011014");
    ii++; Tags.push_back("0x20011018");
    ii++; Tags.push_back("0x20011081");
    ii++; Tags.push_back("0x20051021");
    ii++; Tags.push_back("0x20051414");
    ii++; Tags.push_back("0x20051415");
    ii++; Tags.push_back("0x20051428");
    ii++; Tags.push_back("0x00185100");
    ii++; Tags.push_back("0x2005107B");
    ii++; Tags.push_back("0x20011020");
    ii++; Tags.push_back("0x2005101D");
    ii++; Tags.push_back("0x00180089");
    ii++; Tags.push_back("0x2005106F");
    ii++; Tags.push_back("0x20051030");
    ii++; Tags.push_back("0x20051074");
    ii++; Tags.push_back("0x20051075");
    ii++; Tags.push_back("0x20051076");
    ii++; Tags.push_back("0x20011022");
    ii++; Tags.push_back("0x20051071");
    ii++; Tags.push_back("0x20051072");
    ii++; Tags.push_back("0x20051073");
    ii++; Tags.push_back("0x20051078");
    ii++; Tags.push_back("0x20051079");
    ii++; Tags.push_back("0x2005107A");
    ii++; Tags.push_back("0x20051016");
    ii++; Tags.push_back("0x2005102F");
    ii++; Tags.push_back("0x2001101A");
    ii++; Tags.push_back("0x2005101C");
    ii++; Tags.push_back("0x20011021");
    ii++; Tags.push_back("0x20011013");
    ii++; Tags.push_back("0x20011012");
    ii++; Tags.push_back("0x20051014");
    ii++; Tags.push_back("0x20011011");
    ii++; Tags.push_back("0x00280004");

    //    ..

    ii = -1;
    ii++; Levels.push_back("Patient");
    ii++; Levels.push_back("Examination");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("Examination");
    ii++; Levels.push_back("Examination");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRImage");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRStack");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRSeries");
    ii++; Levels.push_back("MRImage");
    //    ..

    ii = -1;
    ii++; Types.push_back("String");
    ii++; Types.push_back("String");
    ii++; Types.push_back("String");
    ii++; Types.push_back("Date");
    ii++; Types.push_back("Time");
    ii++; Types.push_back("String");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int16");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("String");
    ii++; Types.push_back("String");
    ii++; Types.push_back("String");
    ii++; Types.push_back("Int16");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("String");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Boolean");
    ii++; Types.push_back("Boolean");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Boolean");
    ii++; Types.push_back("Boolean");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Boolean");
    ii++; Types.push_back("Boolean");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("String");
    //    ..
}
//--------------------------------------------------------------------------------------------------
void XMLRecReader::get_xml_rec_image_defs(vector<QString> &Names, vector<QString> &Tags, vector<QString> &Types, vector<QString> &EnumTypes, vector<QString> &Calcs, vector<QString> &ArraySizes)
{
    Names.clear();
    Tags.clear();
    Types.clear();

    int ii = -1;
    ii++; Names.push_back("Pixel Size");
    ii++; Names.push_back("Scan Percentage");
    ii++; Names.push_back("Resolution X");
    ii++; Names.push_back("Resolution Y");
    ii++; Names.push_back("Rescale Intercept");
    ii++; Names.push_back("Rescale Slope");
    ii++; Names.push_back("Scale Slope");
    ii++; Names.push_back("Window Center");
    ii++; Names.push_back("Window Width");
    ii++; Names.push_back("Slice Thickness");
    ii++; Names.push_back("Slice Gap");
    ii++; Names.push_back("Display Orientation");
    ii++; Names.push_back("fMRI Status Indication");
    ii++; Names.push_back("Image Type Ed Es");
    ii++; Names.push_back("Pixel Spacing");
    ii++; Names.push_back("Echo Time");
    ii++; Names.push_back("Dyn Scan Begin Time");
    ii++; Names.push_back("Trigger Time");
    ii++; Names.push_back("Diffusion B Factor");
    ii++; Names.push_back("No Averages");
    ii++; Names.push_back("Image Flip Angle");
    ii++; Names.push_back("Cardiac Frequency");
    ii++; Names.push_back("Min RR Interval");
    ii++; Names.push_back("Max RR Interval");
    ii++; Names.push_back("TURBO Factor");
    ii++; Names.push_back("Inversion Delay");
    ii++; Names.push_back("Contrast Type");
    ii++; Names.push_back("Diffusion Anisotropy Type");
    ii++; Names.push_back("Diffusion AP");
    ii++; Names.push_back("Diffusion FH");
    ii++; Names.push_back("Diffusion RL");
    ii++; Names.push_back("Angulation AP");
    ii++; Names.push_back("Angulation FH");
    ii++; Names.push_back("Angulation RL");
    ii++; Names.push_back("Offcenter AP");
    ii++; Names.push_back("Offcenter FH");
    ii++; Names.push_back("Offcenter RL");
    ii++; Names.push_back("Slice Orientation");
    ii++; Names.push_back("Image Planar Configuration");
    ii++; Names.push_back("Samples Per Pixel");

    //    ..


    ii = -1;
    ii++; Tags.push_back("0x00280100");
    ii++; Tags.push_back("0x00180093");
    ii++; Tags.push_back("0x00280011");
    ii++; Tags.push_back("0x00280010");
    ii++; Tags.push_back("0x00281052");
    ii++; Tags.push_back("0x00281053");
    ii++; Tags.push_back("0x2005100E");
    ii++; Tags.push_back("0x00281050");
    ii++; Tags.push_back("0x00281051");
    ii++; Tags.push_back("0x00180050");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("0x20051004");
    ii++; Tags.push_back("0x20051063");
    ii++; Tags.push_back("0x20011007");
    ii++; Tags.push_back("0x00280030");
    ii++; Tags.push_back("0x00180081");
    ii++; Tags.push_back("0x200510A0");
    ii++; Tags.push_back("0x00181060");
    ii++; Tags.push_back("0x20011003");
    ii++; Tags.push_back("0x00180083");
    ii++; Tags.push_back("0x00181314");
    ii++; Tags.push_back("0x00181088");
    ii++; Tags.push_back("0x00181081");
    ii++; Tags.push_back("0x00181082");
    ii++; Tags.push_back("0x00180091");
    ii++; Tags.push_back("0x00180082");
    ii++; Tags.push_back("0x00089209");
    ii++; Tags.push_back("0x00189147");
    ii++; Tags.push_back("0x200510B1");
    ii++; Tags.push_back("0x200510B2");
    ii++; Tags.push_back("0x200510B0");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("");
    ii++; Tags.push_back("0x00280006");
    ii++; Tags.push_back("0x00280002");


    //    ..

    //    ii = -1;
    //    ii++; Levels.push_back("Patient");
    //    ..

    ii = -1;
    ii++; Types.push_back("UInt16");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("UInt16");
    ii++; Types.push_back("UInt16");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Enumeration");
    ii++; Types.push_back("Int16");
    ii++; Types.push_back("Enumeration");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Int32");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("String");
    ii++; Types.push_back("String");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Float");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Double");
    ii++; Types.push_back("Enumeration");
    ii++; Types.push_back("UInt16");
    ii++; Types.push_back("UInt16");

    ii = -1;
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("Display_Orientation");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("Type_ed_es");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("Slice_Orientation");
    ii++; EnumTypes.push_back("");
    ii++; EnumTypes.push_back("");


    ii = -1;
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("SliceGap");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("AngulationAP");
    ii++; Calcs.push_back("AngulationFH");
    ii++; Calcs.push_back("AngulationRL");
    ii++; Calcs.push_back("OffcenterAP");
    ii++; Calcs.push_back("OffcenterFH");
    ii++; Calcs.push_back("OffcenterRL");
    ii++; Calcs.push_back("SliceOrient");
    ii++; Calcs.push_back("");
    ii++; Calcs.push_back("");

    //    ArraySizes.clear();
    //    ArraySizes.resize(40);
    //    ArraySizes.at(14) = "2";

    ii = -1;
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("2");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
    ii++; ArraySizes.push_back("");
}
//--------------------------------------------------------------------------------------------------
QString XMLRecReader::get_xml_rec_series_element(SeriesClass S, int ii)
{
    QVariant x, x2, x3, x4;
    QString y;

    switch(ii)
    {
    case 0: x.setValue(S.Patient_Name); y = QString(x.toString());  break;
    case 1: x.setValue(S.Examination_Name); y = QString(x.toString());  break;
    case 2: x.setValue(S.Protocol_Name); y = QString(x.toString());  break;
    case 3: x.setValue(S.Examination_Date); y = QString(x.toString());  break;
    case 4: x.setValue(S.Examination_Time); y = QString(x.toString());  break;
    case 5: x.setValue(S.Series_Data_Type); y = QString(x.toString());  break;
    case 6: x.setValue(S.Aquisition_Number); y = QString(x.toString());  break;
    case 7: x.setValue(S.Reconstruction_Number); y = QString(x.toString());  break;
    case 8: x.setValue(S.Scan_Duration); y = QString(x.toString());  break;
    case 9: x.setValue(S.Max_No_Phases); y = QString(x.toString());  break;
    case 10: x.setValue(S.Max_No_Echoes); y = QString(x.toString());  break;
    case 11: x.setValue(S.Max_No_Slices); y = QString(x.toString());  break;
    case 12: x.setValue(S.Max_No_Dynamics); y = QString(x.toString());  break;
    case 13: x.setValue(S.Max_No_Mixes); y = QString(x.toString());  break;
    case 14: x.setValue(S.Max_No_B_Values); y = QString(x.toString());  break;
    case 15: x.setValue(S.Max_No_Gradient_Orients); y = QString(x.toString());  break;
    case 16: x.setValue(S.No_Label_Types); y = QString(x.toString());  break;
    case 17: x.setValue(S.Patient_Position); y = QString(x.toString());  break;
    case 18: x.setValue(S.Preparation_Direction); y = QString(x.toString());  break;
    case 19: x.setValue(S.Technique); y = QString(x.toString());  break;
    case 20: x.setValue(S.Scan_Resolution_X); y = QString(x.toString());  break;
    case 21: x.setValue(S.Scan_Resolution_Y); y = QString(x.toString());  break;
    case 22: x.setValue(S.Scan_Mode); y = QString(x.toString());  break;
    case 23: x.setValue(S.Repetition_Times[0]);
        y = QString(x.toString());
        if(S.Repetition_Times[1] > -1.0)
        {
            x2.setValue(S.Repetition_Times[1]);
            y = y + " " + QString::number(x2.toDouble());
        }
        if(S.Repetition_Times[2] > -1.0)
        {
            x3.setValue(S.Repetition_Times[2]);
            y = y + " " + QString::number(x3.toDouble());
        }
        break;
    case 24: x.setValue(S.FOV_AP); y = QString(x.toString());  break;
    case 25: x.setValue(S.FOV_FH); y = QString(x.toString());  break;
    case 26: x.setValue(S.FOV_RL); y = QString(x.toString());  break;
    case 27: x.setValue(S.Water_Fat_Shift); y = QString(x.toString());  break;
    case 28: x.setValue(S.Angulation_AP); y = QString(x.toString());  break;
    case 29: x.setValue(S.Angulation_FH); y = QString(x.toString());  break;
    case 30: x.setValue(S.Angulation_RL); y = QString(x.toString());  break;
    case 31: x.setValue(S.Off_Center_AP); y = QString(x.toString());  break;
    case 32: x.setValue(S.Off_Center_FH); y = QString(x.toString());  break;
    case 33: x.setValue(S.Off_Center_RL); y = QString(x.toString());  break;
    case 34: x.setValue(S.Flow_Compensation); y = QString(x.toString());  break;
    case 35: x.setValue(S.Presaturation); y = QString(x.toString());  break;
    case 36: x.setValue(S.Phase_Encoding_Velocity[0]);
        y = QString(x.toString());
        if(S.Phase_Encoding_Velocity[1] > -1.0)
        {
            x2.setValue(S.Phase_Encoding_Velocity[1]);
            y = y + " " + QString(x2.toString());
        }
        if(S.Phase_Encoding_Velocity[2] > -1.0)
        {
            x3.setValue(S.Phase_Encoding_Velocity[2]);
            y = y + " " + QString(x3.toString());
        }
        break;
    case 37: x.setValue(S.MTC); y = QString(x.toString());  break;
    case 38: x.setValue(S.SPIR); y = QString(x.toString());  break;
    case 39: x.setValue(S.EPI_factor); y = QString(x.toString());  break;
    case 40: x.setValue(S.Dynamic_Scan); y = QString(x.toString());  break;
    case 41: x.setValue(S.Diffusion); y = QString(x.toString());  break;
    case 42: x.setValue(S.Diffusion_Echo_Time); y = QString(x.toString());  break;
    case 43: x.setValue(S.PhotometricInterpretation); y = QString(x.toString());  break;
    default: x.setValue(QString(""));
    }

    return y;
}
//--------------------------------------------------------------------------------------------------
QString XMLRecReader::get_xml_rec_image_element(ImInfoClass S, int ii)
{
    QVariant x, x2;
    QString y;
    switch(ii)
    {
    case 0: x.setValue(S.Pixel_Size);y = QString(x.toString());  break;
    case 1: x.setValue(S.Scan_Percentage);y = QString(x.toString());  break;
    case 2: x.setValue(S.Resolution_X);y = QString(x.toString());  break;
    case 3: x.setValue(S.Resolution_Y);y = QString(x.toString());  break;
    case 4: x.setValue(S.Rescale_Intercept);y = QString(x.toString());  break;
    case 5: x.setValue(S.Rescale_Slope);y = QString(x.toString());  break;
    case 6: x.setValue(S.Scale_Slope);y = QString(x.toString());  break;
    case 7: x.setValue(S.Window_Center);y = QString(x.toString());  break;
    case 8: x.setValue(S.Window_Width);y = QString(x.toString());  break;
    case 9: x.setValue(S.Slice_Thickness);y = QString(x.toString());  break;
    case 10: x.setValue(S.Slice_Gap);y = QString(x.toString());  break;
    case 11: x.setValue(S.Display_Orientation);y = QString(x.toString());  break;
    case 12: x.setValue(S.fMRI_Status_Indication);y = QString(x.toString());  break;
    case 13: x.setValue(S.Image_Type_Ed_Es);y = QString(x.toString());  break;
    case 14: x.setValue(S.Pixel_Spacing[0]);  x2.setValue(S.Pixel_Spacing[1]); y = QString(x.toString()) + " " + QString(x2.toString());  break;        // HAVE TO EDIT TO HANDLE ARRAYS
    case 15: x.setValue(S.Echo_Time);y = QString(x.toString());  break;
    case 16: x.setValue(S.Dyn_Scan_Begin_Time);y = QString(x.toString());  break;
    case 17: x.setValue(S.Trigger_Time);y = QString(x.toString());  break;
    case 18: x.setValue(S.Diffusion_B_Factor);y = QString(x.toString());  break;
    case 19: x.setValue(S.No_Averages);y = QString(x.toString());  break;
    case 20: x.setValue(S.Image_Flip_Angle);y = QString(x.toString());  break;
    case 21: x.setValue(S.Cardiac_Frequency);y = QString(x.toString());  break;
    case 22: x.setValue(S.Min_RR_Interval);y = QString(x.toString());  break;
    case 23: x.setValue(S.Max_RR_Interval);y = QString(x.toString());  break;
    case 24: x.setValue(S.TURBO_Factor);y = QString(x.toString());  break;
    case 25: x.setValue(S.Inversion_Delay);y = QString(x.toString());  break;
    case 26: x.setValue(S.Contrast_Type);y = QString(x.toString());  break;
    case 27: x.setValue(S.Diffusion_Anisotropy_Type);y = QString(x.toString());  break;
    case 28: x.setValue(S.Diffusion_AP);y = QString(x.toString());  break;
    case 29: x.setValue(S.Diffusion_FH);y = QString(x.toString());  break;
    case 30: x.setValue(S.Diffusion_RL);y = QString(x.toString());  break;
    case 31: x.setValue(S.Angulation_AP);y = QString(x.toString());  break;
    case 32: x.setValue(S.Angulation_FH);y = QString(x.toString());  break;
    case 33: x.setValue(S.Angulation_RL);y = QString(x.toString());  break;
    case 34: x.setValue(S.Offcenter_AP);y = QString(x.toString());  break;
    case 35: x.setValue(S.Offcenter_FH);y = QString(x.toString());  break;
    case 36: x.setValue(S.Offcenter_RL);y = QString(x.toString());  break;
    case 37: x.setValue(S.Slice_Orientation);y = QString(x.toString());  break;
    case 38: x.setValue(S.Image_Planar_Configuration);y = QString(x.toString());  break;
    case 39: x.setValue(S.Samples_Per_Pixel);y = QString(x.toString());  break;
    default: y = QString("");
    }

    return y;
}
//--------------------------------------------------------------------------------------------------
QDomElement XMLRecReader::ImageToNode( QDomDocument &d, ImInfoClass iminfo, const int slc, const int ec, const int dyn, const int ph, const int bv, const int gr, const QString lty, const QString ty, const QString sq, const int idx)
{    
    vector<QString> Names;
    vector<QString> Tags;
    vector<QString> Types;
    vector<QString> EnumTypes;
    vector<QString> Calcs;
    vector<QString> ArraySizes;

    // set a fixed hash value for a consistent order of attributes in the XML file
    qputenv("QT_HASH_SEED", "7");


    get_xml_rec_image_defs(Names, Tags, Types, EnumTypes, Calcs, ArraySizes);

    QDomElement im_info = d.createElement("Image_Info");
    QDomElement im_key = d.createElement("Key");

    vector<QDomElement> XX;
    QDomElement xx;
    QDomText  xx_value;


    QDomText  slc_value = d.createTextNode("");
    QDomText  ec_value = d.createTextNode("");
    QDomText  dyn_value = d.createTextNode("");
    QDomText  ph_value = d.createTextNode("");
    QDomText  bv_value = d.createTextNode("");
    QDomText  gr_value = d.createTextNode("");
    QDomText  lb_value = d.createTextNode("");
    QDomText  ty_value = d.createTextNode("");
    QDomText  sq_value = d.createTextNode("");
    QDomText  idx_value = d.createTextNode("");

    QDomElement Slice = d.createElement("Attribute");
    QDomElement Echo = d.createElement("Attribute");
    QDomElement Dynamic = d.createElement("Attribute");
    QDomElement Phase = d.createElement("Attribute");
    QDomElement BValue = d.createElement("Attribute");
    QDomElement Grad_Orient = d.createElement("Attribute");
    QDomElement Label_Type = d.createElement("Attribute");
    QDomElement Type = d.createElement("Attribute");
    QDomElement Sequence = d.createElement("Attribute");
    QDomElement Index = d.createElement("Attribute");

    Slice.setAttribute("Name", "Slice");      Slice.setAttribute("Tag", "0x2001100A");      Slice.setAttribute("Type", "Int32");         slc_value.setNodeValue(QString::number(slc));   Slice.appendChild(slc_value);
    Echo.setAttribute("Name", "Echo");        Echo.setAttribute("Tag", "0x00180086");       Echo.setAttribute("Type", "Int32");          ec_value.setNodeValue(QString::number(ec));     Echo.appendChild(ec_value);
    Dynamic.setAttribute("Name", "Dynamic");  Dynamic.setAttribute("Tag", "0x00200100");    Dynamic.setAttribute("Type", "Int32");       dyn_value.setNodeValue(QString::number(dyn));   Dynamic.appendChild(dyn_value);
    Phase.setAttribute("Name", "Phase");      Phase.setAttribute("Tag", "0x20011008");      Phase.setAttribute("Type", "Int32");          ph_value.setNodeValue(QString::number(ph));     Phase.appendChild(ph_value);
    BValue.setAttribute("Name", "BValue");    BValue.setAttribute("Tag", "0x20051412");     BValue.setAttribute("Type", "Int32");         bv_value.setNodeValue(QString::number(bv)); BValue.appendChild(bv_value);
    Grad_Orient.setAttribute("Name", "Grad Orient");    Grad_Orient.setAttribute("Tag", "0x20051413");     Grad_Orient.setAttribute("Type", "Int32");         gr_value.setNodeValue(QString::number(gr)); Grad_Orient.appendChild(gr_value);
    Label_Type.setAttribute("Name", "Label Type");    Label_Type.setAttribute("Tag", "0x20051429");     Label_Type.setAttribute("Type", "Enumeration"); Label_Type.setAttribute("EnumType", "Label_Type");    lb_value.setNodeValue(lty);   Label_Type.appendChild(lb_value);
    Type.setAttribute("Name", "Type");        Type.setAttribute("Tag", "0x20051011");       Type.setAttribute("Type", "Enumeration"); Type.setAttribute("EnumType", "Image_Type");    ty_value.setNodeValue(ty);                      Type.appendChild(ty_value);
    Sequence.setAttribute("Name", "Sequence");Sequence.setAttribute("Tag", "0x2005106E");   Sequence.setAttribute("Type", "Enumeration");   Sequence.setAttribute("EnumType", "Image_Sequence");  sq_value.setNodeValue(sq);                      Sequence.appendChild(sq_value);
    Index.setAttribute("Name", "Index");      Index.setAttribute("Type", "Int32");          Index.setAttribute("Calc", "Index");          idx_value.setNodeValue(QString::number(idx));   Index.appendChild(idx_value);

    im_key.appendChild(Slice);
    im_key.appendChild(Echo);
    im_key.appendChild(Dynamic);
    im_key.appendChild(Phase);
    im_key.appendChild(BValue);
    im_key.appendChild(Grad_Orient);
    im_key.appendChild(Label_Type);
    im_key.appendChild(Type);
    im_key.appendChild(Sequence);
    im_key.appendChild(Index);

    im_info.appendChild(im_key);

    for(int ii = 0; ii< 40; ii++)
    {
        xx = d.createElement("Attribute");
        xx_value = d.createTextNode("");
        xx.setAttribute("Name", Names[ii]);
        //        xx.setAttribute("Tag", Tags[ii] );
        xx.setAttribute("Type", Types[ii]);
        if(Tags[ii].length() > 0)
        {
            QString s = Tags[ii];
            xx.setAttribute("Tag", s);
        }
        if(Types[ii]=="Enumeration")
        {
            QString s = EnumTypes[ii];
            xx.setAttribute("EnumType", s);
        }
        if(Calcs[ii].length() > 0)
        {
            QString s = Calcs[ii];
            xx.setAttribute("Calc", s);
        }
        if(ArraySizes[ii].length() > 0)
        {
            QString s = ArraySizes[ii];
            xx.setAttribute("ArraySize", s);
        }

        xx_value.setNodeValue(get_xml_rec_image_element(iminfo, ii));
        xx.appendChild(xx_value);
        XX.push_back(xx);
        im_info.appendChild(XX[ii]);
    }

    // reset hash seed with new random value.
    qputenv("QT_HASH_SEED", "-1");

    return im_info;
}
//--------------------------------------------------------------------------------------------------
int XMLRecReader::saveXmlRec(QString xmlfilename, ImageC* ImageToWrite)
//int XMLRecReader::saveXmlRec(QString xmlfilename, ImageToWriteageC& ImageToWrite)
{
    qDebug() << "in XMLRecReader::saveXmlRec";

    QDomDocument document;
    QString recfilename(xmlfilename);


    if (xmlfilename.contains(".xml"))
        recfilename.replace(".xml",".rec");
    else if (xmlfilename.contains(".XML"))
        recfilename.replace(".XML",".REC");
    else    {
        qDebug("Failed to open XML/REC files for writing.");
        return -1;
    }

    QFile xmlfile(xmlfilename);
    QFile recfile(recfilename);

    //    qDebug() << "xmlfilename = " << xmlfilename;
    //    qDebug() << "recfilename = " << recfilename;

    if( !recfile.open( QIODevice::WriteOnly) )    {
        qDebug("Failed to open REC file for writing.");
        return -1;
    }

    if( !xmlfile.open( QIODevice::WriteOnly | QIODevice::Text ) )    {
        qDebug("Failed to open XML file for writing.");
        return -1;
    }

    QDomElement root = document.createElement("PRIDE_V5");

    QDomElement im_array = document.createElement("Image_Array");

    int slc=0, ec=0, dyn=0, ph=0, idx=0, bv=0, gr=0;
    QString ty="M", lty = "-";
    QString sq="IR";

    XMLRecReader X;
    double iMax = X.imageMaxValue(ImageToWrite);
    double iMin = X.imageMinValue(ImageToWrite);


    if(ImageToWrite->NumDimensions > 4)
    {
        qDebug() << "Error saving XML/REC. Unsupported dimension";
        return -1;
    }

    // scale data to [0,4095]
    for(int i=0; i<(ImageToWrite->data.size()); i++)
        ImageToWrite->data.at(i) = 4095*(ImageToWrite->data.at(i)-iMin)/(iMax-iMin);


    float slope = (iMax-iMin) / 4095;
    float intercept = iMin;

    ImInfoClass iminfo;

    //int Pixel_Size = 16;
    //double Scan_Percentage = 100;
    iminfo.Resolution_X = ImageToWrite->array_size[0];
    iminfo.Resolution_Y = ImageToWrite->array_size[1];
    iminfo.Rescale_Intercept = intercept / slope;
    iminfo.Rescale_Slope = 1.0;
    iminfo.Scale_Slope = 1 / slope;
    //    double Window_Center = 0;
    //    double Window_Width = 0;
    iminfo.Slice_Thickness = ImageToWrite->voxel_size[2];
    iminfo.Slice_Gap = ImageToWrite->voxel_size[3];
    //    QString Display_Orientation;
    //    int fMRI_Status_Indication = 0;
    //    QString Image_Type_Ed_Es;
    iminfo.Pixel_Size = 16;
    iminfo.Scan_Percentage = 100;
    iminfo.Pixel_Spacing[0] = ImageToWrite->voxel_size[0];    iminfo.Pixel_Spacing[1] = ImageToWrite->voxel_size[1];

    QDomNode im_node1;

    //    SeriesStruct series_info = ImageToWrite->series_info;
    //    QDomNodeList series_info_list = ImageToWrite->series_info_list;
    //    QDomNodeList ImageArray = ImageToWrite->ImageArray;

    // find image max and min
    double MaxValue = iMax;//imageMaxValue(im);
    double MinValue = iMin;//imageMinValue(im);

    //qDebug() << "Image MaxValue-MinValue = " << MaxValue << "  -  " << MinValue;

    if(MaxValue==MinValue)  // handle this case
        MaxValue = MaxValue + 1;


    double Rescale_Slope = 1.0;
    double Scale_Slope = 4095.0 / (MaxValue-MinValue);
    double Rescale_Intercept = (-4095.0 / (MaxValue-MinValue)) * MinValue;

    SeriesClass  SeriesInfo;

    QString patient_name("PATIENT^NAME");

    SeriesInfo.Max_No_Slices = 1;
    SeriesInfo.Max_No_Echoes = 1;
    SeriesInfo.Max_No_Dynamics = 1;
    SeriesInfo.Max_No_Phases = 1;
    SeriesInfo.Max_No_Mixes = 1;
    SeriesInfo.Max_No_B_Values = 1;
    SeriesInfo.Max_No_Gradient_Orients = 1;
    SeriesInfo.No_Label_Types = 0;
    SeriesInfo.Max_No_Types = 1;

    if(ImageToWrite->NumDimensions>=3)
        SeriesInfo.Max_No_Slices = ImageToWrite->array_size[2];
    if(ImageToWrite->NumDimensions>=4)
        SeriesInfo.Max_No_Echoes = ImageToWrite->array_size[3];

    QDomElement series_info_node = SeriesInfoToNode( document , patient_name, SeriesInfo.Max_No_Phases, SeriesInfo.Max_No_Echoes, SeriesInfo.Max_No_Slices, SeriesInfo.Max_No_Dynamics,
                                                     SeriesInfo.Max_No_Mixes,SeriesInfo.Max_No_B_Values,SeriesInfo.Max_No_Gradient_Orients,SeriesInfo.Max_No_Types);

    root.appendChild(series_info_node);

    //    int imageSize_bytes = imageInfo[0].Resolution_X * imageInfo[0].Resolution_Y * imageInfo[0].Pixel_Size / 8;
    int imageSize_bytes = ImageToWrite->array_size[0] * ImageToWrite->array_size[1] * 2; // assuming 16-bit
    char *data = new char[imageSize_bytes];
    int16_t * data_int = (int16_t*)(data);

    if(0)
    {
        qDebug() << "num dimensions = " << ImageToWrite->NumDimensions;
        qDebug() << "size = " << ImageToWrite->array_size[0] << ImageToWrite->array_size[1] << ImageToWrite->array_size[2] << ImageToWrite->array_size[3];
        qDebug() << "SeriesInfo.Max_No_Slices = " << SeriesInfo.Max_No_Slices;
        qDebug() << "SeriesInfo.Max_No_Dynamics = " << SeriesInfo.Max_No_Dynamics;
        qDebug() << "SeriesInfo.Max_No_Phases = " << SeriesInfo.Max_No_Phases;
        qDebug() << "SeriesInfo.Max_No_B_Values = " << SeriesInfo.Max_No_B_Values;
        qDebug() << "SeriesInfo.Max_No_Gradient_Orients = " << SeriesInfo.Max_No_Gradient_Orients;
        qDebug() << "SeriesInfo.No_Label_Types = " << SeriesInfo.No_Label_Types;
        qDebug() << "SeriesInfo.Max_No_Types = " << SeriesInfo.Max_No_Types;
        //    if(SeriesInfo.No_Label_Types == 0)
        //        SeriesInfo.No_Label_Types = 1;
    }

    int ImageIndex = 0;

    for(int Slice = 0; Slice < SeriesInfo.Max_No_Slices; Slice++)    {
        for(int Echo = 0; Echo < SeriesInfo.Max_No_Echoes; Echo++)        {
            for(int Dynamic = 0; Dynamic < SeriesInfo.Max_No_Dynamics; Dynamic++)            {
                for(int Phase =0; Phase < SeriesInfo.Max_No_Phases; Phase++)                {
                    for(int BValue = 0; BValue < SeriesInfo.Max_No_B_Values; BValue++)                    {
                        for(int Grad_Orient = 0; Grad_Orient < SeriesInfo.Max_No_Gradient_Orients; Grad_Orient++)                        {
                            for(int LType = 0; LType < (SeriesInfo.No_Label_Types+1); LType++)                            {
                                //                                QString Label_Type = "-";
                                //                                QString Label_Type = SeriesInfo.EnumLabelType.at(LType);
                                for(int ty = 0; ty < SeriesInfo.Max_No_Types; ty++)                                {

                                    //                                    QString Type = "M";
                                    //                                    QString Type = SeriesInfo.EnumImageType.at(ty);
                                    //                                    QString Sequence = imageKey[0].Sequence;

                                    iminfo.Rescale_Slope = Rescale_Slope;
                                    iminfo.Scale_Slope = Scale_Slope;
                                    iminfo.Rescale_Intercept = Rescale_Intercept;

                                    im_node1 =  ImageToNode(document, iminfo, Slice+1, Echo+1, Dynamic+1, Phase+1,
                                                            BValue+1, Grad_Orient+1, "-", "M", "SE", ImageIndex);

                                    im_array.appendChild(im_node1);


                                    float v;
                                    if(ImageToWrite->NumDimensions == 2)                                            {
                                        for(int y=0; y<ImageToWrite->array_size[1]; y++)
                                            for(int x=0; x<ImageToWrite->array_size[0]; x++) {
                                                //*(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->data2d[x][y];
                                                *(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->getVoxel(x, y);
                                            }
                                    }
                                    else if(ImageToWrite->NumDimensions == 3)                                        {
                                        // qDebug() << "slice = " << Slice;
                                        for(int y=0; y<ImageToWrite->array_size[1]; y++)
                                            for(int x=0; x<ImageToWrite->array_size[0]; x++)  {
                                                //*(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->data3d[x][y][Slice];
                                                *(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->getVoxel(x,y,Slice);
                                            }
                                    }
                                    else if(ImageToWrite->NumDimensions == 4)                                        {
                                        for(int y=0; y<ImageToWrite->array_size[1]; y++)
                                            for(int x=0; x<ImageToWrite->array_size[0]; x++) {
                                                //*(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->data4d[x][y][Slice][Echo];
                                                *(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->getVoxel(x,y,Slice,Echo);
                                            }
                                    }

                                    recfile.seek(ImageIndex*imageSize_bytes);

                                    int num_read_bytes = recfile.write(data, imageSize_bytes);
                                    if(num_read_bytes < imageSize_bytes)
                                    {
                                        qDebug() << "ERROR writing the raw data";
                                        return -1;
                                    }
                                    ImageIndex++;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //delete [] data;

    recfile.close();

    root.appendChild(im_array);
    document.appendChild(root);
    QTextStream ts( &xmlfile );
    ts << document.toString();
    ts.flush();
    xmlfile.close();


    qDebug() << "Done writing REC file.";

    return 0;
}
//--------------------------------------------------------------------------------------------------
int XMLRecReader::saveXmlRec(QString xmlfilename, ImageC* ImageToSave, ImageC* HeaderImage, bool anonymous)
{
    QDomDocument document;
    QString recfilename(xmlfilename);

    if (xmlfilename.contains(".xml"))
        recfilename.replace(".xml",".rec");
    else if (xmlfilename.contains(".XML"))
        recfilename.replace(".XML",".REC");
    else    {
        qDebug("saveXmlRec: ERROR: File name should contain xml or XML extension.");
        return -1;
    }

    QFile xmlfile(xmlfilename);
    QFile recfile(recfilename);

    if( !recfile.open( QIODevice::WriteOnly) )    {
        qDebug("Failed to open REC file for writing.");
        return -1;
    }

    if( !xmlfile.open( QIODevice::WriteOnly | QIODevice::Text ) )    {
        qDebug("Failed to open XML file for writing.");
        return -1;
    }

    // copy ImageToSave to ImageToWrite for scaling
    ImageC* ImageToWrite = new ImageC(ImageToSave, true);

    QDomElement root = document.createElement("PRIDE_V5");

    QDomElement im_array = document.createElement("Image_Array");


    int slc=0, ec=0, dyn=0, ph=0, idx=0, bv=0, gr=0;
    QString ty="M", lty = "-";
    QString sq="IR";

    XMLRecReader X;
    double iMax = X.imageMaxValue(ImageToWrite);
    double iMin = X.imageMinValue(ImageToWrite);

    int NumImages = 1;
    for(int dd = 2; dd < ImageToWrite->numDimensions(); dd++)
        NumImages *= ImageToWrite->arraySize(dd);


    if(ImageToWrite->NumDimensions > 4)
    {
        qDebug() << "Error saving XML/REC. Unsupported dimension";
        return -1;
    }

    // scale data to [0,4095]
    for(int i=0; i<(ImageToWrite->data.size()); i++)
        ImageToWrite->data.at(i) = 4095*(ImageToWrite->data.at(i)-iMin)/(iMax-iMin);


    ImInfoClass iminfo;
    iminfo = HeaderImage->image_info.at(0);

    QDomNode im_node1;

    // find image max and min
    double MaxValue = iMax;//imageMaxValue(im);
    double MinValue = iMin;//imageMinValue(im);

    //qDebug() << "MaxValue-MinValue = " << MaxValue << "  -  " << MinValue;

    if(MaxValue==MinValue)  // handle this case
        MaxValue = MaxValue + 1;

    double Rescale_Slope = 1.0;
    double Scale_Slope = 4095.0 / (MaxValue-MinValue);
    double Rescale_Intercept = (-4095.0 / (MaxValue-MinValue)) * MinValue;

    SeriesClass  SeriesInfo = HeaderImage->series_info;
    QFileInfo info1(xmlfilename);

    SeriesInfo.Protocol_Name = info1.baseName();
    //qDebug() << "SeriesInfo.Protocol_Name = " << info1.baseName();
    if(anonymous)
        SeriesInfo.Patient_Name = "Anonymous";

    // TO BE UPDATED
    if(ImageToWrite->numDimensions() != HeaderImage->numDimensions())    {
        if(ImageToWrite->numDimensions() < HeaderImage->numDimensions())        {
            if(ImageToWrite->numDimensions()==2)            {
                SeriesInfo.Max_No_Slices = 1;
                SeriesInfo.Max_No_Echoes = 1;
                SeriesInfo.EnumImageType.clear(); SeriesInfo.EnumImageType.push_back("M");
//                QString ttt = SeriesInfo.EnumLabelType.at(1); SeriesInfo.EnumLabelType.clear(); SeriesInfo.EnumLabelType.push_back(ttt);
                SeriesInfo.EnumSequence.clear(); SeriesInfo.EnumSequence.push_back("FFE");
                SeriesInfo.Max_No_Types = 1;
                SeriesInfo.Max_No_Mixes = 1;
            }
            if(ImageToWrite->numDimensions()==3)            {
                SeriesInfo.Max_No_Slices = ImageToWrite->arraySize(2);
                SeriesInfo.Max_No_Echoes = 1;
                SeriesInfo.EnumImageType.clear(); SeriesInfo.EnumImageType.push_back("M");
//                QString ttt = SeriesInfo.EnumLabelType.at(1); SeriesInfo.EnumLabelType.clear(); SeriesInfo.EnumLabelType.push_back(ttt);
                SeriesInfo.EnumSequence.clear(); SeriesInfo.EnumSequence.push_back("FFE");
                SeriesInfo.Max_No_Types = 1;
                SeriesInfo.Max_No_Mixes = 1;
            }
            if(ImageToWrite->numDimensions()==4)            {
                SeriesInfo.Max_No_Slices = ImageToWrite->arraySize(2);
                SeriesInfo.Max_No_Echoes = ImageToWrite->arraySize(3);
                SeriesInfo.EnumImageType.clear(); SeriesInfo.EnumImageType.push_back("M");
//                QString ttt = SeriesInfo.EnumLabelType.at(1); SeriesInfo.EnumLabelType.clear(); SeriesInfo.EnumLabelType.push_back(ttt);
                SeriesInfo.EnumSequence.clear(); SeriesInfo.EnumSequence.push_back("FFE");
                SeriesInfo.Max_No_Types = 1;
                SeriesInfo.Max_No_Mixes = 1;
            }
        }
    }

    QDomElement series_info_node = SeriesInfoToNode(document , SeriesInfo);

    //    if(SeriesInfo.No_Label_Types == 0)
    //        SeriesInfo.No_Label_Types = 1;

    //    qDebug() << "No Label Types = " << SeriesInfo.No_Label_Types;

    root.appendChild(series_info_node);

    // read the REC file
    //    int imageSize_bytes = imageInfo[0].Resolution_X * imageInfo[0].Resolution_Y * imageInfo[0].Pixel_Size / 8;
    int imageSize_bytes = ImageToWrite->array_size[0] * ImageToWrite->array_size[1] * 2; // assuming 16-bit
    char *data = new char[imageSize_bytes];
    int16_t * data_int = (int16_t*)(data);


    if(0)
    {
        qDebug() << "num dimensions = " << ImageToWrite->NumDimensions;
        qDebug() << "size = " << ImageToWrite->array_size[0] << ImageToWrite->array_size[1] << ImageToWrite->array_size[2] << ImageToWrite->array_size[3];
        qDebug() << "SeriesInfo.Max_No_Slices = " << SeriesInfo.Max_No_Slices;
        qDebug() << "SeriesInfo.Max_No_Dynamics = " << SeriesInfo.Max_No_Dynamics;
        qDebug() << "SeriesInfo.Max_No_Phases = " << SeriesInfo.Max_No_Phases;
        qDebug() << "SeriesInfo.Max_No_B_Values = " << SeriesInfo.Max_No_B_Values;
        qDebug() << "SeriesInfo.Max_No_Gradient_Orients = " << SeriesInfo.Max_No_Gradient_Orients;
        qDebug() << "SeriesInfo.No_Label_Types = " << SeriesInfo.No_Label_Types;
        qDebug() << "SeriesInfo.Max_No_Types = " << SeriesInfo.Max_No_Types;
        //    if(SeriesInfo.No_Label_Types == 0)
        //        SeriesInfo.No_Label_Types = 1;
    }

    int kk = 0;
    int num_write_bytes;
    ImKeyClass key;


    int ImageIndex = 0;
    for(int Slice = 0; Slice < SeriesInfo.Max_No_Slices; Slice++)
    {
        for(int Echo = 0; Echo < SeriesInfo.Max_No_Echoes; Echo++)
        {
            for(int Dynamic = 0; Dynamic < SeriesInfo.Max_No_Dynamics; Dynamic++)
            {
                for(int Phase =0; Phase < SeriesInfo.Max_No_Phases; Phase++)
                {
                    for(int BValue = 0; BValue < SeriesInfo.Max_No_B_Values; BValue++)
                    {
                        for(int Grad_Orient = 0; Grad_Orient < SeriesInfo.Max_No_Gradient_Orients; Grad_Orient++)
                        {
                            for(int LType = 0; LType < (SeriesInfo.No_Label_Types+1); LType++)
                            {
                                //                                QString Label_Type = "-";
                                //                                QString Label_Type = SeriesInfo.EnumLabelType.at(LType);
                                for(int ty = 0; ty < SeriesInfo.Max_No_Types; ty++)
                                {
                                    for(int sq = 0; sq < SeriesInfo.Max_No_Mixes; sq++)
                                    {
                                        QString strImageType = SeriesInfo.EnumImageType.at(ty);
                                        QString strLabelType = SeriesInfo.EnumLabelType.at(LType);
                                        QString strSequence = SeriesInfo.EnumSequence.at(sq);

                                        // find the image index
                                        kk = 0;//ImageIndex;
                                        //for(kk = 0; kk<NumImages; kk++)
                                        for(kk = 0; kk<HeaderImage->image_info.size(); kk++)
                                        {
                                            key = HeaderImage->image_info.at(kk).Key;

                                            if(0 && key.Slice==1)
                                            {
                                                QString KeyString = makeKeyString(key);
                                                qDebug() << "KeyString = " << KeyString;
                                                qDebug() << "slice = " << (key.Slice) << (Slice+1);
                                                qDebug() << "echo = " << key.Echo << (Echo+1);
                                                qDebug() << "dyn = " << key.Dynamic << (Dynamic+1);
                                                qDebug() << "ph = "  << key.Phase << (Phase+1);
                                                qDebug() << "b = " << key.BValue << (BValue+1);
                                                qDebug() << "gr = " << key.Grad_Orient << (Grad_Orient+1);
                                                qDebug() << "ty = " << key.Type << strImageType;
                                                qDebug() << "label = "  << key.Label_Type << strLabelType;
                                                qDebug() << "sq = " << key.Sequence << strSequence;
                                            }


                                            if(     (key.Slice == (Slice+1)) &&
                                                    (key.Echo == (Echo+1)) &&
                                                    (key.Dynamic == (Dynamic+1)) &&
                                                    (key.Phase == (Phase+1)) &&
                                                    (key.BValue == (BValue+1)) &&
                                                    (key.Grad_Orient == (Grad_Orient+1)) &&
                                                    (key.Type == strImageType)    &&
                                                    (key.Label_Type == strLabelType)    &&
                                                    (key.Sequence == strSequence) )
                                                break; // image index found
                                        }




                                        if(kk == HeaderImage->image_info.size())
                                        {
                                            qDebug() << "ERROR writing the raw data: no matching image header is found.";
                                            qDebug() << "kk = " << kk;
                                            qDebug() << "NumImages = " << NumImages;
                                            return -1;
                                        }

                                        ImKeyClass imageKey = HeaderImage->image_info.at(kk).Key;



                                        imageKey.Index = ImageIndex;

                                        //ImageIndex =  HeaderImage->ImageIndex[KeyString];

                                        //qDebug() << "Key at " << kk << "Has Slice " << Slice+1 << "and Index" << ImageIndex;
                                        //                                    iminfo = HeaderImage->image_info.at(ImageIndex);
                                        iminfo = HeaderImage->image_info.at(kk);

                                        //iminfo = HeaderImage->image_info.at(HeaderImage->ImageIndex(KeyString));

                                        iminfo.Rescale_Slope = Rescale_Slope;
                                        iminfo.Scale_Slope = Scale_Slope;
                                        iminfo.Rescale_Intercept = Rescale_Intercept;

                                        //                                    im_node1 =  ImageToNode(document, iminfo, Slice+1, Echo+1, Dynamic+1, Phase+1,
                                        //                                                            BValue+1, Grad_Orient+1, "-", "M", "SE", ImageIndex);

                                        im_node1 =  ImageToNode(document, iminfo, Slice+1, Echo+1, Dynamic+1, Phase+1,
                                                                BValue+1, Grad_Orient+1, "-", imageKey.Type, imageKey.Sequence, ImageIndex);

                                        im_array.appendChild(im_node1);

                                        // qDebug() << "ImageToWrite->NumDimensions" << ImageToWrite->NumDimensions;
                                        if(ImageToWrite->NumDimensions == 2)                                            {
                                            for(int y=0; y<ImageToWrite->array_size[1]; y++)
                                                for(int x=0; x<ImageToWrite->array_size[0]; x++) {
                                                    *(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->getVoxel(x, y);
                                                }
                                        }
                                        else if(ImageToWrite->NumDimensions == 3)                                        {
                                            // qDebug() << "slice = " << Slice;
                                            for(int y=0; y<ImageToWrite->array_size[1]; y++)
                                                for(int x=0; x<ImageToWrite->array_size[0]; x++)  {
                                                    *(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->getVoxel(x,y,Slice);
                                                }
                                        }
                                        else if(ImageToWrite->NumDimensions == 4)                                        {
                                            for(int y=0; y<ImageToWrite->array_size[1]; y++)
                                                for(int x=0; x<ImageToWrite->array_size[0]; x++) {
                                                    *(data_int+x+ImageToWrite->array_size[0]*y) = ImageToWrite->getVoxel(x,y,Slice,Echo);
                                                }
                                        }

                                        recfile.seek(ImageIndex*imageSize_bytes);

                                        num_write_bytes = recfile.write(data, imageSize_bytes);
                                        if(num_write_bytes < imageSize_bytes)
                                        {
                                            qDebug() << "ERROR writing the raw data";
                                            return -1;
                                        }

                                        ImageIndex++;
                                        //                                    count++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    //delete [] data;

    recfile.close();

    root.appendChild(im_array);
    document.appendChild(root);
    QTextStream ts( &xmlfile );
    ts << document.toString();
    ts.flush();
    xmlfile.close();


    qDebug() << "Done writing REC file.";

    return 0;
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMaxValue(const vector<float> &v1d)
{
    return *(std::max_element(v1d.begin(), v1d.end()));
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMaxValue(const vector<vector<float> > &v2d)
{
    double M1=0.0;
    double M = v2d[0][0];

    for(int i = 0; i <v2d.size(); i++)
    {
        M1 = vectorMaxValue(v2d[i]);
        if( M < M1 )   M = M1;
    }

    return M;
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMaxValue(const vector<vector<vector<float> > > &v3d)
{
    double M1=0.0;
    double M = v3d[0][0][0];
    for(int i = 0; i <v3d.size(); i++)
    {
        M1 = vectorMaxValue(v3d[i]);
        if( M < M1 )
            M = M1;
    }

    return M;
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMaxValue(const vector<vector<vector<vector<float> > > > &v4d)
{
    double M1, M = v4d[0][0][0][0];
    for(int i = 0; i <v4d.size(); i++)
    {
        M1 = vectorMaxValue(v4d[i]);
        if( M < M1 )
            M = M1;
    }

    return M;
}
//--------------------------------------------------------------------------------------------------
int XMLRecReader::indexofSmallestElement(const vector<float> &v)
{
    int index = 0;

    for(int i = 1; i < v.size(); i++)
        if(v[i] < v[index])
            index = i;

    return index;
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMinValue(const vector<float> &v)
{
    return *(std::min_element(v.begin(), v.end()));
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMinValue(const vector<vector<float> > &v2d)
{
    double M1, M = v2d[0][0];
    for(int i = 0; i <v2d.size(); i++)
    {
        M1 = vectorMinValue(v2d[i]);
        if( M > M1 )
            M = M1;
    }

    return M;
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMinValue(const vector<vector<vector<float> > > &v3d)
{
    double M1, M = v3d[0][0][0];
    for(int i = 0; i <v3d.size(); i++)
    {
        M1 = vectorMinValue(v3d[i]);
        if( M > M1 )
            M = M1;
    }

    return M;
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::vectorMinValue(const vector<vector<vector<vector<float> > > > &v4d)
{
    double M1, M = v4d[0][0][0][0];
    for(int i = 0; i <v4d.size(); i++)
    {
        M1 = vectorMinValue(v4d[i]);
        if( M > M1 )
            M = M1;
    }

    return M;
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::imageMaxValue(ImageC* im)
{
    return vectorMaxValue(im->data);
}
//--------------------------------------------------------------------------------------------------
double XMLRecReader::imageMinValue(ImageC* im)
{
    return vectorMinValue(im->data);
}
//--------------------------------------------------------------------------------------------------
