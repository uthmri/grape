/* DiagramItem.h
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

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <DiagramTypes.h>
#include <DiagramWidgets.h>
#include "Nodes/nodeinterface.h"

class Diagram;
class DiagramItemPoint;
class DiagramItemPropertiesDialog;

class DiagramItem
{
	friend class Diagram;
	friend class DiagramItemsUndoCommand;
	friend class DiagramItemGroup;

public:
	enum Reason { PositionChange, UnitsChange, SelectedChange, VisibleChange, PropertyChange,
		AddNewItemToDiagram, UserChange };

	enum Flag { CanMove = 0x01, CanRotate = 0x02, CanFlip = 0x04, CanResize = 0x08,
        CanInsertRemovePoints = 0x10, MatchUnitsWithParent = 0x1000, UsePalette = 0x2000, CanAlignVertical=0x4000, CanAlignHorizontal=0x5000 };
	Q_DECLARE_FLAGS(Flags, Flag)

	enum PlaceType { PlaceMouseUp, PlaceMouseDownAndUp, PlaceTwoClick };

private:
	Diagram* mDiagram;

    NodeInterface Ni;

	QPointF mPosition;
	DiagramUnits mUnits;

	Flags mFlags;
	PlaceType mPlaceType;

	QList<DiagramItemPoint*> mPoints;
	DiagramItemPoint* mSelectedPoint;

	QHash<QString, QVariant> mProperties;

	QList<DiagramItem*> mChildren;
	DiagramItem* mParent;

	bool mSelected;
	bool mVisible;

	qreal mRotationAngle;
	bool mFlipped;

public:
    //QToolTip mToolTip;

public:
	DiagramItem();
	DiagramItem(const DiagramItem& item);
	virtual ~DiagramItem();

	Diagram* diagram() const;
	virtual DiagramItem* copy() const = 0;
	virtual QString uniqueKey() const = 0;
	virtual QString iconPath() const;

	// Selectors
	void setPosition(const QPointF& parentPos);
	void setPosition(qreal x, qreal y);
	void setUnits(DiagramUnits units);
	QPointF position() const;
	qreal positionX() const;
	qreal positionY() const;
	DiagramUnits units() const;

	void setFlags(Flags flags);
	void setPlaceType(PlaceType type);
	Flags flags() const;
	PlaceType placeType() const;
	bool canMove() const;
	bool canRotate() const;
    bool canFlip() const;
    bool canAlignVertical() const;
    bool canAlignHorizontal() const;
    bool canResize() const;
	bool canInsertRemovePoints() const;
	bool shouldMatchUnitsWithParent() const;

	// Properties
	void addProperty(const QString& property, const QVariant& value);
	void removeProperty(const QString& property);
	void clearProperties();
	int numberOfProperties() const;
	bool containsProperty(const QString& property) const;
	QStringList propertiesList() const;
	QList<QVariant> propertyValues() const;

	void setPropertyValue(const QString& property, const QVariant& value);
	QVariant propertyValue(const QString& property) const;

	void setProperties(const QHash<QString, QVariant>& properties);
	QHash<QString, QVariant> properties() const;

	// Points
	void addPoint(DiagramItemPoint* itemPoint);
	void insertPoint(int index, DiagramItemPoint* itemPoint);
	void removePoint(DiagramItemPoint* itemPoint);
	void clearPoints();
	QList<DiagramItemPoint*> points() const;
	int numberOfPoints() const;
	bool containsPoint(DiagramItemPoint* itemPoint) const;
	DiagramItemPoint* point(int index) const;

	DiagramItemPoint* pointAt(const QPointF& itemPos) const;
	DiagramItemPoint* pointNearest(const QPointF& itemPos) const;
	void adjustReferencePoint();

	virtual bool canInsertPoint(DiagramItemPoint* itemPoint) const;
	virtual bool canRemovePoint(DiagramItemPoint* itemPoint) const;
	virtual int insertPointIndex(DiagramItemPoint* itemPoint) const;

	bool hasSelectedPoint() const;
	DiagramItemPoint* selectedPoint() const;

	// Children
	void addChild(DiagramItem* child);
	void insertChild(int index, DiagramItem* child);
	void removeChild(DiagramItem* child);
	void clearChildren();
	QList<DiagramItem*> children() const;
	int numberOfChildren() const;
	bool containsChild(DiagramItem* child) const;
	DiagramItem* child(int index) const;

	void setParent(DiagramItem* parent);
	DiagramItem* parent() const;

	// Transformations and status
	void setSelected(bool select);
	void setVisible(bool visible);
	bool isSelected() const;
	bool isVisible() const;

	void setRotationAngle(qreal angle);
	void setFlipped(bool flipped);
	qreal rotationAngle() const;
	bool isFlipped() const;

	QPointF mapToParent(const QPointF& itemPos) const;
	QPointF mapFromParent(const QPointF& parentPos) const;
	QRectF mapToParent(const QRectF& itemRect) const;
	QRectF mapFromParent(const QRectF& parentRect) const;

	QPointF mapToDiagram(const QPointF& itemPos) const;
	QPointF mapFromDiagram(const QPointF& diagramPos) const;
	QRectF mapToDiagram(const QRectF& itemRect) const;
	QRectF mapFromDiagram(const QRectF& diagramRect) const;

	// Description
	virtual QRectF boundingRect() const = 0;
	virtual QPainterPath shape() const;
	virtual QPointF centerPosition() const;

	virtual bool isSuperfluous() const;

    virtual QString caption();
    virtual QString caption1();

	// Render
	virtual void render(DiagramPainter* painter) = 0;

	// Save
	virtual void writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	virtual void writeXmlChildElements(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	virtual void readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);
	virtual void readXmlChildElement(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);

	virtual DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
	virtual void contextMenuEvent(DiagramMouseEvent& event);
	virtual void mouseDoubleClickEvent(DiagramMouseEvent& event);
	virtual void mousePressEvent(DiagramMouseEvent& event);
	virtual void mouseMoveEvent(DiagramMouseEvent& event);
	virtual void mouseReleaseEvent(DiagramMouseEvent& event);

	virtual QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
	virtual void changedEvent(Reason reason, const QVariant& value);

	virtual void moveEvent(const QPointF& parentPos);
	virtual void resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos);
	virtual void rotateEvent(const QPointF& parentPos);
	virtual void rotateBackEvent(const QPointF& parentPos);
    virtual void flipEvent(const QPointF& parentPos);
    virtual void alignVerticalEvent(const QPointF& parentPos);
    virtual void alignHorizontalEvent(const QPointF& parentPos);
    virtual void insertPointEvent(int index, DiagramItemPoint* itemPoint);
	virtual void removePointEvent(DiagramItemPoint* itemPoint);
	virtual bool createEvent(Diagram* diagram);

	qreal adjustOutlineForView(qreal penWidth) const;

	DiagramItem& operator=(const DiagramItem& item);

public:
	static QList<DiagramItem*> copyItems(const QList<DiagramItem*>& items);
    NodeInterface getNi() const;
    void setNi(const NodeInterface &value);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramItem::Flags)

//==================================================================================================

class DiagramItemPropertiesDialog : public DiagramTwoButtonDialog
{
	Q_OBJECT

private:
	QHash<QString, QVariant> mProperties;
	QPointF mPosition;
	QList<QPointF> mPointPositions;

public:
	DiagramItemPropertiesDialog(QWidget* parent = NULL);
	virtual ~DiagramItemPropertiesDialog();

	virtual void setProperties(const QHash<QString, QVariant>& properties);
	virtual void setPosition(const QPointF& position);
	virtual void setPointPositions(const QList<QPointF>& pointPositions);
	virtual QHash<QString, QVariant> properties() const;
	virtual QPointF position() const;
	virtual QList<QPointF> pointPositions() const;

	QSizeF diagramSizeFromParent(QWidget* parent);
	qreal maximumDiagramSizeFromParent(QWidget* parent);
};

#endif
