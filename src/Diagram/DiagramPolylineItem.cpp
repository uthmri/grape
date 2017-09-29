/* DiagramPolylineItem.cpp
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

#include <DiagramPolylineItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramPolylineItem::DiagramPolylineItem() : DiagramPolyItem()
{
	QVariant variant;
	variant.setValue(DiagramArrow());

	addProperty("Start Arrow", variant);
	addProperty("End Arrow", variant);

	addPoint(new DiagramItemPoint(QPointF(0.0, 0.0),
		DiagramItemPoint::Control | DiagramItemPoint::Connection | DiagramItemPoint::Free, 0));
	addPoint(new DiagramItemPoint(QPointF(0.0, 0.0),
		DiagramItemPoint::Control | DiagramItemPoint::Connection | DiagramItemPoint::Free, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramPolylineItem::DiagramPolylineItem(const DiagramPolylineItem& item) : DiagramPolyItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramPolylineItem::~DiagramPolylineItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramPolylineItem::copy() const
{
	return new DiagramPolylineItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramPolylineItem::uniqueKey() const
{
	return "polyline";
}
//--------------------------------------------------------------------------------------------------
QString DiagramPolylineItem::iconPath() const
{
	return ":/icons/oxygen/draw-polyline.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPolylineItem::setArrow(int pointIndex, const DiagramArrow& arrow)
{
	QVariant variant;
	variant.setValue(arrow);

	if (pointIndex == 0) setPropertyValue("Start Arrow", variant);
	else setPropertyValue("End Arrow", variant);
}
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramPolylineItem::arrow(int pointIndex) const
{
	DiagramArrow arrow;

	if (pointIndex == 0) arrow = propertyValue("Start Arrow").value<DiagramArrow>();
	else arrow = propertyValue("End Arrow").value<DiagramArrow>();

	return arrow;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramPolylineItem::startPoint() const
{
	return point(0);
}
//--------------------------------------------------------------------------------------------------
DiagramItemPoint* DiagramPolylineItem::endPoint() const
{
	return point(numberOfPoints() - 1);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramPolylineItem::canInsertPoint(DiagramItemPoint* point) const
{
	return DiagramPolyItem::canInsertPoint(point);
}
//--------------------------------------------------------------------------------------------------
bool DiagramPolylineItem::canRemovePoint(DiagramItemPoint* point) const
{
	return (DiagramPolyItem::canRemovePoint(point) && numberOfPoints() > 2);
}
//--------------------------------------------------------------------------------------------------
int DiagramPolylineItem::insertPointIndex(DiagramItemPoint* point) const
{
	int index = -1;

	if (point)
	{
		QList<DiagramItemPoint*> lPoints = points();
		QList<DiagramItemPoint*>::Iterator prevIter = lPoints.begin();
		QList<DiagramItemPoint*>::Iterator nextIter = lPoints.begin();
		nextIter++;
		int currentIndex = 0;

		int minimumIndex = -1;
		qreal distance, minimumDistance = 1E9;

		point->setFlags(DiagramItemPoint::Control | DiagramItemPoint::Connection | DiagramItemPoint::Free);
		point->setCategory(0);

		// Find where to insert point within polygon
		while (prevIter != lPoints.end() && nextIter != lPoints.end())
		{
			distance = distanceFromPointToLineSegment(point->position(),
				QLineF((*prevIter)->position(), (*nextIter)->position()));

			if (distance < minimumDistance)
			{
				minimumDistance = distance;
				minimumIndex = currentIndex;
			}

			prevIter++;
			nextIter++;
			currentIndex++;
		}

		index = minimumIndex + 1;
	}

	return index;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramPolylineItem::boundingRect() const
{
	qreal l = 1E9, t = 1E9, r = -1E9, b = -1E9;
	QList<DiagramItemPoint*> lPoints = points();

	for(QList<DiagramItemPoint*>::ConstIterator pointIter = lPoints.begin();
		pointIter != lPoints.end(); pointIter++)
	{
		l = std::min(l, (*pointIter)->positionX());
		t = std::min(t, (*pointIter)->positionY());
		r = std::max(r, (*pointIter)->positionX());
		b = std::max(b, (*pointIter)->positionY());
	}

	return QRectF(l, t, r - l, b - t);
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramPolylineItem::shape() const
{
	QPainterPath mainPath;
	mainPath.setFillRule(Qt::WindingFill);

	qreal outlineWidth = adjustOutlineForView(pen().widthF());

	QList<DiagramItemPoint*> lPoints = points();
	QList<DiagramItemPoint*>::ConstIterator pointIter = lPoints.begin();
	QList<DiagramItemPoint*>::ConstIterator nextIter = lPoints.begin();
	nextIter++;
	while (nextIter != lPoints.end())
	{
		mainPath.addPolygon(outlineLineShape((*pointIter)->position(),
			(*nextIter)->position(), outlineWidth));
		pointIter++;
		nextIter++;
	}

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
void DiagramPolylineItem::render(DiagramPainter* painter)
{
	DiagramItemPoint* point0 = point(0);
	DiagramItemPoint* point1 = point(numberOfPoints() - 1);
	QList<DiagramItemPoint*> lPoints = points();
	QPolygonF polygon;
	DiagramItemPoint* otherPoint;
	qreal theta;
	QPen lPen = pen();

	if (flags() & UsePalette)
	{
		if (isSelected()) lPen.setBrush(painter->outputBrush(DiagramPainter::Item1));
		else lPen.setBrush(painter->outputBrush(DiagramPainter::Item0));
	}

	// Polyline
	for(QList<DiagramItemPoint*>::Iterator pointIter = lPoints.begin();
		pointIter != lPoints.end(); pointIter++)
	{
		polygon.append((*pointIter)->position());
	}

	painter->setForItem(lPen);
	painter->drawPolyline(polygon);

	// Arrows
	painter->setForItem(lPen, painter->outputBrush(DiagramPainter::Background));

	otherPoint = point(1);
	if (otherPoint)
	{
		theta = qAtan2(otherPoint->positionY() - point0->positionY(),
			otherPoint->positionX() - point0->positionX()) * 180.0 / 3.1414592654;

		if (magnitude(otherPoint->position() - point0->position()) > arrow(0).size())
			arrow(0).render(painter, point0->position(), theta);
	}

	otherPoint = point(numberOfPoints() - 2);
	if (otherPoint)
	{
		theta = qAtan2(otherPoint->positionY() - point1->positionY(),
			otherPoint->positionX() - point1->positionX()) * 180.0 / 3.1414592654;

		if (magnitude(otherPoint->position() - point1->position()) > arrow(numberOfPoints() - 1).size())
			arrow(numberOfPoints() - 1).render(painter, point1->position(), theta);
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPolylineItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("startArrow", arrow(0).toString());
	xmlWriter.writeAttribute("endArrow", arrow(numberOfPoints() - 1).toString());
}
//--------------------------------------------------------------------------------------------------
void DiagramPolylineItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("pen"))
		setPen(penFromString(attributes.value("pen").toString()));
	if (attributes.hasAttribute("startArrow"))
		setArrow(0, DiagramArrow::fromString(attributes.value("startArrow").toString()));
	if (attributes.hasAttribute("endArrow"))
		setArrow(numberOfPoints() - 1, DiagramArrow::fromString(attributes.value("endArrow").toString()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramPolylineItem::propertiesDialog(QWidget* parent) const
{
	DiagramPolylineItemDialog::Flags dialogFlags = DiagramPolylineItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramPolylineItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	return new DiagramPolylineItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramPolylineItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	QVariant result = DiagramPolyItem::aboutToChangeEvent(reason, value);

	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);
		DiagramArrow lArrow;

		lArrow = arrow(0);
		lArrow.setSize(lArrow.size() * scaleFactor);
		setArrow(0, lArrow);

		lArrow = arrow(numberOfPoints() - 1);
		lArrow.setSize(lArrow.size() * scaleFactor);
		setArrow(numberOfPoints() - 1, lArrow);
	}

	return result;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPolylineItemDialog::DiagramPolylineItemDialog(QWidget* parent, Flags flags) :
	DiagramItemPropertiesDialog(parent)
{
	DiagramPenGroup::Capabilities penGroupCapabilities;
	mDiagramSize = diagramSizeFromParent(parent);

	mFlags = flags;
	penGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramPenGroup::ShowWidth | DiagramPenGroup::ShowStyle : DiagramPenGroup::ShowAll;

	penGroup = new DiagramPenGroup(penGroupCapabilities, mDiagramSize);
	startPointGroup = new DiagramItemPointGroup("Start Point", DiagramItemPointGroup::ShowArrow, mDiagramSize);
	endPointGroup = new DiagramItemPointGroup("End Point", DiagramItemPointGroup::ShowArrow, mDiagramSize);

	pointsFrame = new QFrame();
	pointFormLayout = new QFormLayout();
	pointsFrame->setLayout(pointFormLayout);

	QScrollArea* scrollWidget = new QScrollArea();
	scrollWidget->setWidget(pointsFrame);
	scrollWidget->setWidgetResizable(true);

	QFrame* generalFrame = new QFrame();
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(penGroup);
	vLayout->addWidget(startPointGroup);
	vLayout->addWidget(endPointGroup);
	vLayout->addWidget(new QWidget(), 100);
	generalFrame->setLayout(vLayout);

	QFrame* pointFrame = new QFrame();
	vLayout = new QVBoxLayout();
	vLayout->addWidget(scrollWidget, 100);
	pointFrame->setLayout(vLayout);

	QTabWidget* tabWidget = new QTabWidget();
	tabWidget->addTab(generalFrame, "General");
	tabWidget->addTab(pointFrame, "Points");

    addWidget(tabWidget);
    addButtonBox();

	setWindowTitle("Polyline Properties");
	resize(320, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramPolylineItemDialog::~DiagramPolylineItemDialog() { }
//--------------------------------------------------------------------------------------------------
void DiagramPolylineItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());

	if (properties.contains("Start Arrow"))
		startPointGroup->setArrow(properties["Start Arrow"].value<DiagramArrow>());

	if (properties.contains("End Arrow"))
		endPointGroup->setArrow(properties["End Arrow"].value<DiagramArrow>());
}
//--------------------------------------------------------------------------------------------------
void DiagramPolylineItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	DiagramPositionLayout* positionLayout;

	pointPositionLayouts.clear();
	delete pointFormLayout;
	pointFormLayout = new QFormLayout();

	for(qint32 i = 0; i < pointPositions.size(); i++)
	{
		positionLayout = new DiagramPositionLayout(mDiagramSize);
		positionLayout->setPosition(pointPositions[i]);
		pointPositionLayouts.append(positionLayout);

		if (i == 0) pointFormLayout->addRow("First Point:", positionLayout);
		else if (i == pointPositions.size() - 1) pointFormLayout->addRow("Last Point:", positionLayout);
		else pointFormLayout->addRow("", positionLayout);
	}

	pointFormLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	pointFormLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	pointFormLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	pointFormLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	pointsFrame->setLayout(pointFormLayout);
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramPolylineItemDialog::properties() const
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
QList<QPointF> DiagramPolylineItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	for(int i = 0; i < pointPositionLayouts.size(); i++)
		pointPositions.append(pointPositionLayouts[i]->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
