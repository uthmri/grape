/* PreferencesDialog.cpp
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

#include <PreferencesDialog.h>
#include <DiagramProperties.h>

PreferencesDialog::PreferencesDialog(QWidget* parent) : DiagramTwoButtonDialog(parent)
{
	listWidget = new QListWidget();
	stackedWidget = new QStackedWidget();

	listWidget->setIconSize(QSize(32, 32));
	listWidget->setGridSize(QSize(108, 64));
	listWidget->setViewMode(QListView::IconMode);
	listWidget->setMovement(QListView::Static);
	listWidget->setMaximumWidth(120);
	listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	connect(listWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

	setupGeneralFrame();
	setupDiagramDefaultsFrame();

	QWidget *widget = new QWidget();
	QHBoxLayout *hLayout = new QHBoxLayout();
	hLayout->addWidget(listWidget);
	hLayout->addWidget(stackedWidget, 100);
	hLayout->setSpacing(16);
	widget->setLayout(hLayout);

    addWidget(widget, 100);
    addButtonBox();

	setWindowTitle("Preferences");
	resize(500, 500);
}
//--------------------------------------------------------------------------------------------------
PreferencesDialog::~PreferencesDialog() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setPrompts(bool closingUnsaved, bool overwrite)
{
	promptCloseUnsavedCheck->setChecked(closingUnsaved);
	promptOverwriteCheck->setChecked(overwrite);
}
//--------------------------------------------------------------------------------------------------
bool PreferencesDialog::shouldPromptOnClosingUnsaved() const
{
	return promptCloseUnsavedCheck->isChecked();
}
//--------------------------------------------------------------------------------------------------
bool PreferencesDialog::shouldPromptOnOverwrite() const
{
	return promptOverwriteCheck->isChecked();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setUnits(DiagramUnits units)
{
	diagramPropertiesWidget->setUnits(units);
}
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setSize(qreal width, qreal height)
{
	diagramPropertiesWidget->setSize(width, height);
}
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setBackground(const QBrush& brush)
{
	diagramPropertiesWidget->setBackground(brush);
}
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setBorder(qreal width, const QBrush& brush)
{
	diagramPropertiesWidget->setBorder(width, brush);
}
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setGrid(qreal grid, DiagramBase::GridStyle style, int spacingMajor,
	int spacingMinor, const QBrush& brush)
{
	diagramPropertiesWidget->setGrid(grid, style, spacingMajor, spacingMinor, brush);
}
//--------------------------------------------------------------------------------------------------
DiagramUnits PreferencesDialog::units() const
{
	return diagramPropertiesWidget->units();
}
//--------------------------------------------------------------------------------------------------
qreal PreferencesDialog::width() const
{
	return diagramPropertiesWidget->width();
}
//--------------------------------------------------------------------------------------------------
qreal PreferencesDialog::height() const
{
	return diagramPropertiesWidget->height();
}
//--------------------------------------------------------------------------------------------------
QBrush PreferencesDialog::backgroundBrush() const
{
	return diagramPropertiesWidget->backgroundBrush();
}
//--------------------------------------------------------------------------------------------------
qreal PreferencesDialog::borderWidth() const
{
	return diagramPropertiesWidget->borderWidth();
}
//--------------------------------------------------------------------------------------------------
QBrush PreferencesDialog::borderBrush() const
{
	return diagramPropertiesWidget->borderBrush();
}
//--------------------------------------------------------------------------------------------------
qreal PreferencesDialog::grid() const
{
	return diagramPropertiesWidget->grid();
}
//--------------------------------------------------------------------------------------------------
DiagramBase::GridStyle PreferencesDialog::gridStyle() const
{
	return diagramPropertiesWidget->gridStyle();
}
//--------------------------------------------------------------------------------------------------
int PreferencesDialog::majorGridSpacing() const
{
	return diagramPropertiesWidget->majorGridSpacing();
}
//--------------------------------------------------------------------------------------------------
int PreferencesDialog::minorGridSpacing() const
{
	return diagramPropertiesWidget->minorGridSpacing();
}
//--------------------------------------------------------------------------------------------------
QBrush PreferencesDialog::gridBrush() const
{
	return diagramPropertiesWidget->gridBrush();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setupGeneralFrame()
{
	promptOverwriteCheck = new QCheckBox("Prompt when overwriting existing files");
	promptCloseUnsavedCheck = new QCheckBox("Prompt when closing unsaved files");

	QGroupBox* promptGroup = new QGroupBox("Warnings");
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(promptOverwriteCheck);
	vLayout->addWidget(promptCloseUnsavedCheck);
	promptGroup->setLayout(vLayout);

	QFrame* generalFrame = new QFrame();
	vLayout = new QVBoxLayout();
	vLayout->addWidget(promptGroup);
	vLayout->addWidget(new QWidget(), 100);
	vLayout->setContentsMargins(0, 0, 0, 0);
	generalFrame->setLayout(vLayout);

	QListWidgetItem *item = new QListWidgetItem("General");
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	item->setTextAlignment(Qt::AlignCenter);
	item->setIcon(QIcon(":/icons/oxygen/configure.png"));
	listWidget->addItem(item);
	stackedWidget->addWidget(generalFrame);
}
//--------------------------------------------------------------------------------------------------
void PreferencesDialog::setupDiagramDefaultsFrame()
{
	diagramPropertiesWidget = new DiagramPropertiesWidget();

	QFrame* diagramDefaultsFrame = new QFrame();
	QVBoxLayout *vLayout = new QVBoxLayout();
	vLayout->addWidget(diagramPropertiesWidget);
	vLayout->addWidget(new QWidget(), 100);
	vLayout->setContentsMargins(0, 0, 0, 0);
	diagramPropertiesWidget->layout()->setContentsMargins(0, 0, 0, 0);
	diagramDefaultsFrame->setLayout(vLayout);

	QListWidgetItem *item = new QListWidgetItem("Diagram Defaults");
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	item->setTextAlignment(Qt::AlignCenter);
	item->setIcon(QIcon(":/icons/custom/diagram.png"));
	listWidget->addItem(item);
	stackedWidget->addWidget(diagramDefaultsFrame);
}
//--------------------------------------------------------------------------------------------------
