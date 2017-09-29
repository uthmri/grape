/* DiagramPolygonItem.cpp
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

#include <DiagramPolygonItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramPolygonItem::DiagramPolygonItem() : DiagramPolyItem()
{
	addProperty("Brush", QVariant(QBrush(Qt::transparent)));
	setPlaceType(PlaceMouseUp);

	addPoint(new DiagramItemPoint(QPointF(-200.0, -200.0),
								  DiagramItemPoint::Control | DiagramItemPoint::Connection, 0));
	addPoint(new DiagramItemPoint(QPointF(-200.0, 200.0),
								  DiagramItemPoint::Control | DiagramItemPoint::Connection, 0));
	addPoint(new DiagramItemPoint(QPointF(200.0, 0.0),
								  DiagramItemPoint::Control | DiagramItemPoint::Connection, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramPolygonItem::DiagramPolygonItem(const DiagramPolygonItem& item) : DiagramPolyItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramPolygonItem::~DiagramPolygonItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramPolygonItem::copy() const
{
	return new DiagramPolygonItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramPolygonItem::uniqueKey() const
{
	return "polygon";
}
//--------------------------------------------------------------------------------------------------
QString DiagramPolygonItem::iconPath() const
{
	return ":/icons/oxygen/draw-polygon.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPolygonItem::setBrush(const QBrush& brush)
{
	setPropertyValue("Brush", brush);
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramPolygonItem::brush() const
{
	return propertyValue("Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramPolygonItem::canInsertPoint(DiagramItemPoint* point) const
{
	return DiagramPolyItem::canInsertPoint(point);
}
//--------------------------------------------------------------------------------------------------
bool DiagramPolygonItem::canRemovePoint(DiagramItemPoint* point) const
{
	return (DiagramPolyItem::canRemovePoint(point) && numberOfPoints() > 3);
}
//--------------------------------------------------------------------------------------------------
int DiagramPolygonItem::insertPointIndex(DiagramItemPoint* point) const
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

		point->setFlags(DiagramItemPoint::Control | DiagramItemPoint::Connection);
		point->setCategory(0);

		// Find where to insert point within polygon
		while (prevIter != lPoints.end())
		{
			if (nextIter == lPoints.end()) nextIter = lPoints.begin();

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
QRectF DiagramPolygonItem::boundingRect() const
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
QPainterPath DiagramPolygonItem::shape() const
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
	mainPath.addPolygon(outlineLineShape((*pointIter)->position(),
										 lPoints.first()->position(), outlineWidth));

	if (brush().color().alpha() > 0)
	{
		QPainterPath polygonPath;
		polygonPath.setFillRule(Qt::WindingFill);

		QPolygonF polygon;
		for(pointIter = lPoints.begin(); pointIter != lPoints.end(); pointIter++)
			polygon.append((*pointIter)->position());
		polygonPath.addPolygon(polygon);
		mainPath = mainPath.united(polygonPath);
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
void DiagramPolygonItem::render(DiagramPainter* painter)
{
	QList<DiagramItemPoint*> lPoints = points();
	QPen lPen = pen();
	QBrush lBrush = brush();

	if (flags() & UsePalette)
	{
		QColor color;

		if (isSelected()) lBrush = painter->outputBrush(DiagramPainter::Item1);
		else lBrush = painter->outputBrush(DiagramPainter::Item0);
		lPen.setBrush(lBrush);

		color = lBrush.color();
		color.setAlpha(brush().color().alpha());
		lBrush.setColor(color);
	}

#ifdef WIN32
	if (painter->paintEngine()->type() == QPaintEngine::Pdf && lBrush.color().alpha() == 0)
	{
		// Hack to get transparency output to PDF to work properly on Windows
		painter->setForItem(lPen, Qt::transparent);

		for(int i = 0; i < numberOfPoints() - 1; i++)
			painter->drawLine(point(i)->position(), point(i+1)->position());
		painter->drawLine(point(numberOfPoints() - 1)->position(), point(0)->position());
	}
	else
	{
		QPolygonF points;

		for(QList<DiagramItemPoint*>::Iterator pointIter = lPoints.begin();
			pointIter != lPoints.end(); pointIter++)
		{
			points.append((*pointIter)->position());
		}

		painter->setForItem(lPen, lBrush);
		painter->drawPolygon(points);
	}
#else
	QPolygonF points;

	for(QList<DiagramItemPoint*>::Iterator pointIter = lPoints.begin();
		pointIter != lPoints.end(); pointIter++)
	{
		points.append((*pointIter)->position());
	}

	painter->setForItem(lPen, lBrush);
	painter->drawPolygon(points);
#endif
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPolygonItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("brush", brushToString(brush()));
}
//--------------------------------------------------------------------------------------------------
void DiagramPolygonItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("pen"))
		setPen(penFromString(attributes.value("pen").toString()));
	if (attributes.hasAttribute("brush"))
		setBrush(brushFromString(attributes.value("brush").toString()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramPolygonItem::propertiesDialog(QWidget* parent) const
{
	DiagramPolygonItemDialog::Flags dialogFlags = DiagramPolygonItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramPolygonItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	return new DiagramPolygonItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramPolygonItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	QVariant result = DiagramPolyItem::aboutToChangeEvent(reason, value);

	if (reason == AddNewItemToDiagram) adjustReferencePoint();

	return result;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPolygonItemDialog::DiagramPolygonItemDialog(QWidget* parent, Flags flags) :
	DiagramItemPropertiesDialog(parent)
{
	DiagramPenGroup::Capabilities penGroupCapabilities;
	DiagramBrushGroup::Capabilities brushGroupCapabilities;
	mDiagramSize = diagramSizeFromParent(parent);

	mFlags = flags;
	penGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramPenGroup::ShowWidth | DiagramPenGroup::ShowStyle : DiagramPenGroup::ShowAll;
	brushGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramBrushGroup::ShowFilled : DiagramBrushGroup::ShowColor;

	penGroup = new DiagramPenGroup(penGroupCapabilities, mDiagramSize);
	brushGroup = new DiagramBrushGroup(brushGroupCapabilities);

	pointsFrame = new QFrame();
	pointFormLayout = new QFormLayout();
	pointsFrame->setLayout(pointFormLayout);

	QScrollArea* scrollWidget = new QScrollArea();
	scrollWidget->setWidget(pointsFrame);
	scrollWidget->setWidgetResizable(true);

	QFrame* generalFrame = new QFrame();
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(penGroup);
	vLayout->addWidget(brushGroup);
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

	setWindowTitle("Polygon Properties");
	resize(320, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramPolygonItemDialog::~DiagramPolygonItemDialog() { }
//--------------------------------------------------------------------------------------------------
void DiagramPolygonItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());
}
//--------------------------------------------------------------------------------------------------
void DiagramPolygonItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
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
QHash<QString, QVariant> DiagramPolygonItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> DiagramPolygonItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	for(int i = 0; i < pointPositionLayouts.size(); i++)
		pointPositions.append(pointPositionLayouts[i]->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
