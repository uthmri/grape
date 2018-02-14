/* ImageRegistrationItem.h
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

#ifndef ImageRegistrationITEM_H
#define ImageRegistration_H

#include <NodeItem.h>


class ImageRegistrationItem : public NodeItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
    ImageRegistrationItem(const NodeInterface &NI);
    ImageRegistrationItem(const ImageRegistrationItem& item);
    ~ImageRegistrationItem();

    DiagramItem* copy() const;
    virtual QString uniqueKey() const;
	QString iconPath() const;

    // Selectors
    void setImageRegistrationType(const QString& imageRegistrationType);
    void setImageSimilarityMeasure(const QString& imageSimilarityMeasure); //<<<<<<<<<<<<<<<<<<<<
    void setRegistrationWorkModel(const QString& registrationWorkModel); //<<<<<<<<<<<<<<<<<<<<
    void setCommandPath(const QString& commandPath1);
//    void setCommandPath2(const QString& commandPath2);
    //void setFixedImagePort(const QString& fixedImagePort);
    //void setMovingImagePort(const QString& movingImagePort);
    void setParam1(const QString& param1);
    void setParam2(const QString& param2);
    void setInput1(const QString& input1);
    void setFixedImage(const QString& fixedimage);
    void setMovingImage(const QString& movingimage);
    void setOutputPath(const QString& outputPath);
    void setCaption(const QString& caption);
    void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
    QString imageRegistrationType() const;
    QString imageSimilarityMeasure() const; //<<<<<<<<<<<<<<<<<<<<
    QString registrationWorkModel() const;  //<<<<<<<<<<<<<<<<<<<<
    QString commandPath() const;
//    QString commandPath2() const;
    //QString fixedImagePort() const;
    //QString movingImagePort() const;
    QString param1() const;
    QString param2() const;
    QString fixedimage() const;
    QString movingimage() const;
    QString outputPath() const;
    QString caption() const;
    QString caption1();
    QFont font() const;
	QBrush textBrush() const;

	void markDirty();

	// Description
	QRectF boundingRect() const;
	QPainterPath shape() const;
	bool isSuperfluous() const;

	// Render
	void render(DiagramPainter* painter);

	// Misc
	void writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	void readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);

	DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
	void rotateEvent(const QPointF& diagramPos);
	void rotateBackEvent(const QPointF& diagramPos);
	void flipEvent(const QPointF& diagramPos);
	void resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos);

	QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
	void changedEvent(Reason reason, const QVariant& value);

	void updateLabel(const QFont& font, QPaintDevice* device);
    void typeChanged(int index);
	qreal orientedTextAngle() const;
};

//==================================================================================================
class ImageRegistrationItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;
    QLineEdit* commandPathEdit;
//    QLineEdit* commandPath2Edit;
    QLineEdit* param1Edit;
    QLineEdit* param2Edit;
    QLineEdit* input1Edit;
    QLineEdit* fixedimageEdit;
    QLineEdit* movingimageEdit;
    QLineEdit* outputPathEdit;
    int imageregistrationTypeIndex;
    QComboBox* imageregistrationTypeCombo;
    QComboBox* similarityMeasureCombo; //<<<<<<<<<<<<<<<
    QComboBox* workModelCombo;       //<<<<<<<<<<<<<<<
    //QComboBox* fixedImagePortCombo;
    //QComboBox* movingImagePortCombo;
    QString mPreviousType;
	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
    enum Capability { NoCapabilities = 0x00, ShowValue = 0x01, ShowType = 0x02,ShowAll  = 0x04};
    Q_DECLARE_FLAGS(Capabilities, Capability)

    ImageRegistrationItemDialog(QWidget* parent = NULL);
    ~ImageRegistrationItemDialog();
	void setProperties(const QHash<QString, QVariant>& properties);
	void setPointPositions(const QList<QPointF>& pointPositions);
	QHash<QString, QVariant> properties() const;
	QList<QPointF> pointPositions() const;

public slots:
	void accept();

private:
	void hideEvent(QHideEvent* event);
};

#endif
