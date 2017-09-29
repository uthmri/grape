/* ElectricItems.h
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

#ifndef ELECTRICITEMS_H
#define ELECTRICITEMS_H

#include <DiagramPathItem.h>

class ElectricItem : public DiagramPathItem
{
private:
	QString mIconPath;
	QString mUniqueKey;

public:
	ElectricItem(const QString& key, const QString& iconPath);
	ElectricItem(const ElectricItem& item);
	~ElectricItem();

	DiagramItem* copy() const;
	QString uniqueKey() const;
	QString iconPath() const;


public:
	static QList<DiagramItem*> create();

private:
	static DiagramItem* createResistor1Item();
	static DiagramItem* createResistor2Item();
	static DiagramItem* createCapacitor1Item();
	static DiagramItem* createCapacitor2Item();
	static DiagramItem* createInductor1Item();

	static DiagramItem* createDiodeItem();
	static DiagramItem* createZenerDiodeItem();
	static DiagramItem* createSchottkyDiodeItem();
	static DiagramItem* createLedItem();

	static DiagramItem* createNPNBJTItem();
	static DiagramItem* createPNPBJTItem();
	static DiagramItem* createNMOSFETItem();
	static DiagramItem* createPMOSFETItem();

	static DiagramItem* createGround1Item();
	static DiagramItem* createGround2Item();
	static DiagramItem* createDCVoltageItem();
	static DiagramItem* createACVoltageItem();
	static DiagramItem* createDCCurrentItem();
	static DiagramItem* createACCurrentItem();

	static DiagramItem* createOpAmpItem();
};

#endif
