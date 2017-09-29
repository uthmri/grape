/* DiagramPolygonItem.h
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

#ifndef DIAGRAMPOLYGONITEM_H
#define DIAGRAMPOLYGONITEM_H

#include <DiagramBaseItems.h>

class DiagramPolygonItem : public DiagramPolyItem
{
public:
	DiagramPolygonItem();
	DiagramPolygonItem(const DiagramPolygonItem& item);
	virtual ~DiagramPolygonItem();

	virtual DiagramItem* copy() const;
	virtual QString uniqueKey() const;
	virtual QString iconPath() const;

	void setBrush(const QBrush& brush);
	QBrush brush() const;

	virtual bool canInsertPoint(DiagramItemPoint* point) const;
	virtual bool canRemovePoint(DiagramItemPoint* point) const;
	virtual int insertPointIndex(DiagramItemPoint* point) const;

	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;

	virtual void render(DiagramPainter* painter);

	virtual void writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	virtual void readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);

	virtual DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
	virtual QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
};

//==================================================================================================

class DiagramPolygonItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

public:
	enum Flag { NoFlags = 0, ItemUsesPalette = 0x01 };
	Q_DECLARE_FLAGS(Flags, Flag)

private:
	Flags mFlags;
	QSizeF mDiagramSize;

	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

	QFrame* pointsFrame;
	QFormLayout* pointFormLayout;
	QList<DiagramPositionLayout*> pointPositionLayouts;

public:
	DiagramPolygonItemDialog(QWidget* parent = NULL, Flags flags = NoFlags);
	~DiagramPolygonItemDialog();

	void setProperties(const QHash<QString, QVariant>& properties);
	void setPointPositions(const QList<QPointF>& pointPositions);
	QHash<QString, QVariant> properties() const;
	QList<QPointF> pointPositions() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramPolygonItemDialog::Flags)

#endif


