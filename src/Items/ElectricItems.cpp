/* ElectricItems.cpp
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

#include <ElectricItems.h>

ElectricItem::ElectricItem(const QString& key, const QString& iconPath) : DiagramPathItem()
{
	mUniqueKey = key;
	mIconPath = iconPath;
}
//--------------------------------------------------------------------------------------------------
ElectricItem::ElectricItem(const ElectricItem& item) : DiagramPathItem(item)
{
	mUniqueKey = item.mUniqueKey;
	mIconPath = item.mIconPath;
}
//--------------------------------------------------------------------------------------------------
ElectricItem::~ElectricItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::copy() const
{
	return new ElectricItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString ElectricItem::uniqueKey() const
{
	return mUniqueKey;
}
//--------------------------------------------------------------------------------------------------
QString ElectricItem::iconPath() const
{
	return mIconPath;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> ElectricItem::create()
{
	QList<DiagramItem*> items;

	items.append(createResistor1Item());
	items.append(createResistor2Item());
	items.append(createCapacitor1Item());
	items.append(createCapacitor2Item());
	items.append(createInductor1Item());
	items.append(createDiodeItem());
	items.append(createZenerDiodeItem());
	items.append(createSchottkyDiodeItem());
	items.append(createNPNBJTItem());
	items.append(createPNPBJTItem());
	items.append(createNMOSFETItem());
	items.append(createPMOSFETItem());
	items.append(createGround1Item());
	items.append(createGround2Item());
	items.append(createOpAmpItem());
	items.append(createLedItem());
	items.append(createDCVoltageItem());
	items.append(createACVoltageItem());
	items.append(createDCCurrentItem());
	items.append(createACCurrentItem());

	return items;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createResistor1Item()
{
	ElectricItem* item = new ElectricItem("Resistor1", ":/icons/custom/items/resistor1.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-150, 0);
	path.lineTo(-125, -50);
	path.lineTo(-75, 50);
	path.lineTo(-25, -50);
	path.lineTo(25, 50);
	path.lineTo(75, -50);
	path.lineTo(125, 50);
	path.lineTo(150, 0);
	path.lineTo(200, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createResistor2Item()
{
	ElectricItem* item = new ElectricItem("Resistor2", ":/icons/custom/items/resistor2.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-150, 0);
	path.moveTo(200, 0);
	path.lineTo(150, 0);

	path.moveTo(-150, -50);
	path.lineTo(150, -50);
	path.lineTo(150, 50);
	path.lineTo(-150, 50);
	path.lineTo(-150, -50);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createCapacitor1Item()
{
	ElectricItem* item = new ElectricItem("Capacitor1", ":/icons/custom/items/capacitor1.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-25, 0);
	path.moveTo(25, 0);
	path.lineTo(200, 0);

	path.moveTo(-25, -75);
	path.lineTo(-25, 75);
	path.moveTo(25, -75);
	path.lineTo(25, 75);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createCapacitor2Item()
{
	ElectricItem* item = new ElectricItem("Capacitor2", ":/icons/custom/items/capacitor2.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-25, 0);
	path.moveTo(25, 0);
	path.lineTo(200, 0);

	path.moveTo(-25, -75);
	path.lineTo(-25, 75);

	path.moveTo(45, -75);
	path.cubicTo(5, -60, 5, 60, 45, 75);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createInductor1Item()
{
	ElectricItem* item = new ElectricItem("Inductor1", ":/icons/custom/items/inductor1.png");

	QPainterPath path;
	path.moveTo(-300, 0);
	path.lineTo(-240, 0);

	path.arcTo(-240, -75, 120, 150, 180, -180);
	path.arcTo(-120, -75, 120, 150, 180, -180);
	path.arcTo(0, -75, 120, 150, 180, -180);
	path.arcTo(120, -75, 120, 150, 180, -180);

	path.moveTo(240, 0);
	path.lineTo(300, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(-300, 0);
	item->addConnectionPoint(300, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createDiodeItem()
{
	ElectricItem* item = new ElectricItem("Diode", ":/icons/custom/items/diode.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(50, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	path.moveTo(50, -75);
	path.lineTo(50, 75);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createZenerDiodeItem()
{
	ElectricItem* item = new ElectricItem("Zener_Diode", ":/icons/custom/items/zener_diode.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(50, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	path.moveTo(100, -125);
	path.lineTo(50, -75);
	path.lineTo(50, 75);
	path.lineTo(0, 125);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createSchottkyDiodeItem()
{
	ElectricItem* item = new ElectricItem("Schottky_Diode", ":/icons/custom/items/schottky_diode.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(50, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	path.moveTo(10, 60);
	path.lineTo(10, 100);
	path.lineTo(50, 100);
	path.lineTo(50, -100);
	path.lineTo(90, -100);
	path.lineTo(90, -60);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createLedItem()
{
	ElectricItem* item = new ElectricItem("LED", ":/icons/custom/items/led.png");

	QPainterPath path;
	path.moveTo(-200, 0);
	path.lineTo(-50, 0);
	path.moveTo(50, 0);
	path.lineTo(200, 0);

	path.moveTo(-50, -75);
	path.lineTo(-50, 75);
	path.lineTo(50, 0);
	path.lineTo(-50, -75);

	path.moveTo(50, -75);
	path.lineTo(50, 75);

	path.moveTo(70, -35);
	path.lineTo(110, -75);
	path.moveTo(80, -75);
	path.lineTo(110, -75);
	path.lineTo(110, -45);

	path.moveTo(130, -35);
	path.lineTo(170, -75);
	path.moveTo(140, -75);
	path.lineTo(170, -75);
	path.lineTo(170, -45);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(200, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createNPNBJTItem()
{
	ElectricItem* item = new ElectricItem("NPN_BJT", ":/icons/custom/items/npn_bjt.png");

	QPainterPath path;
	path.moveTo(175, 0);
	path.arcTo(-145, -160, 320, 320, 0, 360);

	path.moveTo(100, -200);
	path.lineTo(100, -100);
	path.lineTo(-75, -50);
	path.moveTo(-75, 50);
	path.lineTo(100, 100);
	path.lineTo(100, 200);

	path.moveTo(-75, -100);
	path.lineTo(-75, 100);

	path.moveTo(-200, 0);
	path.lineTo(-75, 0);

	path.moveTo(70.86, 47.55);
	path.lineTo(100, 100);
	path.lineTo(41.55, 129.14);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(100, -200);
	item->addConnectionPoint(100, 200);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createPNPBJTItem()
{
	ElectricItem* item = new ElectricItem("PNP_BJT", ":/icons/custom/items/pnp_bjt.png");

	QPainterPath path;
	path.moveTo(175, 0);
	path.arcTo(-145, -160, 320, 320, 0, 360);

	path.moveTo(100, -200);
	path.lineTo(100, -100);
	path.lineTo(-75, -50);
	path.moveTo(-75, 50);
	path.lineTo(100, 100);
	path.lineTo(100, 200);

	path.moveTo(-75, -100);
	path.lineTo(-75, 100);

	path.moveTo(-200, 0);
	path.lineTo(-75, 0);

	path.moveTo(-22.55, 20.86);
	path.lineTo(-75, 50);
	path.lineTo(-45.86, 102.45);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(100, -200);
	item->addConnectionPoint(100, 200);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createNMOSFETItem()
{
	ElectricItem* item = new ElectricItem("NMOS_FET",  ":/icons/custom/items/nmos_fet.png");

	QPainterPath path;
	path.moveTo(175, 0);
	path.arcTo(-145, -160, 320, 320, 0, 360);

	path.moveTo(100, -200);
	path.lineTo(100, -75);
	path.lineTo(0, -75);
	path.moveTo(0, 0);
	path.lineTo(100, 0);
	path.lineTo(100, 200);
	path.moveTo(0, 75);
	path.lineTo(100, 75);

	path.moveTo(0, 100);
	path.lineTo(0, 50);
	path.moveTo(0, 25);
	path.lineTo(0, -25);
	path.moveTo(0, -50);
	path.lineTo(0, -100);

	path.moveTo(-75, -100);
	path.lineTo(-75, 100);

	path.moveTo(-200, 0);
	path.lineTo(-75, 0);

	path.moveTo(42.43, -42.43);
	path.lineTo(0, 0);
	path.lineTo(42.43, 42.43);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(100, -200);
	item->addConnectionPoint(100, 200);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createPMOSFETItem()
{
	ElectricItem* item = new ElectricItem("PMOS_FET",  ":/icons/custom/items/pmos_fet.png");

	QPainterPath path;
	path.moveTo(175, 0);
	path.arcTo(-145, -160, 320, 320, 0, 360);

	path.moveTo(100, 200);
	path.lineTo(100, 75);
	path.lineTo(0, 75);
	path.moveTo(0, 0);
	path.lineTo(100, 0);
	path.lineTo(100, -200);
	path.moveTo(0, -75);
	path.lineTo(100, -75);

	path.moveTo(0, 100);
	path.lineTo(0, 50);
	path.moveTo(0, 25);
	path.lineTo(0, -25);
	path.moveTo(0, -50);
	path.lineTo(0, -100);

	path.moveTo(-75, -100);
	path.lineTo(-75, 100);

	path.moveTo(-200, 0);
	path.lineTo(-75, 0);

	path.moveTo(57.57, -42.43);
	path.lineTo(100, 0);
	path.lineTo(57.57, 42.43);

	item->setInitialPath(path);
	item->addConnectionPoint(-200, 0);
	item->addConnectionPoint(100, -200);
	item->addConnectionPoint(100, 200);

	return item;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createGround1Item()
{
	ElectricItem* item = new ElectricItem("Ground1", ":/icons/custom/items/ground1.png");

	QPainterPath path;
	path.moveTo(0, 0);
	path.lineTo(0, 50);

	path.moveTo(-100, 50);
	path.lineTo(100, 50);
	path.moveTo(-75, 100);
	path.lineTo(75, 100);
	path.moveTo(-50, 150);
	path.lineTo(50, 150);

	item->setInitialPath(path);
	item->addConnectionPoint(0, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createGround2Item()
{
	ElectricItem* item = new ElectricItem("Ground2", ":/icons/custom/items/ground2.png");

	QPainterPath path;
	path.moveTo(0, 0);
	path.lineTo(0, 50);

	path.moveTo(-100, 50);
	path.lineTo(100, 50);
	path.lineTo(0, 150);
	path.lineTo(-100, 50);

	item->setInitialPath(path);
	item->addConnectionPoint(0, 0);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createDCVoltageItem()
{
	ElectricItem* item = new ElectricItem("DC_Voltage", ":/icons/custom/items/vdc.png");

	QPainterPath path;
	path.moveTo(150, 0);
	path.arcTo(-150, -200, 300, 400, 0, 360);

	path.moveTo(0, -300);
	path.lineTo(0, -200);
	path.moveTo(0, 200);
	path.lineTo(0, 300);

	path.moveTo(-40, -65);
	path.lineTo(40, -65);
	path.moveTo(0, -105);
	path.lineTo(0, -25);

	path.moveTo(-40, 90);
	path.lineTo(40, 90);

	item->setInitialPath(path);
	item->addConnectionPoint(0, -300);
	item->addConnectionPoint(0, 300);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createACVoltageItem()
{
	ElectricItem* item = new ElectricItem("AC_Voltage", ":/icons/custom/items/vac.png");

	QPainterPath path;
	path.moveTo(150, 0);
	path.arcTo(-150, -200, 300, 400, 0, 360);

	path.moveTo(0, -300);
	path.lineTo(0, -200);
	path.moveTo(0, 200);
	path.lineTo(0, 300);

	path.moveTo(-50, 0);
	path.cubicTo(-10, -120, 10, 120, 50, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(0, -300);
	item->addConnectionPoint(0, 300);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createDCCurrentItem()
{
	ElectricItem* item = new ElectricItem("DC_Current", ":/icons/custom/items/idc.png");

	QPainterPath path;
	path.moveTo(150, 0);
	path.arcTo(-150, -200, 300, 400, 0, 360);

	path.moveTo(0, -300);
	path.lineTo(0, -200);
	path.moveTo(0, 200);
	path.lineTo(0, 300);

	path.moveTo(0, 100);
	path.lineTo(0, -100);
	path.moveTo(-42.43, -57.57);
	path.lineTo(0, -100);
	path.lineTo(42.43, -57.57);

	item->setInitialPath(path);
	item->addConnectionPoint(0, -300);
	item->addConnectionPoint(0, 300);

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createACCurrentItem()
{
	ElectricItem* item = new ElectricItem("AC_Current", ":/icons/custom/items/iac.png");

	QPainterPath path;
	path.moveTo(150, 0);
	path.arcTo(-150, -200, 300, 400, 0, 360);

	path.moveTo(0, -300);
	path.lineTo(0, -200);
	path.moveTo(0, 200);
	path.lineTo(0, 300);

	path.moveTo(0, 100);
	path.lineTo(0, -100);
	path.moveTo(-42.43, -57.57);
	path.lineTo(0, -100);
	path.lineTo(42.43, -57.57);

	path.moveTo(-50, 0);
	path.cubicTo(-10, -120, 10, 120, 50, 0);

	item->setInitialPath(path);
	item->addConnectionPoint(0, -300);
	item->addConnectionPoint(0, 300);

	return item;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ElectricItem::createOpAmpItem()
{
	ElectricItem* item = new ElectricItem("Op_Amp", ":/icons/custom/items/opamp.png");

	QPainterPath path;
	path.moveTo(-200, -200);
	path.lineTo(-200, 200);
	path.lineTo(200, 0);
	path.lineTo(-200, -200);

	path.moveTo(-300, -100);
	path.lineTo(-200, -100);
	path.moveTo(-300, 100);
	path.lineTo(-200, 100);
	path.moveTo(300, 0);
	path.lineTo(200, 0);

	path.moveTo(-170, -100);
	path.lineTo(-90, -100);
	path.moveTo(-130, -140);
	path.lineTo(-130, -60);

	path.moveTo(-170, 100);
	path.lineTo(-90, 100);

	item->setInitialPath(path);
	item->addConnectionPoint(-300, -100);
	item->addConnectionPoint(-300, 100);
	item->addConnectionPoint(300, 0);
	item->addConnectionPoint(0, -100);
	item->addConnectionPoint(0, 100);

	return item;
}
//--------------------------------------------------------------------------------------------------
