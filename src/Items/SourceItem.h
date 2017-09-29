/* SourceItem.h
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



#ifndef SOURCEITEM_H
#define SOURCEITEM_H

#include <NodeItem.h>


class SourceItem : public NodeItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
    SourceItem(const NodeInterface &NI);
    SourceItem(const SourceItem& item);
    ~SourceItem();

	DiagramItem* copy() const;
    virtual QString uniqueKey() const;
	QString iconPath() const;

    // Selectors
    void setDataType(const QString& dataType);
    void setDataValue(const QString& dataValue);
    void setArray(const QString& v);
    void setDimension1(const QString& dim1);
    void setDimension2(const QString& dim2);
    void setDimension3(const QString& dim3);
    void setDimension4(const QString& dim4);
    void setCaption(const QString& caption);
    void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
    QString dataType() const;
    QString dataValue() const;
    QString Array() const;
    QString dim1() const;
    QString dim2() const;
    QString dim3() const;
    QString dim4() const;
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
class SourceItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;
    QCheckBox* promptDimensionCheck;
    QCheckBox* arrayCheck;
//    QTextEdit* inputPathEdit;

    QLineEdit* dataValueEdit;
    QSpinBox* dim1Edit;
    QSpinBox* dim2Edit;
    QSpinBox* dim3Edit;
    QSpinBox* dim4Edit;
   // QSlider *slider;
    int dataTypeIndex;
    QComboBox* dataTypeCombo;
    QString mPreviousType;
	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
    enum Capability { NoCapabilities = 0x00, ShowValue = 0x01, ShowType = 0x02,ShowAll  = 0x04};
    Q_DECLARE_FLAGS(Capabilities, Capability)

    SourceItemDialog(QWidget* parent = NULL);
    ~SourceItemDialog();
	void setProperties(const QHash<QString, QVariant>& properties);
	void setPointPositions(const QList<QPointF>& pointPositions);
	QHash<QString, QVariant> properties() const;
	QList<QPointF> pointPositions() const;
    void setPrompts(bool overwrite);
    bool shouldPromptOnDimension() const;

public slots:
	void accept();

private:
	void hideEvent(QHideEvent* event);
};

#endif
