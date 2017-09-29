/* PreferencesDialog.h
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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <DiagramTypes.h>
#include <DiagramWidgets.h>
#include <DiagramBase.h>

class DiagramPropertiesWidget;

class PreferencesDialog : public DiagramTwoButtonDialog
{
	Q_OBJECT

private:
	QListWidget* listWidget;
	QStackedWidget* stackedWidget;

	QCheckBox* promptOverwriteCheck;
	QCheckBox* promptCloseUnsavedCheck;
	DiagramPropertiesWidget* diagramPropertiesWidget;

private:
	void setupGeneralFrame();
	void setupDiagramDefaultsFrame();

public:
	PreferencesDialog(QWidget* parent = NULL);
	~PreferencesDialog();

	void setPrompts(bool closingUnsaved, bool overwrite);
	bool shouldPromptOnClosingUnsaved() const;
	bool shouldPromptOnOverwrite() const;

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
};

#endif
