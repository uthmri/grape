/* BETItem.h
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

#ifndef BETITEM_H
#define BETITEM_H

#include <NodeItem.h>

class BETItem : public NodeItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;


public:

    BETItem(const NodeInterface &NI);
    BETItem(const BETItem& item);
    ~BETItem();

	DiagramItem* copy() const;
    virtual QString uniqueKey() const;
    QString iconPath() const;

	// Selectors
    void setCommandPath(const QString& path);
    void setOutputFile(const QString& path);
    void setArg1(const QString& path);
    void setArg2(const QString& path);
    void setArg3(const QString& path);
    void setArg4(const QString& path);
    void setArg5(const QString& path);
    void setArg6(const QString& path);
    void setArg7(const QString& path);
    //void setInput2Path(const QString& path);
   // void setArgPath(const QString& path);
    void setCaption(const QString& caption);
    void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
    QString commandPath() const;
    QString outputFile() const;
    //QString input2Path() const;
    QString arg1() const;
    QString arg2() const;
    QString arg3() const;
    QString arg4() const;
    QString arg5() const;
    QString arg6() const;
    QString arg7() const;
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

	qreal orientedTextAngle() const;
};

//==================================================================================================
class BETItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;
    QLineEdit* commandPathEdit;
    QLineEdit* outputFileEdit;
   // QLineEdit* arg1Edit;
    QLineEdit* arg2Edit;
    QLineEdit* arg3Edit;
    QDoubleSpinBox* fractionalIntensityThreshold;
    QDoubleSpinBox* VGFractionalIntensityThreshold;
    QLineEdit* arg6Edit;
    QVBoxLayout* mLayout;
    QCheckBox* generateBrainSurfaceOutlineCheck;
    QCheckBox* binaryBrainMaskCheck;
    QCheckBox* approximateSkullImageCheck;
    QCheckBox* segmentedBrainImageCheck;
    QCheckBox* applyThresholdingToSegmentedBrainImageAndMaskCheck;
	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
    BETItemDialog(QWidget* parent = NULL);
    ~BETItemDialog();

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
