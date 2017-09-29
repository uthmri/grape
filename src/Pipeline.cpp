/* Pipeline.cpp
 *
 Copyright (C) 2016 UTHealth MRI Research
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

#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramItem.h>
#include <Pipeline.h>
#include <QMessageBox>
#include <DiagramItemGroup.h>
#include <DiagramMultipleItemProperties.h>
#include <DiagramProperties.h>
#include <DiagramUndo.h>
#include <DiagramWidgets.h>
#include <QApplication>
#include <QDebug>
#include <QtCore>

#include <Nodes/bet.h>
#include <Nodes/end.h>
#include <Nodes/Node.h>
#include <Nodes/sink.h>
#include <Nodes/source.h>
#include <Nodes/line.h>
#include <Nodes/arc.h>
#include <Nodes/imagesegmentation.h>
#include <Nodes/imageprocessing.h>
#include <Nodes/loop.h>
#include <Nodes/branch.h>
#include <Nodes/synch.h>
#include <Nodes/divider.h>
#include <Nodes/delay.h>
#include <Nodes/reloperator.h>
#include <Nodes/imagereader.h>
#include <Nodes/imagestack.h>
#include <Nodes/parreader.h>
#include <Nodes/parwriter.h>
#include <Nodes/imageslicer.h>
#include <Nodes/imagereformat.h>
#include <Nodes/imagewriter.h>
#include <Nodes/imagenormalizer.h>
#include <Nodes/imagearithmetic.h>
#include <Nodes/imagestatistics.h>
#include <Nodes/logoperation.h>
#include <Nodes/Systemcall.h>
#include <Nodes/develop.h>
#include <Nodes/imageregistration.h>
#include <Nodes/biascorrection.h>
#include <Nodes/curvefitting.h>
#include <Nodes/select.h>
#include <xmlreclib.h>
#include <BETItem.h>
#include <Items/EndItem.h>
#include <SourceItem.h>
#include <ImageSegmentationItem.h>
#include <ImageProcessingItem.h>
#include <SelectItem.h>
#include <ParWriterItem.h>
#include <RelOperatorItem.h>
#include <ImageReaderItem.h>
#include <ImageStackItem.h>
#include <ImageReformatItem.h>
#include <ParReaderItem.h>
#include <ImageWriterItem.h>
#include <ImageNormalizerItem.h>
#include <ImageArithmeticItem.h>
#include <ImageStatisticsItem.h>
#include <LoopItem.h>
#include <BranchItem.h>
#include <SynchItem.h>
#include <DividerItem.h>
#include <LogOperationItem.h>
#include <SystemcallItem.h>
#include <DevelopItem.h>
#include <ImageRegistrationItem.h>
#include <BiasCorrectionItem.h>
#include <CurveFittingItem.h>
#include <DelayItem.h>
#include <SinkItem.h>
#include <common.h>

//DiagramItemFactory Pipeline::itemFactory;

//--------------------------------------------------------------------------------------------------
Pipeline::Pipeline()
{
    //mDiagram = NULL;
    pipelineLoaded = false;
    FlashColor = QColor(100,150,55);
    FlashTime = 1000;
    GraphicalMode = true;
}
//--------------------------------------------------------------------------------------------------
Pipeline::Pipeline(const Pipeline& pipeline) { }
//--------------------------------------------------------------------------------------------------
Pipeline::~Pipeline() { }
//--------------------------------------------------------------------------------------------------
void Pipeline::init()
{
    foreach (Node* n, NodeList)
    {
       n->setNodeOutputUpdated(false);
       n->markUnExecuted();
    }
}
//--------------------------------------------------------------------------------------------------
int Pipeline::validateDesign()
{
    int  err_status;

    int num_error = 0;
    int num_warn = 0;
    foreach (Node* n, NodeList)
    {
       err_status = n->validateDesign();
       //qDebug() << "err_status = " << err_status;
       if(err_status == -1)
       {
           num_error++;
           qDebug() << "error(s) found in node" << n->getType();
       }
       else if(err_status == -2) // warning
       {
           num_warn++;
           qDebug() << "warning(s) found in node" << n->getType();
       }
    }

    //let nodes clear any allocated memory
    foreach (Node* n, NodeList)
    {
//        n->clearNodeData();
    }


    if(num_error > 0)
    {
        qDebug() << "Pipeline validation found " << num_error << " errors.";
        return -1;
    }

    if(num_warn > 0)
    {
        qDebug() << "Pipeline validation found " << num_error << " warnings.";
        return -2;
    }


    return 0;
}
//--------------------------------------------------------------------------------------------------
int Pipeline::validateRun()
{
    return 0;
}
//--------------------------------------------------------------------------------------------------
void Pipeline::setGraphical(const bool isGraphicalFlag)
{
    GraphicalMode = isGraphicalFlag;
}
//--------------------------------------------------------------------------------------------------
bool Pipeline::isGraphical()
{
    return GraphicalMode;
}
//--------------------------------------------------------------------------------------------------
//void Pipeline::Pipeline(QObject *parent):QObject(parent)
//{

//}
//--------------------------------------------------------------------------------------------------
void Pipeline::SetupRunThread(QThread &cThread)
{
    connect(&cThread, SIGNAL(started()),this,SLOT(run()));
}
//--------------------------------------------------------------------------------------------------
QList<Node *> Pipeline::getNodeList()
{
    return NodeList;
}
//--------------------------------------------------------------------------------------------------
int Pipeline::writeToLog(const QString& LogString)
{
    writeStrToLog(LogString);
    return 0;

    QString logFilename = "log.txt";
    QFile logFile(logFilename);

    QDateTime now =  QDateTime::currentDateTime();
    QFileInfo info1("log.txt");
    //  qDebug()  << "LOG FILE size" << info1.size();
    if(info1.size() > 1e6)               // returns 56201
    {
        qDebug() << "Log file reached size limit, creating a new log file...";

        QDate date = QDate::currentDate();
        QString datestring = date.toString("yyyy-MM-dd");
        QTime time = QTime::currentTime();
        QString timestring = time.toString("hh-mm-ss");

        QString newName = info1.absolutePath() + "/" + info1.baseName() + "-" + datestring + "-" + timestring;

        if (!info1.completeSuffix().isEmpty())
            newName += "." + info1.completeSuffix();

        QString oldName = info1.absoluteFilePath();

        QFile ff(oldName);
        bool rename_success = ff.rename(newName);
        if(rename_success)
            qDebug()  << "log file backed up successfully!";
        else
            qDebug()  << "could not back up log file.";
    }


    logFile.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append);

    /* Check it opened OK */
    if(!logFile.isOpen()){
        qDebug()  << "- Error: unable to open" << logFilename << "for output";
    }

    /* Point a QTextStream object at the file */
    QTextStream outStream(&logFile);

    /* Write the line to the file */
    outStream << now.toString() << ": " << LogString << "\r\n";

    /* Close the file */
    logFile.close();

    return 0;
}
//--------------------------------------------------------------------------------------------------
bool Pipeline::load(QList<DiagramItem*> items)
{
    buildPipeline(items);
    pipelineLoaded = true;
    return (items.length() > 0);
}
//--------------------------------------------------------------------------------------------------
bool Pipeline::load(const QString& path)
{
    bool fileError = false;
    QFile dataFile(path);

    fileError = !dataFile.open(QIODevice::ReadOnly);
    if (!fileError)
    {
        QXmlStreamReader xmlReader(&dataFile);

        xmlReader.readNextStartElement();
        if (xmlReader.name() == "diagram")
        {
            while (xmlReader.readNextStartElement())
                readXmlChildElement(xmlReader);
        }
        else fileError = true;

        if (fileError || xmlReader.hasError())
        {
            qDebug() << "Error Reading File. File does not appear to be a valid GRAPE pipeline.";
            return false;
        }

        pipelineLoaded = true;

        qDebug() << "Pipeline loaded successfully!";

        dataFile.close();
    }

    return (!fileError);
}

