/* DiagramLineItem.cpp
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

#include <DiagramLineItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramLineItem::DiagramLineItem() : DiagramTwoPointItem()
{
	// Midpoint
	addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Connection, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramLineItem::DiagramLineItem(const DiagramLineItem& item) : DiagramTwoPointItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramLineItem::~DiagramLineItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramLineItem::copy() const
{
	return new DiagramLineItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramLineItem::uniqueKey() const
{
	return "line";
}
//--------------------------------------------------------------------------------------------------
QString DiagramLineItem::iconPath() const
{
	return ":/icons/oxygen/draw-line.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramLineItem::boundingRect() const
{
	return rectFromPoints(startPoint()->position(), endPoint()->position());
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramLineItem::shape() const
{
	QPainterPath mainPath;
	qreal outlineWidth = adjustOutlineForView(pen().widthF());

	// Main path
	mainPath.addPolygon(
		outlineLineShape(startPoint()->position(), endPoint()->position(), outlineWidth));

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
void DiagramLineItem::render(DiagramPainter* painter)
{
	qreal theta = 0.0;
	QPen lPen = pen();

	if (flags() & UsePalette)
	{
		if (isSelected()) lPen.setBrush(painter->outputBrush(DiagramPainter::Item1));
		else lPen.setBrush(painter->outputBrush(DiagramPainter::Item0));
	}

	// Line
	//painter->setBrush(Qt::magenta);
	//painter->setPen(QPen(Qt::magenta, 1));
	//painter->drawPath(shape());

	painter->setForItem(lPen);
	painter->drawLine(startPoint()->position(), endPoint()->position());

	// Arrows
	painter->setForItem(lPen, painter->outputBrush(DiagramPainter::Background));
	theta = qAtan2(endPoint()->positionY() - startPoint()->positionY(),
				   endPoint()->positionX() - startPoint()->positionX()) * 180.0 / 3.1414592654;

	if (magnitude(endPoint()->position() - startPoint()->position()) > arrow(0).size())
		arrow(0).render(painter, startPoint()->position(), theta);
	if (magnitude(endPoint()->position() - startPoint()->position()) > arrow(1).size())
		arrow(1).render(painter, endPoint()->position(), theta - 180.0);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramLineItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("startArrow", arrow(0).toString());
	xmlWriter.writeAttribute("endArrow", arrow(1).toString());
}
//--------------------------------------------------------------------------------------------------
void DiagramLineItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
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
DiagramItemPropertiesDialog* DiagramLineItem::propertiesDialog(QWidget* parent) const
{
	DiagramLineItemDialog::Flags dialogFlags = DiagramLineItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramLineItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	return new DiagramLineItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramLineItem::resizeEvent(DiagramItemPoint* point, const QPointF& parentPos)
{
	DiagramTwoPointItem::resizeEvent(point, parentPos);
	DiagramItem::point(2)->setPosition((startPoint()->position() + endPoint()->position()) / 2.0);
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramLineItemDialog::DiagramLineItemDialog(QWidget* parent, Flags flags) : DiagramItemPropertiesDialog(parent)
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

	setWindowTitle("Line Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramLineItemDialog::~DiagramLineItemDialog() { }
//--------------------------------------------------------------------------------------------------
void DiagramLineItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());

	if (properties.contains("Start Arrow"))
		startPointGroup->setArrow(properties["Start Arrow"].value<DiagramArrow>());

	if (properties.contains("End Arrow"))
		endPointGroup->setArrow(properties["End Arrow"].value<DiagramArrow>());
}
//--------------------------------------------------------------------------------------------------
void DiagramLineItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		startPointGroup->setPosition(pointPositions[0]);
		endPointGroup->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramLineItemDialog::properties() const
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
QList<QPointF> DiagramLineItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(startPointGroup->position());
	pointPositions.append(endPointGroup->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
