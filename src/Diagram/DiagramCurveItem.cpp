/* DiagramCurveItem.cpp
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

#include <DiagramCurveItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramCurveItem::DiagramCurveItem() : DiagramTwoPointItem()
{
	setPlaceType(PlaceMouseUp);

	// Control points
	point(0)->setPosition(QPointF(-200.0, -200.0));
	point(1)->setPosition(QPointF( 200.0,  200.0));
	addPoint(new DiagramItemPoint(QPointF(0.0, -200.0), DiagramItemPoint::Control, 0));
	addPoint(new DiagramItemPoint(QPointF(0.0, 200.0), DiagramItemPoint::Control, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramCurveItem::DiagramCurveItem(const DiagramCurveItem& item) : DiagramTwoPointItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramCurveItem::~DiagramCurveItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramCurveItem::copy() const
{
	return new DiagramCurveItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramCurveItem::uniqueKey() const
{
	return "curve";
}
//--------------------------------------------------------------------------------------------------
QString DiagramCurveItem::iconPath() const
{
	return ":/icons/oxygen/draw-curve.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramCurveItem::boundingRect() const
{
	QRectF rect;
	QPainterPath path;

	path.moveTo(point(0)->position());
	path.cubicTo(point(2)->position(), point(3)->position(), point(1)->position());
	rect = path.boundingRect();

	return rect;
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramCurveItem::shape() const
{
	QPainterPath mainPath;
	QPointF previousPoint, nextPoint;
	qreal outlineWidth = adjustOutlineForView(pen().widthF());

	// Main path
	mainPath.setFillRule(Qt::WindingFill);
	previousPoint = pointFromRatio(0.0);
	for(qreal f = 0.02; f <= 1.0; f += 0.02)
	{
		nextPoint = pointFromRatio(f);
		mainPath.addPolygon(outlineLineShape(previousPoint, nextPoint, outlineWidth));
		previousPoint = nextPoint;
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
bool DiagramCurveItem::isSuperfluous() const
{
	return !boundingRect().isValid();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramCurveItem::render(DiagramPainter* painter)
{
	QPainterPath path;
	qreal theta = 0.0;
	DiagramItemPoint* itemPoint0 = point(0);
	DiagramItemPoint* itemPoint1 = point(1);
	DiagramItemPoint* itemPoint2 = point(2);
	DiagramItemPoint* itemPoint3 = point(3);
	QPen lPen = pen();

	if (flags() & UsePalette)
	{
		if (isSelected()) lPen.setBrush(painter->outputBrush(DiagramPainter::Item1));
		else lPen.setBrush(painter->outputBrush(DiagramPainter::Item0));
	}

	// Curve
	painter->setForItem(lPen);
	path.moveTo(itemPoint0->position());
	path.cubicTo(itemPoint2->position(), itemPoint3->position(), itemPoint1->position());
	painter->drawPath(path);

	// Arrows
	theta = atan2(itemPoint1->positionY() - itemPoint0->positionY(),
				  itemPoint1->positionX() - itemPoint0->positionX()) * 180.0 / 3.1414592654;
	painter->setForItem(lPen, painter->outputBrush(DiagramPainter::Background));
	//if (path.boundingRect().width() >= arrow(0).size() && path.boundingRect().height() >= arrow(0).size())
	if (path.boundingRect().width() > 0 || path.boundingRect().height() > 0)
	{
		QPointF p = pointFromRatio(0.05);
		theta = atan2(p.y() - itemPoint0->positionY(),
					  p.x() - itemPoint0->positionX()) * 180.0 / 3.14159;
		arrow(0).render(painter, itemPoint0->position(), theta);
	}
	//if (path.boundingRect().width() >= arrow(1).size() && path.boundingRect().height() >= arrow(1).size())
	if (path.boundingRect().width() > 0 || path.boundingRect().height() > 0)
	{
		QPointF p = pointFromRatio(0.95);
		theta = atan2(p.y() - itemPoint1->positionY(),
			 p.x() - itemPoint1->positionX()) * 180.0 / 3.14159;
		arrow(1).render(painter, itemPoint1->position(), theta);
	}

	painter->setForItem(lPen);
	if (isSelected())
	{
		QPen dottedPen = painter->pen();
		dottedPen.setStyle(Qt::DotLine);
		dottedPen.setWidthF(0.75 * dottedPen.widthF());
		painter->setPen(dottedPen);

		painter->drawLine(itemPoint0->position(), itemPoint2->position());
		painter->drawLine(itemPoint1->position(), itemPoint3->position());
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramCurveItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("startArrow", arrow(0).toString());
	xmlWriter.writeAttribute("endArrow", arrow(1).toString());
}
//--------------------------------------------------------------------------------------------------
void DiagramCurveItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
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
DiagramItemPropertiesDialog* DiagramCurveItem::propertiesDialog(QWidget* parent) const
{
	DiagramCurveItemDialog::Flags dialogFlags = DiagramCurveItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramCurveItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	return new DiagramCurveItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramCurveItem::resizeEvent(DiagramItemPoint* point, const QPointF& parentPos)
{
	if (point && point->isControlPoint())
	{
		QPointF deltaPosition = parentPos - mapToParent(point->position());
		DiagramItemPoint* otherPoint = NULL;

		if (point == DiagramCurveItem::point(0)) otherPoint = DiagramCurveItem::point(2);
		else if (point == DiagramCurveItem::point(1)) otherPoint = DiagramCurveItem::point(3);

		if (otherPoint) otherPoint->setPosition(otherPoint->position() + deltaPosition);
		point->setPosition(point->position() + deltaPosition);

		adjustReferencePoint();
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPointF DiagramCurveItem::pointFromRatio(qreal ratio) const
{
	QPointF pos;
	DiagramItemPoint* point0 = point(0);
	DiagramItemPoint* point1 = point(1);
	DiagramItemPoint* point2 = point(2);
	DiagramItemPoint* point3 = point(3);

	pos.setX((1 - ratio)*(1 - ratio)*(1 - ratio) * point0->positionX() +
		3*ratio*(1 - ratio)*(1 - ratio) * point2->positionX() +
		3*ratio*ratio*(1 - ratio) * point3->positionX() +
		ratio*ratio*ratio * point1->positionX());

	pos.setY((1 - ratio)*(1 - ratio)*(1 - ratio) * point0->positionY() +
		3*ratio*(1 - ratio)*(1 - ratio) * point2->positionY() +
		3*ratio*ratio*(1 - ratio) * point3->positionY() +
		ratio*ratio*ratio * point1->positionY());

	return pos;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramCurveItemDialog::DiagramCurveItemDialog(QWidget* parent, Flags flags) : DiagramItemPropertiesDialog(parent)
{
	DiagramPenGroup::Capabilities penGroupCapabilities;
	QSizeF diagramSize = diagramSizeFromParent(parent);

	mFlags = flags;
	penGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramPenGroup::ShowWidth | DiagramPenGroup::ShowStyle : DiagramPenGroup::ShowAll;

	penGroup = new DiagramPenGroup(penGroupCapabilities, diagramSize);
	startPointGroup = new DiagramItemPointGroup("Start Point", DiagramItemPointGroup::ShowAll, diagramSize);
	endPointGroup = new DiagramItemPointGroup("End Point", DiagramItemPointGroup::ShowAll, diagramSize);
	controlPoint0Layout = new DiagramPositionLayout(diagramSize);
	controlPoint1Layout = new DiagramPositionLayout(diagramSize);

	QGroupBox* controlGroup = new QGroupBox("Control Points");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Start Control:", controlPoint0Layout);
	fLayout->addRow("End Control:", controlPoint1Layout);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	controlGroup->setLayout(fLayout);

    addWidget(penGroup);
    addWidget(startPointGroup);
    addWidget(endPointGroup);
    addWidget(controlGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	setWindowTitle("Line Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramCurveItemDialog::~DiagramCurveItemDialog() { }
//--------------------------------------------------------------------------------------------------
void DiagramCurveItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());

	if (properties.contains("Start Arrow"))
		startPointGroup->setArrow(properties["Start Arrow"].value<DiagramArrow>());

	if (properties.contains("End Arrow"))
		endPointGroup->setArrow(properties["End Arrow"].value<DiagramArrow>());
}
//--------------------------------------------------------------------------------------------------
void DiagramCurveItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		startPointGroup->setPosition(pointPositions[0]);
		endPointGroup->setPosition(pointPositions[1]);
	}
	if (pointPositions.size() >= 4)
	{
		controlPoint0Layout->setPosition(pointPositions[2]);
		controlPoint1Layout->setPosition(pointPositions[3]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramCurveItemDialog::properties() const
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
QList<QPointF> DiagramCurveItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(startPointGroup->position());
	pointPositions.append(endPointGroup->position());
	pointPositions.append(controlPoint0Layout->position());
	pointPositions.append(controlPoint1Layout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
