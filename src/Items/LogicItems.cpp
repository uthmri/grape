/* LogicItems.cpp
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

#include <LogicItems.h>

LogicItem::LogicItem(const QString& key, const QString& iconPath) : DiagramPathItem()
{
	mUniqueKey = key;
	mIconPath = iconPath;
}
//--------------------------------------------------------------------------------------------------
LogicItem::LogicItem(const LogicItem& item) : DiagramPathItem(item)
{
	mUniqueKey = item.mUniqueKey;
	mIconPath = item.mIconPath;
}
//--------------------------------------------------------------------------------------------------
LogicItem::~LogicItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::copy() const
{
	return new LogicItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString LogicItem::uniqueKey() const
{
	return mUniqueKey;
}
//--------------------------------------------------------------------------------------------------
QString LogicItem::iconPath() const
{
	return mIconPath;
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> LogicItem::create()
{
	QList<DiagramItem*> items;

	items.append(createAndGateItem());
	items.append(createOrGateItem());
	items.append(createXorGateItem());
	items.append(createXnorGateItem());
	items.append(createNandGateItem());
	items.append(createNorGateItem());
	items.append(createMultiplexerItem());
	items.append(createDemultiplexerItem());
	items.append(createBufferItem());
	items.append(createNotGateItem());
	items.append(createTristateBuffer1Item());
	items.append(createTristateBuffer2Item());
	items.append(createFlipFlop1Item());
	items.append(createFlipFlop2Item());
	return items;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createAndGateItem()
{
	LogicItem* item = new LogicItem("AND_Gate", ":/icons/custom/items/and_gate.png");

	QPainterPath path;
	path.moveTo(-150, -100);
	path.cubicTo(250, -100, 250, 100, -150, 100);
	path.lineTo(-150, -100);

	path.moveTo(-200, -50);
	path.lineTo(-150, -50);
	path.moveTo(-200, 50);
	path.lineTo(-150, 50);
	path.moveTo(150, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -50);
	item->addConnectionPoint(-200, 50);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createOrGateItem()
{
	LogicItem* item = new LogicItem("OR_Gate", ":/icons/custom/items/or_gate.png");

	QPainterPath path;
	path.moveTo(-150, -100);
	path.cubicTo(250, -100, 250, 100, -150, 100);
	path.cubicTo(-70, 100, -70, -100, -150, -100);

	path.moveTo(-200, -50);
	path.lineTo(-100, -50);
	path.moveTo(-200, 50);
	path.lineTo(-100, 50);
	path.moveTo(150, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -50);
	item->addConnectionPoint(-200, 50);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createXorGateItem()
{
	LogicItem* item = new LogicItem("XOR_Gate", ":/icons/custom/items/xor_gate.png");

	QPainterPath path;
	path.moveTo(-150, -100);
	path.cubicTo(250, -100, 250, 100, -150, 100);
	path.cubicTo(-70, 100, -70, -100, -150, -100);

	path.moveTo(-200, 100);
	path.cubicTo(-120, 100, -120, -100, -200, -100);

	path.moveTo(-200, -50);
	path.lineTo(-150, -50);
	path.moveTo(-200, 50);
	path.lineTo(-150, 50);
	path.moveTo(150, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -50);
	item->addConnectionPoint(-200, 50);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createXnorGateItem()
{
	LogicItem* item = new LogicItem("XNOR_Gate", ":/icons/custom/items/xnor_gate.png");

	QPainterPath path;
	path.moveTo(-150, -100);
	path.cubicTo(250, -100, 250, 100, -150, 100);
	path.cubicTo(-70, 100, -70, -100, -150, -100);

	path.moveTo(-200, 100);
	path.cubicTo(-120, 100, -120, -100, -200, -100);

	path.moveTo(190, 0);
	path.arcTo(150, -20, 40, 40, 0, 360);

	path.moveTo(-200, -50);
	path.lineTo(-150, -50);
	path.moveTo(-200, 50);
	path.lineTo(-150, 50);
	path.moveTo(190, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -50);
	item->addConnectionPoint(-200, 50);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createNandGateItem()
{
	LogicItem* item = new LogicItem("NAND_Gate", ":/icons/custom/items/nand_gate.png");

	QPainterPath path;
	path.moveTo(-150, -100);
	path.cubicTo(250, -100, 250, 100, -150, 100);
	path.lineTo(-150, -100);

	path.moveTo(190, 0);
	path.arcTo(150, -20, 40, 40, 0, 360);

	path.moveTo(-200, -50);
	path.lineTo(-150, -50);
	path.moveTo(-200, 50);
	path.lineTo(-150, 50);
	path.moveTo(190, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -50);
	item->addConnectionPoint(-200, 50);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createNorGateItem()
{
	LogicItem* item = new LogicItem("NOR_Gate", ":/icons/custom/items/nor_gate.png");

	QPainterPath path;
	path.moveTo(-150, -100);
	path.cubicTo(250, -100, 250, 100, -150, 100);
	path.cubicTo(-70, 100, -70, -100, -150, -100);

	path.moveTo(190, 0);
	path.arcTo(150, -20, 40, 40, 0, 360);

	path.moveTo(-200, -50);
	path.lineTo(-100, -50);
	path.moveTo(-200, 50);
	path.lineTo(-100, 50);
	path.moveTo(190, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -50);
	item->addConnectionPoint(-200, 50);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createNotGateItem()
{
	LogicItem* item = new LogicItem("NOT_Gate", ":/icons/custom/items/not_gate.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(90, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	path.moveTo(90, 0);
	path.arcTo(50, -20, 40, 40, 0, 360);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createMultiplexerItem()
{
	LogicItem* item = new LogicItem("Multiplexer", ":/icons/custom/items/multiplexer.png");

	QPainterPath path;
	path.moveTo(-100, -200);
	path.lineTo(-100, 200);
	path.lineTo(100, 100);
	path.lineTo(100, -100);
	path.lineTo(-100, -200);

	path.moveTo(-200, -100);
	path.lineTo(-100, -100);
	path.moveTo(-200, 100);
	path.lineTo(-100, 105);

	path.moveTo(100, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -100);
	item->addConnectionPoint(-200, 100);
	item->addConnectionPoint(200, 0);
	item->addConnectionPoint(0, -150);
	item->addConnectionPoint(0, 150);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createDemultiplexerItem()
{
	LogicItem* item = new LogicItem("Demultiplexer", ":/icons/custom/items/demultiplexer.png");

	QPainterPath path;
	path.moveTo(100, -200);
	path.lineTo(100, 200);
	path.lineTo(-100, 100);
	path.lineTo(-100, -100);
	path.lineTo(100, -200);

	path.moveTo(200, -100);
	path.lineTo(100, -100);
	path.moveTo(200, 100);
	path.lineTo(100, 105);

	path.moveTo(-100, 0);
	path.lineTo(-200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, -100);
	item->addConnectionPoint(200, 100);
	item->addConnectionPoint(0, -150);
	item->addConnectionPoint(0, 150);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createBufferItem()
{
	LogicItem* item = new LogicItem("Buffer", ":/icons/custom/items/buffer.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(50, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createTristateBuffer1Item()
{
	LogicItem* item = new LogicItem("Tristate_Buffer1", ":/icons/custom/items/tristate_buffer.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(50, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	path.moveTo(0, -37.5);
	path.lineTo(0, -75);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);
	item->addConnectionPoint(0, -75);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createTristateBuffer2Item()
{
	LogicItem* item = new LogicItem("Tristate_Buffer2", ":/icons/custom/items/tristate_buffer2.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(90, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	path.moveTo(90, 0);
	path.arcTo(50, -20, 40, 40, 0, 360);

	path.moveTo(0, -37.5);
	path.lineTo(0, -75);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);
	item->addConnectionPoint(0, -75);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createFlipFlop1Item()
{
	LogicItem* item = new LogicItem("Flip-Flop1", ":/icons/custom/items/flip_flop.png");

	QPainterPath path;
	path.moveTo(-150, -200);
	path.lineTo(-150, 200);
	path.lineTo(150, 200);
	path.lineTo(150, -200);
	path.lineTo(-150, -200);

	path.moveTo(-200, -100);
	path.lineTo(-150, -100);
	path.moveTo(-200, 100);
	path.lineTo(-150, 100);

	path.moveTo(200, -100);
	path.lineTo(150, -100);
	path.moveTo(200, 100);
	path.lineTo(150, 100);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -100);
	item->addConnectionPoint(-200, 100);
	item->addConnectionPoint(200, -100);
	item->addConnectionPoint(200, 100);
	item->addConnectionPoint(0, -200);
	item->addConnectionPoint(0, 200);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* LogicItem::createFlipFlop2Item()
{
	LogicItem* item = new LogicItem("Flip-Flop2", ":/icons/custom/items/flip_flop2.png");

	QPainterPath path;
	path.moveTo(-150, -200);
	path.lineTo(-150, 200);
	path.lineTo(150, 200);
	path.lineTo(150, -200);
	path.lineTo(-150, -200);

	path.moveTo(-200, -100);
	path.lineTo(-150, -100);
	path.moveTo(-200, 100);
	path.lineTo(-150, 100);

	path.moveTo(200, -100);
	path.lineTo(150, -100);
	path.moveTo(200, 100);
	path.lineTo(150, 100);

	path.moveTo(-150, 50);
	path.lineTo(-100, 100);
	path.lineTo(-150, 150);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, -100);
	item->addConnectionPoint(-200, 100);
	item->addConnectionPoint(200, -100);
	item->addConnectionPoint(200, 100);
	item->addConnectionPoint(0, -200);
	item->addConnectionPoint(0, 200);

	return item;
}
//--------------------------------------------------------------------------------------------------
