/* DiagramItemFactory.h
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

#ifndef DIAGRAMITEMFACTORY_H
#define DIAGRAMITEMFACTORY_H

#include <QtCore>

class DiagramItem;
class DiagramItemGroup;

class DiagramItemFactory : public QObject
{
	Q_OBJECT

private:
	QList<DiagramItem*> mItems;
	QHash<QString, QVariant> mDefaultProperties;

public:
	DiagramItemFactory();
	~DiagramItemFactory();

	QHash<QString, QVariant> defaultProperties() const;
	bool containsDefaultProperty(const QString& key) const;
	QVariant defaultPropertyValue(const QString& key) const;

public slots:
	void setDefaultProperties(const QHash<QString, QVariant>& defaultProperties);

public:
	void registerItem(DiagramItem* item);
	void clear();
	bool isEmpty() const;

	DiagramItem* create(const QString& key) const;
	DiagramItemGroup* createGroup() const;
};

#endif
