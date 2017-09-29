/* DiagramBaseItems.h
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

#ifndef DIAGRAMBASEITEMS_H
#define DIAGRAMBASEITEMS_H

#include <DiagramItem.h>

class DiagramTwoPointItem : public DiagramItem
{
public:
	DiagramTwoPointItem();
	DiagramTwoPointItem(const DiagramTwoPointItem& item);
	virtual ~DiagramTwoPointItem();

	void setPointPosition(int pointIndex, const QPointF& diagramPos);
	void setPointPosition(int pointIndex, qreal diagramX, qreal diagramY);
	void setPointPositionX(int pointIndex, qreal diagramX);
	void setPointPositionY(int pointIndex, qreal diagramY);
	QPointF pointPosition(int pointIndex) const;
	qreal pointPositionX(int pointIndex) const;
	qreal pointPositionY(int pointIndex) const;

	void setArrow(int pointIndex, const DiagramArrow& arrow);
	DiagramArrow arrow(int pointIndex) const;

	void setPen(const QPen& pen);
	QPen pen() const;

	DiagramItemPoint* startPoint() const;
	DiagramItemPoint* endPoint() const;
	virtual bool isSuperfluous() const;

protected:
	virtual QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
	virtual void resizeEvent(DiagramItemPoint* point, const QPointF& parentPos);
};

//==================================================================================================

class DiagramBoxResizeItem : public DiagramItem
{
public:
	DiagramBoxResizeItem();
	DiagramBoxResizeItem(const DiagramBoxResizeItem& item);
	virtual ~DiagramBoxResizeItem();

	void setRect(const QRectF& diagramRect);
	void setPen(const QPen& pen);
	void setBrush(const QBrush& brush);
	QRectF rect() const;
	QPen pen() const;
	QBrush brush() const;

	virtual bool isSuperfluous() const;

	void adjustBoxControlPoints(DiagramItemPoint* activePoint);
	void adjustEllipseControlPoints(DiagramItemPoint* activePoint);

protected:
	virtual QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
	virtual void resizeEvent(DiagramItemPoint* point, const QPointF& parentPos);
};

//==================================================================================================

class DiagramPolyItem : public DiagramItem
{
public:
	DiagramPolyItem();
	DiagramPolyItem(const DiagramPolyItem& item);
	virtual ~DiagramPolyItem();

	void setPointPosition(int index, const QPointF& diagramPos);
	void setPointPosition(int index, qreal diagramX, qreal diagramY);
	void setPointPositionX(int index, qreal diagramX);
	void setPointPositionY(int index, qreal diagramY);
	QPointF pointPosition(int index) const;
	qreal pointPositionX(int index) const;
	qreal pointPositionY(int index) const;

	void setPen(const QPen& pen);
	QPen pen() const;

	virtual bool isSuperfluous() const;

protected:
	virtual QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
	virtual void resizeEvent(DiagramItemPoint* point, const QPointF& parentPos);
};

#endif