//--------------------------------------------------------------------------------------------------
void Pipeline::readXmlChildElement(QXmlStreamReader& xmlReader)
{
    if (xmlReader.name() == "items")
    {
        QList<DiagramItem*> items = readItemsFromXml(xmlReader);
        buildPipeline(items);
    }
    else
        xmlReader.skipCurrentElement();
}
//--------------------------------------------------------------------------------------------------
//void Pipeline::pause()
//{
//    if(!pipelineLoaded)
//    {
//        return;
//    }
//}
//--------------------------------------------------------------------------------------------------
//void Pipeline::stop()
//{
//    if(!pipelineLoaded)
//    {
//        return;
//    }
//}
//--------------------------------------------------------------------------------------------------
void Pipeline::run()
{
    QString msg;
    int node_flag;

    if(!pipelineLoaded)
    {
        qDebug() << "Pipeline not loaded!!";
//        this->PipelineTimer.stop();
//        return;
    }
    int target_pin_id_on_target, source_pin_id_on_src;
    int exec_status = 0;
    int num_nodes_executed = 0;
    int num_nodes_output_updated = 0;
    bool debug_mode = false;

    foreach (Node* nii, NodeList)
    {
        if(!nii->isExecutable()) continue;

        if(debug_mode)
            qDebug() << "now checking node:" << nii->getType().toUpper() << "1";

        if( ! nii->isExecuted() )
        {
            if(debug_mode)
                qDebug() << "now checking node:" << nii->getType().toUpper() << "2";
            if(!nii->isExecuting())
            {
                if(debug_mode)
                    qDebug() << "now checking node:" << nii->getType().toUpper() << "3";
                if(nii->isAllInputsReady() && nii->AnyFreshInPorts())
                {
                    if(debug_mode)
                        qDebug() << "now checking node:" << nii->getType().toUpper() << "4";
                    // flash the node before execution
                    if( isGraphical() && !nii->isFlashing())
                    {
                        if(debug_mode)
                            qDebug() << "now checking node:" << nii->getType().toUpper() << "5";
                        nii->flashNode(FlashColor, FlashTime);

                    }
                    else // begin execution
                    {
                        if(debug_mode)
                            qDebug() << "now checking node:" << nii->getType().toUpper() << "6";
                        // level 2 validation at run time
                        if(nii->getRunTimeError() == -1) // do not re-execute if you end with error
                            continue;

                        node_flag = nii->validateRun();

                        nii->setRunTimeError(node_flag);

                        if(node_flag == -1)  // an error in validation
                        {
                            msg = QString("PIPELINE: ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ") failed at run-time validation. Node is not executable at this stage.";
                            emit pipelineMessage(msg);
                        }

                        if(node_flag == -2)  // node not ready yet
                        {
                        }

                        if(!node_flag)  // node can execute
                        {
                            msg = QString("PIPELINE: ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ") is starting execution...";
                            emit pipelineMessage(msg);

                            try
                            {
                                exec_status = nii->execute();
                            }
                            catch (const std::exception& e)
                            {
                                // will be executed if f() throws std::runtime_error
                                exec_status = -1;
                                msg = QString("PIPELINE: ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ") failed with error:" + QString(e.what());
                                emit pipelineMessage(msg);
                            }
                            catch (const std::runtime_error& e)
                            {
                                // dead code!
                                exec_status = -1;
                                msg = QString("PIPELINE: ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ") failed with error:" + QString(e.what());
                                emit pipelineMessage(msg);
                            }
                            catch (std::logic_error e)
                            {
                                qDebug() << "ERROR: = " << e.what();
                                exec_status = -1;
                                msg = QString("PIPELINE: ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ") failed with error:" + QString(e.what());
                                emit pipelineMessage(msg);
                            }

                            if(exec_status == -1) // error
                            {
                                msg = QString("PIPELINE: ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ") return an error or is not executed.";
                                emit pipelineMessage(msg);
                            }
                            else
                            {
                                num_nodes_executed++;
                                nii->markExecuted();
                                nii->setNodeOutputUpdated(false);

                                if(nii->getType()== "End")
                                    handleStopFlag(((End*)nii)->getEndType());
                            }
                        }//if(!node_flag)
                    }// else {if( isGraphical() && !nii->isFlashing())}, BEGIN EXECUTION
                } // if(nii->isAllInputsReady() && nii->AnyFreshInPorts())
            }// if(!nodes[ii]->isExecuting())
        } // if(!nodes[ii]->isExecuted())
        else // node is executed, check for output updating
        {
            if( ! nii->nodeOutputUpdated())
            {
                msg = QString("PIPELINE: ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ") executed.";
                emit pipelineMessage(msg);

                msg = QString("PIPELINE: ") + "Updating outputs from node: " + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ")";
                emit pipelineMessage(msg);

                Node* njj = NULL;

                if(debug_mode)
                    qDebug() << "Updating Outputs fron Node " << nii->getType();

                if(debug_mode)
                    qDebug() << "ConnectionList.length() = " << ConnectionList.length();

                foreach (ConnectionStruct conn, ConnectionList)
                {

                    if(debug_mode)
                    {
                        qDebug() << "conn.sourceItemIndex = " << conn.sourceItemIndex;
                        qDebug() << "conn.targetItemIndex = " << conn.targetItemIndex;
                        qDebug() << "conn.sourcePointItemIndex = " << conn.sourcePointItemIndex;
                        qDebug() << "conn.targetPointItemIndex = " << conn.targetPointItemIndex;
                    }
                    if( conn.sourceItemIndex == NodeList.indexOf(nii))
                    {
                        source_pin_id_on_src = conn.sourcePointItemIndex;
                        target_pin_id_on_target = conn.targetPointItemIndex;
                        njj = NodeList.at(conn.targetItemIndex);
                        if(nii->hasOutputData(source_pin_id_on_src))
                        {
                            if(debug_mode)
                            {
                                qDebug() << "Updating Outputs fron Node " << nii->getType() << " to node " << njj->getType();
                                qDebug() << "source_pin_id_on_src=" << source_pin_id_on_src;
                                qDebug() << "target_pin_id_on_target=" << target_pin_id_on_target;
                            }
                            njj->set_input( target_pin_id_on_target, nii->get_output(source_pin_id_on_src) );
                            njj->setFreshInPorts( target_pin_id_on_target, true );
                        }
                        else
                        {
                            msg = QString("PIPELINE: ERROR in node ") + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + "), one of inputs not needed.";
                            emit pipelineMessage(msg);
                        }
                    }
                }

                if(debug_mode)
                    qDebug() << "Finished updating Outputs fron Node " << nii->getType();

                nii->setNodeOutputUpdated(true);
                nii->markUnExecuted(); // so it can be executed again later if it has fresh inputs

                msg = QString("PIPELINE: ") + "Finished updating outputs fron node: " + nii->getType().toUpper() + "(" + nii->getDiagramItem()->caption1() + ")";
                emit pipelineMessage(msg);

                // mark the old inputs as consumed (non-fresh), except when the node is re-executable
                if(debug_mode)
                    qDebug() << "consuming inputs on node: " << nii->getType();

                if( ! nii->isReExecutable() )
                {
                    nii->setFreshInPorts(false);
                }

                num_nodes_output_updated++;
                //                    nii->clearNodeData();

                // special treatment for the LOOP node
                //                        if(nii->getType()=="Loop")
                //                            if( ((Loop*)(nii))->isLoopDone() )
                //                                nii->setExecutionMode(Node::HALTED);

            }//if(!NodeOutputUpdateList.at(ii))
        } // if(!nodes[ii]->isExecuted())
    }// loop on nodes


    if((num_nodes_executed == 0) && (num_nodes_output_updated == 0))
    {
        if(!idleMode)
        {
            msg = "idle...";
            emit pipelineMessage(msg);
            idleMode = true;
        }
    }
    else
        idleMode = false;

}
//--------------------------------------------------------------------------------------------------
PIPELINE_EXECUTION_STATES Pipeline::check_pipeline_status()
{
    PIPELINE_EXECUTION_STATES st = PIPELINE_EXECUTED;

    foreach (Node* n, NodeList)
    {
        if(n->isExecutable())
        {
            if(n->isExecuted())
            {
                if(!n->nodeOutputUpdated())
                {
                    st = PIPELINE_EXECUTING;
                    return st;
                }
            }
            else
            {
                //if(n->isExecuting())
                {
                    st = PIPELINE_EXECUTING;
                    return st;
                }
            }
        }
    }


    return st;
}
//--------------------------------------------------------------------------------------------------
int Pipeline::handleStopFlag(QString StopFalg)
{
    if(StopFalg=="Pause")
    {
        emit pause();
        emit pipelineMessage("Pausing execution..");
    }
    else if(StopFalg=="Stop")
    {
        emit stop();
        emit pipelineMessage("Stopping execution..");
    }
    else if(StopFalg=="Exit")
    {
        emit exit();
        emit pipelineMessage("Exiting GRAPE..");
    }

    return 0;
}
//--------------------------------------------------------------------------------------------------
Pipeline* Pipeline::copy() const
{
    return new Pipeline(*this);
}
//--------------------------------------------------------------------------------------------------
QString Pipeline::uniqueKey() const
{
    return "Pipeline";
}
//--------------------------------------------------------------------------------------------------
void Pipeline::markDirty()
{
//	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
bool Pipeline::isSuperfluous() const
{
    return false;
    //return (DiagramOvalItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void Pipeline::addItem(DiagramItem* item)
{
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> Pipeline::readItemsFromXml(QXmlStreamReader& xmlReader)
{
    QList<DiagramItem*> items;
    DiagramItem* item;
    QXmlStreamAttributes attributes = xmlReader.attributes();

    while (xmlReader.readNextStartElement())
    {
        item = Diagram::itemFactory.create(xmlReader.name().toString());

        if (attributes.hasAttribute("connections"))
        {
             qDebug() << QString("line number = ") + QString:: number(xmlReader.lineNumber());
        }

        if (item)
        {
            item->clearPoints();
            item->readXmlAttributes(xmlReader, items);
            while (xmlReader.readNextStartElement())
                item->readXmlChildElement(xmlReader, items);
            items.append(item);
        }
    }

    return items;
}
//--------------------------------------------------------------------------------------------------
void Pipeline::buildPipeline(QList<DiagramItem*> items)
{
    // Find the number of real nodes (excluding line)
    int nr_item_node=0, nr_items=0, nr_item_line=0;
    QString item_name, item_name_lower;

    nr_items = items.length();

    Node* n;
    NodeItem* item;

    NodeList.clear();

    //    for(QList<DiagramItem*>::Iterator itemIter = items.begin(); itemIter != items.end(); itemIter++)
    foreach(DiagramItem *itemD, items)
    {
        item = (NodeItem*)itemD;
        item_name = item->uniqueKey();
        item_name_lower = item_name.toLower();

             if(item_name_lower == "source")              n = new source(this);
        else if(item_name_lower == "bet")                 n = new BET(this);
        else if(item_name_lower == "end")                 n = new End(this);
        else if(item_name_lower == "biascorrection")      n = new BiasCorrection(this);
        else if(item_name_lower == "curvefitting")        n = new CurveFitting(this);
        else if(item_name_lower == "delay")               n = new Delay(this);
        else if(item_name_lower == "imagesegmentation")   n = new ImageSegmentation(this);
        else if(item_name_lower == "imageprocessing")     n = new ImageProcessing(this);
        else if(item_name_lower == "loop")                n = new Loop(this);
        else if(item_name_lower == "branch")              n = new Branch(this);
        else if(item_name_lower == "synch")             n = new Synch(this);
        else if(item_name_lower == "divider")             n = new Divider(this);
        else if(item_name_lower == "select")              n = new Select(this);
        else if(item_name_lower == "reloperator")         n = new RelOperator(this);
        else if(item_name_lower == "imagearithmetic")     n = new ImageArithmetic(this);
        else if(item_name_lower == "imagestatistics")     n = new ImageStatistics(this);
        else if(item_name_lower == "imagereader")         n = new ImageReader(this);
        else if(item_name_lower == "imagestack")          n = new ImageStack(this);
        else if(item_name_lower == "parreader")           n = new ParReader(this);
        else if(item_name_lower == "parwriter")           n = new ParWriter(this);
        else if(item_name_lower == "imageslicer")         n = new ImageSlicer(this);
        else if(item_name_lower == "imagereformat")       n = new ImageReformat(this);
        else if(item_name_lower == "imagewriter")         n = new ImageWriter(this);
        else if(item_name_lower == "imagenormalizer")     n = new ImageNormalizer(this);
        else if(item_name_lower == "logoperation")        n = new LogOperation(this);
        else if(item_name_lower == "systemcall")          n = new Systemcall(this);
        else if(item_name_lower == "develop")             n = new Develop(this);
        else if(item_name_lower == "imageregistration")   n = new ImageRegistration(this);
        else if(item_name_lower == "sink")                n = new sink(this);
        else if(item_name_lower == "line")                n =  new line(this);
        else if(item_name_lower == "arc")                 n =  new arc(this);
        else
        {
            if(1)
            {
                qDebug() << "ERROR: unsupported node type: " << item_name <<". Replacing with a develop node";
                n = new Develop(this);
            }
            else
            {
                qDebug() << "ERROR: unsupported node type.";
                continue;
            }
        }

        n->setDiagram(this->diagram());
        n->setGraphical(this->isGraphical());

        if( (item_name_lower != "line") && (item_name_lower != "arc") )
        {
            item->setNi(mNodeInterface[item_name]);
            n->setNi(mNodeInterface[item_name]);
        }

        n->setDiagramItem(item);

        NodeList.append(n);
        if((item_name == "line")||(item_name == "arc"))
            nr_item_line++;
    }

    nr_item_node = nr_items - nr_item_line;

    ConnectionList.clear();
    foreach(DiagramItem *item, items)
    {
        QString item_name = item->uniqueKey().toLower();

        if((item_name == "line")||(item_name == "arc"))            
        {
          if(findItemConnections(item, items))
          {
              qDebug() << "Error during search of connections of " << item->uniqueKey();
          }
        }
    }

    if( 0 )// print the connect list
    {
        for(int c = 0; c < ConnectionList.length(); c++)
        {
            qDebug() << ("Connection " +  QString::number(c) + " has " +
                         QString::number(ConnectionList.at(c).sourceItemIndex) +
                         " " + QString::number(ConnectionList.at(c).sourcePointItemIndex) +
                         " " + QString::number(ConnectionList.at(c).targetItemIndex) +
                         " " + QString::number(ConnectionList.at(c).targetPointItemIndex) );
        }
    }

    qDebug() << "Pipeline loaded successfully!";
}
//--------------------------------------------------------------------------------------------------
void Pipeline::clear()
{
    QString item_name_lower ;
    foreach(Node* n, NodeList)
    {
        item_name_lower = n->getType().toLower();
        n->clearNodeData();
    }
    NodeList.clear();
}
//--------------------------------------------------------------------------------------------------
int Pipeline::findItemConnections(DiagramItem* LineItem, QList<DiagramItem*> items)
{
    // find the two items connected by a line (or arc)
    QList<DiagramItemPoint*> points = LineItem->points();

    ConnectionStruct connect;
    DiagramItemPoint* ItemPoint;
    DiagramItem* Item;
    int sourceItemIndex=-1, targetItemIndex=-1;
    int sourcePointItemIndex=-1, targetPointItemIndex=-1;

    int lineItemIndex, ItemIndex, PointItemIndex;

    bool PointIsInput, PointIsOutput, TargetPointIsInput, TargetPointIsOutput, SourcePointIsInput, SourcePointIsOutput;

    // Points
    int line_point_idx = 0;

    lineItemIndex = items.indexOf(LineItem);

//    for(QList<DiagramItemPoint*>::Iterator pointIter = points.begin(); pointIter != points.end(); pointIter++)
    // loop on the pivot points of the line (3 points)
    foreach(DiagramItemPoint* line_point, points)
    {
        if(line_point_idx > 1)  // the line end points have index 0 and 1; middle points have index=2>1
            break;

        if( line_point->numberOfTargets() < 1)  // floating line (one end of the line is not connected to anything), do nothing
            return 0;  // floating line; normal return

        // get all information about the connected point (if multiple points take first point in a non-line item)
        QString item_name;
        for(int ppp = 0; ppp<line_point->numberOfTargets(); ppp++)
        {
            ItemPoint = line_point->target(ppp);
            Item = ItemPoint->item();

            item_name = Item->uniqueKey();
            if((item_name == "line")||(item_name == "arc"))
              continue;
            else
                break;
        }

        PointIsInput = ItemPoint->isInputPoint();
        PointIsOutput = ItemPoint->isOutputPoint();

        Item = ItemPoint->item();  // get the node connected to the line end
        if (Item)
        {
            ItemIndex = items.indexOf(Item);
            PointItemIndex = getPointIndexInItem(ItemPoint, Item);
        }
        else
            return -1;  // could not find target item, do nothing

        // will assign line end 0 as source and end 1 as target; this is NOT the correct source/target
        // The correct source/target will be assigned afterwards
        if (line_point_idx == 0)
        {
            // copy target as source
            sourceItemIndex = ItemIndex;
            sourcePointItemIndex = PointItemIndex;

            SourcePointIsInput = PointIsInput;
            SourcePointIsOutput = PointIsOutput;
        }
        else if (line_point_idx == 1)
        {
            targetItemIndex = ItemIndex;
            targetPointItemIndex = PointItemIndex;

            TargetPointIsInput = PointIsInput;
            TargetPointIsOutput = PointIsOutput;
        }

        line_point_idx++;

    }// loop on the two line points

    if((SourcePointIsOutput && TargetPointIsInput))
    {
        NodeList.at(sourceItemIndex)->setConnectedOutPorts(sourcePointItemIndex);
        NodeList.at(targetItemIndex)->setConnectedInPorts(targetPointItemIndex);

        connect.sourceItemIndex = sourceItemIndex;
        connect.sourcePointItemIndex = sourcePointItemIndex;
        connect.targetItemIndex = targetItemIndex;
        connect.targetPointItemIndex = targetPointItemIndex;
        ConnectionList.append(connect);
    }
    else if(SourcePointIsInput && TargetPointIsOutput)
    {
        NodeList.at(sourceItemIndex)->setConnectedInPorts(sourcePointItemIndex);
        NodeList.at(targetItemIndex)->setConnectedOutPorts(targetPointItemIndex);

        connect.sourceItemIndex = targetItemIndex;
        connect.sourcePointItemIndex = targetPointItemIndex;
        connect.targetItemIndex = sourceItemIndex;
        connect.targetPointItemIndex = sourcePointItemIndex;
        ConnectionList.append(connect);
    }
    else
    {
        qDebug() << "ERROR: Incompatible connection";
        qDebug() << "SourcePointIsOutput = " << SourcePointIsOutput;
        qDebug() << "TargetPointIsInput = " << TargetPointIsInput;
        qDebug() << "lineItemIndex = " << lineItemIndex;
        qDebug() << "sourceItemIndex = " << sourceItemIndex;
        qDebug() << "targetItemIndex = " << targetItemIndex;
        return -1;  // error
    }

    return 0; // nromal
}
//--------------------------------------------------------------------------------------------------
int Pipeline::getPointIndexInItem(DiagramItemPoint* point, DiagramItem* item)
{
    for(int index=0; index<item->numberOfPoints(); index++)
        if(point == item->point(index))
            return index;
}
//--------------------------------------------------------------------------------------------------
void Pipeline::setDiagram(Diagram* d)
{
    mDiagram = d;
    load(d->items());
}
//--------------------------------------------------------------------------------------------------
Diagram* Pipeline::diagram()
{
    return mDiagram;
}
//--------------------------------------------------------------------------------------------------
void Pipeline::setDiagramManager(DiagramManager* dm)
{
    mDiagramManager = dm;
}
//--------------------------------------------------------------------------------------------------
DiagramManager* Pipeline::diagramManager()
{
    return mDiagramManager;
}
//--------------------------------------------------------------------------------------------------
//void Pipeline::loadLibraryItems()
//{
//    // open and read the NodeDef.XML file
//    // pars into diffrent libraries
//    // load each Library
//    QString filename = "../nodedef.xml";
//    QDomDocument document;
//    QFile file(filename);

//    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
//    {
//        qDebug( "Failed to open file for reading." );
//        return;
//    }

//    if( !document.setContent( &file ) )
//    {
//        qDebug( "Failed to parse the file into a DOM tree." );
//        file.close();
//        return;
//    }

//    file.close();

//    QDomElement root = document.firstChildElement();

//    QDomNodeList grape_lib_list = root.elementsByTagName("GRAPE");
//    QDomNodeList users_lib_list = root.elementsByTagName("USERS");

//    if(grape_lib_list.count()== 0)
//    {
//        qDebug() << "Could not find GRAPE Library";
//        return;
//    }
//    if(grape_lib_list.length() > 1)
//    {
//        qDebug() << "grape_lib_list.length() > 1";
//        return;
//    }

//    // qDebug()<<node.toElement().attribute("type")<<NodeDefList.length();
//    QTreeWidgetItem *m1 = new QTreeWidgetItem;
//    QTreeWidgetItem *m2 = new QTreeWidgetItem;


//    ParseNodeDef(users_lib_list.at(0), m2);
//    ParseNodeDef(grape_lib_list.at(0), m1);
//}
//--------------------------------------------------------------------------------------------------
QString Pipeline::getAppPath(QString AppName) const
{
    return AppPath[AppName];
}
//--------------------------------------------------------------------------------------------------
void Pipeline::setAppPath(QString AppName, QString Path)
{
    AppPath[AppName] = Path;
}
//--------------------------------------------------------------------------------------------------
