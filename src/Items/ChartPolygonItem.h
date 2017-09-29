/* ChartPolygonItem.h
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

#ifndef CHARTPOLYGONITEM_H
#define CHARTPOLYGONITEM_H

#include <DiagramPolygonItem.h>

class ChartPolygonItem : public DiagramPolygonItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
	ChartPolygonItem();
	ChartPolygonItem(const ChartPolygonItem& item);
	~ChartPolygonItem();

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
	QPointF centerPoint() const;
};

//==================================================================================================

class ChartPolygonItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	QSizeF mDiagramSize;

	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;

	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

	QFrame* pointsFrame;
	QFormLayout* pointFormLayout;
	QList<DiagramPositionLayout*> pointPositionLayouts;

public:
	ChartPolygonItemDialog(QWidget* parent = NULL);
	~ChartPolygonItemDialog();

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
