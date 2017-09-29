/* DiagramItemFactory.cpp
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

#include <DiagramItemFactory.h>
#include <DiagramItem.h>
#include <DiagramItemGroup.h>

DiagramItemFactory::DiagramItemFactory() : QObject()
{
	registerItem(new DiagramItemGroup());
}
//--------------------------------------------------------------------------------------------------
DiagramItemFactory::~DiagramItemFactory()
{
	clear();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemFactory::setDefaultProperties(const QHash<QString, QVariant>& defaultProperties)
{
	mDefaultProperties = defaultProperties;
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramItemFactory::defaultProperties() const
{
	return mDefaultProperties;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemFactory::containsDefaultProperty(const QString& key) const
{
	return mDefaultProperties.contains(key);
}
//--------------------------------------------------------------------------------------------------
QVariant DiagramItemFactory::defaultPropertyValue(const QString& key) const
{
	QVariant value;
	if (mDefaultProperties.contains(key)) value = mDefaultProperties[key];
	return value;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemFactory::registerItem(DiagramItem* item)
{
	if (item) mItems.append(item);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemFactory::clear()
{
	while (!mItems.isEmpty()) delete mItems.takeFirst();
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemFactory::isEmpty() const
{
	return mItems.isEmpty();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramItemFactory::create(const QString& key) const
{
	DiagramItem* item = NULL;

	for(QList<DiagramItem*>::ConstIterator itemIter = mItems.begin();
		item == NULL && itemIter != mItems.end(); itemIter++)
	{
		if ((*itemIter)->uniqueKey() == key) item = (*itemIter)->copy();
	}

	if (item)
	{
		for(QHash<QString, QVariant>::ConstIterator pIter = mDefaultProperties.begin();
			pIter != mDefaultProperties.end(); pIter++)
		{
			if (item->containsProperty(pIter.key()))
				item->setPropertyValue(pIter.key(), pIter.value());
		}
	}

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItemGroup* DiagramItemFactory::createGroup() const
{
	DiagramItemGroup* itemGroup = NULL;
	DiagramItem* item = NULL;

	for(QList<DiagramItem*>::ConstIterator itemIter = mItems.begin();
		itemGroup == NULL && itemIter != mItems.end(); itemIter++)
	{
		itemGroup = dynamic_cast<DiagramItemGroup*>(*itemIter);
		if (itemGroup)
		{
			item = itemGroup->copy();
			itemGroup = dynamic_cast<DiagramItemGroup*>(item);

			if (itemGroup == NULL) delete item;
		}
	}

	if (itemGroup)
	{
		for(QHash<QString, QVariant>::ConstIterator pIter = mDefaultProperties.begin();
			pIter != mDefaultProperties.end(); pIter++)
		{
			if (itemGroup->containsProperty(pIter.key()))
				itemGroup->setPropertyValue(pIter.key(), pIter.value());
		}
	}

	if (itemGroup == NULL) itemGroup = new DiagramItemGroup();
	return itemGroup;
}
//--------------------------------------------------------------------------------------------------
