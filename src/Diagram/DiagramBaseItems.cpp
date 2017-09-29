/* DiagramBaseItems.cpp
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


#include <DiagramBaseItems.h>
#include <DiagramItemPoint.h>

DiagramTwoPointItem::DiagramTwoPointItem() : DiagramItem()
{
	QVariant variant;
	variant.setValue(DiagramArrow());

	addProperty("Pen", QPen(QColor(255, 0, 255), 12.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	addProperty("Start Arrow", variant);
	addProperty("End Arrow", variant);

	setFlags(CanMove | CanRotate | CanFlip | CanResize | MatchUnitsWithParent);
	setPlaceType(PlaceMouseDownAndUp);

	addPoint(new DiagramItemPoint(QPointF(0.0, 0.0),
		DiagramItemPoint::Control | DiagramItemPoint::Connection | DiagramItemPoint::Free, 0));
	addPoint(new DiagramItemPoint(QPointF(0.0, 0.0),
		DiagramItemPoint::Control | DiagramItemPoint::Connection | DiagramItemPoint::Free, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramTwoPointItem::DiagramTwoPointItem(const DiagramTwoPointItem& item) : DiagramItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramTwoPointItem::~DiagramTwoPointItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTwoPointItem::setPointPosition(int pointIndex, const QPointF& diagramPos)
{
	resizeEvent(point(pointIndex), mapToParent(mapFromDiagram(diagramPos)));
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPointItem::setPointPosition(int pointIndex, qreal diagramX, qreal diagramY)
{
	setPointPosition(pointIndex, QPointF(diagramX, diagramY));
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPointItem::setPointPositionX(int pointIndex, qreal diagramX)
{
	setPointPosition(pointIndex, QPointF(diagramX, pointPositionY(pointIndex)));
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPointItem::setPointPositionY(int pointIndex, qreal diagramY)
{
	setPointPosition(pointIndex, QPointF(pointPositionX(pointIndex), diagramY));
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramTwoPointItem::pointPosition(int pointIndex) const
{
	QPointF diagramPos;

	DiagramItemPoint* itemPoint = point(pointIndex);
	if (itemPoint) diagramPos = mapToDiagram(itemPoint->position());

	return diagramPos;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramTwoPointItem::pointPositionX(int pointIndex) const
{
	return pointPosition(pointIndex).x();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramTwoPointItem::pointPositionY(int pointIndex) const
{
	return pointPosition(pointIndex).y();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTwoPointItem::setArrow(int pointIndex, const DiagramArrow& arrow)
{
	QVariant variant;
	variant.setValue(arrow);

	if (pointIndex == 1) setPropertyValue("End Arrow", variant);
	else setPropertyValue("Start Arrow", variant);
}
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramTwoPointItem::arrow(int pointIndex) const
{
	DiagramArrow arrow;

	if (pointIndex == 1) arrow = propertyValue("End Arrow").value<DiagramArrow>();
	else arrow = propertyValue("Start Arrow").value<DiagramArrow>();

	return arrow;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTwoPointItem::setPen(const QPen& pen)
{
	setPropertyValue("Pen", pen);
}
//--------------------------------------------------------------------------------------------------
QPen DiagramTwoPointItem::pen() const
{
	return propertyValue("Pen").value<QPen>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramTwoPointItem::startPoint() const
{
	return point(0);
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramTwoPointItem::endPoint() const
{
	return point(1);
}
//--------------------------------------------------------------------------------------------------
bool DiagramTwoPointItem::isSuperfluous() const
{
	return (startPoint()->position() == endPoint()->position());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramTwoPointItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);
		QPen lPen = pen();
		DiagramArrow lArrow;

		lArrow = arrow(0);
		lArrow.setSize(lArrow.size() * scaleFactor);
		setArrow(0, lArrow);

		lArrow = arrow(1);
		lArrow.setSize(lArrow.size() * scaleFactor);
		setArrow(1, lArrow);

		lPen.setWidthF(lPen.widthF() * scaleFactor);
		setPen(lPen);
	}
	else if (reason == AddNewItemToDiagram) adjustReferencePoint();

	return value;
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPointItem::resizeEvent(DiagramItemPoint* point, const QPointF& parentPos)
{
	DiagramItem::resizeEvent(point, parentPos);
	adjustReferencePoint();
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramBoxResizeItem::DiagramBoxResizeItem() : DiagramItem()
{
	addProperty("Pen", QPen(QColor(255, 0, 255), 12.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	addProperty("Brush", QVariant(QBrush(Qt::transparent)));

	setFlags(CanMove | CanRotate | CanFlip | CanResize | MatchUnitsWithParent);
	setPlaceType(PlaceMouseDownAndUp);

	for(int i = 0; i < 4; i++)
		addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Control, 0));

	for(int i = 4; i < 8; i++)
	{
		addPoint(new DiagramItemPoint(QPointF(0.0, 0.0),
			DiagramItemPoint::Control | DiagramItemPoint::Connection, 0));
	}
}
//--------------------------------------------------------------------------------------------------
DiagramBoxResizeItem::DiagramBoxResizeItem(const DiagramBoxResizeItem& item) : DiagramItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramBoxResizeItem::~DiagramBoxResizeItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBoxResizeItem::setRect(const QRectF& diagramRect)
{
	resizeEvent(point(0), mapToParent(mapFromDiagram(diagramRect.topLeft())));
	resizeEvent(point(1), mapToParent(mapFromDiagram(diagramRect.bottomRight())));
}
//--------------------------------------------------------------------------------------------------
void DiagramBoxResizeItem::setPen(const QPen& pen)
{
	setPropertyValue("Pen", pen);
}
//--------------------------------------------------------------------------------------------------
void DiagramBoxResizeItem::setBrush(const QBrush& brush)
{
	setPropertyValue("Brush", brush);
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramBoxResizeItem::rect() const
{
	return mapToDiagram(boundingRect());
}
//--------------------------------------------------------------------------------------------------
QPen DiagramBoxResizeItem::pen() const
{
	return propertyValue("Pen").value<QPen>();
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramBoxResizeItem::brush() const
{
	return propertyValue("Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramBoxResizeItem::isSuperfluous() const
{
	return (point(0)->position() == point(1)->position());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBoxResizeItem::adjustBoxControlPoints(DiagramItemPoint* activePoint)
{
	if (activePoint == NULL) activePoint = selectedPoint();

	if (activePoint)
	{
		DiagramItemPoint* otherPoint = NULL;
		DiagramItemPoint* midPoint = NULL;
		DiagramItemPoint* leftPoint = NULL;
		DiagramItemPoint* rightPoint = NULL;
		int index = points().indexOf(activePoint);
		bool rotated = (rotationAngle() == 90.0 || rotationAngle() == 270.0);

		switch (index)
		{
		case 0:
			otherPoint = point(2);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionX(activePoint->positionX());
				else otherPoint->setPositionY(activePoint->positionY());
			}
			otherPoint = point(3);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionY(activePoint->positionY());
				else otherPoint->setPositionX(activePoint->positionX());
			}
			break;
		case 2:
			otherPoint = point(0);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionX(activePoint->positionX());
				else otherPoint->setPositionY(activePoint->positionY());
			}
			otherPoint = point(1);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionY(activePoint->positionY());
				else otherPoint->setPositionX(activePoint->positionX());
			}
			break;
		case 1:
			otherPoint = point(3);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionX(activePoint->positionX());
				else otherPoint->setPositionY(activePoint->positionY());
			}
			otherPoint = point(2);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionY(activePoint->positionY());
				else otherPoint->setPositionX(activePoint->positionX());
			}
			break;
		case 3:
			otherPoint = point(1);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionX(activePoint->positionX());
				else otherPoint->setPositionY(activePoint->positionY());
			}
			otherPoint = point(0);
			if (otherPoint)
			{
				if (rotated) otherPoint->setPositionY(activePoint->positionY());
				else otherPoint->setPositionX(activePoint->positionX());
			}
			break;
		case 4:
			leftPoint = point(0);
			rightPoint = point(2);
			if (leftPoint && rightPoint)
			{
				if (rotated)
				{
					activePoint->setPositionY((leftPoint->positionY() + rightPoint->positionY()) / 2);
					leftPoint->setPositionX(activePoint->positionX());
					rightPoint->setPositionX(activePoint->positionX());
				}
				else
				{
					activePoint->setPositionX((leftPoint->positionX() + rightPoint->positionX()) / 2);
					leftPoint->setPositionY(activePoint->positionY());
					rightPoint->setPositionY(activePoint->positionY());
				}
			}
			break;
		case 5:
			leftPoint = point(2);
			rightPoint = point(1);
			if (leftPoint && rightPoint)
			{
				if (rotated)
				{
					activePoint->setPositionX((leftPoint->positionX() + rightPoint->positionX()) / 2);
					leftPoint->setPositionY(activePoint->positionY());
					rightPoint->setPositionY(activePoint->positionY());
				}
				else
				{
					activePoint->setPositionY((leftPoint->positionY() + rightPoint->positionY()) / 2);
					leftPoint->setPositionX(activePoint->positionX());
					rightPoint->setPositionX(activePoint->positionX());
				}
			}
			break;
		case 6:
			leftPoint = point(1);
			rightPoint = point(3);
			if (leftPoint && rightPoint)
			{
				if (rotated)
				{
					activePoint->setPositionY((leftPoint->positionY() + rightPoint->positionY()) / 2);
					leftPoint->setPositionX(activePoint->positionX());
					rightPoint->setPositionX(activePoint->positionX());
				}
				else
				{
					activePoint->setPositionX((leftPoint->positionX() + rightPoint->positionX()) / 2);
					leftPoint->setPositionY(activePoint->positionY());
					rightPoint->setPositionY(activePoint->positionY());
				}
			}
			break;
		case 7:
			leftPoint = point(3);
			rightPoint = point(0);
			if (leftPoint && rightPoint)
			{
				if (rotated)
				{
					activePoint->setPositionX((leftPoint->positionX() + rightPoint->positionX()) / 2);
					leftPoint->setPositionY(activePoint->positionY());
					rightPoint->setPositionY(activePoint->positionY());
				}
				else
				{
					activePoint->setPositionY((leftPoint->positionY() + rightPoint->positionY()) / 2);
					leftPoint->setPositionX(activePoint->positionX());
					rightPoint->setPositionX(activePoint->positionX());
				}
			}
			break;
		}

		// Update midpoints
		midPoint = point(4); leftPoint = point(0); rightPoint = point(2);
		if (midPoint && leftPoint && rightPoint)
			midPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);

		midPoint = point(5); leftPoint = point(2); rightPoint = point(1);
		if (midPoint && leftPoint && rightPoint)
			midPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);

		midPoint = point(6); leftPoint = point(1); rightPoint = point(3);
		if (midPoint && leftPoint && rightPoint)
			midPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);

		midPoint = point(7); leftPoint = point(3); rightPoint = point(0);
		if (midPoint && leftPoint && rightPoint)
			midPoint->setPosition((leftPoint->position() + rightPoint->position()) / 2);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramBoxResizeItem::adjustEllipseControlPoints(DiagramItemPoint* activePoint)
{
	const qreal PI = 3.14159265358;
	DiagramItemPoint* startPoint = point(0);
	DiagramItemPoint* endPoint = point(1);

	adjustBoxControlPoints(activePoint);

	if (startPoint && endPoint)
	{
		// Adjust points at 45 degree offsets
		QRectF rect = QRectF(startPoint->position(), endPoint->position());
		qreal radiusX = rect.width() / 2;
		qreal radiusY = rect.height() / 2;

		DiagramItemPoint* cornerPoint = NULL;
		qreal angle = -3 * PI / 4;
		qreal angleStep = (rotationAngle() == 90.0 || rotationAngle() == 270.0) ? -PI / 2 : PI / 2;

		for(int i = 8; i < 12; i++)
		{
			cornerPoint = point(i);
			if (cornerPoint)
			{
				cornerPoint->setPosition(rect.center() +
										 QPointF(radiusX * qCos(angle), radiusY * qSin(angle)));
			}
			angle += angleStep;
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramBoxResizeItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);
		QPen lPen = pen();

		lPen.setWidthF(lPen.widthF() * scaleFactor);
		setPen(lPen);
	}

	return value;
}
//--------------------------------------------------------------------------------------------------
void DiagramBoxResizeItem::resizeEvent(DiagramItemPoint* point, const QPointF& parentPos)
{
	DiagramItem::resizeEvent(point, parentPos);
	adjustBoxControlPoints(point);
	adjustReferencePoint();
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPolyItem::DiagramPolyItem() : DiagramItem()
{
	addProperty("Pen", QPen(QColor(255, 0, 255), 12.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	setFlags(CanMove | CanRotate | CanFlip | CanResize | CanInsertRemovePoints | MatchUnitsWithParent);
	setPlaceType(PlaceMouseDownAndUp);
}
//--------------------------------------------------------------------------------------------------
DiagramPolyItem::DiagramPolyItem(const DiagramPolyItem& item) : DiagramItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramPolyItem::~DiagramPolyItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPolyItem::setPointPosition(int index, const QPointF& diagramPos)
{
	resizeEvent(point(index), mapToParent(mapFromDiagram(diagramPos)));
}
//--------------------------------------------------------------------------------------------------
void DiagramPolyItem::setPointPosition(int index, qreal diagramX, qreal diagramY)
{
	setPointPosition(index, QPointF(diagramX, diagramY));
}
//--------------------------------------------------------------------------------------------------
void DiagramPolyItem::setPointPositionX(int index, qreal diagramX)
{
	setPointPosition(index, QPointF(diagramX, pointPositionY(index)));
}
//--------------------------------------------------------------------------------------------------
void DiagramPolyItem::setPointPositionY(int index, qreal diagramY)
{
	setPointPosition(index, QPointF(pointPositionX(index), diagramY));
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramPolyItem::pointPosition(int index) const
{
	QPointF diagramPos;

	DiagramItemPoint* itemPoint = point(index);
	if (itemPoint) diagramPos = mapToDiagram(itemPoint->position());

	return diagramPos;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPolyItem::pointPositionX(int index) const
{
	return pointPosition(index).x();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPolyItem::pointPositionY(int index) const
{
	return pointPosition(index).y();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPolyItem::setPen(const QPen& pen)
{
	setPropertyValue("Pen", pen);
}
//--------------------------------------------------------------------------------------------------
QPen DiagramPolyItem::pen() const
{
	return propertyValue("Pen").value<QPen>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramPolyItem::isSuperfluous() const
{
	bool superfluous = true;
	QPointF pos = point(0)->position();
	QList<DiagramItemPoint*> lPoints = points();

	for(QList<DiagramItemPoint*>::ConstIterator pointIter = lPoints.begin();
		superfluous && pointIter != lPoints.end(); pointIter++)
	{
		superfluous = (pos == (*pointIter)->position());
	}

	return superfluous;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramPolyItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);
		QPen lPen = pen();

		lPen.setWidthF(lPen.widthF() * scaleFactor);
		setPen(lPen);
	}

	return value;
}
//--------------------------------------------------------------------------------------------------
void DiagramPolyItem::resizeEvent(DiagramItemPoint* point, const QPointF& parentPos)
{
	DiagramItem::resizeEvent(point, parentPos);
	adjustReferencePoint();
}
//--------------------------------------------------------------------------------------------------
