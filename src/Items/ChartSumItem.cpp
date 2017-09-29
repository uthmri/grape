/* ChartSumItem.cpp
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

#include <ChartSumItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

ChartSumItem::ChartSumItem() : DiagramEllipseItem() { }
//--------------------------------------------------------------------------------------------------
ChartSumItem::ChartSumItem(const ChartSumItem& item) : DiagramEllipseItem(item) { }
//--------------------------------------------------------------------------------------------------
ChartSumItem::~ChartSumItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ChartSumItem::copy() const
{
	return new ChartSumItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString ChartSumItem::uniqueKey() const
{
	return "ChartSum";
}
//--------------------------------------------------------------------------------------------------
QString ChartSumItem::iconPath() const
{
	return ":/icons/custom/items/chartsum.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartSumItem::render(DiagramPainter* painter)
{
	QRectF bounds = boundingRect();
	QPointF center = bounds.center();
	qreal xOffset = bounds.width() * qSqrt(2) / 4;
	qreal yOffset = bounds.height() * qSqrt(2) / 4;

	DiagramEllipseItem::render(painter);

	painter->drawLine(center + QPointF(-xOffset, -yOffset), center + QPointF(xOffset, yOffset));
	painter->drawLine(center + QPointF(-xOffset, yOffset), center + QPointF(xOffset, -yOffset));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ChartSumItem::propertiesDialog(QWidget* parent) const
{
	DiagramItemPropertiesDialog* dialog;

	if (parent == NULL) parent = diagram();

	dialog = new DiagramEllipseItemDialog(parent);
	dialog->setWindowTitle("Chart Sum Properties");

	return dialog;
}
//--------------------------------------------------------------------------------------------------
