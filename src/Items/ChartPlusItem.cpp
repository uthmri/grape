/* ChartPlusItem.cpp
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

#include <ChartPlusItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

ChartPlusItem::ChartPlusItem() : DiagramEllipseItem() { }
//--------------------------------------------------------------------------------------------------
ChartPlusItem::ChartPlusItem(const ChartPlusItem& item) : DiagramEllipseItem(item) { }
//--------------------------------------------------------------------------------------------------
ChartPlusItem::~ChartPlusItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ChartPlusItem::copy() const
{
	return new ChartPlusItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString ChartPlusItem::uniqueKey() const
{
	return "ChartPlus";
}
//--------------------------------------------------------------------------------------------------
QString ChartPlusItem::iconPath() const
{
	return ":/icons/custom/items/chartplus.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPlusItem::render(DiagramPainter* painter)
{
	QRectF bounds = boundingRect();
	QPointF center = bounds.center();

	DiagramEllipseItem::render(painter);

	painter->drawLine(QPointF(bounds.left(), center.y()), QPointF(bounds.right(), center.y()));
	painter->drawLine(QPointF(center.x(), bounds.top()), QPointF(center.x(), bounds.bottom()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ChartPlusItem::propertiesDialog(QWidget* parent) const
{
	DiagramItemPropertiesDialog* dialog;

	if (parent == NULL) parent = diagram();

	dialog = new DiagramEllipseItemDialog(parent);
	dialog->setWindowTitle("Chart Plus Properties");

	return dialog;
}
//--------------------------------------------------------------------------------------------------
