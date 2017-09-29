/* LogicItems.h
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

#ifndef LOGICITEMS_H
#define LOGICITEMS_H

#include <DiagramPathItem.h>

class LogicItem : public DiagramPathItem
{
private:
	QString mIconPath;
	QString mUniqueKey;

public:
	LogicItem(const QString& key, const QString& iconPath);
	LogicItem(const LogicItem& item);
	~LogicItem();

	DiagramItem* copy() const;
    virtual QString uniqueKey() const;
	QString iconPath() const;


public:
	static QList<DiagramItem*> create();

private:
	static DiagramItem* createAndGateItem();
	static DiagramItem* createOrGateItem();
	static DiagramItem* createXorGateItem();
	static DiagramItem* createXnorGateItem();
	static DiagramItem* createNandGateItem();
	static DiagramItem* createNorGateItem();
	static DiagramItem* createNotGateItem();

	static DiagramItem* createMultiplexerItem();
	static DiagramItem* createDemultiplexerItem();

	static DiagramItem* createBufferItem();
	static DiagramItem* createTristateBuffer1Item();
	static DiagramItem* createTristateBuffer2Item();

	static DiagramItem* createFlipFlop1Item();
	static DiagramItem* createFlipFlop2Item();
};

#endif
