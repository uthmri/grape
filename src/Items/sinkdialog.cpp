/* sinkdialog.cpp
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

#include "sinkdialog.h"
#include <image/image.hpp>
#include <xmlreclib.h>


//================================================================================
void SinkDialog::on_slider_valueChanged1(int arg1 )
{
    updateImage();
    label2->setText("Image Number is: "+ QString::number(arg1) );
}
//================================================================================
void SinkDialog::on_slider_valueChanged2(int arg2 )
{
    updateImage();
    label3->setText("Stack Number is: "+ QString::number(arg2) );
}
//================================================================================
void SinkDialog::on_slider_valueChanged3(int arg3 )
{
    updateImage();
    label4->setText("brightness value is: "+ QString::number(arg3) );
}
//================================================================================
void SinkDialog::on_slider_valueChanged4(int arg4 )
{
    updateImage();
    label5->setText("contrast is: "+ QString::number(arg4) );
}
//================================================================================
void SinkDialog::setImage(ImageC* I)
{
    im = I;
    slider1->setMinimum(1);
    slider1->setMaximum(1);
    slider2->setMinimum(1);
    slider2->setMaximum(1);
    if(im->NumDimensions == 2)
    {
        slider1->setEnabled(false);
        slider2->setEnabled(false);
    }
    else if(im->NumDimensions == 3)
    {
        slider1->setEnabled(true);
        slider2->setEnabled(false); //<<<<<<<<<<<setEnabled(false)
        updateLabel(0);
        slider1->setMinimum(1);
        slider1->setMaximum(im->array_size[2]);
        int mid_slc = (im->array_size[2]/2);
        slider1->setSliderPosition(mid_slc);
    }
    else if(im->NumDimensions == 4)
    {
        slider1->setEnabled(true);
        slider2->setEnabled(true);
        slider1->setMinimum(1);
        slider1->setMaximum(im->array_size[2]);
        slider2->setMinimum(1);
        slider2->setMaximum(im->array_size[3]);
        int mid_slc = (im->array_size[2]/2);
        slider1->setSliderPosition(mid_slc);
        slider2->setSliderPosition(1);
    }

    BrightnessSlider->setMinimum(-100);
    BrightnessSlider->setMaximum(100);
    BrightnessSlider->setSliderPosition(0);
    BrightnessSlider->setEnabled(true);

    ContrastSlider->setMinimum(1);
    ContrastSlider->setMaximum(1000);
    ContrastSlider->setSliderPosition(100);  // 100%
    ContrastSlider->setEnabled(true);

    XMLRecReader X;
    maxV = X.imageMaxValue(im);
    minV = X.imageMinValue(im);
    updateImage();
}
//================================================================================
SinkDialog::SinkDialog(QWidget *parent) : QDialog(parent)
{
    label = new QLabel("");
    label2= new QLabel("");
    label3= new QLabel("");
    label4= new QLabel("");
    label5= new QLabel("");

    slider1 = new QSlider(Qt::Horizontal, this);
    slider2 = new QSlider(Qt::Horizontal, this);
    BrightnessSlider = new QSlider(Qt::Horizontal, this);
    ContrastSlider = new QSlider(Qt::Horizontal, this);

    //Slice number
    slider1->setMinimum(1);
    slider1->setMaximum(2);

    //Stack number
    slider2->setMinimum(1);
    slider2->setMaximum(2);

    //Brightness
    BrightnessSlider->setMinimum(1);
    BrightnessSlider->setMaximum(2);

    //Contrast
    ContrastSlider->setMinimum(1);
    ContrastSlider->setMaximum(2);

    okButton = new QPushButton(tr("&OK"));
    okButton->setDefault(true);
    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(okButton, QDialogButtonBox::ActionRole);

   // QGroupBox* sliderGroup = new QGroupBox("Slider");
    QGroupBox* imageGroup = new QGroupBox("Image");
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->addWidget(label5);
    vLayout->addWidget(label4);
    vLayout->addWidget(label3);
    vLayout->addWidget(label2);

//    vLayout->addWidget(label);
    QFormLayout* pLayoutIm = new QFormLayout();
    pLayoutIm->addWidget(label);
//    pLayoutIm->SetFixedSize();
    vLayout->addLayout(pLayoutIm);

//    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("slice     ", slider1);
    pLayout->addRow("stack     ", slider2);
    pLayout->addRow("brightness", BrightnessSlider);
    pLayout->addRow("contrast  ", ContrastSlider);
    vLayout->addLayout(pLayout);

    vLayout->addWidget(okButton);
    vLayout->addWidget(buttonBox);
    imageGroup->setLayout(vLayout);

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(imageGroup, 0, 0);
    setLayout(grid);

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(slider1, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged1(int)));
    connect(slider2, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged2(int)));
    connect(BrightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged3(int)));
    connect(ContrastSlider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged4(int)));
}
//=============================================================================================
SinkDialog::~SinkDialog()
{
    delete label;
    delete label2;
    delete label3;
    delete label4;
    delete label5;
}
//=============================================================================================
void SinkDialog::updateImage()
{
    int d3 = slider1->value()-1;
    int d4 = slider2->value()-1;
    int d5 = BrightnessSlider->value();
    int d6 = ContrastSlider->value();

    // define image and a grey-scale colormap
    QImage img(im->array_size[0], im->array_size[1], QImage::Format_Indexed8);
    for(int i=0; i<256; i++)
        img.setColor(i, qRgb(i, i, i));

    int ClippedPixel;
    for(int y=0; y<im->array_size[1]; y++)
        for(int x=0; x<im->array_size[0]; x++)
        {
            switch(im->NumDimensions)
            {
            case 2: ClippedPixel = (im->getVoxel(x,y)-minV)*255.0/(maxV-minV);  break;
            case 3: ClippedPixel = (im->getVoxel(x,y,d3)-minV)*255.0/(maxV-minV); break;
            case 4: ClippedPixel = (im->getVoxel(x,y,d3,d4)-minV)*255.0/(maxV-minV); break;
            default: ClippedPixel = 0; break;
            }
            ClippedPixel = changeBrightness(ClippedPixel, d5);
            ClippedPixel = changeContrast(ClippedPixel, d6, 127+d5/100*255);
            img.setPixel(x,y,ClippedPixel);
        }
    QPixmap px = QPixmap::fromImage(img);
    label->setPixmap(px);
}
//=============================================================================================
int SinkDialog::kClamp( int x, int low, int high )
{
    if ( x < low )       return low;
    else if ( high < x ) return high;
    else                 return x;
}
//=============================================================================================
int SinkDialog::changeBrightness( int value, int d5 )
{
    return kClamp( value + d5 * 255 / 100, 0, 255 );
}
//=============================================================================================
int SinkDialog::changeContrast( int value, int d6, int meanV)
{
    return kClamp((( value - meanV ) * d6 / 100 ) + meanV, 0, 255 );
}
//=============================================================================================
void SinkDialog::updateLabel(int value)
{
    int height0 = (100-value)*(slider1->height())/100;
    int height= QStyle::sliderPositionFromValue(0,100,slider1->value(),slider1->height(), true);
    label->move(slider1->width(), height);
}
//=============================================================================================
void SinkDialog::Convert_vec3d_to_vec1d(vector<vector<vector<double> > > v3d, vector <double> &v1d, int sz1, int sz2, int sz3)
{
    v1d.resize(sz1*sz2*sz3);

    int cnt = 0;
    for (int i = 0; i < sz1; ++i)
        for (int j = 0; j < sz2; ++j)
            for (int k = 0; k < sz3; ++k)
            {
                v1d[cnt] = v3d[i][j][k];
                cnt++;
            }
}

//=============================================================================================
