/* DiagramPathItem.cpp
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

#include <DiagramPathItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramPathItem::DiagramPathItem() : DiagramItem()
{
	QRect rect(-100, -100, 200, 200);

	addProperty("Pen", QPen(QColor(255, 0, 255), 12.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

	setFlags(CanMove | CanRotate | CanFlip | CanResize | MatchUnitsWithParent);
	setPlaceType(PlaceMouseUp);

	for(int i = 0; i < 8; i++)
		addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Control, 0));

	point(0)->setPosition(rect.left(), rect.top());
	point(1)->setPosition(rect.right(), rect.bottom());
	point(2)->setPosition(rect.right(), rect.top());
	point(3)->setPosition(rect.left(), rect.bottom());
	point(4)->setPosition((point(0)->position() + point(2)->position()) / 2);
	point(5)->setPosition((point(2)->position() + point(1)->position()) / 2);
	point(6)->setPosition((point(1)->position() + point(3)->position()) / 2);
	point(7)->setPosition((point(3)->position() + point(0)->position()) / 2);
}
//--------------------------------------------------------------------------------------------------
DiagramPathItem::DiagramPathItem(const DiagramPathItem& item) : DiagramItem(item)
{
	mPath = item.mPath;
}
//--------------------------------------------------------------------------------------------------
DiagramPathItem::~DiagramPathItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramPathItem::copy() const
{
	return new DiagramPathItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramPathItem::uniqueKey() const
{
	return "path";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::setPath(const QPainterPath& path)
{
	mPath = path;
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramPathItem::path() const
{
	return mPath;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::setRect(const QRectF& diagramRect)
{
	resizeEvent(point(0), mapToParent(mapFromDiagram(diagramRect.topLeft())));
	resizeEvent(point(1), mapToParent(mapFromDiagram(diagramRect.bottomRight())));
}
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::setPen(const QPen& pen)
{
	setPropertyValue("Pen", pen);
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramPathItem::rect() const
{
	return mapToDiagram(boundingRect());
}
//--------------------------------------------------------------------------------------------------
QPen DiagramPathItem::pen() const
{
	return propertyValue("Pen").value<QPen>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPointF DiagramPathItem::mapToPath(const QPointF& itemPos) const
{
	QPointF newPoint;

	QRectF pathRect = mPath.boundingRect();
	QRectF bounds = boundingRect();
	QPointF topLeft = DiagramItem::point(0)->position();
	QSizeF size;
	qreal cosAngle, sinAngle;
	qreal ratioX, ratioY;

	if (rotationAngle() == 90 || rotationAngle() == 270)
	{
		size.setWidth(bounds.height());
		size.setHeight(bounds.width());
	}
	else size = bounds.size();

	if (rotationAngle() == 90.0) { cosAngle = 0; sinAngle = 1; }
	else if (rotationAngle() == 180.0) { cosAngle = -1; sinAngle = 0; }
	else if (rotationAngle() == 270.0) { cosAngle = 0; sinAngle = -1; }
	else { cosAngle = 1; sinAngle = 0; }

	if (isFlipped())
	{
		ratioX = (-cosAngle * (itemPos.x() - topLeft.x()) + sinAngle * (itemPos.y() - topLeft.y())) / size.width();
		ratioY = (sinAngle * (itemPos.x() - topLeft.x()) + cosAngle * (itemPos.y() - topLeft.y())) / size.height();
	}
	else
	{
		ratioX = (cosAngle * (itemPos.x() - topLeft.x()) + sinAngle * (itemPos.y() - topLeft.y())) / size.width();
		ratioY = (-sinAngle * (itemPos.x() - topLeft.x()) + cosAngle * (itemPos.y() - topLeft.y())) / size.height();
	}

	newPoint.setX(ratioX * pathRect.width() + pathRect.left());
	newPoint.setY(ratioY * pathRect.height() + pathRect.top());

	return newPoint;
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramPathItem::mapToPath(const QRectF& itemRect) const
{
	return QRectF(mapToPath(itemRect.topLeft()), mapToPath(itemRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramPathItem::mapFromPath(const QPointF& pathPos) const
{
	QPointF newPoint;

	QRectF pathRect = mPath.boundingRect();
	QRectF bounds = boundingRect();
	QPointF topLeft = DiagramItem::point(0)->position();
	QSizeF size;
	qreal cosAngle, sinAngle;
	qreal ratioX, ratioY;

	if (rotationAngle() == 90 || rotationAngle() == 270)
	{
		size.setWidth(bounds.height());
		size.setHeight(bounds.width());
	}
	else size = bounds.size();

	if (rotationAngle() == 90.0) { cosAngle = 0; sinAngle = 1; }
	else if (rotationAngle() == 180.0) { cosAngle = -1; sinAngle = 0; }
	else if (rotationAngle() == 270.0) { cosAngle = 0; sinAngle = -1; }
	else { cosAngle = 1; sinAngle = 0; }

	ratioX = (pathPos.x() - pathRect.left()) / pathRect.width();
	ratioY = (pathPos.y() - pathRect.top()) / pathRect.height();

	if (isFlipped())
		newPoint.setX(topLeft.x() - cosAngle * size.width() * ratioX + sinAngle * size.height() * ratioY);
	else
		newPoint.setX(topLeft.x() + cosAngle * size.width() * ratioX - sinAngle * size.height() * ratioY);

	newPoint.setY(topLeft.y() + sinAngle * size.width() * ratioX + cosAngle * size.height() * ratioY);

	return newPoint;
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramPathItem::mapFromPath(const QRectF& pathRect) const
{
	return QRectF(mapFromPath(pathRect.topLeft()), mapFromPath(pathRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramPathItem::boundingRect() const
{
	return rectFromPoints(point(0)->position(), point(1)->position());
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramPathItem::shape() const
{
	QPainterPath mainPath;

	// Main path
	mainPath.addRect(boundingRect());

	// Points path
	mainPath.setFillRule(Qt::WindingFill);
	if (isSelected())
	{
		QList<DiagramItemPoint*> lPoints = points();
		QList<DiagramItemPoint*>::ConstIterator pointIter;

		for(pointIter = lPoints.begin(); pointIter != lPoints.end(); pointIter++)
			mainPath.addRect((*pointIter)->boundingRect().translated((*pointIter)->position()));
	}

	return mainPath;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::render(DiagramPainter* painter)
{
	QPainterPath transformedPath;
	QList<QPointF> curveDataPoints;
	QPen lPen = pen();

	if (flags() & UsePalette)
	{
		if (isSelected()) lPen.setBrush(painter->outputBrush(DiagramPainter::Item1));
		else lPen.setBrush(painter->outputBrush(DiagramPainter::Item0));
	}

	for(int i = 0; i < mPath.elementCount(); i++)
	{
		QPainterPath::Element element = mPath.elementAt(i);

		switch (element.type)
		{
		case QPainterPath::MoveToElement:
			transformedPath.moveTo(mapFromPath(QPointF(element.x, element.y)));
			break;
		case QPainterPath::LineToElement:
			transformedPath.lineTo(mapFromPath(QPointF(element.x, element.y)));
			break;
		case QPainterPath::CurveToElement:
			curveDataPoints.append(mapFromPath(QPointF(element.x, element.y)));
			break;
		case QPainterPath::CurveToDataElement:
			if (curveDataPoints.size() >= 2)
			{
				transformedPath.cubicTo(curveDataPoints[0], curveDataPoints[1],
					mapFromPath(QPointF(element.x, element.y)));
				curveDataPoints.pop_front();
				curveDataPoints.pop_front();
			}
			else curveDataPoints.append(mapFromPath(QPointF(element.x, element.y)));
			break;
		}
	}

	painter->setForItem(lPen);
	painter->drawPath(transformedPath);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("path", pathToString(path()));
}
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("pen"))
		setPen(penFromString(attributes.value("pen").toString()));
	if (attributes.hasAttribute("path"))
		setPath(pathFromString(attributes.value("path").toString()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramPathItem::propertiesDialog(QWidget* parent) const
{
	DiagramItemPropertiesDialog* dialog;

	DiagramPathItemDialog::Flags dialogFlags = DiagramPathItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramPathItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	dialog = new DiagramPathItemDialog(parent, dialogFlags);
	dialog->setWindowTitle(uniqueKey() + " Properties");
	return dialog;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::setInitialPath(const QPainterPath& path)
{
	QRectF rect = path.boundingRect();

	point(0)->setPosition(rect.left(), rect.top());
	point(1)->setPosition(rect.right(), rect.bottom());
	point(2)->setPosition(rect.right(), rect.top());
	point(3)->setPosition(rect.left(), rect.bottom());
	point(4)->setPosition((point(0)->position() + point(2)->position()) / 2);
	point(5)->setPosition((point(2)->position() + point(1)->position()) / 2);
	point(6)->setPosition((point(1)->position() + point(3)->position()) / 2);
	point(7)->setPosition((point(3)->position() + point(0)->position()) / 2);

	setPath(path);
}
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::addConnectionPoint(const QPointF& itemPos)
{
	bool pointFound = false;

	QList<DiagramItemPoint*> lPoints = points();
	for(QList<DiagramItemPoint*>::Iterator iter = lPoints.begin();
		!pointFound && iter != lPoints.end(); iter++)
	{
		pointFound = ((*iter)->position() == itemPos);
		if (pointFound) (*iter)->setFlags((*iter)->flags() | DiagramItemPoint::Connection);
	}

	if (!pointFound)
		addPoint(new DiagramItemPoint(itemPos, DiagramItemPoint::Connection, 0));
}
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::addConnectionPoint(qreal x, qreal y)
{
	addConnectionPoint(QPointF(x, y));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramPathItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);
		QPen lPen = pen();

		lPen.setWidthF(lPen.widthF() * scaleFactor);
		setPen(lPen);
	}
	else if (reason == AddNewItemToDiagram) adjustReferencePoint();

	return value;
}
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::resizeEvent(DiagramItemPoint* point, const QPointF& parentPos)
{
	if (point == NULL) point = selectedPoint();
	if (point)
	{
		DiagramItemPoint* startPoint = DiagramItem::point(0);
		DiagramItemPoint* endPoint = DiagramItem::point(1);

		QPointF originalPos = mapToDiagram(point->position());
		QPointF actualPos = parentPos;
		bool wasEndXGreater = (startPoint->positionX() < endPoint->positionX());
		bool wasEndYGreater = (startPoint->positionY() < endPoint->positionY());
		QList<QPointF> additionalConnectionPoints;

		// Save locations of "additional" connection points
		for(int i = 8; i < numberOfPoints(); i++)
			additionalConnectionPoints.append(mapToPath(DiagramItem::point(i)->position()));

		DiagramItem::resizeEvent(point, parentPos);
		adjustBoxControlPoints(point);
		adjustReferencePoint();
		for(int i = 8; i < numberOfPoints(); i++)
			DiagramItem::point(i)->setPosition(mapFromPath(additionalConnectionPoints[i-8]));

		if ((wasEndXGreater && startPoint->positionX() >= endPoint->positionX()) ||
			(!wasEndXGreater && startPoint->positionX() <= endPoint->positionX()))
		{
			actualPos.setX(originalPos.x());
		}
		if ((wasEndYGreater && startPoint->positionY() >= endPoint->positionY()) ||
			(!wasEndYGreater && startPoint->positionY() <= endPoint->positionY()))
		{
			actualPos.setY(originalPos.y());
		}

		if (actualPos != parentPos)
		{
			DiagramItem::resizeEvent(point, actualPos);
			adjustBoxControlPoints(point);
			adjustReferencePoint();
			for(int i = 8; i < numberOfPoints(); i++)
				DiagramItem::point(i)->setPosition(mapFromPath(additionalConnectionPoints[i-8]));
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPathItem::adjustBoxControlPoints(DiagramItemPoint* activePoint)
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
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPathItemDialog::DiagramPathItemDialog(QWidget* parent, Flags flags) : DiagramItemPropertiesDialog(parent)
{
	DiagramPenGroup::Capabilities penGroupCapabilities;
	QSizeF diagramSize = diagramSizeFromParent(parent);

	mFlags = flags;
	penGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramPenGroup::ShowWidth | DiagramPenGroup::ShowStyle : DiagramPenGroup::ShowAll;

	topLeftLayout = new DiagramPositionLayout(diagramSize);
	bottomRightLayout = new DiagramPositionLayout(diagramSize);
	penGroup = new DiagramPenGroup(penGroupCapabilities, diagramSize);

	QGroupBox* positionGroup = new QGroupBox("Position");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Top Left: ", topLeftLayout);
	fLayout->addRow("Bottom Right: ", bottomRightLayout);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	positionGroup->setLayout(fLayout);

    addWidget(positionGroup);
    addWidget(penGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	setWindowTitle("Path Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramPathItemDialog::~DiagramPathItemDialog() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPathItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
}
//--------------------------------------------------------------------------------------------------
void DiagramPathItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramPathItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> DiagramPathItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
