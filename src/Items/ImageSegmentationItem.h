/* ImageSegmentationItem.h
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

#ifndef ImageSegmentationITEM_H
#define ImageSegmentationITEM_H

#include <NodeItem.h>


class ImageSegmentationItem : public NodeItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
    ImageSegmentationItem(const NodeInterface &NI);
    ImageSegmentationItem(const ImageSegmentationItem& item);
    ~ImageSegmentationItem();

	DiagramItem* copy() const;
    virtual QString uniqueKey() const;
	QString iconPath() const;

    // Selectors
    void setImageSegmentationType(const QString& dataType);
    void setCommandPath(const QString& commandPath);
    void setArg1(const QString& arg1);
     void setArg2(const QString& arg2);
      void setOutput1(const QString& output1);
      void setOutput2(const QString& output1);
    void setCaption(const QString& caption);
    void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
    QString imagesegmentationType() const;
    QString commandPath() const;
    QString arg1() const;
    QString arg2() const;
    QString output1() const;
    QString output2() const;
    QString caption() const;
     QString caption1() ;
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
class ImageSegmentationItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;
//    QTextEdit* inputPathEdit;
    QLineEdit* commandPathEdit;
    QLineEdit* arg1Edit;
    QLineEdit* arg2Edit;
    QLineEdit* output1Edit;
    QLineEdit* output2Edit;
    int ImageSegmentationTypeIndex;
    QComboBox* imagesegmentationTypeCombo;
    QString mPreviousType;
	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
    enum Capability { NoCapabilities = 0x00, ShowValue = 0x01, ShowType = 0x02,ShowAll  = 0x04};
    Q_DECLARE_FLAGS(Capabilities, Capability)

    ImageSegmentationItemDialog(QWidget* parent = NULL);
    ~ImageSegmentationItemDialog();
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
