/* MainWindow.h
 *
 * Copyright (C) 2016 UTHealth MRI Research
 *
 * This file is part of the GRAPE Diagram Editor.
 *
 * GRAPE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GRAPE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GRAPE.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Diagram.h>
#include "nodetreewidget.h"
#include <QDomDocument>

class DiagramManager;
class DiagramToolsWidget;
class DiagramItemDefaultsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum ActionIndex { NewAction, OpenAction, SaveAction, SaveAsAction, CloseAction,
        ExportPngAction, ExportSvgAction, PrintPreviewAction, PrintSetupAction,
        PrintAction, PrintPdfAction, PreferencesAction, ExitAction,
        AboutAction, AboutQtAction,
        SelectModeAction, ScrollModeAction, ZoomModeAction,
        PlaceLineAction, PlaceArcAction, PlacePolylineAction, PlaceCurveAction,
        PlaceBoxAction, PlaceEllipseAction, PlacePolygonAction, PlaceTextAction, NumberOfActions };

private:
    Diagram* mDiagram;

    NodeTreeWidget* mToolsTree;

    QHash <QString, NodeInterface> mNodeInterface;

    DiagramManager* mManager;
    DiagramToolsWidget* mToolsWidget;
    DiagramItemDefaultsWidget* mItemDefaultsWidget;

    QToolBar* mFileToolBar;
    QToolBar* mDiagramToolBar;
    QComboBox* mDiagramZoomCombo;

    QLabel* mModeLabel;
    QLabel* mStatusLabel;
    QLabel* mNumberOfItemsLabel;
    QLabel* mMouseLabel;

    QByteArray mWindowState;

    DiagramUnits mDefaultUnits;
    qreal mDefaultWidth;
    qreal mDefaultHeight;
    QBrush mDefaultBackgroundBrush;
    qreal mDefaultBorderWidth;
    QBrush mDefaultBorderBrush;
    qreal mDefaultGridSize;
    Diagram::GridStyle mDefaultGridStyle;
    int mDefaultGridMajorSpacing;
    int mDefaultGridMinorSpacing;
    QBrush mDefaultGridBrush;

public:
    MainWindow(const QString& filePath = QString(), const bool isRun = false, const bool isInteractive = true);
    ~MainWindow();

    void savePreferences();
    void loadPreferences();

    int loadNodeLib();
    void ParseNodeDef(QDomNode node, QTreeWidgetItem *ParentLib);
    Diagram *diagram() const;
    void setDiagram(Diagram *diagram);

public slots:
    void preferences();
    void about();
    // QString qStdOut();
    void quit();

    void printText(const QString &s);
private slots:
    void updateWindowTitle(const QString& fileName);
    void diagramVisibilityChanged(bool visible);
    void clearDiagram();

    void updateDiagramZoom(const QString& zoomText);
    void updateDiagramZoomCombo(qreal zoomLevel);

private:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    void closeEvent(QCloseEvent* event);

    QLayout* createToolsWidgets();
    void createStatusBar();

    void createActions();
    void createMenus();
    void createToolBars();
    QAction* addAction(const QString& text, QObject* slotObj, const char* slotFunction,
        const QString& iconPath = QString(), const QString& shortcut = QString());

    void loadLibraryItems();
    void addLibrary(const QString& name, const QList<DiagramItem*>& items);
};

#endif
