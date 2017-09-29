/* DiagramItemPoint.h
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

#ifndef DIAGRAMITEMPOINT_H
#define DIAGRAMITEMPOINT_H

#include <DiagramTypes.h>

class DiagramItem;

class DiagramItemPoint
{
	friend class DiagramItem;

public:
	enum Flag { Control = 0x01,         // Point can be used to resize its item
				Connection = 0x02,      // Other items can connect to this point
                Free = 0x04, Input = 0x08, Output = 0x10, FreshInput = 0x20, HasOutput = 0x40};          // For points that are both Control and Connection,
										// indicates that resizing this point does not affect any
										// other points in the same item that are also both Control
										// and Connection points
	Q_DECLARE_FLAGS(Flags, Flag)

private:
	DiagramItem* mItem;
	QPointF mPosition;
	QSize mSize;
	int mCategory;
	Flags mFlags;
	QList<DiagramItemPoint*> mTargets;

public:
	DiagramItemPoint(const QPointF& position = QPointF(), Flags flags = Control, int category = 0);
	DiagramItemPoint(const DiagramItemPoint& other);
	virtual ~DiagramItemPoint();

	virtual DiagramItemPoint* copy() const;

	DiagramItem* item() const;

	// Properties
	void setPosition(const QPointF& position);
	void setPosition(qreal x, qreal y);
	void setPositionX(qreal x);
	void setPositionY(qreal y);
	QPointF position() const;
	qreal positionX() const;
	qreal positionY() const;

	void setSize(const QSize &size);
	void setSize(int width, int height);
	void setWidth(int width);
	void setHeight(int height);
	QSize size() const;
	int width() const;
	int height() const;

	void setFlags(Flags flags);
	Flags flags() const;
	bool isControlPoint() const;
    bool isConnectionPoint() const;
    bool isInputPoint() const;
    bool isOutputPoint() const;
    bool isFreshInput() const;
    bool hasOutputData() const;
    bool isFree() const;

	void setCategory(int category);
	int category() const;

	// Targets
	void addTarget(DiagramItemPoint* itemPoint);
	void removeTarget(DiagramItemPoint* itemPoint);
	void clearTargets();
	QList<DiagramItemPoint*> targets() const;
	int numberOfTargets() const;
	bool isTarget(DiagramItemPoint* itemPoint) const;
	bool isTarget(DiagramItem* item) const;
	DiagramItemPoint* target(int index) const;

	bool shouldConnect(DiagramItemPoint* otherItemPoint, DiagramItemPlaceMode placeMode) const;
	bool shouldDisconnect(DiagramItemPoint* otherItemPoint) const;

	// Shape
	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;

protected:
	DiagramItemPoint& operator=(const DiagramItemPoint& other);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramItemPoint::Flags)

#endif
