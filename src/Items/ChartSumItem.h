/* ChartSumItem.h
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

#ifndef CHARTSUMITEM_H
#define CHARTSUMITEM_H

#include <DiagramEllipseItem.h>

class ChartSumItem : public DiagramEllipseItem
{
public:
	ChartSumItem();
	ChartSumItem(const ChartSumItem& item);
	~ChartSumItem();

	DiagramItem* copy() const;
	QString uniqueKey() const;
	QString iconPath() const;

	// Render
	void render(DiagramPainter* painter);

	// Misc
	DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;
};

#endif
