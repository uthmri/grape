/* DiagramToolsWidget.cpp
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


#include <DiagramToolsWidget.h>
#include <Diagram.h>
#include <DiagramItem.h>

DiagramToolsWidget::DiagramToolsWidget() : QFrame()
{
	QVBoxLayout* vLayout;
    QFrame* hSeparator;

	mModeActionGroup = new QActionGroup(this);
	connect(mModeActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(setModeFromAction(QAction*)));

	hSeparator = new QFrame();
	hSeparator->setFrameStyle(QFrame::HLine | QFrame::Sunken);

	vLayout = new QVBoxLayout();
	vLayout->addWidget(createMainTools());
    vLayout->addWidget(hSeparator);
    vLayout->addWidget(createPipelineTools());
    vLayout->addWidget(hSeparator);
    vLayout->addWidget(hSeparator);
    vLayout->addWidget(hSeparator);
    //vLayout->addWidget(createLibraryTools(), 100);
	vLayout->setContentsMargins(2, 2, 2, 2);
    setLayout(vLayout);
}
//--------------------------------------------------------------------------------------------------
DiagramToolsWidget::~DiagramToolsWidget() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramToolsWidget::addLibrary(const QString& name, const QList<DiagramItem*>& items)
{
	QFrame* libraryFrame = new QFrame();
	QGridLayout* libraryLayout = new QGridLayout();
	QToolButton* toolButton;
    DiagramItem* item;

    for(int i = 0; i < items.size(); i++)
	{
		item = items[i];

		toolButton = new QToolButton();
//        toolButton->setDefaultAction(addModeAction("Place " + item->uniqueKey(), item->iconPath(), "", item->uniqueKey()));
        toolButton->setDefaultAction(addModeAction(item->uniqueKey(), item->iconPath(), "", item->uniqueKey()));
        toolButton->setMinimumSize(QSize(26*4, 26));
//        libraryLayout->addWidget(toolButton, i / 4, i % 4);
        libraryLayout->addWidget(toolButton, i, 0, 1, 1);
    }

    libraryLayout->setSpacing(0);
    libraryLayout->setContentsMargins(2, 2, 2, 2);
//    libraryLayout->addWidget(new QWidget(), items.size() / 4 + 1, 0, 1, 4);
    libraryLayout->addWidget(new QWidget(), items.size(), 0, 1, 1);
//	libraryLayout->setRowStretch(items.size() / 4 + 1, 100);
	libraryFrame->setLayout(libraryLayout);

	mLibraryToolsCombo->addItem(name);
	mLibraryToolsWidget->addWidget(libraryFrame);
	mLibraryToolsWidget->adjustSize();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramToolsWidget::resetMode()
{
	QAction* selectAction = actions()[SelectModeAction];
	if (!selectAction->isChecked()) selectAction->setChecked(true);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramToolsWidget::setModeFromAction(QAction* action)
{
	if (action)
	{
		if (action->text() == "Scroll Mode") emit scrollModeClicked();
		else if (action->text() == "Zoom Mode") emit zoomModeClicked();
        else if (action->text() == "Select Mode") emit defaultModeClicked();
        else if (action->text() == "Run pipeline") emit runModeClicked();
        else if (action->text() == "Stop pipeline") emit stopModeClicked();
        else emit newModeClicked(action->data().toString());
	}
	else emit defaultModeClicked();
}
//--------------------------------------------------------------------------------------------------
void DiagramToolsWidget::properties()
{
	emit propertiesClicked();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QWidget* DiagramToolsWidget::createMainTools()
{
	QFrame* mainToolsFrame;
	QGridLayout* mainToolsLayout;
	QList<QAction*> actions;
	QToolButton* toolButton;

	addModeAction("Select Mode", ":/icons/oxygen/edit-select.png", "Escape");
	addModeAction("Scroll Mode", ":/icons/oxygen/transform-move.png");
    addModeAction("Zoom Mode", ":/icons/oxygen/page-zoom.png");
	addAction("Properties...", this, SLOT(properties()), ":/icons/oxygen/games-config-board.png");
	addModeAction("Place Line", ":/icons/oxygen/draw-line.png", "", "line");
	addModeAction("Place Arc", ":/icons/oxygen/draw-arc.png", "", "arc");
   // addModeAction("Place Polyline", ":/icons/oxygen/draw-polyline.png", "", "polyline");
   // addModeAction("Place Curve", ":/icons/oxygen/draw-curve.png", "", "curve");
   // addModeAction("Place Box", ":/icons/oxygen/draw-rectangle.png", "", "box");
    //addModeAction("Place Ellipse", ":/icons/oxygen/draw-ellipse.png", "", "ellipse");
    //addModeAction("Place Polygon", ":/icons/oxygen/draw-polygon.png", "", "polygon");
	addModeAction("Place Text", ":/icons/oxygen/draw-text.png", "", "text");
//    addModeAction("Run pipeline", ":/icons/oxygen/run4.png", "", "Run pipeline");

//    addModeAction("Run pipeline", ":/icons/oxygen/merge.png", "", "Run pipeline");
//    addModeAction("Pause pipeline", ":/icons/oxygen/Pause6.png", "", "Pause pipeline");
//    addModeAction("Stop pipeline", ":/icons/oxygen/application-exit.png", "", "Stop pipeline");

    mainToolsLayout = new QGridLayout();
	actions = QFrame::actions();
	actions[SelectModeAction]->setChecked(true);
	for(int toolIndex = 0; toolIndex < actions.size(); toolIndex++)
	{
		toolButton = new QToolButton();
		toolButton->setDefaultAction(actions[toolIndex]);
		toolButton->setMinimumSize(QSize(32, 32));
		mainToolsLayout->addWidget(toolButton, toolIndex / 4, toolIndex % 4);
	}

	mainToolsFrame = new QFrame();
	mainToolsFrame->setLayout(mainToolsLayout);
	mainToolsLayout->setSpacing(0);
	mainToolsLayout->setContentsMargins(0, 0, 0, 0);

	return mainToolsFrame;
}
//--------------------------------------------------------------------------------------------------
QWidget* DiagramToolsWidget::createPipelineTools()
{
    QFrame* mainToolsFrame;
    QGridLayout* mainToolsLayout;
    QList<QAction*> actions;
    QToolButton* toolButton;

    addModeAction("Run pipeline", ":/icons/oxygen/merge.png", "", "Run pipeline");
    addModeAction("Stop pipeline", ":/icons/oxygen/application-exit.png", "", "Stop pipeline");
    mainToolsLayout = new QGridLayout();
    actions = QFrame::actions();
    actions[SelectModeAction]->setChecked(true);
    int t;
    for(int toolIndex = actions.size()-2; toolIndex < actions.size(); toolIndex++)
    {
        t = toolIndex - (actions.size()-2);
        toolButton = new QToolButton();
        toolButton->setDefaultAction(actions[toolIndex]);
//        toolButton->setMinimumSize(QSize(32, 32));
        toolButton->setMinimumSize(QSize(32*4, 32));
//        mainToolsLayout->addWidget(toolButton, t / 4, t % 4);
        mainToolsLayout->addWidget(toolButton, t / 1, 0 * t % 4, 1, 5);
    }

    mainToolsFrame = new QFrame();

    // add a status box
    if(0)
    {
        QGridLayout* statusPipelineLayout = new QGridLayout();
        QLineEdit* pipelineEdit = new QLineEdit();
        statusPipelineLayout->addWidget(pipelineEdit, 1, 0, Qt::AlignJustify);;
        mainToolsLayout->addLayout(statusPipelineLayout, 1, 0, 1, 4, Qt::AlignJustify);
    }

    mainToolsFrame->setLayout(mainToolsLayout);
    mainToolsLayout->setSpacing(0);
    mainToolsLayout->setContentsMargins(0, 0, 0, 0);

    return mainToolsFrame;
}
//--------------------------------------------------------------------------------------------------
QWidget* DiagramToolsWidget::createLibraryTools()
{
	QFrame* libraryToolsFrame;
	QScrollArea* libraryGroupArea;
	QVBoxLayout* libraryLayout;

	mLibraryToolsCombo = new QComboBox();
	mLibraryToolsWidget = new QStackedWidget();
	connect(mLibraryToolsCombo, SIGNAL(currentIndexChanged(int)),
		mLibraryToolsWidget, SLOT(setCurrentIndex(int)));

	libraryGroupArea = new QScrollArea();
	libraryGroupArea->setWidget(mLibraryToolsWidget);
	libraryGroupArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	libraryGroupArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	libraryLayout = new QVBoxLayout();
	libraryLayout->addWidget(mLibraryToolsCombo);
	libraryLayout->addWidget(libraryGroupArea, 100);
	libraryLayout->setSpacing(2);
	libraryLayout->setContentsMargins(2, 2, 2, 2);

	libraryToolsFrame = new QFrame();
	libraryToolsFrame->setLayout(libraryLayout);
	mLibraryToolsWidget->adjustSize();

	return libraryToolsFrame;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QAction* DiagramToolsWidget::addAction(const QString& text, QObject* slotObj,
	const char* slotFunction, const QString& iconPath, const QString& shortcut)
{
	QAction* action = new QAction(text, this);
	connect(action, SIGNAL(triggered()), slotObj, slotFunction);

	if (!iconPath.isEmpty()) action->setIcon(QIcon(iconPath));
	if (!shortcut.isEmpty()) action->setShortcut(QKeySequence(shortcut));

	QFrame::addAction(action);
	return action;
}
//--------------------------------------------------------------------------------------------------
QAction* DiagramToolsWidget::addModeAction(const QString& text, const QString& iconPath,
	const QString& shortcut, const QVariant& data)
{
	QAction* action = new QAction(text, this);

	if (!iconPath.isEmpty()) action->setIcon(QIcon(iconPath));
	if (!shortcut.isEmpty()) action->setShortcut(QKeySequence(shortcut));
	action->setData(data);

	action->setCheckable(true);
	action->setActionGroup(mModeActionGroup);

	QFrame::addAction(action);
	return action;
}
//--------------------------------------------------------------------------------------------------
