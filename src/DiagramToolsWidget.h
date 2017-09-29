/* DiagramToolsWidget.h
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


#ifndef DIAGRAMTOOLSWIDGET_H
#define DIAGRAMTOOLSWIDGET_H

#include <QtCore>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui>
#else
#include <QtWidgets>
#endif


class Diagram;
class DiagramItem;

class DiagramToolsWidget : public QFrame
{
	Q_OBJECT

public:
	enum ActionIndex { SelectModeAction, ScrollModeAction, ZoomModeAction, PropertiesAction,
		PlaceLineAction, PlaceArcAction, PlacePolylineAction, PlaceCurveAction,
        PlaceBoxAction, PlaceEllipseAction, PlacePolygonAction, PlaceTextAction, NumberOfActions };

private:
	QComboBox* mLibraryToolsCombo;
	QStackedWidget* mLibraryToolsWidget;

	Diagram* mDiagram;
	QActionGroup* mModeActionGroup;

public:
	DiagramToolsWidget();
	~DiagramToolsWidget();

    void addLibrary(const QString& name, const QList<DiagramItem*>& items);

public slots:
	void resetMode();

signals:
	void defaultModeClicked();
	void scrollModeClicked();
    void zoomModeClicked();
    void runModeClicked();
    void stopModeClicked();
    void newModeClicked(const QString& key);
	void propertiesClicked();

private slots:
	void setModeFromAction(QAction* action);
	void properties();

private:
    QWidget* createMainTools();
    QWidget* createPipelineTools();
    QWidget* createLibraryTools();

	QAction* addAction(const QString& text, QObject* slotObj, const char* slotFunction,
		const QString& iconPath = QString(), const QString& shortcut = QString());
	QAction* addModeAction(const QString& text, const QString& iconPath = QString(),
		const QString& shortcut = QString(), const QVariant& data = QVariant());

	//void registerItems();
    //void registerItem(DiagramItem* item, const QString& category, int row, int column, const QString& iconPath);
};

#endif
