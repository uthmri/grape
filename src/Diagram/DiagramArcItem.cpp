/* DiagramArcItem.cpp
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


#include <DiagramArcItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramArcItem::DiagramArcItem() : DiagramTwoPointItem() { }
//--------------------------------------------------------------------------------------------------
DiagramArcItem::DiagramArcItem(const DiagramArcItem& item) : DiagramTwoPointItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramArcItem::~DiagramArcItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramArcItem::copy() const
{
	return new DiagramArcItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramArcItem::uniqueKey() const
{
	return "arc";
}
//--------------------------------------------------------------------------------------------------
QString DiagramArcItem::iconPath() const
{
	return ":/icons/oxygen/draw-arc.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramArcItem::boundingRect() const
{
	return rectFromPoints(startPoint()->position(), endPoint()->position());
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramArcItem::shape() const
{
	QPainterPath mainPath;
	qreal outlineWidth = adjustOutlineForView(pen().widthF());

	qreal arcRadiusX = orientedRadiusX();
	qreal arcRadiusY = orientedRadiusY();
	qreal arcCenterX = orientedCenterX();
	qreal arcCenterY = orientedCenterY();
	QPointF arcStart = point(0)->position();
	QPointF arcFinish = point(1)->position();

	// Main path
	if (arcRadiusX == 0 || arcRadiusY == 0)
	{
		mainPath.addPolygon(outlineLineShape(arcStart, arcFinish, outlineWidth));
	}
	else
	{
		QPainterPath outerPath, innerPath, boundsPath, endPath1, endPath2;

		QRectF outerBounds(arcCenterX - arcRadiusX - outlineWidth/2, arcCenterY - arcRadiusY - outlineWidth/2,
						   2 * (arcRadiusX + outlineWidth/2), 2 * (arcRadiusY + outlineWidth/2));
		QRectF innerBounds(arcCenterX - arcRadiusX + outlineWidth/2, arcCenterY - arcRadiusY + outlineWidth/2,
						   2 * (arcRadiusX - outlineWidth/2), 2 * (arcRadiusY - outlineWidth/2));

		//QRectF areaMask = boundingRect().adjusted(-penAdjust, -penAdjust, penAdjust, penAdjust);
		QPointF vector = (point(0)->position() - QPointF(arcCenterX, arcCenterY)) +
						 (point(1)->position() - QPointF(arcCenterX, arcCenterY));
		QRectF areaMask = boundingRect();
		if (vector.x() > 0) areaMask.adjust(0, 0, outlineWidth/2, 0);
		else if (vector.x() < 0) areaMask.adjust(-outlineWidth/2, 0, 0, 0);
		if (vector.y() > 0) areaMask.adjust(0, 0, 0, outlineWidth/2);
		else if (vector.y() < 0) areaMask.adjust(0, -outlineWidth/2, 0, 0);

		outerPath.addEllipse(outerBounds);
		innerPath.addEllipse(innerBounds);
		boundsPath.addRect(areaMask);
		mainPath = outerPath.subtracted(innerPath);
		mainPath &= boundsPath;

		endPath1.addEllipse(QRectF(arcStart - QPointF(outlineWidth/2, outlineWidth/2),
								   QSizeF(outlineWidth, outlineWidth)));
		endPath2.addEllipse(QRectF(arcFinish - QPointF(outlineWidth/2, outlineWidth/2),
								   QSizeF(outlineWidth, outlineWidth)));
		mainPath = mainPath.united(endPath1);
		mainPath = mainPath.united(endPath2);
	}

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
void DiagramArcItem::render(DiagramPainter* painter)
{
	DiagramItemPoint* itemPoint0 = startPoint();
	DiagramItemPoint* itemPoint1 = endPoint();

	qreal arcRadiusX = orientedRadiusX();
	qreal arcRadiusY = orientedRadiusY();
	qreal arcCenterX = orientedCenterX();
	qreal arcCenterY = orientedCenterY();
	qreal arcStartAngle = orientedStartAngle();
	qreal arcSpanAngle = orientedSpanAngle();
	QPen lPen = pen();

	if (flags() & UsePalette)
	{
		if (isSelected()) lPen.setBrush(painter->outputBrush(DiagramPainter::Item1));
		else lPen.setBrush(painter->outputBrush(DiagramPainter::Item0));
	}

	// Arc
	painter->setForItem(lPen);
	painter->drawArc(QRectF(arcCenterX - arcRadiusX, arcCenterY - arcRadiusY,
		arcRadiusX * 2, arcRadiusY * 2), (int)(arcStartAngle * 16), (int)(arcSpanAngle * 16));

	// Arrows
	painter->setForItem(lPen, painter->outputBrush(DiagramPainter::Background));
	if (magnitude(itemPoint1->position() - itemPoint0->position()) > arrow(0).size())
	{
		if (isFlipped()) arrow(0).render(painter, itemPoint0->position(), 90 - arcStartAngle);
		else arrow(0).render(painter, itemPoint0->position(), -90 - arcStartAngle);
	}
	if (magnitude(itemPoint1->position() - itemPoint0->position()) > arrow(1).size())
		arrow(1).render(painter, itemPoint1->position(), -arcStartAngle);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramArcItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("startArrow", arrow(0).toString());
	xmlWriter.writeAttribute("endArrow", arrow(1).toString());
}
//--------------------------------------------------------------------------------------------------
void DiagramArcItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("pen"))
		setPen(penFromString(attributes.value("pen").toString()));
	if (attributes.hasAttribute("startArrow"))
		setArrow(0, DiagramArrow::fromString(attributes.value("startArrow").toString()));
	if (attributes.hasAttribute("endArrow"))
		setArrow(1, DiagramArrow::fromString(attributes.value("endArrow").toString()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramArcItem::propertiesDialog(QWidget* parent) const
{
	DiagramArcItemDialog::Flags dialogFlags = DiagramArcItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramArcItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	return new DiagramArcItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
qreal DiagramArcItem::orientedCenterX() const
{
	DiagramItemPoint* itemPoint0 = startPoint();
	DiagramItemPoint* itemPoint1 = endPoint();
	qreal centerX = 0.0;

	if (itemPoint1->positionX() == itemPoint0->positionX()) centerX = itemPoint0->positionX();
	else
	{
		qreal slope = (itemPoint1->positionY() - itemPoint0->positionY()) /
					  (itemPoint1->positionX() - itemPoint0->positionX());

		if ((!isFlipped() && slope < 0) || (isFlipped() && slope > 0))
			centerX = itemPoint0->positionX();
		else centerX = itemPoint1->positionX();
	}

	return centerX;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramArcItem::orientedCenterY() const
{
	DiagramItemPoint* itemPoint0 = startPoint();
	DiagramItemPoint* itemPoint1 = endPoint();
	qreal centerY = 0.0;

	if (itemPoint1->positionX() == itemPoint0->positionX()) centerY = itemPoint1->positionY();
	else
	{
		qreal slope = (itemPoint1->positionY() - itemPoint0->positionY()) /
					  (itemPoint1->positionX() - itemPoint0->positionX());

		if ((!isFlipped() && slope < 0) || (isFlipped() && slope > 0))
			centerY = itemPoint1->positionY();
		else centerY = itemPoint0->positionY();
	}

	return centerY;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramArcItem::orientedRadiusX() const
{
	return boundingRect().width();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramArcItem::orientedRadiusY() const
{
	return boundingRect().height();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramArcItem::orientedStartAngle() const
{
	qreal angle = 0.0;

	DiagramItemPoint* itemPoint0 = startPoint();
	angle = qAtan2(orientedCenterY() - itemPoint0->positionY(),
				   itemPoint0->positionX() - orientedCenterX()) * 180 / 3.14159;

	return angle;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramArcItem::orientedSpanAngle() const
{
	qreal angle = 90.0;
	if (isFlipped()) angle = -90.0;
	return angle;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramArcItemDialog::DiagramArcItemDialog(QWidget* parent, Flags flags) : DiagramItemPropertiesDialog(parent)
{
	DiagramPenGroup::Capabilities penGroupCapabilities;
	QSizeF diagramSize = diagramSizeFromParent(parent);

	mFlags = flags;
	penGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramPenGroup::ShowWidth | DiagramPenGroup::ShowStyle : DiagramPenGroup::ShowAll;

	penGroup = new DiagramPenGroup(penGroupCapabilities, diagramSize);
	startPointGroup = new DiagramItemPointGroup("Start Point", DiagramItemPointGroup::ShowAll, diagramSize);
	endPointGroup = new DiagramItemPointGroup("End Point", DiagramItemPointGroup::ShowAll, diagramSize);

    addWidget(penGroup);
    addWidget(startPointGroup);
    addWidget(endPointGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	setWindowTitle("Arc Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramArcItemDialog::~DiagramArcItemDialog() { }
//--------------------------------------------------------------------------------------------------
void DiagramArcItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());

	if (properties.contains("Start Arrow"))
		startPointGroup->setArrow(properties["Start Arrow"].value<DiagramArrow>());

	if (properties.contains("End Arrow"))
		endPointGroup->setArrow(properties["End Arrow"].value<DiagramArrow>());
}
//--------------------------------------------------------------------------------------------------
void DiagramArcItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		startPointGroup->setPosition(pointPositions[0]);
		endPointGroup->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramArcItemDialog::properties() const
{
	QHash<QString, QVariant> properties;
	QVariant startArrow, endArrow;

	startArrow.setValue(startPointGroup->arrow());
	endArrow.setValue(endPointGroup->arrow());

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Start Arrow"] = startArrow;
	properties["End Arrow"] = endArrow;

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> DiagramArcItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(startPointGroup->position());
	pointPositions.append(endPointGroup->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------

