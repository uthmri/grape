/* DiagramItemPoint.cpp
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

#include <DiagramItemPoint.h>
#include <DiagramItem.h>
#include <Diagram.h>

DiagramItemPoint::DiagramItemPoint(const QPointF& position, Flags flags, int category)
{
	mItem = NULL;

	mPosition = position;
	mFlags = flags;
	mCategory = category;

	mSize = QSize(8, 8);
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint::DiagramItemPoint(const DiagramItemPoint& point)
{
	mItem = NULL;

	mPosition = point.mPosition;
	mSize = point.mSize;
	mFlags = point.mFlags;
	mCategory = point.mCategory;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint::~DiagramItemPoint()
{
	clearTargets();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramItemPoint::copy() const
{
	return new DiagramItemPoint(*this);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramItemPoint::item() const
{
	return mItem;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setPosition(const QPointF& position)
{
	mPosition = position;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setPosition(qreal x, qreal y)
{
	mPosition = QPointF(x, y);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setPositionX(qreal x)
{
	mPosition.setX(x);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setPositionY(qreal y)
{
	mPosition.setY(y);
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItemPoint::position() const
{
	return mPosition;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramItemPoint::positionX() const
{
	return mPosition.x();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramItemPoint::positionY() const
{
	return mPosition.y();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setSize(const QSize &size)
{
	mSize = size;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setSize(int width, int height)
{
	mSize.setWidth(width);
	mSize.setHeight(height);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setWidth(int width)
{
	mSize.setWidth(width);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setHeight(int height)
{
	mSize.setHeight(height);
}
//--------------------------------------------------------------------------------------------------
QSize DiagramItemPoint::size() const
{
	return mSize;
}
//--------------------------------------------------------------------------------------------------
int DiagramItemPoint::width() const
{
	return mSize.width();
}
//--------------------------------------------------------------------------------------------------
int DiagramItemPoint::height() const
{
	return mSize.height();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setFlags(Flags flags)
{
	mFlags = flags;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint::Flags DiagramItemPoint::flags() const
{
	return mFlags;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::isControlPoint() const
{
	return ((mFlags & Control) > 0);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::isConnectionPoint() const
{
    return ((mFlags & Connection) > 0);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::isInputPoint() const
{
    return ((mFlags & Input) > 0);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::isOutputPoint() const
{
    return ((mFlags & Output) > 0);
}
//--------------------------------------------------------------------------------------------------

bool DiagramItemPoint::isFreshInput() const
{
    return ((mFlags & FreshInput) > 0);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::hasOutputData() const
{
    return ((mFlags & HasOutput) > 0);
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::isFree() const
{
	return ((mFlags & Free) > 0);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::setCategory(int category)
{
	mCategory = category;
}
//--------------------------------------------------------------------------------------------------
int DiagramItemPoint::category() const
{
	return mCategory;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::addTarget(DiagramItemPoint* point)
{
	if (point && !isTarget(point)) mTargets.append(point);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::removeTarget(DiagramItemPoint* point)
{
	if (point) mTargets.removeAll(point);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPoint::clearTargets()
{
	DiagramItemPoint* point;

	while (numberOfTargets() > 0)
	{
		point = mTargets.first();

		removeTarget(point);
		point->removeTarget(this);
	}
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItemPoint*> DiagramItemPoint::targets() const
{
	return mTargets;
}
//--------------------------------------------------------------------------------------------------
int DiagramItemPoint::numberOfTargets() const
{
	return mTargets.size();
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::isTarget(DiagramItemPoint* point) const
{
	bool contains = false;

	if (point) contains = mTargets.contains(point);

	return contains;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::isTarget(DiagramItem* item) const
{
	bool target = false;

	if (item)
	{
		for(QList<DiagramItemPoint*>::ConstIterator pointIter = mTargets.begin();
			!target && pointIter != mTargets.end(); pointIter++)
		{
			target = ((*pointIter)->item() == item);
		}
	}

	return target;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramItemPoint::target(int index) const
{
	DiagramItemPoint* target = NULL;

	if (0 <= index && index < numberOfTargets()) target = mTargets[index];

	return target;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::shouldConnect(DiagramItemPoint* otherItemPoint, DiagramItemPlaceMode placeMode) const
{
	// Assume both points are members of different items in the same diagram
	bool result = false;

	if (mItem->diagram() && placeMode != DoNotPlace)
	{
//        qreal threshold = (placeMode == PlaceLoose) ? mItem->diagram()->grid() / 2 : 0.001;
//        qreal threshold = (placeMode == PlaceLoose) ? mItem->diagram()->grid() / 4 : 0.001;
        qreal threshold = (placeMode == PlaceLoose) ? mItem->diagram()->grid() / 1 : 0.001;
        qreal distance = magnitude(mItem->mapToDiagram(
			position()) - otherItemPoint->item()->mapToDiagram(otherItemPoint->position()));

        result = (isConnectionPoint() && otherItemPoint->isConnectionPoint() &&
            (isFree() || otherItemPoint->isFree()) && category() == otherItemPoint->category() &&
            !isTarget(otherItemPoint) && !isTarget(otherItemPoint->item()) && distance < threshold);
    }

	return result;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemPoint::shouldDisconnect(DiagramItemPoint* otherItemPoint) const
{
//    return !(mItem->mapToDiagram(position()) !=
//		otherItemPoint->item()->mapToDiagram(otherItemPoint->position()) &&
//		otherItemPoint->item()->canResize() && otherItemPoint->isControlPoint());

    // bug fixed by UT 2016-06-22
    return !( ( mItem->mapToDiagram(position()) != otherItemPoint->item()->mapToDiagram(otherItemPoint->position()) )
              && otherItemPoint->item()->canResize()
              && otherItemPoint->isControlPoint() );
//        return false;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramItemPoint::boundingRect() const
{
	QRectF rect;

	if (mItem && mItem->diagram())
	{
		QPoint viewPos = mItem->diagram()->mapFromDiagram(mItem->position() +
			mItem->mapToDiagram(position()));
		QPoint delta(mSize.width() / 2, mSize.height() / 2);

		QPointF topLeft = mItem->mapFromDiagram(mItem->diagram()->mapToDiagram(viewPos - delta));
		QPointF bottomRight = mItem->mapFromDiagram(mItem->diagram()->mapToDiagram(viewPos + delta));

		rect = QRectF(topLeft, bottomRight).translated(-mItem->position() - position());
	}

	return rect;
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramItemPoint::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPoint& DiagramItemPoint::operator=(const DiagramItemPoint& point)
{
	mItem = NULL;

	mPosition = point.mPosition;
	mSize = point.mSize;
	mFlags = point.mFlags;
	mCategory = point.mCategory;

	return *this;
}
//--------------------------------------------------------------------------------------------------
