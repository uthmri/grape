/* ChartBoxItem.h
 *
 * Copyright (C) 2013 Jason Allen
 *
 * This file is part of the Jade Diagram Editor.
 *
 * Jade is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Jade is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Jade.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef CHARTBOXITEM_H
#define CHARTBOXITEM_H

#include <DiagramBoxItem.h>

class ChartBoxItem : public DiagramBoxItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
	ChartBoxItem();
	ChartBoxItem(const ChartBoxItem& item);
	~ChartBoxItem();

	DiagramItem* copy() const;
	QString uniqueKey() const;
	QString iconPath() const;

	// Selectors
	void setCaption(const QString& caption);
	void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
	QString caption() const;
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

class ChartBoxItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;

	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPositionLayout* cornerRadiiLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
	ChartBoxItemDialog(QWidget* parent = NULL);
	~ChartBoxItemDialog();

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
