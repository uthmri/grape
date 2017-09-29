/* DiagramMultipleItemProperties.h
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

#ifndef DIAGRAMMULTIPLEITEMPROPERTIES_H
#define DIAGRAMMULTIPLEITEMPROPERTIES_H

#include <DiagramWidgets.h>

class DiagramMultipleItemPropertiesDialog : public DiagramTwoButtonDialog
{
	Q_OBJECT

public:
	enum Flag { NoFlags = 0, ItemsUsePalette = 0x01 };
	Q_DECLARE_FLAGS(Flags, Flag)

protected:
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;
	QGroupBox* fontGroup;
	DiagramBrushGroup* textBrushGroup;
	DiagramFontToolBar* fontToolBar;
	DiagramItemPointGroup* startArrowGroup;
	DiagramItemPointGroup* endArrowGroup;

	Flags mFlags;
	bool mShouldExec;

public:
	DiagramMultipleItemPropertiesDialog(QWidget* parent = NULL, Flags flags = NoFlags);
	~DiagramMultipleItemPropertiesDialog();

	void setProperties(const QList< QHash<QString, QVariant> >& propertiesList);
	QHash<QString, QVariant> properties() const;
	bool shouldExec() const;

private:
	QVariant checkForProperty(const QList< QHash<QString, QVariant> >& propertiesList,
		const QString& propertyName, QGroupBox* group,
		bool* allItemsHaveProperty = NULL, bool* allItemsMatchProperty = NULL);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramMultipleItemPropertiesDialog::Flags)

#endif
