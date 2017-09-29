/* DiagramItem.cpp
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

#include <DiagramItem.h>
#include <DiagramItemPoint.h>
#include <Diagram.h>


DiagramItem::DiagramItem()
{
    mDiagram = NULL;

    mPosition = QPointF(0.0, 0.0);
	mUnits = UnitsMils;

	mFlags = CanMove | CanRotate | CanFlip | CanResize | MatchUnitsWithParent;
	mPlaceType = PlaceMouseUp;

	mSelectedPoint = NULL;
	mParent = NULL;

	mSelected = false;
	mVisible = true;

	mRotationAngle = 0;
    mFlipped = false;

}
//--------------------------------------------------------------------------------------------------
DiagramItem::DiagramItem(const DiagramItem& item)
{
	mDiagram = NULL;

	mPosition = item.mPosition;
	mUnits = item.mUnits;

	mFlags = item.mFlags;
	mPlaceType = item.mPlaceType;

	for(QList<DiagramItemPoint*>::ConstIterator pointIter = item.mPoints.begin();
		pointIter != item.mPoints.end(); pointIter++) addPoint((*pointIter)->copy());
	mSelectedPoint = NULL;

	mProperties = item.mProperties;

	for(QList<DiagramItem*>::ConstIterator childIter = item.mChildren.begin();
		childIter != item.mChildren.end(); childIter++) addChild((*childIter)->copy());
	mParent = NULL;

	mSelected = item.mSelected;
	mVisible = item.mVisible;

	mRotationAngle = item.mRotationAngle;
	mFlipped = item.mFlipped;
}
//--------------------------------------------------------------------------------------------------
DiagramItem::~DiagramItem()
{
	mDiagram = NULL;
	mSelectedPoint = NULL;
	mParent = NULL;

	clearPoints();
	clearChildren();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Diagram* DiagramItem::diagram() const
{
	return mDiagram;
}
//--------------------------------------------------------------------------------------------------
QString DiagramItem::iconPath() const
{
	return QString();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::setPosition(const QPointF& parentPos)
{
	QPointF posActual = aboutToChangeEvent(PositionChange, QVariant(parentPos)).toPointF();
	mPosition = posActual;
	changedEvent(PositionChange, QVariant(posActual));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::setPosition(qreal x, qreal y)
{
	setPosition(QPointF(x, y));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::setUnits(DiagramUnits units)
{
	DiagramUnits newUnits = (DiagramUnits)aboutToChangeEvent(UnitsChange,
		QVariant((int)units)).toInt();
	DiagramUnits oldUnits = mUnits;
	qreal scaleFactor = unitsScale(oldUnits, newUnits);

	mUnits = newUnits;

	// Adjust position of points
	for(QList<DiagramItemPoint*>::Iterator pointIter = mPoints.begin();
		pointIter != mPoints.end(); pointIter++)
	{
		(*pointIter)->setPosition((*pointIter)->position() * scaleFactor);
	}

	// Adjust position and units of children
	for(QList<DiagramItem*>::Iterator childIter = mChildren.begin();
		childIter != mChildren.end(); childIter++)
	{
		(*childIter)->setPosition((*childIter)->position() * scaleFactor);
		if ((*childIter)->shouldMatchUnitsWithParent()) (*childIter)->setUnits(units);
	}

	changedEvent(UnitsChange, QVariant((int)newUnits));
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItem::position() const
{
	return mPosition;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramItem::positionX() const
{
	return mPosition.x();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramItem::positionY() const
{
	return mPosition.y();
}
//--------------------------------------------------------------------------------------------------
DiagramUnits DiagramItem::units() const
{
	return mUnits;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::setFlags(Flags flags)
{
	mFlags = flags;
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::setPlaceType(PlaceType type)
{
	mPlaceType = type;
}
//--------------------------------------------------------------------------------------------------
DiagramItem::Flags DiagramItem::flags() const
{
	return mFlags;
}
//--------------------------------------------------------------------------------------------------
DiagramItem::PlaceType DiagramItem::placeType() const
{
	return mPlaceType;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::canMove() const
{
	return (mFlags & CanMove);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::canRotate() const
{
	return (mFlags & CanRotate);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::canFlip() const
{
	return (mFlags & CanFlip);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::canResize() const
{
	return (mFlags & CanResize);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::canInsertRemovePoints() const
{
	return (mFlags & CanInsertRemovePoints);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::shouldMatchUnitsWithParent() const
{
	return (mFlags & MatchUnitsWithParent);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::addProperty(const QString& property, const QVariant& value)
{
	mProperties[property] = value;
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::removeProperty(const QString& property)
{
	mProperties.remove(property);
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::clearProperties()
{
	mProperties.clear();
}
//--------------------------------------------------------------------------------------------------
int DiagramItem::numberOfProperties() const
{
	return mProperties.size();
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::containsProperty(const QString& property) const
{
	return mProperties.contains(property);
}
//--------------------------------------------------------------------------------------------------
QStringList DiagramItem::propertiesList() const
{
	return mProperties.keys();
}
//--------------------------------------------------------------------------------------------------
QList<QVariant> DiagramItem::propertyValues() const
{
	return mProperties.values();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::setPropertyValue(const QString& property, const QVariant& value)
{
	QVariant result = aboutToChangeEvent(PropertyChange, value);
	mProperties[property] = result;
	changedEvent(PropertyChange, result);
}
//--------------------------------------------------------------------------------------------------
QVariant DiagramItem::propertyValue(const QString& property) const
{
	QVariant value;
	if (mProperties.contains(property)) value = mProperties[property];
	return value;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::setProperties(const QHash<QString, QVariant>& properties)
{
	aboutToChangeEvent(PropertyChange, QVariant());
	mProperties = properties;
	changedEvent(PropertyChange, QVariant());
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramItem::properties() const
{
	return mProperties;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::addPoint(DiagramItemPoint* itemPoint)
{
	if (itemPoint && !containsPoint(itemPoint))
	{
		mPoints.append(itemPoint);
		itemPoint->mItem = this;
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::insertPoint(int index, DiagramItemPoint* itemPoint)
{
	if (itemPoint && !containsPoint(itemPoint))
	{
		mPoints.insert(index, itemPoint);
		itemPoint->mItem = this;
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::removePoint(DiagramItemPoint* itemPoint)
{
	if (itemPoint && containsPoint(itemPoint))
	{
		mPoints.removeAll(itemPoint);
		itemPoint->mItem = NULL;
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::clearPoints()
{
	DiagramItemPoint* itemPoint = NULL;

	while (!mPoints.empty())
	{
		itemPoint = mPoints.first();
		removePoint(itemPoint);
		delete itemPoint;
		itemPoint = NULL;
	}
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItemPoint*> DiagramItem::points() const
{
	return mPoints;
}
//--------------------------------------------------------------------------------------------------
int DiagramItem::numberOfPoints() const
{
	return mPoints.size();
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::containsPoint(DiagramItemPoint* itemPoint) const
{
	bool contains = false;
	if (itemPoint) contains = mPoints.contains(itemPoint);
	return contains;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramItem::point(int index) const
{
	DiagramItemPoint *itemPoint = NULL;
	if (0 <= index && index < mPoints.size()) itemPoint = mPoints[index];
	return itemPoint;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramItem::pointAt(const QPointF& itemPos) const
{
	DiagramItemPoint *itemPoint = NULL;

	for(QList<DiagramItemPoint*>::ConstIterator pointIter = mPoints.begin();
		itemPoint == NULL && pointIter != mPoints.end(); pointIter++)
	{
		if ((*pointIter)->shape().contains(itemPos - (*pointIter)->position()))
			itemPoint = *pointIter;
	}

	return itemPoint;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramItem::pointNearest(const QPointF& itemPos) const
{
	DiagramItemPoint *itemPoint = NULL;
	qreal minimumDistance = 1E9, distance;

	for(QList<DiagramItemPoint*>::ConstIterator pointIter = mPoints.begin();
		pointIter != mPoints.end(); pointIter++)
	{
		distance = magnitude((*pointIter)->position() - itemPos);
		if (distance < minimumDistance)
		{
			itemPoint = *pointIter;
			minimumDistance = distance;
		}
	}

	return itemPoint;
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::adjustReferencePoint()
{
	DiagramItemPoint* startPoint = point(0);

	// Adjust position of item, points, and children so that point(0)->position() == QPointF(0, 0)
	if (startPoint)
	{
		QPointF deltaPos = -startPoint->position();
		QPointF parentPos = mapToParent(startPoint->position());

		for(QList<DiagramItemPoint*>::Iterator pointIter = mPoints.begin();
			pointIter != mPoints.end(); pointIter++)
		{
			(*pointIter)->setPosition((*pointIter)->position() + deltaPos);
		}

		for(QList<DiagramItem*>::Iterator childIter = mChildren.begin();
			childIter != mChildren.end(); childIter++)
		{
			(*childIter)->setPosition((*childIter)->position() + deltaPos);
		}

		setPosition(parentPos);
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramItem::canInsertPoint(DiagramItemPoint* itemPoint) const
{
	return (canInsertRemovePoints() && itemPoint && !containsPoint(itemPoint));
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::canRemovePoint(DiagramItemPoint* itemPoint) const
{
	return (canInsertRemovePoints() && itemPoint && containsPoint(itemPoint) &&
		itemPoint->numberOfTargets() == 0);
}
//--------------------------------------------------------------------------------------------------
int DiagramItem::insertPointIndex(DiagramItemPoint* itemPoint) const
{
	Q_UNUSED(itemPoint);
	return -1;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramItem::hasSelectedPoint() const
{
	return (mSelectedPoint != NULL);
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramItem::selectedPoint() const
{
	return mSelectedPoint;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::addChild(DiagramItem* childItem)
{
	if (childItem)
	{
		mChildren.append(childItem);
		childItem->mParent = this;
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::insertChild(int index, DiagramItem* childItem)
{
	if (childItem)
	{
		mChildren.insert(index, childItem);
		childItem->mParent = this;
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::removeChild(DiagramItem* childItem)
{
	if (childItem)
	{
		mChildren.removeAll(childItem);
		childItem->mParent = NULL;
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::clearChildren()
{
	DiagramItem* childItem = NULL;

	while (!mChildren.empty())
	{
		childItem = mChildren.first();
		removeChild(childItem);
		delete childItem;
		childItem = NULL;
	}
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> DiagramItem::children() const
{
	return mChildren;
}
//--------------------------------------------------------------------------------------------------
int DiagramItem::numberOfChildren() const
{
	return mChildren.size();
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::containsChild(DiagramItem* child) const
{
	bool contains = false;
	if (child) contains = mChildren.contains(child);
	return contains;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramItem::child(int index) const
{
	DiagramItem* child = NULL;
	if (0 <= index && index < mChildren.size()) child = mChildren[index];
	return child;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::setParent(DiagramItem* parent)
{
	mParent = parent;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramItem::parent() const
{
	return mParent;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::setSelected(bool select)
{
	select = aboutToChangeEvent(SelectedChange, QVariant(select)).toBool();

	mSelected = select;
	if (!mSelected) mSelectedPoint = NULL;

	for(QList<DiagramItem*>::Iterator childIter = mChildren.begin();
		childIter != mChildren.end(); childIter++) (*childIter)->setSelected(select);

	changedEvent(SelectedChange, QVariant(select));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::setVisible(bool visible)
{
	visible = aboutToChangeEvent(VisibleChange, QVariant(visible)).toBool();

	mVisible = visible;

	for(QList<DiagramItem*>::Iterator childIter = mChildren.begin();
		childIter != mChildren.end(); childIter++) (*childIter)->setVisible(visible);

	changedEvent(VisibleChange, QVariant(visible));
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::isSelected() const
{
	return mSelected;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::isVisible() const
{
	return mVisible;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::setRotationAngle(qreal angle)
{
	mRotationAngle = angle;
	while (mRotationAngle >= 360.0) mRotationAngle -= 360.0;
	while (mRotationAngle < 0.0) mRotationAngle += 360.0;
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::setFlipped(bool flipped)
{
	mFlipped = flipped;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramItem::rotationAngle() const
{
	return mRotationAngle;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::isFlipped() const
{
	return mFlipped;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPointF DiagramItem::mapToDiagram(const QPointF& itemPos) const
{
	QPointF diagramPos = itemPos;

	if (mParent) diagramPos = mParent->mapToDiagram(mapToParent(itemPos));
	else if (mDiagram) diagramPos = itemPos / unitsScale(mUnits, mDiagram->units()) + mPosition;

	return diagramPos;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItem::mapFromDiagram(const QPointF& diagramPos) const
{
	QPointF itemPos = diagramPos;

	if (mParent) itemPos = mapFromParent(mParent->mapFromDiagram(diagramPos));
	else if (mDiagram) itemPos = (diagramPos - mPosition) * unitsScale(mUnits, mDiagram->units());

	return itemPos;
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramItem::mapToDiagram(const QRectF& itemRect) const
{
	return QRectF(mapToDiagram(itemRect.topLeft()), mapToDiagram(itemRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramItem::mapFromDiagram(const QRectF& diagramRect) const
{
	return QRectF(mapFromDiagram(diagramRect.topLeft()), mapFromDiagram(diagramRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPointF DiagramItem::mapToParent(const QPointF& itemPos) const
{
	QPointF parentPos = itemPos;

	if (mParent) parentPos = itemPos / unitsScale(mUnits, mParent->units()) + mPosition;
	else if (mDiagram) parentPos = itemPos / unitsScale(mUnits, mDiagram->units()) + mPosition;

	return parentPos;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItem::mapFromParent(const QPointF& parentPos) const
{
	QPointF itemPos = parentPos;

	if (mParent) itemPos = (parentPos - mPosition) * unitsScale(mUnits, mParent->units());
	else if (mDiagram) itemPos = (parentPos - mPosition) * unitsScale(mUnits, mDiagram->units());

	return itemPos;
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramItem::mapToParent(const QRectF& itemRect) const
{
	return QRectF(mapToParent(itemRect.topLeft()), mapToParent(itemRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramItem::mapFromParent(const QRectF& parentRect) const
{
	return QRectF(mapFromParent(parentRect.topLeft()), mapFromParent(parentRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItem::centerPosition() const
{
	return boundingRect().center();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramItem::isSuperfluous() const
{
    return false;
}

QString DiagramItem::caption()
{
    return "";
}
QString DiagramItem::caption1()
{
    return "";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	Q_UNUSED(items);

    xmlWriter.writeAttribute("ItemNumber", QString::number(items.indexOf(this)));
    xmlWriter.writeAttribute("x", QString::number(positionX()));
	xmlWriter.writeAttribute("y", QString::number(positionY()));
	xmlWriter.writeAttribute("units", QString::number((quint16)units()));
	xmlWriter.writeAttribute("flags", QString::number((quint32)flags()));
	xmlWriter.writeAttribute("placeType", QString::number((quint16)placeType()));
	xmlWriter.writeAttribute("visible", (isVisible()) ? "true" : "false");
	xmlWriter.writeAttribute("rotationAngle", QString::number(rotationAngle()));
	xmlWriter.writeAttribute("flipped", (isFlipped()) ? "true" : "false");


	// Any properties should be saved in derived class writeXmlAttributes
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::writeXmlChildElements(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	QList<DiagramItemPoint*> points = DiagramItem::points();
	DiagramItemPoint* targetItemPoint;
	DiagramItem* targetItem;
	int itemIndex, targetItemIndex;
	QString pointConnections;

	// Points
	for(QList<DiagramItemPoint*>::Iterator pointIter = points.begin();
		pointIter != points.end(); pointIter++)
	{
		xmlWriter.writeStartElement("itemPoint");

		xmlWriter.writeAttribute("x", QString::number((*pointIter)->positionX()));
		xmlWriter.writeAttribute("y", QString::number((*pointIter)->positionY()));
		xmlWriter.writeAttribute("width", QString::number((*pointIter)->width()));
		xmlWriter.writeAttribute("height", QString::number((*pointIter)->height()));
		xmlWriter.writeAttribute("flags", QString::number((quint32)(*pointIter)->flags()));
		xmlWriter.writeAttribute("category", QString::number((*pointIter)->category()));

		// Targets
		pointConnections = QString();
		itemIndex = items.indexOf(this);
		if (itemIndex >= 0)
		{
			for(int i = 0; i < (*pointIter)->numberOfTargets(); i++)
			{
				targetItemPoint = (*pointIter)->target(i);
				targetItem = targetItemPoint->item();

				if (targetItem) targetItemIndex = items.indexOf(targetItem);
				else targetItemIndex = -1;

				if (targetItemIndex >= 0)
				{
					pointConnections += QString::number(targetItemIndex) + ",";
					pointConnections += QString::number(targetItem->points().indexOf(targetItemPoint)) + ",";
				}
			}
		}
		pointConnections = pointConnections.left(pointConnections.size() - 1);

		if (!pointConnections.isEmpty())
			xmlWriter.writeAttribute("connections", pointConnections);

		xmlWriter.writeEndElement();
	}

	// Children should be saved in derived class writeXmlChildElements
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();
	Q_UNUSED(items);

	if (attributes.hasAttribute("units"))
		setUnits((DiagramUnits)attributes.value("units").toString().toUShort());

	if (attributes.hasAttribute("x"))
		setPosition(attributes.value("x").toString().toDouble(), positionY());
	if (attributes.hasAttribute("y"))
		setPosition(positionX(), attributes.value("y").toString().toDouble());

	if (attributes.hasAttribute("flags"))
		setFlags((Flags)attributes.value("flags").toString().toUInt());
	if (attributes.hasAttribute("placeType"))
		setPlaceType((PlaceType)attributes.value("placeType").toString().toUShort());

	if (attributes.hasAttribute("visible"))
		setVisible(attributes.value("visible").toString().toLower() == "true");

	if (attributes.hasAttribute("rotationAngle"))
		setRotationAngle(attributes.value("rotationAngle").toString().toDouble());
	if (attributes.hasAttribute("flipped"))
		setFlipped(attributes.value("flipped").toString().toLower() == "true");

	// Any properties should be saved in derived class writeXmlAttributes
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::readXmlChildElement(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	if (xmlReader.name() == "itemPoint")
	{
		DiagramItemPoint* newPoint = new DiagramItemPoint();
		QXmlStreamAttributes attributes = xmlReader.attributes();

		QStringList pointConnections;
		int itemIndex, pointIndex;
		DiagramItem* targetItem;
		DiagramItemPoint* targetItemPoint;

		if (attributes.hasAttribute("x"))
			newPoint->setPositionX(attributes.value("x").toString().toDouble());
		if (attributes.hasAttribute("y"))
			newPoint->setPositionY(attributes.value("y").toString().toDouble());

		if (attributes.hasAttribute("width"))
			newPoint->setWidth(attributes.value("width").toString().toDouble());
		if (attributes.hasAttribute("height"))
			newPoint->setHeight(attributes.value("height").toString().toDouble());

		if (attributes.hasAttribute("flags"))
			newPoint->setFlags((DiagramItemPoint::Flags)attributes.value("flags").toString().toUInt());
		if (attributes.hasAttribute("category"))
			newPoint->setCategory(attributes.value("category").toString().toInt());

		if (attributes.hasAttribute("connections"))
		{
			pointConnections = attributes.value("connections").toString().split(
				",", QString::SkipEmptyParts);

			for(int i = 0; i < pointConnections.size(); i += 2)
			{
				itemIndex = pointConnections[i].toInt();
				pointIndex = pointConnections[i+1].toInt();

				if (itemIndex < items.size())
				{
					targetItem = items[itemIndex];
					if (pointIndex < targetItem->numberOfPoints())
					{
						targetItemPoint = targetItem->point(pointIndex);

						newPoint->addTarget(targetItemPoint);
						targetItemPoint->addTarget(newPoint);
					}
				}
			}
		}

		xmlReader.skipCurrentElement();

		addPoint(newPoint);
	}
	else xmlReader.skipCurrentElement();

	// Children should be saved in derived class readXmlChildElements
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramItem::propertiesDialog(QWidget* parent) const
{
	Q_UNUSED(parent);
	return NULL;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::contextMenuEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::mouseDoubleClickEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::mousePressEvent(DiagramMouseEvent& event)
{
	// Update mSelectedPoint
	if (mParent == NULL)
	{
		mSelectedPoint = pointAt(mapFromDiagram(event.diagramPos()));

		if (mSelectedPoint)
		{
			if ((mSelectedPoint->flags() & DiagramItemPoint::Control) == 0) mSelectedPoint = NULL;
			else if ((flags() & CanResize) == 0) mSelectedPoint = NULL;
		}
	}
	else mSelectedPoint = NULL;
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::mouseMoveEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::mouseReleaseEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(reason);
	return value;
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(reason);
	Q_UNUSED(value);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItem::moveEvent(const QPointF& parentPos)
{
	setPosition(parentPos);
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
	if (itemPoint) itemPoint->setPosition(mapFromParent(parentPos));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::rotateEvent(const QPointF& parentPos)
{
	QPointF difference(mPosition - parentPos);

	// Calculate new position of reference point
	setPosition(parentPos.x() - difference.y(), parentPos.y() + difference.x());

	// Update orientation
	if (mFlipped) setRotationAngle(mRotationAngle - 90.0);
	else setRotationAngle(mRotationAngle + 90.0);

	// Calculate new offsets of control points
	for(QList<DiagramItemPoint*>::Iterator pointIter = mPoints.begin();
		pointIter != mPoints.end(); pointIter++)
	{
		(*pointIter)->setPosition(-(*pointIter)->positionY(), (*pointIter)->positionX());
	}

	// Rotate child items as well
	for(QList<DiagramItem*>::Iterator childIter = mChildren.begin(); childIter != mChildren.end(); childIter++)
		(*childIter)->rotateEvent(QPointF(0.0, 0.0));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::rotateBackEvent(const QPointF& parentPos)
{
	QPointF difference(mPosition - parentPos);

	// Calculate new position of reference point
	setPosition(parentPos.x() + difference.y(), parentPos.y() - difference.x());

	// Update orientation
	if (mFlipped) setRotationAngle(mRotationAngle + 90.0);
	else setRotationAngle(mRotationAngle - 90.0);

	// Calculate new offsets of control points
	for(QList<DiagramItemPoint*>::Iterator pointIter = mPoints.begin();
		pointIter != mPoints.end(); pointIter++)
	{
		(*pointIter)->setPosition((*pointIter)->positionY(), -(*pointIter)->positionX());
	}

	// Rotate child items as well
	for(QList<DiagramItem*>::Iterator childIter = mChildren.begin(); childIter != mChildren.end(); childIter++)
		(*childIter)->rotateBackEvent(QPointF(0.0, 0.0));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::flipEvent(const QPointF& parentPos)
{
	// Calculate new position of reference point
	setPosition(2 * parentPos.x() - positionX(), positionY());

	// Update orientation
	mFlipped = !mFlipped;

	// Calculate new offsets of control points
	for(QList<DiagramItemPoint*>::Iterator pointIter = mPoints.begin();
		pointIter != mPoints.end(); pointIter++)
	{
		(*pointIter)->setPosition(-(*pointIter)->positionX(), (*pointIter)->positionY());
	}

	// Flip child items as well
	for(QList<DiagramItem*>::Iterator childIter = mChildren.begin(); childIter != mChildren.end(); childIter++)
		(*childIter)->flipEvent(QPointF(0.0, 0.0));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::alignVerticalEvent(const QPointF& parentPos)
{
    QRectF itemRect;
    itemRect = this->mapToDiagram(this->shape().boundingRect());


    // Calculate new position of reference point
    //    setPosition(2 * parentPos.x() - positionX(), positionY());
    setPosition(parentPos.x()-itemRect.width()/2, positionY());   // vertical alignment
    //    setPosition( positionX(), parentPos.y() );   // horizontal alignment

    // Calculate new offsets of control points
    for(QList<DiagramItemPoint*>::Iterator pointIter = mPoints.begin();
        pointIter != mPoints.end(); pointIter++)
    {
        (*pointIter)->setPosition((*pointIter)->positionX(), (*pointIter)->positionY());
    }

    // Flip child items as well
    for(QList<DiagramItem*>::Iterator childIter = mChildren.begin(); childIter != mChildren.end(); childIter++)
        (*childIter)->alignVerticalEvent(QPointF(0.0, 0.0));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::alignHorizontalEvent(const QPointF& parentPos)
{
    QRectF itemRect;
    itemRect = this->mapToDiagram(this->shape().boundingRect());

    // Calculate new position of reference point
//    setPosition(2 * parentPos.x() - positionX(), positionY());
//    setPosition(parentPos.x() , positionY());   // vertical alignment
    setPosition( positionX(), parentPos.y()-itemRect.height()/2 );   // horizontal alignment

    // Calculate new offsets of control points
    for(QList<DiagramItemPoint*>::Iterator pointIter = mPoints.begin();
        pointIter != mPoints.end(); pointIter++)
    {
        (*pointIter)->setPosition((*pointIter)->positionX(), (*pointIter)->positionY());
    }

    // Flip child items as well
    for(QList<DiagramItem*>::Iterator childIter = mChildren.begin(); childIter != mChildren.end(); childIter++)
        (*childIter)->alignHorizontalEvent(QPointF(0.0, 0.0));
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::insertPointEvent(int index, DiagramItemPoint* itemPoint)
{
	insertPoint(index, itemPoint);
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::removePointEvent(DiagramItemPoint* itemPoint)
{
	removePoint(itemPoint);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItem::createEvent(Diagram* diagram)
{
	Q_UNUSED(diagram);
	return true;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
qreal DiagramItem::adjustOutlineForView(qreal penWidth) const
{
	qreal adjustedWidth = penWidth;

	if (diagram())
	{
		QSize lineViewSize = diagram()->mapFromDiagram(mapToDiagram(QRectF(0, 0, penWidth, penWidth))).size();
		if (lineViewSize.width() < 8 && lineViewSize.height() < 8) adjustedWidth *= 4;
	}

	return adjustedWidth;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem& DiagramItem::operator=(const DiagramItem& item)
{
	mDiagram = NULL;

	mPosition = item.mPosition;
	mUnits = item.mUnits;

	mFlags = item.mFlags;
	mPlaceType = item.mPlaceType;

	for(QList<DiagramItemPoint*>::ConstIterator pointIter = item.mPoints.begin();
		pointIter != item.mPoints.end(); pointIter++) addPoint((*pointIter)->copy());
	mSelectedPoint = NULL;

	mProperties = item.mProperties;

	for(QList<DiagramItem*>::ConstIterator childIter = item.mChildren.begin();
		childIter != item.mChildren.end(); childIter++) addChild((*childIter)->copy());
	mParent = NULL;

	mSelected = item.mSelected;
	mVisible = item.mVisible;

	mRotationAngle = item.mRotationAngle;
	mFlipped = item.mFlipped;

	return *this;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> DiagramItem::copyItems(const QList<DiagramItem*>& items)
{
	QList<DiagramItem*> copiedItems;
	QList<DiagramItemPoint*> itemPoints;
	QList<DiagramItemPoint*> targetPoints;
	DiagramItem* targetItem;
	DiagramItem* copiedTargetItem;
	DiagramItemPoint* copiedTargetPoint;

	// Copy items
	for(QList<DiagramItem*>::ConstIterator itemIter = items.begin();
		itemIter != items.end(); itemIter++)
	{
		copiedItems.append((*itemIter)->copy());
	}

	// Maintain connections to other items in this list
	for(int itemIndex = 0; itemIndex < items.size(); itemIndex++)
	{
		itemPoints = items[itemIndex]->points();
		for(int pointIndex = 0; pointIndex < itemPoints.size(); pointIndex++)
		{
			targetPoints = itemPoints[pointIndex]->targets();
			for(QList<DiagramItemPoint*>::Iterator targetIter = targetPoints.begin();
				targetIter != targetPoints.end(); targetIter++)
			{
				targetItem = (*targetIter)->item();
				if (items.contains(targetItem))
				{
					// There is a connection here that must be maintained in the copied items
					copiedTargetItem = copiedItems[items.indexOf(targetItem)];
					copiedTargetPoint =
						copiedTargetItem->point(targetItem->points().indexOf(*targetIter));

					copiedItems[itemIndex]->point(pointIndex)->addTarget(copiedTargetPoint);
				}
			}
		}

	}

	return copiedItems;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramItemPropertiesDialog::maximumDiagramSizeFromParent(QWidget* parent)
{
    QSizeF diagramSize = diagramSizeFromParent(parent);
    qreal maxSize;

    if (diagramSize.width() <= 0 && diagramSize.height() <= 0) maxSize = INT_MAX;
    else maxSize = qMax(diagramSize.width(), diagramSize.height());

    return maxSize;
}
//--------------------------------------------------------------------------------------------------
NodeInterface DiagramItem::getNi() const
{
    return Ni;
}
//--------------------------------------------------------------------------------------------------
void DiagramItem::setNi(const NodeInterface &value)
{
    Ni = value;
}
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
DiagramItemPropertiesDialog::DiagramItemPropertiesDialog(QWidget* parent) : DiagramTwoButtonDialog(parent) { }
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog::~DiagramItemPropertiesDialog() { }
//--------------------------------------------------------------------------------------------------
void DiagramItemPropertiesDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	mProperties = properties;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPropertiesDialog::setPosition(const QPointF& position)
{
	mPosition = position;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPropertiesDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	mPointPositions = pointPositions;
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramItemPropertiesDialog::properties() const
{
	return mProperties;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItemPropertiesDialog::position() const
{
	return mPosition;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> DiagramItemPropertiesDialog::pointPositions() const
{
	return mPointPositions;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QSizeF DiagramItemPropertiesDialog::diagramSizeFromParent(QWidget* parent)
{
	QSizeF diagramSize;

	Diagram* diagram = qobject_cast<Diagram*>(parent);
	if (diagram) diagramSize = QSizeF(diagram->width(), diagram->height());

	return diagramSize;
}

