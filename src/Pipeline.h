/* Pipeline.h
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

#ifndef _Pipeline_H
#define _Pipeline_H

#include <DiagramBaseItems.h>
#include <DiagramItemFactory.h>
#include <Nodes/Node.h>
#include <QThread>
#include <DiagramManager.h>


struct ConnectionStruct
{
    int sourceItemIndex;
    int sourcePointItemIndex;
    int targetItemIndex;
    int targetPointItemIndex;
};

typedef enum {  PIPELINE_NOT_EXECUTED, PIPELINE_EXECUTING, PIPELINE_EXECUTED, PIPELINE_PAUSED } PIPELINE_EXECUTION_STATES ;

//class Pipeline : public QWidget//QObject
class Pipeline : public QObject
{
    Q_OBJECT

protected:

    Diagram * mDiagram;
    DiagramManager * mDiagramManager;
    //Node* nodes[100];
    QList<Node*> NodeList;
    QList<ConnectionStruct> ConnectionList;
    bool pipelineLoaded;

    QHash <QString,QString> AppPath;

    QColor FlashColor;
    int FlashTime;

     PIPELINE_EXECUTION_STATES execution_status;

     bool idleMode;
     //bool GraphicalMode = true;
     bool GraphicalMode;

public:
     QHash <QString, NodeInterface> mNodeInterface;
     QTimer PipelineTimer;

public:
    Pipeline();
    Pipeline(const Pipeline& pipeline);
    ~Pipeline();
    void addItem(DiagramItem* item);
    Pipeline* copy() const;
    void setDiagram(Diagram* d);
    void setDiagramManager(DiagramManager* dm);
    Diagram* diagram();
    DiagramManager* diagramManager();

    virtual QString uniqueKey() const;
//	QString iconPath() const;
    bool isExecutable() const;
    bool isExecuted() const;
    bool isExecuting() const;
    // Selectors
    void someSlot();
    bool load(const QString& path);
    bool load(QList<DiagramItem*> items);
    void init();
    virtual int validateDesign();
    virtual int validateRun();
//    void run();
    void clear();
    void markDirty();
    bool isSuperfluous() const;
    void SetupRunThread(QThread &cThread);
    QList<Node*> getNodeList();

private:
    int getPointIndexInItem(DiagramItemPoint* point, DiagramItem* item);
    QList<DiagramItem*> readItemsFromXml(QXmlStreamReader& xmlReader);
    void buildPipeline(QList<DiagramItem*> items);
    int findItemConnections( DiagramItem* item, QList<DiagramItem*> items);
     bool isAllInputsReady(int node_index) const;
    PIPELINE_EXECUTION_STATES check_pipeline_status();

protected:
    virtual void readXmlChildElement(QXmlStreamReader& xmlReader);

signals:
    void mousePositionChanged(const QString& text);
    void pipelineMessage(const QString& text);

    void stop();
    void pause();
    void exit();

public slots:
    void run();
    int writeToLog(const QString&);

public:
    //static DiagramItemFactory itemFactory;
    void setGraphical(const bool isGraphicalFlag);
    bool isGraphical(void);
    Node setDiagramItem(DiagramItem*);
    int handleStopFlag(QString StopFalg);

    QString getAppPath(QString AppName) const;
    void setAppPath(QString AppName, QString Path);
};


#endif
