/* DiagramMultipleItemProperties.cpp
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

#include <DiagramMultipleItemProperties.h>
#include <Diagram.h>

DiagramMultipleItemPropertiesDialog::DiagramMultipleItemPropertiesDialog(QWidget* parent, Flags flags) :
	DiagramTwoButtonDialog(parent)
{
	DiagramPenGroup::Capabilities penGroupCapabilities = DiagramPenGroup::ShowAll;
	DiagramBrushGroup::Capabilities brushGroupCapabilities = DiagramBrushGroup::ShowColor;

	QSizeF diagramSize;
	Diagram* diagram = qobject_cast<Diagram*>(parent);
	if (diagram) diagramSize = QSizeF(diagram->width(), diagram->height());

	mShouldExec = true;
	mFlags = flags;

	if (mFlags & ItemsUsePalette)
	{
		penGroupCapabilities = DiagramPenGroup::ShowWidth | DiagramPenGroup::ShowStyle;
		brushGroupCapabilities = DiagramBrushGroup::ShowFilled;
	}

	penGroup = new DiagramPenGroup(penGroupCapabilities, diagramSize);
	brushGroup = new DiagramBrushGroup(brushGroupCapabilities);
	fontToolBar = new DiagramFontToolBar(DiagramFontToolBar::ShowFont, diagramSize);
	textBrushGroup = new DiagramBrushGroup(brushGroupCapabilities);
	startArrowGroup = new DiagramItemPointGroup("Start Arrow", DiagramItemPointGroup::ShowArrow, diagramSize);
	endArrowGroup = new DiagramItemPointGroup("End Arrow", DiagramItemPointGroup::ShowArrow, diagramSize);
	penGroup->setCheckable(true);
	brushGroup->setCheckable(true);
	textBrushGroup->setTitle("Text Brush");
	textBrushGroup->setCheckable(true);
	startArrowGroup->setCheckable(true);
	endArrowGroup->setCheckable(true);

	fontGroup = new QGroupBox("Font");
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(fontToolBar);
	fontGroup->setLayout(vLayout);
	fontGroup->setCheckable(true);

	addWidget(fontGroup);
	addWidget(textBrushGroup);
	addWidget(penGroup);
    addWidget(brushGroup);
	addWidget(startArrowGroup);
    addWidget(endArrowGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	setWindowTitle("Item Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramMultipleItemPropertiesDialog::~DiagramMultipleItemPropertiesDialog() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramMultipleItemPropertiesDialog::setProperties(
	const QList< QHash<QString, QVariant> >& propertiesList)
{
	QVariant propertyValue;

	mShouldExec = false;

	propertyValue = checkForProperty(propertiesList, "Pen", penGroup);
	if (propertyValue.isValid()) penGroup->setPen(propertyValue.value<QPen>());

	propertyValue = checkForProperty(propertiesList, "Brush", brushGroup);
	if (propertyValue.isValid()) brushGroup->setBrush(propertyValue.value<QBrush>());

	propertyValue = checkForProperty(propertiesList, "Font", fontGroup);
	if (propertyValue.isValid()) fontToolBar->setFont(propertyValue.value<QFont>());

	if (mFlags & ItemsUsePalette)
	{
		textBrushGroup->setVisible(false);
		textBrushGroup->setChecked(false);
	}
	else
	{
		propertyValue = checkForProperty(propertiesList, "Text Brush", textBrushGroup);
		if (propertyValue.isValid()) textBrushGroup->setBrush(propertyValue.value<QBrush>());
	}

	propertyValue = checkForProperty(propertiesList, "Start Arrow", startArrowGroup);
	if (propertyValue.isValid())
		startArrowGroup->setArrow(propertyValue.value<DiagramArrow>());

	propertyValue = checkForProperty(propertiesList, "End Arrow", endArrowGroup);
	if (propertyValue.isValid())
		endArrowGroup->setArrow(propertyValue.value<DiagramArrow>());
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramMultipleItemPropertiesDialog::properties() const
{
	QHash<QString, QVariant> properties;

	if (penGroup->isChecked()) properties["Pen"] = QVariant(penGroup->pen());
	if (brushGroup->isChecked()) properties["Brush"] = QVariant(brushGroup->brush());
	if (fontGroup->isChecked()) properties["Font"] = QVariant(fontToolBar->font());
	if (textBrushGroup->isChecked()) properties["Text Brush"] = QVariant(textBrushGroup->brush());
	if (startArrowGroup->isChecked())
	{
		QVariant arrow;
		arrow.setValue(startArrowGroup->arrow());
		properties["Start Arrow"] = arrow;
	}
	if (endArrowGroup->isChecked())
	{
		QVariant arrow;
		arrow.setValue(endArrowGroup->arrow());
		properties["End Arrow"] = arrow;
	}

	return properties;
}
//--------------------------------------------------------------------------------------------------
bool DiagramMultipleItemPropertiesDialog::shouldExec() const
{
	return mShouldExec;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramMultipleItemPropertiesDialog::checkForProperty(
	const QList< QHash<QString, QVariant> >& propertiesList, const QString& propertyName,
	QGroupBox* group, bool* allItemsHaveProperty, bool* allItemsMatchProperty)
{
	QVariant propertyValue;
	bool lAllItemsHaveProperty = true;
	bool lAllItemsMatchProperty = true;

	for(QList< QHash<QString, QVariant> >::ConstIterator iter = propertiesList.begin();
		lAllItemsHaveProperty && iter != propertiesList.end(); iter++)
	{
		lAllItemsHaveProperty = (*iter).contains(propertyName);
		if (lAllItemsHaveProperty)
		{
			if (!propertyValue.isValid()) propertyValue = (*iter)[propertyName];
			else lAllItemsMatchProperty = (propertyValue == (*iter)[propertyName]);
		}
	}

	group->setVisible(lAllItemsHaveProperty);
	group->setChecked(lAllItemsHaveProperty && lAllItemsMatchProperty);
	mShouldExec = (mShouldExec || lAllItemsHaveProperty);

	if (allItemsHaveProperty) *allItemsHaveProperty = lAllItemsHaveProperty;
	if (allItemsMatchProperty) *allItemsMatchProperty = lAllItemsMatchProperty;
	return propertyValue;
}
//--------------------------------------------------------------------------------------------------
