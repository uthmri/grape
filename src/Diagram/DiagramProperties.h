/* DiagramProperties.h
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

#ifndef DIAGRAMPROPERTIES_H
#define DIAGRAMPROPERTIES_H

#include <DiagramTypes.h>
#include <DiagramBase.h>

class DiagramColorWidget;

class DiagramPropertiesWidget : public QFrame
{
	Q_OBJECT

private:
	QComboBox* unitsCombo;
	QLineEdit* widthEdit;
	QLineEdit* heightEdit;
	DiagramColorWidget* backgroundColorWidget;

	QLineEdit* gridEdit;
	QComboBox* gridStyleCombo;
	QLineEdit* gridSpacingMajorEdit;
	QLineEdit* gridSpacingMinorEdit;
	DiagramColorWidget* gridColorWidget;

	QLineEdit* borderWidthEdit;
	DiagramColorWidget* borderColorWidget;

	DiagramUnits mPreviousUnits;

public:
	DiagramPropertiesWidget();
	~DiagramPropertiesWidget();

	void setUnits(DiagramUnits units);
	void setSize(qreal width, qreal height);
	void setBackground(const QBrush& brush);
	void setBorder(qreal width, const QBrush& brush);
	void setGrid(qreal grid, DiagramBase::GridStyle style, int spacingMajor,
		int spacingMinor, const QBrush& brush);
	DiagramUnits units() const;
	qreal width() const;
	qreal height() const;
	QBrush backgroundBrush() const;
	qreal borderWidth() const;
	QBrush borderBrush() const;
	qreal grid() const;
	DiagramBase::GridStyle gridStyle() const;
	int majorGridSpacing() const;
	int minorGridSpacing() const;
	QBrush gridBrush() const;

private slots:
	void unitsChanged(int index);
	void gridStyleChanged();

private:
	QGroupBox* createDiagramGroup();
	QGroupBox* createGridGroup();
	QGroupBox* createBorderGroup();
};

#endif
