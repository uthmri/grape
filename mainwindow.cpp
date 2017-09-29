/* MainWindow.cpp
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

#include <MainWindow.h>
#include <AboutDialog.h>
#include <DiagramItemDefaultsWidget.h>
#include <DiagramManager.h>
#include <DiagramToolsWidget.h>
#include <PreferencesDialog.h>
#include <QtSvg>

#include <DiagramLineItem.h>
#include <DiagramBoxItem.h>
#include <DiagramArcItem.h>
#include <DiagramCurveItem.h>
#include <DiagramEllipseItem.h>
#include <DiagramPolylineItem.h>
#include <DiagramPolygonItem.h>
#include <DiagramTextItem.h>
#include <DiagramRoundItem.h>
#include <DiagramOvalItem.h>
#include <DiagramTwoPortItem.h>
#include <BETItem.h>
#include <Items/EndItem.h>
#include <LoopItem.h>
#include <SelectItem.h>
#include <SourceItem.h>
#include <SinkItem.h>
#include <Pipeline.h>
//#include <ChartBoxItem.h>
//#include <ChartEllipseItem.h>
//#include <ChartPlusItem.h>
//#include <ChartPolygonItem.h>
//#include <ChartSumItem.h>
//#include <ElectricItems.h>
#include <LogicItems.h>
#include <NodeItem.h>
#include <ImageRegistrationItem.h>
#include <ImageSegmentationItem.h>
#include <ImageProcessingItem.h>
#include <RelOperatorItem.h>
#include <ImageReaderItem.h>
#include <ImageStackItem.h>
#include <ParReaderItem.h>
#include <ParWriterItem.h>
#include <ImageSlicerItem.h>
#include <ImageReformatItem.h>
#include <ImageWriterItem.h>
#include <ImageNormalizerItem.h>
#include <ImageArithmeticItem.h>
#include <ImageStatisticsItem.h>
#include <LogOperationItem.h>
#include <SystemcallItem.h>
#include <DevelopItem.h>
#include <BiasCorrectionItem.h>
#include <CurveFittingItem.h>
#include <BranchItem.h>
#include <SynchItem.h>
#include <DividerItem.h>
#include <DelayItem.h>
#include <LoopItem.h>
#include <SelectItem.h>
#include <Nodes/bet.h>
#include <Nodes/end.h>
#include <Nodes/line.h>
#include <Nodes/arc.h>
#include <Nodes/imageregistration.h>
#include <Nodes/imagesegmentation.h>
#include <Nodes/imageprocessing.h>
#include <Nodes/reloperator.h>
#include <Nodes/imagereader.h>
#include <Nodes/imagestack.h>
#include <Nodes/parreader.h>
#include <Nodes/parwriter.h>
#include <Nodes/imageslicer.h>
#include "Nodes/imagereformat.h"
#include <Nodes/imagewriter.h>
#include <Nodes/imagenormalizer.h>
#include <Nodes/imagearithmetic.h>
#include <Nodes/imagestatistics.h>
#include <Nodes/logoperation.h>
#include <Nodes/Systemcall.h>
#include <Nodes/develop.h>
#include <Nodes/biascorrection.h>
#include <Nodes/curvefitting.h>
#include <Nodes/branch.h>
#include <Nodes/synch.h>
#include <Nodes/divider.h>
#include <Nodes/delay.h>
#include <Nodes/loop.h>
#include <Nodes/select.h>
#include <xmlreclib.h>
#include <Nodes/source.h>
#include <Nodes/Node.h>
#include <Nodes/sink.h>
#include "nodetreewidget.h"
#include "Nodes/nodeinterface.h"

//#define RELEASE_BUILD
#undef RELEASE_BUILD


Diagram *MainWindow::diagram() const
{
    return mDiagram;
}

void MainWindow::setDiagram(Diagram *diagram)
{
    mDiagram = diagram;
}
MainWindow::MainWindow(const QString& filePath, const bool isRun, const bool isInteractive) : QMainWindow()
{
    Diagram::itemFactory.registerItem(new DiagramLineItem());
    Diagram::itemFactory.registerItem(new DiagramBoxItem());
    Diagram::itemFactory.registerItem(new DiagramArcItem());
    Diagram::itemFactory.registerItem(new DiagramCurveItem());
    Diagram::itemFactory.registerItem(new DiagramEllipseItem());
    Diagram::itemFactory.registerItem(new DiagramPolylineItem());
    Diagram::itemFactory.registerItem(new DiagramPolygonItem());
    Diagram::itemFactory.registerItem(new DiagramTextItem());

    mDefaultUnits = UnitsMils;
    mDefaultWidth = 10000.0;
    mDefaultHeight = 7500.0;
    mDefaultBackgroundBrush = QBrush(Qt::white);
    mDefaultBorderWidth = 24.0;
    mDefaultBorderBrush = QBrush(Qt::black);
    mDefaultGridSize = 100.0;
    mDefaultGridStyle = Diagram::GridGraphPaper;
    mDefaultGridMajorSpacing = 8;
    mDefaultGridMinorSpacing = 2;
    mDefaultGridBrush = QBrush(QColor(0, 128, 128));

    mToolsTree = new NodeTreeWidget(this);
    mDiagram = new Diagram();
    mManager = new DiagramManager(mDiagram);

    mDiagram->mainPipeline.setDiagram(mDiagram);
    mDiagram->mainPipeline.setDiagramManager(mManager);
    mDiagram->mainPipeline.setGraphical(isInteractive);

    mDiagram->mainPipeline.writeToLog("GRAPE started...");

    mManager->setFileInfo("grp", "GRAPE Pipeline (*.grp)");

    connect(mManager, SIGNAL(diagramChanged(const QString&)), this, SLOT(updateWindowTitle(const QString&)));
    connect(mManager, SIGNAL(visibilityChanged(bool)), this, SLOT(diagramVisibilityChanged(bool)));
    connect(mManager, SIGNAL(diagramCleared()), this, SLOT(clearDiagram()));

    QFrame* mainFrame = new QFrame();
    QHBoxLayout* mainLayout = new QHBoxLayout();
    QLayout* AllToolsWidgetLayout = createToolsWidgets();

//    mainFrame->setLayout(mainLayout);
//    setCentralWidget(mainFrame);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    loadLibraryItems();     // also fills mNodeInterface

    mToolsTree->expandAll();

    mDiagram->mainPipeline.mNodeInterface = mNodeInterface;
    //mNodeInterface = mDiagram->mainPipeline.mNodeInterface;



    AllToolsWidgetLayout->addWidget(mToolsTree);
    //mainLayout->insertWidget(0, mToolsTree);

//    QLabel* mNodeHelpLabel = new QLabel("XXXXXXXXXXXXXX");
//    AllToolsWidgetLayout->addWidget(mNodeHelpLabel);

    mainLayout->addLayout(AllToolsWidgetLayout);
    mainLayout->addWidget(mManager, 100);

    mainFrame->setLayout(mainLayout);
    setCentralWidget(mainFrame);

    setWindowTitle("GRAPE");
    setWindowIcon(QIcon(":/icons/custom/diagram.png"));
    resize(1048, 760);

    loadPreferences();
    mManager->loadDiagram(filePath);
    if(isRun)
    {
        emit mToolsWidget->runModeClicked();
    }
}
//--------------------------------------------------------------------------------------------------
MainWindow::~MainWindow() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void MainWindow::savePreferences()
{
//#ifdef RELEASE_BUILD
//#ifdef WIN32
//    QString configPath("config.ini");
//#else
//    QString configPath(QDir::home().absoluteFilePath(".GRAPE/config.ini"));
//#endif
//#else
//    QString configPath("config.ini");
//#endif

    //    QString configPath = QCoreApplication::applicationFilePath() + "../config.ini";
        QString configPath = "../config.ini";

    QSettings settings(configPath, QSettings::IniFormat);

    settings.beginGroup("Window");
    settings.setValue("Geometry", saveGeometry());
    settings.setValue("State", saveState());
    settings.endGroup();

    settings.beginGroup("Prompts");
    settings.setValue("PromptOnClosingUnsaved", mManager->shouldPromptOnClosingUnsaved());
    settings.setValue("PromptOnOverwrite", mManager->shouldPromptOnOverwrite());
    settings.endGroup();

    settings.beginGroup("ItemDefaults");
    settings.setValue("Pen", penToString(mItemDefaultsWidget->pen()));
    settings.setValue("Brush", brushToString(mItemDefaultsWidget->brush()));
    settings.setValue("Font", fontToString(mItemDefaultsWidget->font()));
    settings.setValue("TextBrush", brushToString(mItemDefaultsWidget->textBrush()));
    settings.setValue("StartArrowStyle", (int)mItemDefaultsWidget->startArrow().style());
    settings.setValue("StartArrowSize", mItemDefaultsWidget->startArrow().size());
    settings.setValue("EndArrowStyle", (int)mItemDefaultsWidget->endArrow().style());
    settings.setValue("EndArrowSize", mItemDefaultsWidget->endArrow().size());
    settings.endGroup();

    settings.beginGroup("DiagramDefaults");
    settings.setValue("Units", (int)mDefaultUnits);
    settings.setValue("Width", mDefaultWidth);
    settings.setValue("Height", mDefaultHeight);
    settings.setValue("BackgroundColor", mDefaultBackgroundBrush.color());
    settings.setValue("BorderWidth", mDefaultBorderWidth);
    settings.setValue("BorderColor", mDefaultBorderBrush.color());
    settings.setValue("Grid", mDefaultGridSize);
    settings.setValue("GridStyle", (int)mDefaultGridStyle);
    settings.setValue("GridSpacingMajor", mDefaultGridMajorSpacing);
    settings.setValue("GridSpacingMinor", mDefaultGridMinorSpacing);
    settings.setValue("GridColor", mDefaultGridBrush.color());
    settings.endGroup();
}
//--------------------------------------------------------------------------------------------------
void MainWindow::loadPreferences()
{
//#ifdef RELEASE_BUILD
//#ifdef WIN32
//    QString configPath("config.ini");
//#else
//    QString configPath(QDir::home().absoluteFilePath(".GRAPE/config.ini"));
//#endif
//#else
//    QString configPath("config.ini");
//#endif

//    QString configPath = QCoreApplication::applicationFilePath() + "../config.ini";
    QString configPath = "../config.ini";

    QSettings settings(configPath, QSettings::IniFormat);

    if (!settings.allKeys().isEmpty())
    {
        settings.beginGroup("Window");
        restoreGeometry(settings.value("Geometry", QVariant()).toByteArray());
        restoreState(settings.value("State", QVariant()).toByteArray());
        settings.endGroup();

        settings.beginGroup("Prompts");
        mManager->setPromptOnClosingUnsaved(settings.value("PromptOnClosingUnsaved", QVariant(true)).toBool());
        mManager->setPromptOnOverwrite(settings.value("PromptOnOverwrite", QVariant(true)).toBool());
        settings.endGroup();

        settings.beginGroup("ItemDefaults");
        mItemDefaultsWidget->setPen(penFromString(settings.value("Pen", "").toString()));
        mItemDefaultsWidget->setBrush(brushFromString(settings.value("Brush", "").toString()));
        mItemDefaultsWidget->setFont(fontFromString(settings.value("Font", "").toString()));
        mItemDefaultsWidget->setTextBrush(brushFromString(settings.value("TextBrush", "").toString()));
        mItemDefaultsWidget->setStartArrow(DiagramArrow(
            (DiagramArrow::Style)settings.value("StartArrowStyle", DiagramArrow::None).toInt(),
            settings.value("StartArrowSize", 100.0).toReal()));
        mItemDefaultsWidget->setEndArrow(DiagramArrow(
            (DiagramArrow::Style)settings.value("EndArrowStyle", DiagramArrow::None).toInt(),
            settings.value("EndArrowSize", 100.0).toReal()));
        settings.endGroup();
        Diagram::itemFactory.setDefaultProperties(mItemDefaultsWidget->properties());

        settings.beginGroup("DiagramDefaults");
        mDefaultUnits = (DiagramUnits)settings.value("Units", QVariant(UnitsMils)).toInt();
        mDefaultWidth = settings.value("Width", QVariant(10000.0)).toDouble();
        mDefaultHeight = settings.value("Height", QVariant(7500.0)).toDouble();
        mDefaultBackgroundBrush = settings.value("BackgroundColor", QVariant(QColor(255, 255, 255, 255))).value<QColor>();
        mDefaultBorderWidth = settings.value("BorderWidth", QVariant(24.0)).toDouble();
        mDefaultBorderBrush = settings.value("BorderColor", QVariant(QColor(0, 0, 0, 255))).value<QColor>();
        mDefaultGridSize = settings.value("Grid", QVariant(100.0)).toDouble();
        mDefaultGridStyle = (Diagram::GridStyle)settings.value("GridStyle", QVariant(Diagram::GridGraphPaper)).toInt();
        mDefaultGridMajorSpacing = settings.value("GridSpacingMajor", QVariant(8)).toInt();
        mDefaultGridMinorSpacing = settings.value("GridSpacingMinor", QVariant(2)).toInt();
        mDefaultGridBrush = settings.value("GridColor", QVariant(QColor(0, 128, 128, 255))).value<QColor>();
        settings.endGroup();

        settings.beginGroup("ApplicationPaths");
        restoreGeometry(settings.value("Geometry", QVariant()).toByteArray());
        restoreState(settings.value("State", QVariant()).toByteArray());
        settings.endGroup();

        settings.beginGroup("ApplicationPaths");
        QString app_path;
        app_path = settings.value("BET",QVariant("")).toString();
        qDebug()<<"BET ="<<app_path ;
        diagram()->mainPipeline.setAppPath("BET",app_path);
        app_path = settings.value("ANTS",QVariant("")).toString();
        qDebug()<<"ANTS ="<<app_path ;
        diagram()->mainPipeline.setAppPath("ANTS",app_path);
        settings.endGroup();



    }
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void MainWindow::preferences()
{
    PreferencesDialog dialog(this);
    dialog.setPrompts(mManager->shouldPromptOnClosingUnsaved(), mManager->shouldPromptOnOverwrite());
    dialog.setUnits(mDefaultUnits);
    dialog.setSize(mDefaultWidth, mDefaultHeight);
    dialog.setBackground(mDefaultBackgroundBrush);
    dialog.setBorder(mDefaultBorderWidth, mDefaultBorderBrush);
    dialog.setGrid(mDefaultGridSize, mDefaultGridStyle, mDefaultGridMajorSpacing,
        mDefaultGridMinorSpacing, mDefaultGridBrush);

    if (dialog.exec() == QDialog::Accepted)
    {
        mManager->setPromptOnClosingUnsaved(dialog.shouldPromptOnClosingUnsaved());
        mManager->setPromptOnOverwrite(dialog.shouldPromptOnOverwrite());

        mDefaultUnits = dialog.units();
        mDefaultWidth = dialog.width();
        mDefaultHeight = dialog.height();
        mDefaultBackgroundBrush = dialog.backgroundBrush();
        mDefaultBorderWidth = dialog.borderWidth();
        mDefaultBorderBrush = dialog.borderBrush();
        mDefaultGridSize = dialog.grid();
        mDefaultGridStyle = dialog.gridStyle();
        mDefaultGridMajorSpacing = dialog.majorGridSpacing();
        mDefaultGridMinorSpacing = dialog.minorGridSpacing();
        mDefaultGridBrush = dialog.gridBrush();
    }
}
//--------------------------------------------------------------------------------------------------
void MainWindow::about()
{
    AboutDialog dialog(this);
    dialog.exec();
}
//--------------------------------------------------------------------------------------------------
//QTextStream& qStdOut()
//{
//    static QTextStream ts( stdout );
//    return ts;
//}
//--------------------------------------------------------------------------------------------------
void MainWindow::updateWindowTitle(const QString& fileName)
{
    setWindowTitle(fileName.isEmpty() ? "GRAPE" : fileName + " - GRAPE");
}
//--------------------------------------------------------------------------------------------------
void MainWindow::diagramVisibilityChanged(bool visible)
{
    QList<QAction*> lActions = actions();
    QList<QAction*> diagramActions = mDiagram->actions();

    while (!diagramActions.isEmpty())
        diagramActions.takeFirst()->setEnabled(visible);
    lActions[ExportPngAction]->setEnabled(visible);
    lActions[ExportSvgAction]->setEnabled(visible);
    lActions[PrintPreviewAction]->setEnabled(visible);
    lActions[PrintSetupAction]->setEnabled(visible);
    lActions[PrintAction]->setEnabled(visible);
    lActions[PrintPdfAction]->setEnabled(visible);
    mToolsWidget->setEnabled(visible);
    mItemDefaultsWidget->setEnabled(visible);
    mDiagramZoomCombo->setEnabled(visible);
}
//--------------------------------------------------------------------------------------------------
void MainWindow::clearDiagram()
{
    mDiagram->setUnits(mDefaultUnits);
    mDiagram->setSize(mDefaultWidth, mDefaultHeight);
    mDiagram->setBorderWidth(mDefaultBorderWidth);
    mDiagram->setGrid(mDefaultGridSize);
    mDiagram->setGridStyle(mDefaultGridStyle);
    mDiagram->setGridSpacing(mDefaultGridMajorSpacing, mDefaultGridMinorSpacing);
    mDiagram->painter()->setBrushAt(DiagramPainter::Background, mDefaultBackgroundBrush);
    mDiagram->painter()->setBrushAt(DiagramPainter::Border, mDefaultBorderBrush);
    mDiagram->painter()->setBrushAt(DiagramPainter::Grid, mDefaultGridBrush);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void MainWindow::updateDiagramZoom(const QString& zoomText)
{
    QString zoomTextStr = zoomText;
    QTextStream textStream(&zoomTextStr);
    qreal zoomLevel = 0;

    textStream >> zoomLevel;
    mDiagram->zoomToLevel(zoomLevel / 100.0);
}
//--------------------------------------------------------------------------------------------------
void MainWindow::updateDiagramZoomCombo(qreal zoomLevel)
{
    QString text = QString::number(zoomLevel * 100, 'f', 1) + "%";
    int textIndex = mDiagramZoomCombo->findText(text);

    if (0 <= textIndex && textIndex < mDiagramZoomCombo->count())
        mDiagramZoomCombo->setCurrentIndex(textIndex);
    else
        mDiagramZoomCombo->setEditText(text);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    if (!event->spontaneous()) mDiagram->zoomFit();
    else if (!mWindowState.isEmpty()) restoreState(mWindowState);
}
//--------------------------------------------------------------------------------------------------
void MainWindow::hideEvent(QHideEvent* event)
{
    QMainWindow::hideEvent(event);
    if (event->spontaneous()) mWindowState = saveState();
}
//--------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (mManager->closeDiagram())
    {
        savePreferences();
        event->accept();
    }
    else event->ignore();
}
//--------------------------------------------------------------------------------------------------
int MainWindow::loadNodeLib()
{
    bool fileError = false;
    QString path;
    QFile dataFile(path);

    fileError = !dataFile.open(QIODevice::ReadOnly);
    if (!fileError)
    {
        QXmlStreamReader xmlReader(&dataFile);

        xmlReader.readNextStartElement();
        if (xmlReader.name() == "diagram")
        {
        }
        else
            fileError = true;

        if (fileError || xmlReader.hasError())
        {
            QMessageBox::critical(this, "Error Reading File",
                                  "File does not appear to be a valid GRAPE pipeline.");
        }

        dataFile.close();

        //  setClean();
        //   emitFileStateChanged();
        update();

        //  mainPipeline.load(path);

        //mainPipeline.load(this->items());
        //     mainPipeline.setDiagram(this);
    }
    else
    {
        QMessageBox::critical(this, "Error Opening File",
                              "Unable to open " + path + " for reading.  File not loaded!");
    }

    return (!fileError);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QLayout* MainWindow::createToolsWidgets()
{
    QVBoxLayout* toolsLayout = new QVBoxLayout();
    QFrame* hSeparator = new QFrame();
    hSeparator->setFrameStyle(QFrame::Sunken | QFrame::HLine);

    mToolsWidget = new DiagramToolsWidget();
    mItemDefaultsWidget = new DiagramItemDefaultsWidget();

    toolsLayout->addWidget(mToolsWidget);//, 100);
    toolsLayout->addWidget(hSeparator);
    //toolsLayout->addWidget(mItemDefaultsWidget);

    connect(mDiagram, SIGNAL(modeReset()), mToolsWidget, SLOT(resetMode()));
    connect(mToolsWidget, SIGNAL(defaultModeClicked()), mDiagram, SLOT(setDefaultMode()));
    connect(mToolsWidget, SIGNAL(scrollModeClicked()), mDiagram, SLOT(setScrollMode()));
    connect(mToolsWidget, SIGNAL(zoomModeClicked()), mDiagram, SLOT(setZoomMode()));
    connect(mToolsWidget, SIGNAL(runModeClicked()), mDiagram, SLOT(setRunPipelineMode()));
    connect(mToolsWidget, SIGNAL(stopModeClicked()), mDiagram, SLOT(setStopPipelineMode()));
    connect(mToolsWidget, SIGNAL(newModeClicked(const QString&)), mDiagram, SLOT(setNewMode(const QString&)));


    connect(mToolsTree, SIGNAL(nameItemActivated(const QString&)), mDiagram, SLOT(setNewMode(const QString&)));

    connect(mToolsWidget, SIGNAL(propertiesClicked()), mDiagram, SLOT(properties()));


    return toolsLayout;
}
//--------------------------------------------------------------------------------------------------
void MainWindow::createStatusBar()
{
    mModeLabel = new QLabel("Select Mode");
    mStatusLabel = new QLabel("");
    mNumberOfItemsLabel = new QLabel("0");
    mMouseLabel = new QLabel("");

    mModeLabel->setMinimumWidth(120);
    mStatusLabel->setMinimumWidth(120);
    mNumberOfItemsLabel->setMinimumWidth(80);
    mMouseLabel->setMinimumWidth(120);

    statusBar()->addWidget(mModeLabel);
    statusBar()->addWidget(mStatusLabel);
    statusBar()->addWidget(mNumberOfItemsLabel);
    statusBar()->addWidget(mMouseLabel);

    connect(mDiagram, SIGNAL(modeChanged(const QString&)), mModeLabel, SLOT(setText(const QString&)));
    connect(mDiagram, SIGNAL(cleanChanged(const QString&)), mStatusLabel, SLOT(setText(const QString&)));
    connect(mDiagram, SIGNAL(numberOfItemsChanged(const QString&)), mNumberOfItemsLabel, SLOT(setText(const QString&)));
    connect(mDiagram, SIGNAL(mousePositionChanged(const QString&)), mMouseLabel, SLOT(setText(const QString&)));

    connect(&mDiagram->mainPipeline, SIGNAL(pipelineMessage(const QString&)), mModeLabel, SLOT(setText(const QString&)));
    connect(&mDiagram->mainPipeline, SIGNAL(pipelineMessage(const QString&)), this, SLOT(printText(const QString&)));
    connect(&mDiagram->mainPipeline, SIGNAL(pipelineMessage(const QString&)), &mDiagram->mainPipeline, SLOT(writeToLog(const QString&)));
//    connect(&mDiagram->mainPipeline, SIGNAL(exit()), this, SLOT(close()));
    connect(&mDiagram->mainPipeline, SIGNAL(exit()), this, SLOT(quit()));

}
//--------------------------------------------------------------------------------------------------
void MainWindow::quit()
{
    // need to do some cleanup before exiting..
    this->mDiagram->mainPipeline.writeToLog("Exiting GRAPE...");

    QApplication::quit();
}
//--------------------------------------------------------------------------------------------------
void MainWindow::printText(const QString& s)
{
    std::cout << s.toStdString() << std::endl;
}
//--------------------------------------------------------------------------------------------------
void MainWindow::createActions()
{
    addAction("New...", mManager, SLOT(newDiagram()), ":/icons/oxygen/document-new.png", "Ctrl+N");
    addAction("Open...", mManager, SLOT(openDiagram()), ":/icons/oxygen/document-open.png", "Ctrl+O");
    addAction("Save", mManager, SLOT(saveDiagram()), ":/icons/oxygen/document-save.png", "Ctrl+S");
    addAction("Save As...", mManager, SLOT(saveDiagramAs()), ":/icons/oxygen/document-save-as.png", "Ctrl+Shift+S");
    addAction("Close", mManager, SLOT(closeDiagram()), ":/icons/oxygen/document-close.png", "Ctrl+W");
    addAction("Export PNG...", mManager, SLOT(exportPng()), ":/icons/oxygen/image-x-generic.png");
    addAction("Export SVG...", mManager, SLOT(exportSvg()), ":/icons/oxygen/image-svg+xml.png");
    addAction("Print Preview...", mManager, SLOT(printPreview()), ":/icons/oxygen/document-preview.png");
    addAction("Print Setup...", mManager, SLOT(printSetup()));
    addAction("Print...", mManager, SLOT(printDiagram()), ":/icons/oxygen/document-print.png", "Ctrl+P");
    addAction("Print to PDF...", mManager, SLOT(printPdf()), ":/icons/oxygen/application-pdf.png");
    addAction("Preferences...", this, SLOT(preferences()), ":/icons/oxygen/configure.png");
    addAction("Exit", this, SLOT(close()), ":/icons/oxygen/application-exit.png");

    addAction("About...", this, SLOT(about()), ":/icons/oxygen/help-about.png");
    addAction("About Qt...", qApp, SLOT(aboutQt()));
}
//--------------------------------------------------------------------------------------------------
void MainWindow::createMenus()
{
    QList<QAction*> lActions = actions();
    QList<QAction*> diagramActions = mDiagram->actions();
    QMenu* menu;

    menu = menuBar()->addMenu("File");
    menu->addAction(lActions[NewAction]);
    menu->addAction(lActions[OpenAction]);
    menu->addSeparator();
    menu->addAction(lActions[SaveAction]);
    menu->addAction(lActions[SaveAsAction]);
    menu->addSeparator();
    menu->addAction(lActions[CloseAction]);
    menu->addSeparator();
    menu->addAction(lActions[ExportPngAction]);
    menu->addAction(lActions[ExportSvgAction]);
    menu->addSeparator();
    menu->addAction(lActions[PrintPreviewAction]);
    menu->addAction(lActions[PrintSetupAction]);
    menu->addAction(lActions[PrintAction]);
    menu->addAction(lActions[PrintPdfAction]);
    menu->addSeparator();
    menu->addAction(lActions[PreferencesAction]);
    menu->addSeparator();
    menu->addAction(lActions[ExitAction]);

    menu = menuBar()->addMenu("Edit");
    menu->addAction(diagramActions[Diagram::UndoAction]);
    menu->addAction(diagramActions[Diagram::RedoAction]);
    menu->addSeparator();
    menu->addAction(diagramActions[Diagram::CutAction]);
    menu->addAction(diagramActions[Diagram::CopyAction]);
    menu->addAction(diagramActions[Diagram::PasteAction]);
    menu->addSeparator();
    menu->addAction(diagramActions[Diagram::SelectAllAction]);
    menu->addAction(diagramActions[Diagram::SelectNoneAction]);

    menu = menuBar()->addMenu("Object");
    menu->addAction(diagramActions[Diagram::RotateAction]);
    menu->addAction(diagramActions[Diagram::RotateBackAction]);
    menu->addAction(diagramActions[Diagram::FlipAction]);
    menu->addSeparator();
    menu->addAction(diagramActions[Diagram::AlignVerticalAction]);
    menu->addAction(diagramActions[Diagram::AlignHorizontalAction]);
    menu->addSeparator();
    menu->addAction(diagramActions[Diagram::DeleteAction]);
    menu->addSeparator();
    menu->addAction(diagramActions[Diagram::GroupAction]);
    menu->addAction(diagramActions[Diagram::UngroupAction]);
    menu->addSeparator();
    menu->addAction(diagramActions[Diagram::BringForwardAction]);
    menu->addAction(diagramActions[Diagram::SendBackwardAction]);
    menu->addAction(diagramActions[Diagram::BringToFrontAction]);
    menu->addAction(diagramActions[Diagram::SendToBackAction]);

    menu = menuBar()->addMenu("Diagram");
    menu->addAction(diagramActions[Diagram::PropertiesAction]);
    menu->addSeparator();
    menu->addAction(diagramActions[Diagram::ZoomInAction]);
    menu->addAction(diagramActions[Diagram::ZoomOutAction]);
    menu->addAction(diagramActions[Diagram::ZoomFitAction]);

    menu = menuBar()->addMenu("Help");
    menu->addAction(lActions[AboutAction]);
    menu->addAction(lActions[AboutQtAction]);
}
//--------------------------------------------------------------------------------------------------
void MainWindow::createToolBars()
{
    QList<QAction*> lActions = actions();
    QList<QAction*> diagramActions = mDiagram->actions();

    mDiagramZoomCombo = new QComboBox();
    for(int i = 0; i < Diagram::kZoomLevels.size(); i++)
        mDiagramZoomCombo->addItem(QString::number(Diagram::kZoomLevels[i] * 100) + "%");
    mDiagramZoomCombo->setEditable(true);
    connect(mDiagramZoomCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(updateDiagramZoom(const QString&)));
    connect(mDiagram, SIGNAL(zoomLevelChanged(qreal)), this, SLOT(updateDiagramZoomCombo(qreal)));

    QFontMetrics fontMetrics(mDiagramZoomCombo->font());
    mDiagramZoomCombo->setMinimumWidth(fontMetrics.width("8888.8888%") + 16);

    mFileToolBar = new QToolBar("File");
    mFileToolBar->setObjectName("FileToolBar");
    mFileToolBar->setIconSize(QSize(16, 16));
    mFileToolBar->addAction(lActions[NewAction]);
    mFileToolBar->addAction(lActions[OpenAction]);
    mFileToolBar->addAction(lActions[SaveAction]);
    mFileToolBar->addAction(lActions[CloseAction]);
    mFileToolBar->addAction(lActions[PrintPreviewAction]);
    mFileToolBar->addAction(lActions[PrintAction]);
    addToolBar(mFileToolBar);

    mDiagramToolBar = new QToolBar("Diagram");
    mDiagramToolBar->setObjectName("DiagramToolBar");
    mDiagramToolBar->setIconSize(QSize(16, 16));
    mDiagramToolBar->addAction(diagramActions[Diagram::RotateAction]);
    mDiagramToolBar->addAction(diagramActions[Diagram::RotateBackAction]);
    mDiagramToolBar->addAction(diagramActions[Diagram::FlipAction]);
    mDiagramToolBar->addSeparator();
    mDiagramToolBar->addAction(diagramActions[Diagram::AlignVerticalAction]);
    mDiagramToolBar->addAction(diagramActions[Diagram::AlignHorizontalAction]);
    mDiagramToolBar->addSeparator();
    mDiagramToolBar->addAction(diagramActions[Diagram::ZoomInAction]);
    mDiagramToolBar->addWidget(mDiagramZoomCombo);
    mDiagramToolBar->addAction(diagramActions[Diagram::ZoomOutAction]);
    mDiagramToolBar->addAction(diagramActions[Diagram::ZoomFitAction]);
    addToolBar(mDiagramToolBar);
}
//--------------------------------------------------------------------------------------------------
QAction* MainWindow::addAction(const QString& text, QObject* slotObj,
    const char* slotFunction, const QString& iconPath, const QString& shortcut)
{
    QAction* action = new QAction(text, this);
    connect(action, SIGNAL(triggered()), slotObj, slotFunction);

    if (!iconPath.isEmpty()) action->setIcon(QIcon(iconPath));
    if (!shortcut.isEmpty()) action->setShortcut(QKeySequence(shortcut));

    QMainWindow::addAction(action);
    return action;
}
//--------------------------------------------------------------------------------------------------
void MainWindow::loadLibraryItems()
{
    QList<DiagramItem*> items;


    // open and read the NodeDef.XML file
    // pars into diffrent libraries
    // load each Library


    if(1)
    {
        //QString filename = "../nodedef.xml";
        QString filename = QCoreApplication::applicationDirPath() + "/../nodedef.xml";
        QDomDocument document;
        QFile file(filename);

        if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
        {
            qDebug( "Failed to open file for reading." );
            return;
        }

        if( !document.setContent( &file ) )
        {
            qDebug( "Failed to parse the file into a DOM tree." );
            file.close();
            return;
        }

        file.close();

        QDomElement root = document.firstChildElement();

        QDomNodeList grape_lib_list = root.elementsByTagName("GRAPE");
        QDomNodeList users_lib_list = root.elementsByTagName("USERS");

        if(grape_lib_list.count()== 0)
        {
            qDebug() << "Could not find GRAPE Library";
            return;
        }
        if(grape_lib_list.length() > 1)
        {
            qDebug() << "grape_lib_list.length() > 1";
            return;
        }

       // qDebug()<<node.toElement().attribute("type")<<NodeDefList.length();
        QTreeWidgetItem *m1 = new QTreeWidgetItem;
        QTreeWidgetItem *m2 = new QTreeWidgetItem;


        ParseNodeDef(users_lib_list.at(0), m2);
        ParseNodeDef(grape_lib_list.at(0), m1);

        mToolsTree->setAnimated(true);
        mToolsTree->setWindowTitle("TREE");
        mToolsTree->setAutoScroll(true);
    }


    // Register all diagram items here
    items.append(new SystemcallItem(mNodeInterface["Systemcall"]));
    items.append(new DevelopItem(mNodeInterface["Develop"]));

    items.append(new DelayItem(mNodeInterface["Delay"]));
    items.append(new DividerItem(mNodeInterface["Divider"]));
    items.append(new BranchItem(mNodeInterface["Branch"]));
    items.append(new SynchItem(mNodeInterface["Synch"]));
    items.append(new SelectItem(mNodeInterface["Select"]));
    items.append(new LoopItem(mNodeInterface["Loop"]));
    items.append(new EndItem(mNodeInterface["End"]));

    items.append(new BETItem(mNodeInterface["BET"]));
    items.append(new BiasCorrectionItem(mNodeInterface["BiasCorrection"]));
    items.append(new CurveFittingItem(mNodeInterface["CurveFitting"]));
    items.append(new ImageArithmeticItem(mNodeInterface["ImageArithmetic"]));
    items.append(new ImageNormalizerItem(mNodeInterface["ImageNormalizer"]));
    items.append(new ImageRegistrationItem(mNodeInterface["ImageRegistration"]));
    items.append(new ImageSegmentationItem(mNodeInterface["ImageSegmentation"]));
    items.append(new ImageProcessingItem(mNodeInterface["ImageProcessing"]));

    items.append(new ImageSlicerItem(mNodeInterface["ImageSlicer"]));
    items.append(new ImageReformatItem(mNodeInterface["ImageReformat"]));
    items.append(new ImageStackItem(mNodeInterface["ImageStack"]));

    items.append(new ImageStatisticsItem(mNodeInterface["ImageStatistics"]));
    items.append(new LogOperationItem(mNodeInterface["LogOperation"]));
    items.append(new RelOperatorItem(mNodeInterface["RelOperator"]));

    items.append(new ImageReaderItem(mNodeInterface["ImageReader"]));
    items.append(new ImageWriterItem(mNodeInterface["ImageWriter"]));

    items.append(new ParReaderItem(mNodeInterface["ParReader"]));
    items.append(new ParWriterItem(mNodeInterface["ParWriter"]));

    items.append(new SourceItem(mNodeInterface["Source"]));
    items.append(new SinkItem(mNodeInterface["Sink"]));

    for(int i = 0; i < items.size(); i++)
        Diagram::itemFactory.registerItem(items[i]);
}
//--------------------------------------------------------------------------------------------------
void MainWindow::ParseNodeDef(QDomNode DomNode, QTreeWidgetItem* m1)
{
    // NOTE: This function uses recursion
    QTreeWidgetItem* nn;

    if(!DomNode.toElement().hasAttribute("type"))
    {
        return;
    }

    if(DomNode.toElement().attribute("type")!="Node")
    {
        if(DomNode.toElement().attribute("type")=="Library")
        {
            if((DomNode.toElement().tagName()=="GRAPE")||(DomNode.toElement().tagName()=="USERS"))
            {
                //m1 = new QTreeWidgetItem();
                m1->setText(0,DomNode.toElement().tagName());
                mToolsTree->insertTopLevelItem(0,m1);
                QDomNodeList NodeDefList = DomNode.childNodes();

                for(int i=0; i<NodeDefList.length(); i++)
                {
                    ParseNodeDef(NodeDefList.at(i), m1);
                }
            }
            else
            {
                nn = new QTreeWidgetItem();
                nn->setText(0,DomNode.toElement().tagName());
                QDomNodeList NodeDefList = DomNode.childNodes();
                for(int i=0; i<NodeDefList.length(); i++)
                {
                    ParseNodeDef(NodeDefList.at(i), nn);
                }
                m1->addChild(nn);
            }
        }
        return;
    }

    // test it is not a line or arc
    if( (DomNode.toElement().tagName() == "line") || (DomNode.toElement().tagName()=="arc") )
        return;

    // else it is a node not a library

    int NumInputs = DomNode.toElement().attribute("NumInputs").toInt();
    int NumOutputs = DomNode.toElement().attribute("NumOutputs").toInt();

     QList<QDomNode> Inputs;
    QList<QDomNode> Outputs;

    QString tmp;
    int kk = 0;
    for(kk = 0; kk < NumInputs; kk++)
    {
        tmp = "Input" + QString::number(kk+1);
        Inputs.append(DomNode.toElement().elementsByTagName(tmp).at(0));
    }
    for(kk = 0; kk < NumOutputs; kk++)
    {
        tmp = "Output" + QString::number(kk+1);
        Outputs.append(DomNode.toElement().elementsByTagName(tmp).at(0));
    }

    // store node interface defnition in a hash table
    NodeInterface ni;
    ni.cap = DomNode.toElement().tagName();
    ni.NumInputs = NumInputs;
    ni.NumOutputs = NumOutputs;

    for(kk = 0; kk < NumInputs; kk++)
        ni.SetNodeInputAttr(kk, Inputs.at(kk).toElement().attribute("type"),
                               Inputs.at(kk).toElement().attribute("defaultvalue"),
                               Inputs.at(kk).toElement().attribute("mandatory"),
                               Inputs.at(kk).toElement().attribute("tooltip"),
                               Inputs.at(kk).toElement().attribute("helptext"));

    for(kk = 0; kk < NumOutputs; kk++)
        ni.SetNodeOutputAttr(kk, Outputs.at(kk).toElement().attribute("type"),
                             Outputs.at(kk).toElement().attribute("defaultvalue"),
                             Outputs.at(kk).toElement().attribute("mandatory"),
                             Outputs.at(kk).toElement().attribute("tooltip"),
                             Outputs.at(kk).toElement().attribute("helptext"));

    mNodeInterface[DomNode.toElement().tagName()] = ni;

    nn = new QTreeWidgetItem();
    nn->setText(0,DomNode.toElement().tagName());
    m1->addChild(nn);

    // Hash to save the node I/O infprmation
    //HashVar[DomNode.toElement().tagName()] = n;
}

//--------------------------------------------------------------------------------------------------
void MainWindow::addLibrary(const QString& name, const QList<DiagramItem*>& items)
{
    // qDebug()<<name<<items.size();
    for(int i = 0; i < items.size(); i++)
        Diagram::itemFactory.registerItem(items[i]);

    //mToolsWidget->addLibrary(name, items);


    QTreeWidgetItem *m1 = new QTreeWidgetItem;
    QTreeWidgetItem *n = new QTreeWidgetItem;


    m1->setText(0,name);

    for(int i = 0; i < items.size(); i++)
    {
        n = new QTreeWidgetItem();
        n->setText(0,items.at(i)->caption1());
        m1->addChild(n);
    }

    mToolsTree->insertTopLevelItem(0,m1);

    mToolsTree->setAnimated(true);
    mToolsTree->setWindowTitle("TREE");
    mToolsTree->setAutoScroll(true);
}
//--------------------------------------------------------------------------------------------------
