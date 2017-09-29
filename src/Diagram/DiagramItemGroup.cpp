/* DiagramItemGroup.cpp
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

#include <DiagramItemGroup.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramItemGroup::DiagramItemGroup() : DiagramItem()
{
	setFlags(CanMove | CanRotate | CanFlip | MatchUnitsWithParent);
	setPlaceType(PlaceMouseDownAndUp);

	for(int i = 0; i < 8; i++)
		addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Control, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramItemGroup::DiagramItemGroup(const DiagramItemGroup& item) : DiagramItem(item)
{
	mItems = copyItems(item.mItems);
}
//--------------------------------------------------------------------------------------------------
DiagramItemGroup::~DiagramItemGroup()
{
	clearItems();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramItemGroup::copy() const
{
	return new DiagramItemGroup(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramItemGroup::uniqueKey() const
{
	return "group";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::addItem(DiagramItem* item)
{
	if (item && !containsItem(item))
	{
		mItems.append(item);
		updatePoints();
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::removeItem(DiagramItem* item)
{
	if (item && containsItem(item))
	{
		mItems.removeAll(item);
		updatePoints();
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::clearItems()
{
	while (!mItems.empty()) delete mItems.takeLast();
	updatePoints();
}
//--------------------------------------------------------------------------------------------------
int DiagramItemGroup::numberOfItems() const
{
	return mItems.size();
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemGroup::containsItem(DiagramItem* item) const
{
	bool result = false;
	if (item) result = mItems.contains(item);
	return result;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramItemGroup::item(int index) const
{
	DiagramItem* item = NULL;
	if (0 <= index && index < mItems.size()) item = mItems[index];
	return item;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::setItems(const QList<DiagramItem*>& items)
{
	clearItems();
	mItems = items;
	updatePoints();
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> DiagramItemGroup::items() const
{
	return mItems;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramItemGroup::boundingRect() const
{
	QRectF rect, itemRect;

	for(QList<DiagramItem*>::ConstIterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		itemRect = (*itemIter)->boundingRect().translated((*itemIter)->position());

		if (!rect.isValid()) rect = itemRect;
		else rect = rect.united(itemRect);
	}

	return rect;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::render(DiagramPainter* painter)
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		if ((*itemIter)->isVisible())
		{
			painter->translate((*itemIter)->position());
			(*itemIter)->render(painter);
			painter->translate(-(*itemIter)->position());
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::writeXmlChildElements(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlChildElements(xmlWriter, items);

	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
	{
		xmlWriter.writeStartElement((*itemIter)->uniqueKey());
		(*itemIter)->writeXmlAttributes(xmlWriter, mItems);
		(*itemIter)->writeXmlChildElements(xmlWriter, mItems);
		xmlWriter.writeEndElement();
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::readXmlChildElement(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	DiagramItem* item = Diagram::itemFactory.create(xmlReader.name().toString());
	if (item)
	{
		item->clearPoints();

		item->readXmlAttributes(xmlReader, mItems);

		while (xmlReader.readNextStartElement())
			item->readXmlChildElement(xmlReader, mItems);

		mItems.append(item);
	}
	else DiagramItem::readXmlChildElement(xmlReader, items);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramItemGroup::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
	return new DiagramItemGroupDialog(parent);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramItemGroup::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		DiagramUnits oldUnits = units();

		qreal scaleFactor = unitsScale(oldUnits, newUnits);

		if (scaleFactor != 1.0)
		{
			for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
				itemIter != mItems.end(); itemIter++)
			{
				(*itemIter)->setPosition((*itemIter)->position() * scaleFactor);
				(*itemIter)->setUnits(newUnits);
			}
		}
	}

	return value;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::rotateEvent(const QPointF& diagramPos)
{
	DiagramItem::rotateEvent(diagramPos);

	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		(*itemIter)->rotateEvent(QPointF(0.0, 0.0));
	updatePoints();
}
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::rotateBackEvent(const QPointF& diagramPos)
{
	DiagramItem::rotateBackEvent(diagramPos);

	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		(*itemIter)->rotateBackEvent(QPointF(0.0, 0.0));
	updatePoints();
}
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::flipEvent(const QPointF& diagramPos)
{
	DiagramItem::flipEvent(diagramPos);

	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		(*itemIter)->flipEvent(QPointF(0.0, 0.0));
	updatePoints();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemGroup::updatePoints()
{
	QRectF rect = boundingRect();
	DiagramItemPoint* itemPoint;
	DiagramItemPoint* leftPoint;
	DiagramItemPoint* rightPoint;

	itemPoint = cornerPoint(Qt::TopLeftCorner);
	if (itemPoint) itemPoint->setPosition(rect.topLeft());
	itemPoint = cornerPoint(Qt::BottomRightCorner);
	if (itemPoint) itemPoint->setPosition(rect.bottomRight());
	itemPoint = cornerPoint(Qt::TopRightCorner);
	if (itemPoint) itemPoint->setPosition(rect.topRight());
	itemPoint = cornerPoint(Qt::BottomLeftCorner);
	if (itemPoint) itemPoint->setPosition(rect.bottomLeft());

	itemPoint = point(4); leftPoint = point(0); rightPoint = point(2);
	if (itemPoint && leftPoint && rightPoint) itemPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);

	itemPoint = point(5); leftPoint = point(1);
	if (itemPoint && leftPoint && rightPoint) itemPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);

	itemPoint = point(6); rightPoint = point(3);
	if (itemPoint && leftPoint && rightPoint) itemPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);

	itemPoint = point(7); leftPoint = point(0);
	if (itemPoint && leftPoint && rightPoint) itemPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramItemGroup::cornerPoint(Qt::Corner corner) const
{
	DiagramItemPoint* itemPoint = NULL;
	bool lFlipped = isFlipped();
	qreal lRotationAngle = rotationAngle();

	switch (corner)
	{
	case Qt::TopRightCorner:
		if (lFlipped)
		{
			if (lRotationAngle == 90.0) itemPoint = point(3);
			else if (lRotationAngle == 180.0) itemPoint = point(1);
			else if (lRotationAngle == 270.0) itemPoint = point(2);
			else itemPoint = point(0);
		}
		else
		{
			if (lRotationAngle == 90.0) itemPoint = point(0);
			else if (lRotationAngle == 180.0) itemPoint = point(3);
			else if (lRotationAngle == 270.0) itemPoint = point(1);
			else itemPoint = point(2);
		}
		break;
	case Qt::BottomRightCorner:
		if (lFlipped)
		{
			if (lRotationAngle == 90.0) itemPoint = point(1);
			else if (lRotationAngle == 180.0) itemPoint = point(2);
			else if (lRotationAngle == 270.0) itemPoint = point(0);
			else itemPoint = point(3);
		}
		else
		{
			if (lRotationAngle == 90.0) itemPoint = point(2);
			else if (lRotationAngle == 180.0) itemPoint = point(0);
			else if (lRotationAngle == 270.0) itemPoint = point(3);
			else itemPoint = point(1);
		}
		break;
	case Qt::BottomLeftCorner:
		if (lFlipped)
		{
			if (lRotationAngle == 90.0) itemPoint = point(2);
			else if (lRotationAngle == 180.0) itemPoint = point(0);
			else if (lRotationAngle == 270.0) itemPoint = point(3);
			else itemPoint = point(1);
		}
		else
		{
			if (lRotationAngle == 90.0) itemPoint = point(1);
			else if (lRotationAngle == 180.0) itemPoint = point(2);
			else if (lRotationAngle == 270.0) itemPoint = point(0);
			else itemPoint = point(3);
		}
		break;
	default:    // Qt::TopLeftCorner
		if (lFlipped)
		{
			if (lRotationAngle == 90.0) itemPoint = point(0);
			else if (lRotationAngle == 180.0) itemPoint = point(3);
			else if (lRotationAngle == 270.0) itemPoint = point(1);
			else itemPoint = point(2);
		}
		else
		{
			if (lRotationAngle == 90.0) itemPoint = point(3);
			else if (lRotationAngle == 180.0) itemPoint = point(1);
			else if (lRotationAngle == 270.0) itemPoint = point(2);
			else itemPoint = point(0);
		}
		break;
	}

	return itemPoint;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemGroupDialog::DiagramItemGroupDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	positionLayout = new DiagramPositionLayout(diagramSize);

	QGroupBox* positionGroup = new QGroupBox("Position");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Position: ", positionLayout);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignTop);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	positionGroup->setLayout(fLayout);

    addWidget(positionGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	setWindowTitle("Group Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramItemGroupDialog::~DiagramItemGroupDialog() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemGroupDialog::setPosition(const QPointF& position)
{
	positionLayout->setPosition(position);
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItemGroupDialog::position() const
{
	return positionLayout->position();
}
//--------------------------------------------------------------------------------------------------
