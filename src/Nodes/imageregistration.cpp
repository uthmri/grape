/* ImageRegistration.cpp
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
#include <Nodes/imageregistration.h>
#include <QMessageBox>
#include <common.h>
#include <xmlreclib.h>

//--------------------------------------------------------------------------------------------------

ImageRegistration::ImageRegistration(QObject *parent)
{
    setParent(parent);
    node_type = "ImageRegistration";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
ImageRegistration::~ImageRegistration() { }
//--------------------------------------------------------------------------------------------------
void ImageRegistration::init()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageRegistration::isAllInputsReady()
{
    Imageregistration_Type = nodeItem->propertyValue("Registration Method").toString();
    Work_Model = nodeItem->propertyValue("Work Model").toString();

    if( (Work_Model == "Estimate & Reslice")||(Work_Model == "Estimate"))
    {
        //setInputMode(int index, QString type, QString defaultvalue,QString mandatory, QString tooltip,QString helptext);
        setInputMode(1, "string", "1", "true", "moving image", "file name of moving image");
        setInputMode(2, "string", "1", "true", "fixed image", "file name of fixed image");
        setInputMode(3, "string", "1", "false", "not used", "not used");
        setInputMode(4, "string", "1", "false", "not used", "not used");
        setOutputMode(1, "string", "1", "true", "registered image", "file name of registered image");
        setOutputMode(2, "string", "1", "true", "affine transform txt", "file name of affine transform txt");
        if((Imageregistration_Type =="Elastic")||(Imageregistration_Type =="Diffeomophic"))
            setOutputMode(3, "string", "1", "true", "deformation field", "file name of deformation field");
        else
            setOutputMode(3, "string", "1", "false", "not used", "not used");
    }
    else if(Work_Model == "Reslice")
    {
        setInputMode(1, "string", "1", "true", "moving image", "file name of moving image");
        setInputMode(2, "string", "1", "true", "fixed image", "file name of fixed image");
        setInputMode(3, "string", "1", "true", "affine transform txt", "file name of affine transform txt");
        if((Imageregistration_Type =="Elastic")||(Imageregistration_Type =="Diffeomophic"))
            setInputMode(4, "string", "1", "true", "deformation field", "file name of deformation field");
        else
            setInputMode(4, "string", "1", "false", "not used", "not used");
        setOutputMode(1, "string", "1", "true", "registered image", "file name of registered image");
        setOutputMode(2, "string", "1", "false", "not used", "not used");
        setOutputMode(3, "string", "1", "false", "not used", "not used");
    }

    return Node::isAllInputsReady();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistration::set_input(int in_port_id, QVariant* varvalue)
{    
    Node::set_input(in_port_id, varvalue);//    for (int i =0; i<getNi().NumInputs; i++)
    //    {
    //        qDebug()<<"   in_id="<<in_id<<", varvalue="<<varvalue->toString();
    //    }


}
//--------------------------------------------------------------------------------------------------
void ImageRegistration::set_input_by_index(int in_id, QVariant* varvalue)
{
    Imageregistration_Type = nodeItem->propertyValue("Registration Method").toString();
    Similarity_Fuc = nodeItem->propertyValue("Similarity Measure").toString();
    Work_Model = nodeItem->propertyValue("Work Model").toString();
    param1 = nodeItem->propertyValue("Param1").toString();
    param2 = nodeItem->propertyValue("Param2").toString();
    Out_Image_Path = nodeItem->propertyValue("OutputPath").toString();
    QFileInfo info1(Out_Image_Path);
    QString directory = info1.path();
    QString baseName =  info1.baseName();
    baseNamePath =directory+ "/"+ baseName;

    if(in_id==0)
    {
        vI1 = varvalue;
        src_assigned[in_id] = true;
        Moving_Image_Path =  vI1->toString();
    }
    if(in_id==1)
    {
        vI2 = varvalue;
        src_assigned[in_id] = true;
        Fixed_Image_Path =  vI2->toString();
    }
    if((in_id==2)&&(Work_Model == "Reslice"))
    {
        vI3 = varvalue;
        src_assigned[in_id] = true;
        Text_Transform_File =  vI3->toString();
    }
    else if((in_id==2)&&(Work_Model != "Reslice"))
    {
        qDebug()<<"Error-3: registration type should be Reslice. in_id="<<in_id;
        return ;
    }
    if((in_id==3)&&(Work_Model == "Reslice"))
    {
        if((Imageregistration_Type =="Affine")||(Imageregistration_Type =="Rigid_Body"))
        {
            qDebug()<<"Error: Imageregistration_Type should be Estimate. in_id="<<in_id;
            return ;
        }
        vI4 = varvalue;
        src_assigned[in_id] = true;
        Nii_Transform_File =  vI4->toString();
    }
    else if((in_id==3)&&(Work_Model != "Reslice"))
    {
         qDebug()<<"Error-4: Work_Model should be Estimate with Reslice. in_id="<<in_id;
         return ;
    }

}
//--------------------------------------------------------------------------------------------------
QVariant *ImageRegistration::get_output(int out_port_id)
{    
    return Node::get_output(out_port_id);
}
//--------------------------------------------------------------------------------------------------
QVariant *ImageRegistration::get_output_by_index(int out_id)
{
    return Node::get_output_by_index(out_id);
}
//------------------------------------------------------------------------------
int ImageRegistration::execute()
{

    clearOutputData();
    Out_Image_Path = nodeItem->propertyValue("OutputPath").toString();
    QFileInfo info1(Out_Image_Path);
    QString directory = info1.path();
    QString baseName =  info1.baseName();
    baseNamePath =directory+ "/"+ baseName;


    qDebug() <<"Out_Image_Path = " <<Out_Image_Path;
    qDebug() <<"baseName = " <<baseName;
    qDebug() <<"directory = " <<directory;
    qDebug() <<"baseNamePath = " <<baseNamePath;
    //return -1;

    if(nodeItem->containsProperty("Param1"))
    {
        param1 = nodeItem->propertyValue("Param1").toString();
    }
    if(nodeItem->containsProperty("Param2"))
    {
        param2= nodeItem->propertyValue("Param2").toString();
    }
    if(nodeItem->containsProperty("OutputPath"))
    {
        Out_Image_Path = nodeItem->propertyValue("OutputPath").toString();
    }
    else
    {
        qDebug() << "Cannot find Out_Image_Path = "<<Out_Image_Path;
        return -1;
    }

    if(Imageregistration_Type == "Rigid-Body")
    {
        if( Work_Model =="Estimate & Reslice" ) {
            QString command1 = commandPath +"/ANTS 3 -m "+Similarity_Fuc+"["+Fixed_Image_Path+","+Moving_Image_Path+",1,32] -o "+ Out_Image_Path+" -i 0 --rigid-affine true  ";//rigid
            qDebug() << "command1 =" << command1;
            system(command1.toUtf8());

//            QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ ' '+ Out_Image_Path+ ' ' + baseNamePath + "Affine.txt  -R"+ ' '+ Fixed_Image_Path;
            QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ ' '+ Out_Image_Path+ " -R " + Fixed_Image_Path + ' ' + baseNamePath + "Affine.txt";

            qDebug() << "command2 =" << command2;
            system(command2.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            qv2 = new QVariant(Text_Transform_File);
            outData.clear();
            outData.push_back(qv1);
            outData.push_back(qv2);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            setReadyOutPorts(8 + getNi().NumInputs + 1, true);
            markExecuted();
        }
        if( Work_Model =="Estimate" ) {
            QString command1 = commandPath +"/ANTS 3 -m "+Similarity_Fuc+"["+Fixed_Image_Path+","+Moving_Image_Path+",1,32] -o "+ Out_Image_Path+" -i 0 --rigid-affine true  ";//rigid
            qDebug() << "command1 =" << command1;
            system(command1.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            qv2 = new QVariant(Text_Transform_File);
            outData.clear();
            outData.push_back(qv1);
            outData.push_back(qv2);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            setReadyOutPorts(8 + getNi().NumInputs + 1, true);
            markExecuted();
        }
        if( Work_Model =="Reslice" ) {
            QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ " " + Out_Image_Path+ " " + Text_Transform_File +" -R " + Fixed_Image_Path;
            qDebug() << "command2 =" << command2;
            system(command2.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            outData.clear();
            outData.push_back(qv1);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            markExecuted();
        }
    }  // end of if(Imageregistration_Type=="Rigid-Body")

    if(Imageregistration_Type=="Affine")
    {
        if( Work_Model =="Estimate & Reslice" ) {
            QString command1 = commandPath +"/ANTS 3 -m "+Similarity_Fuc+"["+Fixed_Image_Path+"," + Moving_Image_Path + "] -i 0 -o " + baseNamePath +" --number-of-affine-iterations 3x3x3 --rigid-affine false"; //  --use-Histogram-Matching
            //QString command1 = commandPath +" 3 -m "+Similarity_Fuc+"["+Fixed_Image_Path+"," + Moving_Image_Path + "] -i 0 -o " + baseNamePath +" --number-of-affine-iterations 50x50x50x50 --rigid-affine false"; //  --use-Histogram-Matching
            qDebug() << "command1 =" << command1;
            system(command1.toUtf8());

           // QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ ' '+ Out_Image_Path+ ' ' + baseNamePath + "Affine.txt  -R " + Fixed_Image_Path;
            QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ ' '+ Out_Image_Path+ " -R " + Fixed_Image_Path + ' ' + baseNamePath + "Affine.txt";
            qDebug() << "command2 =" << command2;
            system(command2.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            qv2 = new QVariant(Text_Transform_File);
            outData.clear();
            outData.push_back(qv1);
            outData.push_back(qv2);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            setReadyOutPorts(8 + getNi().NumInputs + 1, true);
            markExecuted();
        }
        if( Work_Model =="Estimate" ) {
            QString command1 = commandPath +"/ANTS 3 -m "+Similarity_Fuc+"["+Fixed_Image_Path+"," + Moving_Image_Path + "] -i 0 -o " + baseNamePath +" --number-of-affine-iterations 3x3x3 --rigid-affine false"; //  --use-Histogram-Matching
            qDebug() << "command1 =" << command1;
            system(command1.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            qv2 = new QVariant(Text_Transform_File);
            outData.clear();
            outData.push_back(qv1);
            outData.push_back(qv2);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            setReadyOutPorts(8 + getNi().NumInputs + 1, true);
            markExecuted();
        }
        if( Work_Model =="Reslice" ) {
            QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ " " + Out_Image_Path+ " " + Text_Transform_File +" -R " + Fixed_Image_Path;
            qDebug() << "command2 =" << command2;
            system(command2.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            outData.clear();
            outData.push_back(qv1);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            markExecuted();
        }
    } // end of if(Imageregistration_Type=="Affine")

    if((Imageregistration_Type=="Elastic")||(Imageregistration_Type=="Diffeomorphic"))
    {
        if( Work_Model =="Estimate & Reslice")
        {
            QString command1 = commandPath +"/ANTS 3 -m "+Similarity_Fuc+"["+Fixed_Image_Path+"," + Moving_Image_Path + ","+param1+"] -o " + baseNamePath + ".nii.gz " + param2;
            qDebug() << "command1 =" << command1;
            system(command1.toUtf8());

            QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ " "+ Out_Image_Path+ " -R " + Fixed_Image_Path +" " + Nii_Transform_File+" " + Text_Transform_File;
            qDebug() << "command2 =" << command2;
            system(command2.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            qv2 = new QVariant(Text_Transform_File);
            qv3 = new QVariant(Nii_Transform_File);
            outData.clear();
            outData.push_back(qv1);
            outData.push_back(qv2);
            outData.push_back(qv3);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            setReadyOutPorts(8 + getNi().NumInputs + 1, true);
            setReadyOutPorts(8 + getNi().NumInputs + 2, true);
            markExecuted();
        }
        if(  Work_Model =="Estimate"  )
        {
            QString command1 = commandPath +"/ANTS 3 -m "+Similarity_Fuc+"["+Fixed_Image_Path+"," + Moving_Image_Path + ","+param1+"] -o " + baseNamePath + ".nii.gz " + param2;
            qDebug() << "command1 =" << command1;
            system(command1.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            qv2 = new QVariant(Text_Transform_File);
            qv3 = new QVariant(Nii_Transform_File);
            outData.clear();
            outData.push_back(qv1);
            outData.push_back(qv2);
            outData.push_back(qv3);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            setReadyOutPorts(8 + getNi().NumInputs + 1, true);
            setReadyOutPorts(8 + getNi().NumInputs + 2, true);
            markExecuted();
        }
        if( Work_Model =="Reslice" )
        {
            QString command2 = commandPath +"/WarpImageMultiTransform 3 "+ Moving_Image_Path+ " "+ Out_Image_Path+ " -R " + Fixed_Image_Path +" " + Nii_Transform_File+" " + Text_Transform_File;
            qDebug() << "command2 =" << command2;
            system(command2.toUtf8());

            qv1 = new QVariant(Out_Image_Path);
            outData.clear();
            outData.push_back(qv1);
            setReadyOutPorts(8 + getNi().NumInputs + 0, true);
            markExecuted();
        }
    }// end of if(Imageregistration_Type=="Elastic")
    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageRegistration::validateDesign()
{
    // check the ANTS executable exists
    bool ANTS_found = false;
    QSysInfo sysInfo;
    QString osType = sysInfo.productType();

    // check the user path, if any ............................
    commandPath = nodeItem->propertyValue("CommandPath").toString();
    // if no user path, check the config filr
    if(!commandPath.length())
    {
        commandPath = this->getDiagram()->mainPipeline.getAppPath("ANTS");
        if(commandPath.length()==0)
        {
            qDebug() << " in validateDesign, ANTS PATH is not correct.";
            return -1;
        }
    }

    // check OS type ............................................
    QFileInfo checkFile(commandPath);
    commandPath = checkFile.path() + "/" + checkFile.baseName();
    QString ANTS_file;
    if (osType=="windows") ANTS_file = commandPath + "/ANTS.exe";
    else ANTS_file = commandPath + "/ANTS";
    QFileInfo checkFile1(ANTS_file);
    if (checkFile1.exists() && checkFile1.isFile())
        ANTS_found = true;
    else
    {
        qDebug() << "Cannot find path to ANTS";
        return -1;
    }


    Out_Image_Path = nodeItem->propertyValue("OutputPath").toString();

    if(Out_Image_Path.length()<1)
    {
        qDebug() << "ERROR: empty output file";
        return -1;
    }


    return 0;
}
//--------------------------------------------------------------------------------------------------
int ImageRegistration::validateRun()
{
    QFileInfo checkFile(Fixed_Image_Path);
    if (!(checkFile.exists() && checkFile.isFile()))
    {
        qDebug()<<" in validateRun, Fixed Image doesn't exist.";
        return -1;
    }

    QFileInfo checkFile1(Moving_Image_Path);
    if (!(checkFile1.exists() && checkFile1.isFile()))
    {
        qDebug()<<" in validateRun, Moving Image doesn't exist.";
        return -1;
    }

    Text_Transform_File =baseNamePath+"Affine.txt";
    Nii_Transform_File =baseNamePath+"Warp.nii.gz";
    if((Imageregistration_Type == "Affine")||(Imageregistration_Type == "Rigid-Body"))
    {
        if (!( (Similarity_Fuc=="MI") || (Similarity_Fuc=="MSQ") ))
        {
            qDebug() << "Error: for Affine or Rigid Registeration, set Similarity Fuc = MI or MSQ";
            return -1;
        }
        if( Work_Model =="Reslice" )
        {
            // check whether file exists.
            QFileInfo checkFile11(Text_Transform_File);
            if (!(checkFile11.exists() && checkFile11.isFile()))
            {
                qDebug()<<"Error: text_Transform_File doesn't exist.";
                return -1;
            }
            // check file suffix.
            QFileInfo info(Text_Transform_File);
            QString suffix = info.completeSuffix();
            if(suffix != "txt")
            {
                qDebug() << "Error: Rigid- or Affine-Registration needs a text file for transform file.";
                return -1;
            }
        }
    }

    if( (Imageregistration_Type=="Elastic")||(Imageregistration_Type=="Diffeomorphic"))                //&&(Work_Model=="Reslice") )
    {
        if( Work_Model =="Reslice" )
        {
            // check whether txt- or nii-files exist.
            QFileInfo checkFile21(Text_Transform_File);
            if (!(checkFile21.exists() && checkFile21.isFile()))
            {
                qDebug()<<"Error: text_Transform_File doesn't exist.";
                return -1;
            }
            QFileInfo checkFile22(Nii_Transform_File);
            if (!(checkFile22.exists() && checkFile22.isFile()))
            {
                qDebug()<<"Error: Nii_Transform_File doesn't exist.";
                return -1;
            }
            // check file suffix.
            QFileInfo info1(Text_Transform_File);
            QString suffix1 = info1.completeSuffix();
            QFileInfo info2(Nii_Transform_File);
            QString suffix2 = info2.completeSuffix();
            if( (suffix1 != "txt")||(suffix2 != "nii.gz"))
            {
                qDebug() << "Error: need a txt-file and nii.gz-file for transform file.";
                return -1;
            }
        }
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------
void ImageRegistration::clearNodeData()
{
}
//--------------------------------------------------------------------------------------------------
ImageRegistration* ImageRegistration::copy() const
{
    return new ImageRegistration(*this);
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistration::uniqueKey() const
{
    return "ImageRegistration";
}
//--------------------------------------------------------------------------------------------------
void ImageRegistration::markDirty()
{
}
//--------------------------------------------------------------------------------------------------
bool ImageRegistration::isSuperfluous() const
{
    return false;
}
//--------------------------------------------------------------------------------------------------
void ImageRegistration::setConnectedInPorts()
{
}
//--------------------------------------------------------------------------------------------------


