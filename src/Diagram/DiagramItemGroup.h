/* DiagramItemGroup.h
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

#ifndef DIAGRAMITEMGROUP_H
#define DIAGRAMITEMGROUP_H

#include <DiagramItem.h>
#include <DiagramWidgets.h>

class DiagramItemGroup : public DiagramItem
{
private:
	QList<DiagramItem*> mItems;

public:
	DiagramItemGroup();
	DiagramItemGroup(const DiagramItemGroup& item);
	virtual ~DiagramItemGroup();

	virtual DiagramItem* copy() const;
	virtual QString uniqueKey() const;

	void addItem(DiagramItem* item);
	void removeItem(DiagramItem* item);
	void clearItems();
	int numberOfItems() const;
	bool containsItem(DiagramItem* item) const;
	DiagramItem* item(int index) const;

	void setItems(const QList<DiagramItem*>& items);
	QList<DiagramItem*> items() const;

	virtual QRectF boundingRect() const;

	virtual void render(DiagramPainter* painter);

	virtual void writeXmlChildElements(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	virtual void readXmlChildElement(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);

	virtual DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
	virtual QVariant aboutToChangeEvent(Reason reason, const QVariant& value);

	virtual void rotateEvent(const QPointF& diagramPos);
	virtual void rotateBackEvent(const QPointF& diagramPos);
	virtual void flipEvent(const QPointF& diagramPos);

	void updatePoints();
	DiagramItemPoint* cornerPoint(Qt::Corner corner) const;
};

//==================================================================================================

class DiagramItemGroupDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramPositionLayout* positionLayout;

public:
	DiagramItemGroupDialog(QWidget* parent = NULL);
	~DiagramItemGroupDialog();

	void setPosition(const QPointF& position);
	QPointF position() const;
};

#endif
