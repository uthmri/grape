/* DiagramEllipseItem.cpp
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

#include <DiagramEllipseItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramEllipseItem::DiagramEllipseItem() : DiagramBoxResizeItem()
{
	for(int i = 8; i < 12; i++)
		addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Connection, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramEllipseItem::DiagramEllipseItem(const DiagramEllipseItem& item) : DiagramBoxResizeItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramEllipseItem::~DiagramEllipseItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramEllipseItem::copy() const
{
	return new DiagramEllipseItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramEllipseItem::uniqueKey() const
{
	return "ellipse";
}
//--------------------------------------------------------------------------------------------------
QString DiagramEllipseItem::iconPath() const
{
	return ":/icons/oxygen/draw-ellipse.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramEllipseItem::boundingRect() const
{
	return rectFromPoints(point(0)->position(), point(1)->position());
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramEllipseItem::shape() const
{
	QPainterPath mainPath;
	qreal outlineWidth = adjustOutlineForView(pen().widthF());

	QRectF bounds = rectFromPoints(point(0)->position(), point(1)->position());

	QRectF outerBounds = bounds.adjusted(-outlineWidth/2, -outlineWidth/2, outlineWidth/2, outlineWidth/2);
	//QRectF innerBounds = bounds.adjusted(outlineWidth/2, outlineWidth/2, -outlineWidth/2, -outlineWidth/2);

	// Main path
	if (brush().color().alpha() > 0 || bounds.width() == 0 || bounds.height() == 0)
	{
		mainPath.addEllipse(outerBounds);
	}
	else
	{
		//This doesn't work
		/*QPainterPath outerPath, innerPath;

		outerPath.addEllipse(outerBounds);
		innerPath.addEllipse(innerBounds);
		mainPath = outerPath.subtracted(innerPath);*/

		QPainterPath arcPath;
		QPainterPathStroker ppStroker;
		arcPath.moveTo(bounds.right(), bounds.center().y());
		arcPath.arcTo(bounds, 0, 360);

		ppStroker.setWidth(outlineWidth);
		ppStroker.setCapStyle(pen().capStyle());
		ppStroker.setJoinStyle(pen().joinStyle());
		mainPath = ppStroker.createStroke(arcPath);
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
void DiagramEllipseItem::render(DiagramPainter* painter)
{
	QRectF bounds = rectFromPoints(point(0)->position(), point(1)->position());

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
		painter->drawArc(bounds, 0, 5760);
	}
	else
	{
		painter->setForItem(lPen, lBrush);
		painter->drawEllipse(bounds);
	}
#else
	painter->setForItem(lPen, lBrush);
	painter->drawEllipse(bounds);
#endif
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramEllipseItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("brush", brushToString(brush()));
}
//--------------------------------------------------------------------------------------------------
void DiagramEllipseItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
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
DiagramItemPropertiesDialog* DiagramEllipseItem::propertiesDialog(QWidget* parent) const
{
	DiagramEllipseItemDialog::Flags dialogFlags = DiagramEllipseItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramEllipseItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	return new DiagramEllipseItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramEllipseItem::resizeEvent(DiagramItemPoint* point, const QPointF& parentPos)
{
	DiagramBoxResizeItem::resizeEvent(point, parentPos);
	adjustEllipseControlPoints(point);
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramEllipseItemDialog::DiagramEllipseItemDialog(QWidget* parent, Flags flags) : DiagramItemPropertiesDialog(parent)
{
	DiagramPenGroup::Capabilities penGroupCapabilities;
	DiagramBrushGroup::Capabilities brushGroupCapabilities;
	QSizeF diagramSize = diagramSizeFromParent(parent);

	mFlags = flags;
	penGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramPenGroup::ShowWidth | DiagramPenGroup::ShowStyle : DiagramPenGroup::ShowAll;
	brushGroupCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramBrushGroup::ShowFilled : DiagramBrushGroup::ShowColor;

	topLeftLayout = new DiagramPositionLayout(diagramSize);
	bottomRightLayout = new DiagramPositionLayout(diagramSize);
	penGroup = new DiagramPenGroup(penGroupCapabilities, diagramSize);
	brushGroup = new DiagramBrushGroup(brushGroupCapabilities);

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
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	setWindowTitle("Ellipse Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramEllipseItemDialog::~DiagramEllipseItemDialog() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramEllipseItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());
}
//--------------------------------------------------------------------------------------------------
void DiagramEllipseItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramEllipseItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> DiagramEllipseItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
