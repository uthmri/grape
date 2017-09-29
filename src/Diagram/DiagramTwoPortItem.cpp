/* DiagramTwoPortItem.cpp
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

#include <DiagramTwoPortItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramTwoPortItem::DiagramTwoPortItem() : DiagramBoxResizeItem()
{
	addProperty("Corner X-Radius", QVariant(0.0));
	addProperty("Corner Y-Radius", QVariant(0.0));

    // PIP-ED
//    for(int i = 0; i < 4; i++)
//		point(i)->setFlags(DiagramItemPoint::Control | DiagramItemPoint::Connection);

    for(int i = 0; i < 4; i++)
        point(i)->setFlags(DiagramItemPoint::Control | DiagramItemPoint::Connection);
    for(int i = 4; i < 8; i++)
        point(i)->setFlags(DiagramItemPoint::Control);
}
//--------------------------------------------------------------------------------------------------
DiagramTwoPortItem::DiagramTwoPortItem(const DiagramTwoPortItem& item) : DiagramBoxResizeItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramTwoPortItem::~DiagramTwoPortItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramTwoPortItem::copy() const
{
    return new DiagramTwoPortItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramTwoPortItem::uniqueKey() const
{
	return "box";
}
//--------------------------------------------------------------------------------------------------
QString DiagramTwoPortItem::iconPath() const
{
	return ":/icons/oxygen/draw-rectangle.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTwoPortItem::setCornerRadii(qreal xRadius, qreal yRadius)
{
	setPropertyValue("Corner X-Radius", xRadius);
	setPropertyValue("Corner Y-Radius", yRadius);
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPortItem::setCornerRadiusX(qreal radius)
{
	setPropertyValue("Corner X-Radius", radius);
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPortItem::setCornerRadiusY(qreal radius)
{
	setPropertyValue("Corner Y-Radius", radius);
}
//--------------------------------------------------------------------------------------------------
qreal DiagramTwoPortItem::cornerRadiusX() const
{
	return propertyValue("Corner X-Radius").toDouble();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramTwoPortItem::cornerRadiusY() const
{
	return propertyValue("Corner Y-Radius").toDouble();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramTwoPortItem::boundingRect() const
{
	return rectFromPoints(point(0)->position(), point(1)->position());
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramTwoPortItem::shape() const
{
	QPainterPath mainPath;
	qreal outlineWidth = adjustOutlineForView(pen().widthF());

	QRectF bounds = orientedRect();
	qreal xRadius = orientedCornerRadiusX();
	qreal yRadius = orientedCornerRadiusY();

	QRectF outerBounds = bounds.adjusted(-outlineWidth/2, -outlineWidth/2, outlineWidth/2, outlineWidth/2);
	//QRectF innerBounds = bounds.adjusted(outlineWidth/2, outlineWidth/2, -outlineWidth/2, -outlineWidth/2);

	// Main path
	if (brush().color().alpha() > 0 || bounds.width() == 0 || bounds.height() == 0)
	{
		mainPath.addRoundedRect(outerBounds, xRadius, yRadius);
	}
	else
	{
		// This doesn't work
		/*QPainterPath outerPath, innerPath;

		outerPath.addRoundedRect(outerBounds, xRadius, yRadius);
		innerPath.addRoundedRect(innerBounds, xRadius, yRadius);
		mainPath = outerPath.subtracted(innerPath);*/

		QPainterPath linePath;
		QPainterPathStroker ppStroker;
		qreal xLeft, xLeftOffset, xRightOffset, xRight;
		qreal yTop, yTopOffset, yBottomOffset, yBottom;

		xLeft = bounds.left();
		xRight = bounds.right();
		xLeftOffset = xLeft + xRadius;
		xRightOffset = xRight - xRadius;
		if (xLeftOffset > xRightOffset)
		{
			xLeftOffset = bounds.center().x();
			xRightOffset = xLeftOffset;
		}

		yTop = bounds.top();
		yBottom = bounds.bottom();
		yTopOffset = yTop + yRadius;
		yBottomOffset = yBottom - yRadius;
		if (yTopOffset > yBottomOffset)
		{
			yTopOffset = bounds.center().y();
			yBottomOffset = yTopOffset;
		}

		linePath.moveTo(xLeftOffset, yTop);
		linePath.lineTo(xRightOffset, yTop);
		linePath.moveTo(xRightOffset, yTop);
		linePath.arcTo(2 * xRightOffset - xRight, yTop,
			2 * (xRight - xRightOffset), 2 * (yTopOffset - yTop), 90, -90);

		linePath.moveTo(xRight, yTopOffset);
		linePath.lineTo(xRight, yBottomOffset);
		linePath.moveTo(xRight, yBottomOffset);
		linePath.arcTo(2 * xRightOffset - xRight, 2 * yBottomOffset - yBottom,
			2 * (xRight - xRightOffset), 2 * (yBottom - yBottomOffset), 0, -90);

		linePath.moveTo(xRightOffset, yBottom);
		linePath.lineTo(xLeftOffset, yBottom);
		linePath.moveTo(xLeftOffset, yBottom);
		linePath.arcTo(xLeft, 2 * yBottomOffset - yBottom,
			2 * (xLeftOffset - xLeft), 2 * (yBottom - yBottomOffset), 270, -90);

		linePath.moveTo(xLeft, yBottomOffset);
		linePath.lineTo(xLeft, yTopOffset);
		linePath.moveTo(xLeft, yTopOffset);
		linePath.arcTo(xLeft, yTop,
			2 * (xLeftOffset - xLeft), 2 * (yTopOffset - yTop), 180, -90);

		ppStroker.setWidth(outlineWidth);
		ppStroker.setCapStyle(pen().capStyle());
		ppStroker.setJoinStyle(pen().joinStyle());
		mainPath = ppStroker.createStroke(linePath);
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
void DiagramTwoPortItem::render(DiagramPainter* painter)
{
	QRectF bounds = orientedRect();
	qreal xRadius = orientedCornerRadiusX();
	qreal yRadius = orientedCornerRadiusY();

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

	//painter->setPen(QPen(Qt::magenta, 1));
	//painter->setBrush(Qt::magenta);
	//painter->drawPath(shape());

#ifdef WIN32
	if (painter->paintEngine()->type() == QPaintEngine::Pdf && lBrush.color().alpha() == 0)
	{
		// Hack to get transparency output to PDF to work properly on Windows
		painter->setForItem(lPen, Qt::transparent);

		if (bounds.width() >= 2*xRadius && bounds.height() >= 2*yRadius)
		{
            painter->drawLine(QPointF(bounds.left() + xRadius, bounds.top()),
                              QPointF(bounds.right() - xRadius, bounds.top()));
			painter->drawLine(QPointF(bounds.left() + xRadius, bounds.bottom()),
							  QPointF(bounds.right() - xRadius, bounds.bottom()));
			painter->drawLine(QPointF(bounds.left(), bounds.top() + yRadius),
							  QPointF(bounds.left(), bounds.bottom() - yRadius));
			painter->drawLine(QPointF(bounds.right(), bounds.top() + yRadius),
							  QPointF(bounds.right(), bounds.bottom() - yRadius));
			painter->drawArc(QRectF(
				bounds.left(), bounds.top(), 2*xRadius, 2*yRadius), 1440, 1440);
			painter->drawArc(QRectF(
				bounds.right() - 2*xRadius, bounds.top(), 2*xRadius, 2*yRadius), 0, 1440);
			painter->drawArc(QRectF(
				bounds.right() - 2*xRadius, bounds.bottom() - 2*yRadius, 2*xRadius, 2*yRadius), -1440, 1440);
			painter->drawArc(QRectF(
				bounds.left(), bounds.bottom() - 2*yRadius, 2*xRadius, 2*yRadius), 2880, 1440);
		}
		else
		{
            painter->drawLine(bounds.topLeft(), bounds.topRight());
            painter->drawLine(bounds.topRight(), bounds.bottomRight());
            painter->drawLine(bounds.bottomRight(), bounds.bottomLeft());
            painter->drawLine(bounds.bottomLeft(), bounds.topLeft());
        }
	}
	else
	{
		painter->setForItem(lPen, lBrush);

        // PIP-ED 20150713

//		if (xRadius > 0 || yRadius > 0) painter->drawRoundedRect(bounds, xRadius, yRadius);
//		else painter->drawRect(bounds);

        qreal p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y;
        // draw a smaller rectangle
        p1x = bounds.topLeft().x()+0.25*bounds.width(); p1y = bounds.topLeft().y();
        p2x = bounds.topRight().x()-0.25*bounds.width(); p2y = bounds.topLeft().y();
        painter->drawLine(QPointF(p1x,p1y), QPointF(p2x,p2y));
        //painter->drawLine(bounds.topLeft(), bounds.topRight());

        p3x = bounds.bottomRight().x()-0.25*bounds.width(); p3y = bounds.bottomLeft().y();
        painter->drawLine(QPointF(p2x,p2y), QPointF(p3x,p3y));
        //painter->drawLine(bounds.topRight(), bounds.bottomRight());

        p4x = bounds.bottomLeft().x()+0.25*bounds.width(); p4y = bounds.bottomLeft().y();
        painter->drawLine(QPointF(p3x,p3y), QPointF(p4x,p4y));
//        painter->drawLine(bounds.bottomRight(), bounds.bottomLeft());

        painter->drawLine(QPointF(p4x,p4y), QPointF(p1x,p1y));
//        painter->drawLine(bounds.bottomLeft(), bounds.topLeft());


        // draw the connection arms of the two port
        p1x = bounds.topLeft().x(); p1y = bounds.topLeft().y()+0.0*0.25*bounds.height();
        p2x = p1x + 0.25*bounds.width(); p2y = p1y+1.0*0.25*bounds.height();
        painter->drawLine(QPointF(p1x,p1y), QPointF(p2x,p2y));

        p1x = bounds.topLeft().x(); p1y = bounds.bottomRight().y()-0.0*0.25*bounds.height();
        p2x = p1x + 0.25*bounds.width(); p2y = p1y-1.0*0.25*bounds.height();
        painter->drawLine(QPointF(p1x,p1y), QPointF(p2x,p2y));

        p1x = bounds.topRight().x() - 0.25*bounds.width(); p1y = bounds.topRight().y()+1.0*0.25*bounds.height();
        p2x = p1x + 0.25*bounds.width(); p2y = p1y-1.0*0.25*bounds.height();
        painter->drawLine(QPointF(p1x,p1y), QPointF(p2x,p2y));

        p1x = bounds.topRight().x() - 0.25*bounds.width(); p1y = bounds.bottomRight().y()-1.0*0.25*bounds.height();
        p2x = p1x + 0.25*bounds.width(); p2y = p1y+1.0*0.25*bounds.height();
        painter->drawLine(QPointF(p1x,p1y), QPointF(p2x,p2y));
    }
#else
	painter->setForItem(lPen, lBrush);
    if (xRadius > 0 || yRadius > 0) painter->drawRoundedRect(bounds, xRadius, yRadius);
    else painter->drawRect(bounds);

#endif
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTwoPortItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("pen", penToString(pen()));
	xmlWriter.writeAttribute("brush", brushToString(brush()));
	xmlWriter.writeAttribute("cornerRadiusX", QString::number(cornerRadiusX()));
	xmlWriter.writeAttribute("cornerRadiusY", QString::number(cornerRadiusY()));
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPortItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("pen"))
		setPen(penFromString(attributes.value("pen").toString()));
	if (attributes.hasAttribute("brush"))
		setBrush(brushFromString(attributes.value("brush").toString()));
	if (attributes.hasAttribute("cornerRadiusX"))
		setCornerRadiusX(attributes.value("cornerRadiusX").toString().toDouble());
	if (attributes.hasAttribute("cornerRadiusY"))
		setCornerRadiusY(attributes.value("cornerRadiusY").toString().toDouble());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramTwoPortItem::propertiesDialog(QWidget* parent) const
{
    DiagramTwoPortItemDialog::Flags dialogFlags = DiagramTwoPortItemDialog::NoFlags;
    if (flags() & UsePalette) dialogFlags |= DiagramTwoPortItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

    return new DiagramTwoPortItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramTwoPortItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	QVariant result = DiagramBoxResizeItem::aboutToChangeEvent(reason, value);

	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);

		setCornerRadii(cornerRadiusX() * scaleFactor, cornerRadiusY() * scaleFactor);
	}

	return result;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramTwoPortItem::orientedRect() const
{
	return rectFromPoints(point(0)->position(), point(1)->position());
}
//--------------------------------------------------------------------------------------------------
qreal DiagramTwoPortItem::orientedCornerRadiusX() const
{
	qreal radius = cornerRadiusX();
	if (rotationAngle() == 90.0 || rotationAngle() == 270.0) radius = cornerRadiusY();
	return radius;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramTwoPortItem::orientedCornerRadiusY() const
{
	qreal radius = cornerRadiusY();
	if (rotationAngle() == 90.0 || rotationAngle() == 270.0) radius = cornerRadiusX();
	return radius;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramTwoPortItemDialog::DiagramTwoPortItemDialog(QWidget* parent, Flags flags) : DiagramItemPropertiesDialog(parent)
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
	cornerRadiiLayout = new DiagramPositionLayout(diagramSize);
	penGroup = new DiagramPenGroup(penGroupCapabilities, diagramSize);
	brushGroup = new DiagramBrushGroup(brushGroupCapabilities);

	QGroupBox* positionGroup = new QGroupBox("Position");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Top Left: ", topLeftLayout);
	fLayout->addRow("Bottom Right: ", bottomRightLayout);
	fLayout->addRow("Corner Radius: ", cornerRadiiLayout);
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

	setWindowTitle("Box Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramTwoPortItemDialog::~DiagramTwoPortItemDialog() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTwoPortItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Corner X-Radius"))
		cornerRadiiLayout->setPositionX(properties["Corner X-Radius"].toDouble());
	if (properties.contains("Corner Y-Radius"))
		cornerRadiiLayout->setPositionY(properties["Corner Y-Radius"].toDouble());
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoPortItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramTwoPortItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Corner X-Radius"] = QVariant(cornerRadiiLayout->positionX());
	properties["Corner Y-Radius"] = QVariant(cornerRadiiLayout->positionY());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> DiagramTwoPortItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
