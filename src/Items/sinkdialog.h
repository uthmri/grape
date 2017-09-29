/* sinkdialog.h
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

#ifndef SINKDIALOG
#define SINKDIALOG


#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include <NodeItem.h>
#include <xmlreclib.h>
#include <QDialog>
#include <QDomDocument>
#include <vector>


//=================================================================================
class SinkDialog : public QDialog
{
   Q_OBJECT

public:
    SinkDialog (QWidget *parent = NULL);
    ~SinkDialog ();
    void updateImage();
    void setImage(ImageC* I);
    //void setImage(ImageC I);
    void updateLabel(int value);
    void Convert_vec3d_to_vec1d(vector<vector<vector<double> > > v3d, vector <double> &v1d, int sz1, int sz2, int sz3);
    int kClamp( int x, int low, int high );
    int changeContrast( int value, int contrast, int meanV);
    int changeBrightness( int value, int brightness );

private:
    ImageC* im;
    //ImageC im;
    float maxV, minV;

    QLabel *label;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *label5;

    QSlider *slider1;
    QSlider *slider2;
    QSlider *BrightnessSlider;
    QSlider *ContrastSlider;

    QDialogButtonBox *buttonBox;
    QPushButton *okButton;

public slots:
    void on_slider_valueChanged1(int arg1);
    void on_slider_valueChanged2(int arg2);
    void on_slider_valueChanged3(int arg3);
    void on_slider_valueChanged4(int arg4);

};


#endif // SINKDIALOG

