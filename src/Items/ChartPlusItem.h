/* ChartPlusItem.h
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

#ifndef CHARTPLUSITEM_H
#define CHARTPLUSITEM_H

#include <DiagramEllipseItem.h>

class ChartPlusItem : public DiagramEllipseItem
{
public:
	ChartPlusItem();
	ChartPlusItem(const ChartPlusItem& item);
	~ChartPlusItem();

	DiagramItem* copy() const;
	QString uniqueKey() const;
	QString iconPath() const;

	// Render
	void render(DiagramPainter* painter);

	// Misc
	DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;
};

#endif
