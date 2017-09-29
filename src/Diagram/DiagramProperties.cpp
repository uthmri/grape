/* DiagramProperties.cpp
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

#include <DiagramProperties.h>
#include <DiagramItem.h>

DiagramPropertiesWidget::DiagramPropertiesWidget() : QFrame()
{
	mPreviousUnits = UnitsMils;

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(createDiagramGroup());
	vLayout->addWidget(createGridGroup());
	vLayout->addWidget(createBorderGroup());
	setLayout(vLayout);

	connect(unitsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(unitsChanged(int)));
	connect(gridStyleCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(gridStyleChanged()));
}
//--------------------------------------------------------------------------------------------------
DiagramPropertiesWidget::~DiagramPropertiesWidget() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesWidget::setUnits(DiagramUnits units)
{
	unitsCombo->setCurrentIndex((int)units);
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesWidget::setSize(qreal width, qreal height)
{
	widthEdit->setText(QString::number(width));
	heightEdit->setText(QString::number(height));
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesWidget::setBackground(const QBrush& brush)
{
	backgroundColorWidget->setCurrentColor(brush.color());
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesWidget::setBorder(qreal width, const QBrush& brush)
{
	borderWidthEdit->setText(QString::number(width));
	borderColorWidget->setCurrentColor(brush.color());
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesWidget::setGrid(qreal grid, DiagramBase::GridStyle style,
	int spacingMajor, int spacingMinor, const QBrush& brush)
{
	gridEdit->setText(QString::number(grid));
	gridStyleCombo->setCurrentIndex((qint32)style);
	gridSpacingMajorEdit->setText(QString::number(spacingMajor));
	gridSpacingMinorEdit->setText(QString::number(spacingMinor));
	gridColorWidget->setCurrentColor(brush.color());
}
//--------------------------------------------------------------------------------------------------
DiagramUnits DiagramPropertiesWidget::units() const
{
	return (DiagramUnits)unitsCombo->currentIndex();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPropertiesWidget::width() const
{
	return widthEdit->text().toDouble();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPropertiesWidget::height() const
{
	return heightEdit->text().toDouble();
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramPropertiesWidget::backgroundBrush() const
{
	return QBrush(backgroundColorWidget->currentColor());
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPropertiesWidget::borderWidth() const
{
	return borderWidthEdit->text().toDouble();
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramPropertiesWidget::borderBrush() const
{
	return QBrush(borderColorWidget->currentColor());
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPropertiesWidget::grid() const
{
	return gridEdit->text().toDouble();
}
//--------------------------------------------------------------------------------------------------
DiagramBase::GridStyle DiagramPropertiesWidget::gridStyle() const
{
	return (DiagramBase::GridStyle)gridStyleCombo->currentIndex();
}
//--------------------------------------------------------------------------------------------------
int DiagramPropertiesWidget::majorGridSpacing() const
{
	return gridSpacingMajorEdit->text().toInt();
}
//--------------------------------------------------------------------------------------------------
int DiagramPropertiesWidget::minorGridSpacing() const
{
	return gridSpacingMinorEdit->text().toInt();
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramPropertiesWidget::gridBrush() const
{
	return QBrush(gridColorWidget->currentColor());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesWidget::unitsChanged(int index)
{
	qreal scaleFactor = 1.0;
	DiagramUnits newUnits = units();

	Q_UNUSED(index);

	scaleFactor = unitsScale(mPreviousUnits, newUnits);

	setSize(width() * scaleFactor, height() * scaleFactor);
	setBorder(borderWidth() * scaleFactor, borderBrush());
	setGrid(grid() * scaleFactor, gridStyle(), majorGridSpacing(), minorGridSpacing(), gridBrush());

	mPreviousUnits = newUnits;
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesWidget::gridStyleChanged()
{
	qint32 index = gridStyleCombo->currentIndex();

	gridSpacingMajorEdit->setEnabled(index > 0);
	gridSpacingMinorEdit->setEnabled(index == 3);
	gridColorWidget->setEnabled(index > 0);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QGroupBox* DiagramPropertiesWidget::createDiagramGroup()
{
	unitsCombo = new QComboBox();
	widthEdit = new QLineEdit();
	heightEdit = new QLineEdit();
	backgroundColorWidget = new DiagramColorWidget();

    widthEdit->setValidator(new QDoubleValidator(0., 1000000., 10, this));
    heightEdit->setValidator(new QDoubleValidator(0., 1000000., 10, this));
	unitsCombo->addItem("Mils");
	unitsCombo->addItem("Millimeters");

	QGroupBox* diagramGroup = new QGroupBox("Diagram");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Units:", unitsCombo);
	fLayout->addRow("Width:", widthEdit);
	fLayout->addRow("Height:", heightEdit);
	fLayout->addRow("Background Color:", backgroundColorWidget);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(100);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	diagramGroup->setLayout(fLayout);

	return diagramGroup;
}
//--------------------------------------------------------------------------------------------------
QGroupBox* DiagramPropertiesWidget::createGridGroup()
{
	gridEdit = new QLineEdit();
	gridStyleCombo = new QComboBox();
	gridSpacingMajorEdit = new QLineEdit();
	gridSpacingMinorEdit = new QLineEdit();
	gridColorWidget = new DiagramColorWidget();

	gridStyleCombo->addItem("None");
	gridStyleCombo->addItem("Dots");
	gridStyleCombo->addItem("Lines");
	gridStyleCombo->addItem("Graph Paper");
    gridEdit->setValidator(new QDoubleValidator(0., 1000000., 10, this));
    gridSpacingMajorEdit->setValidator(new QIntValidator(1, INT_MAX, this));
    gridSpacingMinorEdit->setValidator(new QIntValidator(1, INT_MAX, this));

	gridSpacingMajorEdit->setEnabled(false);
	gridSpacingMinorEdit->setEnabled(false);
	gridColorWidget->setEnabled(false);

	QGroupBox* gridGroup = new QGroupBox("Grid");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Grid:", gridEdit);
	fLayout->addRow("Style:", gridStyleCombo);
	fLayout->addRow("Major Spacing:", gridSpacingMajorEdit);
	fLayout->addRow("Minor Spacing:", gridSpacingMinorEdit);
	fLayout->addRow("Color:", gridColorWidget);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(100);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	gridGroup->setLayout(fLayout);

	return gridGroup;
}
//--------------------------------------------------------------------------------------------------
QGroupBox* DiagramPropertiesWidget::createBorderGroup()
{
	borderWidthEdit = new QLineEdit();
	borderColorWidget = new DiagramColorWidget();

    borderWidthEdit->setValidator(new QDoubleValidator(0., 1000000., 10, this));

	QGroupBox* borderGroup = new QGroupBox("Border");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Width:", borderWidthEdit);
	fLayout->addRow("Color:", borderColorWidget);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(100);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	borderGroup->setLayout(fLayout);

	return borderGroup;
}
//--------------------------------------------------------------------------------------------------
