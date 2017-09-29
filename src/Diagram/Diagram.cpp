/* Diagram.cpp
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


#include <Diagram.h>
#include <DiagramItem.h>
#include <DiagramItemPoint.h>
#include <DiagramItemGroup.h>
#include <DiagramMultipleItemProperties.h>
#include <DiagramProperties.h>
#include <DiagramUndo.h>
#include <DiagramWidgets.h>
#include <Nodes/bet.h>
#include <thread>


const qreal Diagram::kMinimumRectSize = 0.001;
DiagramItemFactory Diagram::itemFactory;

Diagram::Diagram() : DiagramBase()
{
	mSelectionMode = Qt::ContainsItemBoundingRect;
	mForcingItemsInside = true;
	mNewItem = NULL;

	mUndoStack.setUndoLimit(64);

	mMouseState = MouseReady;
	mMouseDownItem = NULL;
	mNewClickCount = 0;
	mConsecutivePastes = 0;

    mPipelineRunning = false;

	addActions();
	createContextMenus();
	connect(&mUndoStack, SIGNAL(cleanChanged(bool)), this, SLOT(emitCleanChanged(bool)));
	connect(&mUndoStack, SIGNAL(cleanChanged(bool)), this, SLOT(emitFileStateChanged()));
}
//--------------------------------------------------------------------------------------------------
Diagram::~Diagram()
{
	setDefaultMode();
	clearItems();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::setSelectionMode(Qt::ItemSelectionMode mode)
{
	mSelectionMode = mode;
}
//--------------------------------------------------------------------------------------------------
void Diagram::setForcingItemsInside(bool inside)
{
	mForcingItemsInside = inside;
}
//--------------------------------------------------------------------------------------------------
Qt::ItemSelectionMode Diagram::selectionMode() const
{
	return mSelectionMode;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::isForcingItemsInside() const
{
	return mForcingItemsInside;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::addItem(DiagramItem* item)
{
	if (item && item->parent() == NULL && !containsItem(item))
	{
		mItems.append(item);
		item->mDiagram = this;
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::insertItem(int index, DiagramItem* item)
{
	if (item && item->parent() == NULL && !containsItem(item))
	{
		mItems.insert(index, item);
		item->mDiagram = this;
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::removeItem(DiagramItem* item)
{
	if (item && containsItem(item))
	{
		deselectItem(item);
		mItems.removeAll(item);
		item->mDiagram = NULL;
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::clearItems()
{
	DiagramItem* item;

	while (!mItems.isEmpty())
	{
		item = mItems.first();
		removeItem(item);
		delete item;
	}

	mMouseDownItem = NULL;
	mUndoStack.clear();
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> Diagram::items() const
{
	return mItems;
}
//--------------------------------------------------------------------------------------------------
int Diagram::numberOfItems() const
{
	return mItems.size();
}
//--------------------------------------------------------------------------------------------------
int Diagram::itemIndex(DiagramItem* item) const
{
	int index = -1;
	if (item) index = mItems.indexOf(item);
	return index;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::containsItem(DiagramItem* item) const
{
	bool result = false;
	if (item) result = mItems.contains(item);
	return result;
}
//--------------------------------------------------------------------------------------------------
void Diagram::reorderItems(const QList<DiagramItem*>& items)
{
	mItems = items;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> Diagram::items(const QRectF& diagramRect) const
{
	QList<DiagramItem*> itemsInRect;
	QRectF itemRect;

	for(QList<DiagramItem*>::ConstIterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		if (mSelectionMode == Qt::ContainsItemShape || mSelectionMode == Qt::IntersectsItemShape)
			itemRect = (*itemIter)->mapToDiagram((*itemIter)->shape().boundingRect());
		else
			itemRect = (*itemIter)->mapToDiagram((*itemIter)->boundingRect());

		if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
		if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);

		if (mSelectionMode == Qt::IntersectsItemBoundingRect ||
			mSelectionMode == Qt::IntersectsItemShape)
		{
			if (diagramRect.intersects(itemRect)) itemsInRect.append(*itemIter);
		}
		else
		{
			if (diagramRect.contains(itemRect)) itemsInRect.append(*itemIter);
		}
	}

	return itemsInRect;
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> Diagram::childItems(const QRectF& diagramRect) const
{
	QList<DiagramItem*> itemsInRect;
	QList<DiagramItem*> children;
	QList<DiagramItem*>::ConstIterator itemIter, childIter;
	QRectF itemRect;

	for(itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
	{
		children = (*itemIter)->children();
		for(childIter = mItems.begin(); childIter != mItems.end(); childIter++)
		{
			if (mSelectionMode == Qt::ContainsItemShape || mSelectionMode == Qt::IntersectsItemShape)
				itemRect = (*childIter)->mapToDiagram((*childIter)->shape().boundingRect());
			else
				itemRect = (*childIter)->mapToDiagram((*childIter)->boundingRect());

			if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
			if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);

			if (mSelectionMode == Qt::IntersectsItemBoundingRect ||
				mSelectionMode == Qt::IntersectsItemShape)
			{
				if (diagramRect.intersects(itemRect)) itemsInRect.append(*childIter);
			}
			else
			{
				if (diagramRect.contains(itemRect)) itemsInRect.append(*childIter);
			}
		}
	}

	return itemsInRect;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* Diagram::itemAt(const QPointF& diagramPos) const
{
	QList<DiagramItem*>::ConstIterator itemIter;
	DiagramItem* item = NULL;

	// Favor selected items
	itemIter = mSelectedItems.end();
	while (item == NULL && itemIter != mSelectedItems.begin())
	{
		itemIter--;
		if ((*itemIter) && (*itemIter)->shape().contains((*itemIter)->mapFromDiagram(diagramPos)))
			item = *itemIter;
	}

	// Search all items
	itemIter = mItems.end();
	while (item == NULL && itemIter != mItems.begin())
	{
		itemIter--;
		if ((*itemIter) && (*itemIter)->shape().contains((*itemIter)->mapFromDiagram(diagramPos)))
			item = *itemIter;
	}

	return item;
}
//--------------------------------------------------------------------------------------------------
DiagramItem* Diagram::childAt(const QPointF& diagramPos) const
{
	QList<DiagramItem*>::ConstIterator itemIter;
	QList<DiagramItem*> children;
	QList<DiagramItem*>::ConstIterator childIter;
	DiagramItem* item = NULL;

	// Favor selected items
	itemIter = mSelectedItems.end();
	while (item == NULL && itemIter != mSelectedItems.begin())
	{
		itemIter--;
		if ((*itemIter) && (*itemIter)->shape().contains((*itemIter)->mapFromDiagram(diagramPos)))
			item = *itemIter;
	}

	// Search all items
	itemIter = mItems.end();
	while (item == NULL && itemIter != mItems.begin())
	{
		itemIter--;

		children = (*itemIter)->children();
		for(childIter = children.begin(); childIter != children.end() && item == NULL; childIter++)
		{
			if ((*childIter)->shape().contains((*childIter)->mapFromDiagram(diagramPos)))
				item = *childIter;
		}
	}

	return item;
}
//--------------------------------------------------------------------------------------------------
QRectF Diagram::itemsRect() const
{
	QRectF rect;

	for(QList<DiagramItem*>::ConstIterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		if (!rect.isValid()) rect = (*itemIter)->boundingRect().translated((*itemIter)->position());
		else rect = rect.united((*itemIter)->boundingRect().translated((*itemIter)->position()));
	}

	return rect;
}
//--------------------------------------------------------------------------------------------------
QRectF Diagram::itemsShapeRect() const
{
	QRectF rect;

	for(QList<DiagramItem*>::ConstIterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		if (!rect.isValid())
			rect = (*itemIter)->shape().boundingRect().translated((*itemIter)->position());
		else
		{
			rect = rect.united((*itemIter)->shape().boundingRect().translated(
				(*itemIter)->position()));
		}
	}

	return rect;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::selectItem(DiagramItem* item)
{
	if (item && !item->isSelected())
	{
		// Ensure that none of the child items are already selected
		for(int i = 0; i < item->numberOfChildren(); i++)
			deselectItem(item->child(i));

		item->setSelected(true);
		mSelectedItems.append(item);
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::deselectItem(DiagramItem* item)
{
	if (item && item->isSelected())
	{
		mSelectedItems.removeAll(item);
		item->setSelected(false);
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::selectItems(const QRectF& diagramRect)
{
	QList<DiagramItem*> lItems = items(diagramRect);

	for(QList<DiagramItem*>::Iterator itemIter = lItems.begin();
		itemIter != lItems.end(); itemIter++) selectItem(*itemIter);
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> Diagram::selectedItems() const
{
	return mSelectedItems;
}
//--------------------------------------------------------------------------------------------------
int Diagram::numberOfSelectedItems() const
{
	return mSelectedItems.size();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::updateSelectionCenter()
{
	int count = 0;

    mSelectionCenter = QPointF(0.0, 0.0);
    for(QList<DiagramItem*>::ConstIterator itemIter = mSelectedItems.begin();
		itemIter != mSelectedItems.end(); itemIter++)
	{
		mSelectionCenter += (*itemIter)->mapToDiagram((*itemIter)->centerPosition());
		count++;
	}

	if (count > 0) mSelectionCenter /= count;

	mSelectionCenter = roundToGrid(mSelectionCenter);
}
//--------------------------------------------------------------------------------------------------
QPointF Diagram::selectionCenter() const
{
	return mSelectionCenter;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::setNewItem(DiagramItem* item)
{
	if (mNewItem) mNewItem->mDiagram = NULL;
	delete mNewItem;

	mNewItem = item;
	if (mNewItem)
	{
		mNewItem->mDiagram = this;
		if (mNewItem->shouldMatchUnitsWithParent()) mNewItem->setUnits(units());
	}
}
//--------------------------------------------------------------------------------------------------
DiagramItem* Diagram::newItem() const
{
	return mNewItem;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::jumpToItem(DiagramItem* item)
{
	if (item && containsItem(item))
	{
		QRectF itemRect;

		itemRect = item->mapToDiagram(item->boundingRect());
		if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
		if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);
		//if (!visibleRect().contains(itemRect)) centerOn(item->position());
		if (!visibleRect().intersects(itemRect)) centerOn(item->position());
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Diagram::canUndo() const
{
	return mUndoStack.canUndo();
}
//--------------------------------------------------------------------------------------------------
bool Diagram::canRedo() const
{
	return mUndoStack.canRedo();
}
//--------------------------------------------------------------------------------------------------
void Diagram::setClean()
{
	mUndoStack.setClean();
}
//--------------------------------------------------------------------------------------------------
bool Diagram::isClean() const
{
	return mUndoStack.isClean();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::render(DiagramPainter* painter, const QRectF& rect)
{
	drawBackground(painter, rect);
	drawItems(painter, rect);
	drawForeground(painter, rect);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::setDefaultMode()
{
    if(mRunThread.isRunning())
    {
//        mRunThread.exit();        
        mainPipeline.PipelineTimer.stop();
    }
    DiagramBase::setDefaultMode();
	deselectAll();
	setNewItem(NULL);

	emit modeChanged("Select Mode");
    emit modeReset();
}
//--------------------------------------------------------------------------------------------------
void Diagram::setZoomMode()
{
    if(mRunThread.isRunning())
    {
//        mRunThread.exit();
        mainPipeline.PipelineTimer.stop();
    }
    DiagramBase::setZoomMode();
    deselectAll();
    setNewItem(NULL);
    emit modeChanged("Zoom Mode");
}
//--------------------------------------------------------------------------------------------------
//void Diagram::setValidatPipelineMode()
//{
//    mPipelineRunning = false;
//   DiagramBase::setStopPipelineMode();
//    deselectAll();
//    setNewItem(NULL);

//    mainPipeline.PipelineTimer.validatedesign();
//    emit modeChanged("validatedesign Pipeline Mode");
//}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::setRunPipelineMode()
{
   // pp.init(5);
    DiagramBase::setRunPipelineMode();
    deselectAll();
    setNewItem(NULL);

    mainPipeline.setParent(this);
    //setDefaultMode();

    emit modeChanged("Run Pipeline Mode");

    //reload to include latest changes
//    mainPipeline.load(this->items());
    mainPipeline.clear();
    mainPipeline.setDiagram(this);

    //     validatedesign the pipeline
    int PipelineValidateStatus = mainPipeline.validateDesign();

    if(PipelineValidateStatus == -1)
    {
        qDebug() << "Error validating the pipeline.";
        return;
    }

    if(PipelineValidateStatus == -2)
    {
        qDebug() << "Warning founds while validating the pipeline. Continue running..";
    }
    emit mainPipeline.pipelineMessage("=======================================================");
    emit mainPipeline.pipelineMessage("===========STARTING PIPELINE EXECUTION=================");
    emit mainPipeline.pipelineMessage("=======================================================");

    if(1)  // timer-triggered
    {
        mainPipeline.init();
        mainPipeline.PipelineTimer.setInterval(10);
        connect(&mainPipeline.PipelineTimer, SIGNAL(timeout()), &mainPipeline, SLOT(run()));
        connect(&mainPipeline, SIGNAL(pause()), &mainPipeline.PipelineTimer, SLOT(stop()));
        connect(&mainPipeline, SIGNAL(stop()), &mainPipeline.PipelineTimer, SLOT(stop()));
        if(mainPipeline.isGraphical())
            connect(&mainPipeline.PipelineTimer, SIGNAL(timeout()), (QObject*)mainPipeline.diagramManager(), SLOT(showDiagram()));

        mainPipeline.PipelineTimer.start();
    }
    else  // threaded
    {
        mainPipeline.SetupRunThread(mRunThread);
        mainPipeline.moveToThread(&mRunThread);
        mRunThread.start();
    }

    mPipelineRunning = true;
}
//--------------------------------------------------------------------------------------------------
void Diagram::setStopPipelineMode()
{
    mPipelineRunning = false;
    DiagramBase::setStopPipelineMode();
    deselectAll();
    setNewItem(NULL);
    //setDefaultMode();
    mainPipeline.PipelineTimer.stop();
    emit modeChanged("Stop Pipeline Mode");
}
//--------------------------------------------------------------------------------------------------
void Diagram::setScrollMode()
{
    if(mRunThread.isRunning())
    {
        //mRunThread.exit();
        mainPipeline.PipelineTimer.stop();
    }
	DiagramBase::setScrollMode();
	deselectAll();
	setNewItem(NULL);
	emit modeChanged("Scroll Mode");
}
//--------------------------------------------------------------------------------------------------
void Diagram::setNewMode(DiagramItem* item)
{
    if(mRunThread.isRunning())
    {
        //mRunThread.exit();
        mainPipeline.PipelineTimer.stop();
    }
    if (item)
	{
		deselectAll();
		setMode(NewMode);
		setNewItem(item);
		setCursor(Qt::CrossCursor);

		mNewClickCount = 0;
		emit modeChanged("Place Item");
	}
	else setDefaultMode();
}
//--------------------------------------------------------------------------------------------------
void Diagram::setNewMode(const QString& key)
{
    if(mRunThread.isRunning())
    {
//        mRunThread.exit();
        mainPipeline.PipelineTimer.stop();
    }
    DiagramItem* item1 = itemFactory.create(key);

    NodeItem* item = 0;

    if(item1)
    {
        item = (NodeItem*)item1;
        if(! ((key=="line") || (key=="arc")) )
        {
            item->setNi(mainPipeline.mNodeInterface[key]);
            item->updateDiagramItemInterface();

            if (item && item->createEvent(this)) setNewMode(item);
            else setNewMode(NULL);
        }
        else
        {
            if (item1 && item1->createEvent(this)) setNewMode(item1);
            else setNewMode(NULL);
        }

    }
}
//--------------------------------------------------------------------------------------------------
void Diagram::setNonInteractiveMode()
{
    if(mRunThread.isRunning())
    {
//        mRunThread.exit();
        mainPipeline.PipelineTimer.stop();
    }
    DiagramBase::setNonInteractiveMode();
	deselectAll();
	setNewItem(NULL);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::selectAll()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++) selectItem(*itemIter);
	updateSelectionCenter();
	update();
}
//--------------------------------------------------------------------------------------------------
void Diagram::deselectAll()
{
	while (!mSelectedItems.empty()) deselectItem(mSelectedItems.last());
	updateSelectionCenter();
	update();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::undo()
{
	mUndoStack.undo();

	setDefaultMode();
	emit mousePositionChanged("");
	emit numberOfItemsChanged(QString::number(numberOfItems()));
	updateScrollBars();
	update();
}
//--------------------------------------------------------------------------------------------------
void Diagram::redo()
{
	mUndoStack.redo();

	setDefaultMode();
	emit mousePositionChanged("");
	emit numberOfItemsChanged(QString::number(numberOfItems()));
	updateScrollBars();
	update();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::cut()
{
	if (mode() == DefaultMode)
	{
		copy();
		deleteSelection();
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::copy()
{
	if (mode() == DefaultMode)
	{
		QClipboard* clipboard = QApplication::clipboard();
		if (clipboard && !mSelectedItems.isEmpty())
		{
			QString xmlItems;
			QXmlStreamWriter xmlWriter(&xmlItems);

			xmlWriter.setAutoFormatting(true);
			xmlWriter.setAutoFormattingIndent(-1);

			xmlWriter.writeStartElement("items");
			writeItemsToXml(xmlWriter, mSelectedItems);
			xmlWriter.writeEndElement();

			clipboard->setText(xmlItems);
		}

		update();
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::paste()
{
	if (mode() == DefaultMode)
	{
		QClipboard* clipboard = QApplication::clipboard();
		QList<DiagramItem*> newItems;

		if (clipboard)
		{
			QString xmlItems = clipboard->text();
			QXmlStreamReader xmlReader(xmlItems);

			if (xmlReader.readNextStartElement() && xmlReader.name() == "items")
				newItems = readItemsFromXml(xmlReader);
		}

		if (!newItems.isEmpty())
		{
			// Offset items based on mConsecutivePastes
			bool addItems = true;
			qreal offset = 2 * mConsecutivePastes * grid();
			QPointF deltaPosition = QPointF(offset, offset) +
				roundToGrid(lastMouseEvent().buttonDownDiagramPos() - newItems.first()->position());
			QList<DiagramItem*>::Iterator itemIter;

			for(itemIter = newItems.begin(); itemIter != newItems.end(); itemIter++)
			{
				if ((*itemIter)->shouldMatchUnitsWithParent())
					(*itemIter)->setUnits(units());

				(*itemIter)->setPosition((*itemIter)->position() + deltaPosition);
				(*itemIter)->setSelected(false);
			}

			if (mForcingItemsInside) addItems = keepItemsInside(newItems);

			if (addItems)
			{
				// Add new items to diagram
				mUndoStack.push(new DiagramAddItemsCommand(this, newItems, DoNotPlace));

				// Ensure new items are selected
				deselectAll();
				for(itemIter = newItems.begin(); itemIter != newItems.end(); itemIter++)
					selectItem(*itemIter);
				updateSelectionCenter();

				mConsecutivePastes++;
			}
			else
			{
				while (!newItems.isEmpty()) delete newItems.takeFirst();
			}
		}

		update();
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::rotateSelection()
{
	switch (mode())
	{
	case NewMode:
		if (mNewItem && mNewItem->placeType() == DiagramItem::PlaceMouseUp)
			mNewItem->rotateEvent(mNewItem->position());
		break;

	case DefaultMode:
		if (numberOfSelectedItems() > 0 && canRotateSelection())
			mUndoStack.push(new DiagramRotateItemsCommand(mSelectedItems, selectionCenter()));
		break;

	default:
		break;
	}

	update();
}
//--------------------------------------------------------------------------------------------------
void Diagram::rotateBackSelection()
{
	switch (mode())
	{
	case NewMode:
		if (mNewItem && mNewItem->placeType() == DiagramItem::PlaceMouseUp)
			mNewItem->rotateBackEvent(mNewItem->position());
		break;

	case DefaultMode:
		if (numberOfSelectedItems() > 0 && canRotateBackSelection())
			mUndoStack.push(new DiagramRotateBackItemsCommand(mSelectedItems, selectionCenter()));
		break;

	default:
		break;
	}

	update();
}
//--------------------------------------------------------------------------------------------------
void Diagram::flipSelection()
{
    switch (mode())
    {
    case NewMode:
        if (mNewItem && mNewItem->placeType() == DiagramItem::PlaceMouseUp)
            mNewItem->flipEvent(mNewItem->position());
        break;

    case DefaultMode:
        if (numberOfSelectedItems() > 0 && canFlipSelection())
            mUndoStack.push(new DiagramFlipItemsCommand(mSelectedItems, selectionCenter()));
        break;

    default:
        break;
    }

    update();
}
//--------------------------------------------------------------------------------------------------
void Diagram::alignVerticalSelection()
{
    switch (mode())
    {
    case NewMode:
        if (mNewItem && mNewItem->placeType() == DiagramItem::PlaceMouseUp)
            mNewItem->alignVerticalEvent(mNewItem->position());
        break;

    case DefaultMode:
        if (numberOfSelectedItems() > 0 && canAlignVerticalSelection())
            mUndoStack.push(new DiagramAlignVerticalItemsCommand(mSelectedItems, selectionCenter()));
        break;

    default:
        break;
    }

    update();
}
//--------------------------------------------------------------------------------------------------
void Diagram::alignHorizontalSelection()
{
    switch (mode())
    {
    case NewMode:
        if (mNewItem && mNewItem->placeType() == DiagramItem::PlaceMouseUp)
            mNewItem->alignHorizontalEvent(mNewItem->position());
        break;

    case DefaultMode:
        if (numberOfSelectedItems() > 0 && canAlignHorizontalSelection())
            mUndoStack.push(new DiagramAlignHorizontalItemsCommand(mSelectedItems, selectionCenter()));
        break;

    default:
        break;
    }

    update();
}
//--------------------------------------------------------------------------------------------------
void Diagram::deleteSelection()
{
	switch (mode())
	{
	case NewMode:
		setDefaultMode();
		break;

	case DefaultMode:
		if (numberOfSelectedItems() > 0)
		{
			QList<DiagramItem*> items;

			for(QList<DiagramItem*>::Iterator itemIter = mSelectedItems.begin();
				itemIter != mSelectedItems.end(); itemIter++)
			{
				if ((*itemIter)->parent() == NULL) items.append(*itemIter);
			}

			mUndoStack.push(new DiagramRemoveItemsCommand(this, items));
			emit numberOfItemsChanged(QString::number(numberOfItems()));
		}
		break;

	default:
		break;
	}

	update();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::sendBackward()
{
	if (mode() == DefaultMode)
	{
		QList<DiagramItem*> lItems = mSelectedItems;
		QList<DiagramItem*> lItemsOrdered = mItems;

		DiagramItem* item;
		qint32 index;

		while (!lItems.empty())
		{
			item = lItems.takeLast();
			index = lItemsOrdered.indexOf(item);
			if (index > 0)
			{
				lItemsOrdered.removeAll(item);
				lItemsOrdered.insert(index - 1, item);
			}
		}

		mUndoStack.push(new DiagramReorderItemsCommand(this, lItemsOrdered));
		update();
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::bringForward()
{
	if (mode() == DefaultMode)
	{
		QList<DiagramItem*> lItems = mSelectedItems;
		QList<DiagramItem*> lItemsOrdered = mItems;

		DiagramItem* item;
		qint32 index;

		while (!lItems.empty())
		{
			item = lItems.takeLast();
			index = lItemsOrdered.indexOf(item);
			if (index >= 0)
			{
				lItemsOrdered.removeAll(item);
				lItemsOrdered.insert(index + 1, item);
			}
		}

		mUndoStack.push(new DiagramReorderItemsCommand(this, lItemsOrdered));
		update();
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::sendToBack()
{
	if (mode() == DefaultMode)
	{
		QList<DiagramItem*> lItems = mSelectedItems;
		QList<DiagramItem*> lItemsOrdered = mItems;

		DiagramItem* item;

		while (!lItems.empty())
		{
			item = lItems.takeLast();
			if (lItemsOrdered.contains(item))
			{
				lItemsOrdered.removeAll(item);
				lItemsOrdered.prepend(item);
			}
		}

		mUndoStack.push(new DiagramReorderItemsCommand(this, lItemsOrdered));
		update();
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::bringToFront()
{
	if (mode() == DefaultMode)
	{
		QList<DiagramItem*> lItems = mSelectedItems;
		QList<DiagramItem*> lItemsOrdered = mItems;

		DiagramItem* item;

		while (!lItems.empty())
		{
			item = lItems.takeLast();
			if (lItemsOrdered.contains(item))
			{
				lItemsOrdered.removeAll(item);
				lItemsOrdered.append(item);
			}
		}

		mUndoStack.push(new DiagramReorderItemsCommand(this, lItemsOrdered));
		update();
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::insertItemPoint()
{
	if (mode() == DefaultMode)
	{
		DiagramItem* item = NULL;
		if (mSelectedItems.size() == 1) item = mSelectedItems.first();

		if (item && item->canInsertRemovePoints())
		{
			DiagramItemPoint* itemPoint = new DiagramItemPoint();
			itemPoint->setPosition(
				item->mapFromDiagram(roundToGrid(lastMouseEvent().buttonDownDiagramPos())));

			if (item->canInsertPoint(itemPoint) && item->insertPointIndex(itemPoint) >= 0)
			{
				mUndoStack.push(new DiagramItemInsertPointCommand(
					item, itemPoint, item->insertPointIndex(itemPoint)));
			}

			update();
		}
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::removeItemPoint()
{
	if (mode() == DefaultMode)
	{
		DiagramItem* item = NULL;
		if (mSelectedItems.size() == 1) item = mSelectedItems.first();

		if (item && item->canInsertRemovePoints())
		{
			DiagramItemPoint* itemPoint = item->pointNearest(
				item->mapFromDiagram(roundToGrid(lastMouseEvent().buttonDownDiagramPos())));

			if (item->canRemovePoint(itemPoint))
				mUndoStack.push(new DiagramItemRemovePointCommand(item, itemPoint));

			update();
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::group()
{
	if (mode() == DefaultMode)
	{
		QList<DiagramItem*> lSelectedItems = selectedItems();
		if (lSelectedItems.size() > 1)
		{
			QUndoCommand* command = new QUndoCommand("Group Items");

			QList<DiagramItem*> items = DiagramItem::copyItems(lSelectedItems);
			DiagramItemGroup* itemGroup = itemFactory.createGroup();

			itemGroup->setPosition(items.first()->position());
			itemGroup->setUnits(units());
			for(QList<DiagramItem*>::Iterator iter = items.begin(); iter != items.end(); iter++)
			{
				(*iter)->setSelected(false);
				(*iter)->setPosition((*iter)->position() - itemGroup->position());
			}
			itemGroup->setItems(items);

			new DiagramRemoveItemsCommand(this, lSelectedItems, command);
			new DiagramAddItemsCommand(this, itemGroup, DoNotPlace, command);

			mUndoStack.push(command);
			selectItem(itemGroup);

			emit numberOfItemsChanged(QString::number(numberOfItems()));
			update();
		}
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::ungroup()
{
	if (mode() == DefaultMode)
	{
		QList<DiagramItem*> lSelectedItems = selectedItems();
		if (lSelectedItems.size() == 1)
		{
			DiagramItemGroup* itemGroup = dynamic_cast<DiagramItemGroup*>(lSelectedItems.first());
			if (itemGroup)
			{
				QUndoCommand* command = new QUndoCommand("Ungroup Items");

				QList<DiagramItem*> items = DiagramItem::copyItems(itemGroup->items());
				for(QList<DiagramItem*>::Iterator iter = items.begin(); iter != items.end(); iter++)
					(*iter)->setPosition((*iter)->position() + itemGroup->position());

				new DiagramRemoveItemsCommand(this, itemGroup, command);
				new DiagramAddItemsCommand(this, items, DoNotPlace, command);

				mUndoStack.push(command);
				for(QList<DiagramItem*>::Iterator iter = items.begin(); iter != items.end(); iter++)
					selectItem(*iter);

				emit numberOfItemsChanged(QString::number(numberOfItems()));
				update();
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Diagram::save(const QString& path)
{
	bool fileError = false;
    QFile dataFile(path);

	fileError = !dataFile.open(QIODevice::WriteOnly);
	if (!fileError)
	{
		QXmlStreamWriter xmlWriter(&dataFile);
		xmlWriter.setAutoFormatting(true);
		xmlWriter.setAutoFormattingIndent(-1);

		xmlWriter.writeStartDocument();

		xmlWriter.writeStartElement("diagram");
		writeXmlAttributes(xmlWriter);
		writeXmlChildElements(xmlWriter);
		xmlWriter.writeEndElement();

		xmlWriter.writeEndDocument();
		dataFile.close();

		setClean();
		emitFileStateChanged();
		update();
	}
	else
	{
		QMessageBox::critical(this, "Error Saving File",
            "Unable to open " + path + "for saving.  File not saved!");
	}

	return (!fileError);
}
//--------------------------------------------------------------------------------------------------
bool Diagram::load(const QString& path)
{
    //qdebug()<< path;
	bool fileError = false;
    QFile dataFile(path);

	fileError = !dataFile.open(QIODevice::ReadOnly);
	if (!fileError)
	{
        QXmlStreamReader xmlReader(&dataFile);

        clearItems();
		setDefaultMode();

        xmlReader.readNextStartElement();
		if (xmlReader.name() == "diagram")
		{
			readXmlAttributes(xmlReader);

			while (xmlReader.readNextStartElement())
				readXmlChildElement(xmlReader);
		}
		else fileError = true;

		if (fileError || xmlReader.hasError())
		{
			QMessageBox::critical(this, "Error Reading File",
				"File does not appear to be a valid jade diagram.");
		}

		dataFile.close();

        setClean();
		emitFileStateChanged();
		update();

        mainPipeline.PipelineTimer.stop();
        //mainPipeline.load(path);
        //mainPipeline.load(this->items());
        mainPipeline.setDiagram(this);
    }
	else
	{
        QMessageBox::critical(this, "Error Opening File", "Unable to open " + path + " for reading.");
	}

	return (!fileError);
}
//--------------------------------------------------------------------------------------------------
void Diagram::clear()
{
    setDefaultMode();
    clearItems();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Diagram::properties()
{
	bool dialogAccepted = false;

	if (mode() == DefaultMode)
	{
		if (numberOfSelectedItems() == 1)
			dialogAccepted = singleItemProperties(selectedItems().first());
		else if (numberOfSelectedItems() > 1)
			dialogAccepted = multipleItemProperties(selectedItems());
		else
			dialogAccepted = diagramProperties();

		update();
	}

	return dialogAccepted;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::defaultContextMenuEvent(DiagramMouseEvent& event)
{
	if (event.modifiers() & Qt::ShiftModifier) mMouseDownItem = childAt(event.diagramPos());
	else mMouseDownItem = itemAt(event.diagramPos());

	if (mMouseDownItem)
		mMouseDownItemPos = mMouseDownItem->mapFromDiagram(event.buttonDownDiagramPos());

	if (mMouseDownItem && mMouseDownItem->isSelected() && mSelectedItems.size() == 1)
	{
		mMouseDownItem->contextMenuEvent(event);

		if (!event.isAccepted())
		{
			if (mMouseDownItem->canInsertRemovePoints())
				mAlternateSingleItemContextMenu.popup(mapToGlobal(event.screenPos()) + QPoint(2, 2));
			else
				mSingleItemContextMenu.popup(mapToGlobal(event.screenPos()) + QPoint(2, 2));

			event.accept();
		}
	}
	else
	{
		if (mMouseDownItem == NULL) deselectAll();

		mDiagramContextMenu.popup(mapToGlobal(event.screenPos()) + QPoint(2, 2));
		event.accept();
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::defaultMouseDoubleClickEvent(DiagramMouseEvent& event)
{
	if (mMouseDownItem && mMouseDownItem->isSelected() && mSelectedItems.size() == 1)
	{
		mMouseDownItem->mouseDoubleClickEvent(event);

		if (!event.isAccepted())
		{
			properties();

			event.accept();
			update();
		}
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::defaultMousePressEvent(DiagramMouseEvent& event)
{
	mMouseState = MouseSelect;

	if (event.modifiers() & Qt::ShiftModifier) mMouseDownItem = childAt(event.diagramPos());
	else mMouseDownItem = itemAt(event.diagramPos());

	if (mMouseDownItem)
		mMouseDownItemPos = mMouseDownItem->mapFromDiagram(event.buttonDownDiagramPos());

	if (mMouseDownItem && mMouseDownItem->isSelected() && mSelectedItems.size() == 1)
		mMouseDownItem->mousePressEvent(event);
}
//--------------------------------------------------------------------------------------------------
void Diagram::defaultMouseMoveEvent(DiagramMouseEvent& event)
{
	QPointF deltaPos;
	bool canMove = true;

	if (event.buttons() & Qt::LeftButton)
	{
		switch (mMouseState)
		{
		case MouseSelect:
			if (event.isDragged())
			{
				if (mMouseDownItem && mMouseDownItem->isSelected())
				{
					bool resizeItem = (mSelectedItems.size() == 1 &&
						mSelectedItems.first()->hasSelectedPoint() &&
						mSelectedItems.first()->selectedPoint()->isControlPoint());
					mMouseState = (resizeItem) ? MouseResizeItem : MouseMoveItems;
				}
				else mMouseState = MouseRubberBand;
			}
			break;

		case MouseMoveItems:
			for(QList<DiagramItem*>::Iterator itemIter = mSelectedItems.begin();
				canMove && itemIter != mSelectedItems.end(); itemIter++)
			{
				canMove = (*itemIter)->canMove();
			}

			if (canMove)
			{
				deltaPos = tryMove(mSelectedItems, roundToGrid(roundToGrid(event.diagramPos()) -
					mMouseDownItem->mapToDiagram(mMouseDownItemPos)));
				mUndoStack.push(new DiagramMoveItemsDeltaCommand(mSelectedItems, deltaPos, DoNotPlace));
				updateSelectionCenter();
			}
			break;

		case MouseResizeItem:
			if (mMouseDownItem->canResize())
			{
                mUndoStack.push(new DiagramResizeItemCommand(mMouseDownItem->selectedPoint(),
                    tryResize(mMouseDownItem->selectedPoint(), roundToGrid(event.diagramPos())), DoNotPlace, true));
                updateSelectionCenter();
			}
			break;

		default:
			break;
		}

        if (mMouseDownItem && mMouseDownItem->isSelected() && mSelectedItems.size() == 1)
            mMouseDownItem->mouseMoveEvent(event);
	}
    // Refaat Gabr 2016-12-16 (begin)
    else        // show tooltip
    {
        for(QList<DiagramItem*>::Iterator itemIter = mSelectedItems.begin();
            itemIter != mSelectedItems.end(); itemIter++)
        {
            QToolTip::showText( this->mapToGlobal( QPoint( 0, 0 ) ), (*itemIter)->caption1() );
        }
    }
    // Refaat Gabr 2016-12-16 (end)
}
//--------------------------------------------------------------------------------------------------
void Diagram::defaultMouseReleaseEvent(DiagramMouseEvent& event)
{
	bool controlDown = ((event.modifiers() & Qt::ControlModifier) != 0);
	bool canMove = true;
	QPointF deltaPos;

	switch (mMouseState)
	{
	case MouseSelect:
		if (!controlDown) deselectAll();
		if (mMouseDownItem)
		{
			if (controlDown && mMouseDownItem->isSelected())
				deselectItem(mMouseDownItem);
			else selectItem(mMouseDownItem);

			updateSelectionCenter();
		}
		break;

	case MouseMoveItems:
		for(QList<DiagramItem*>::Iterator itemIter = mSelectedItems.begin();
			canMove && itemIter != mSelectedItems.end(); itemIter++)
		{
			canMove = (*itemIter)->canMove();
		}

		if (canMove)
		{
            deltaPos = tryMove(mSelectedItems, roundToGrid(roundToGrid(event.diagramPos()) -
                mMouseDownItem->mapToDiagram(mMouseDownItemPos)));
            mUndoStack.push(new DiagramMoveItemsDeltaCommand(mSelectedItems, deltaPos, (lastMouseEvent().modifiers() & Qt::ShiftModifier) ? PlaceStrict : PlaceLoose));
            updateSelectionCenter();
		}
		break;

	case MouseResizeItem:
		if (mMouseDownItem->canResize())
		{
			mUndoStack.push(new DiagramResizeItemCommand(mMouseDownItem->selectedPoint(),
				mMouseDownItem->mapToDiagram(mMouseDownItem->selectedPoint()->position()), (lastMouseEvent().modifiers() & Qt::ShiftModifier) ? PlaceStrict : PlaceLoose, true));
			updateSelectionCenter();
		}
		break;

	case MouseRubberBand:
		if (!controlDown) deselectAll();
		selectItems(rectFromPoints(event.diagramPos(), event.buttonDownDiagramPos()));
		updateSelectionCenter();
		break;

	default:
		break;
	}

	if (mMouseDownItem && mMouseDownItem->isSelected() && mSelectedItems.size() == 1)
		mMouseDownItem->mouseReleaseEvent(event);

	mMouseState = MouseReady;
	mConsecutivePastes = 0;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::newMousePressEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void Diagram::newMouseMoveEvent(DiagramMouseEvent& event)
{
	if (mNewItem)
	{
        QPointF diagramPos = roundToGrid(event.diagramPos());
        DiagramItemPoint* itemPoint = mNewItem->point(1);

		switch (mNewItem->placeType())
		{
		case DiagramItem::PlaceMouseDownAndUp:
			if (event.isDragged())
			{
				mNewItem->setPosition(roundToGrid(event.buttonDownDiagramPos()));
				if (itemPoint && itemPoint->isControlPoint())
					mNewItem->resizeEvent(itemPoint, diagramPos);
			}
			else
				mNewItem->setPosition(diagramPos);
			break;

		case DiagramItem::PlaceTwoClick:
			if (mNewClickCount % 2 == 1)
			{
				if (itemPoint && itemPoint->isControlPoint())
					mNewItem->resizeEvent(itemPoint, diagramPos);
			}
			else
				mNewItem->setPosition(diagramPos);
			break;

		default:    // DiagramItem::PlaceMouseUp
			mNewItem->setPosition(diagramPos);
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::newMouseReleaseEvent(DiagramMouseEvent& event)
{
	if (mNewItem)
	{
		bool itemAdded = false;
		bool adjustPoints = false;

		QRectF newItemRect = mNewItem->mapToDiagram(mNewItem->boundingRect());
		if (newItemRect.width() == 0) newItemRect.setWidth(kMinimumRectSize);
		if (newItemRect.height() == 0) newItemRect.setHeight(kMinimumRectSize);

		if (!mNewItem->isSuperfluous() &&
			(!mForcingItemsInside || diagramRect().contains(newItemRect)))
		{
			itemAdded = (mNewItem->placeType() != DiagramItem::PlaceTwoClick ||
				mNewClickCount % 2 == 1);
			adjustPoints = (itemAdded && mNewItem->placeType() != DiagramItem::PlaceMouseUp);
			mNewClickCount++;
		}
		else adjustPoints = (mNewItem->placeType() != DiagramItem::PlaceMouseUp);

		if (itemAdded)
		{
			DiagramItem* newItem = mNewItem;
			mNewItem = NULL;
			setNewItem(newItem->copy());

            newItem->aboutToChangeEvent(DiagramItem::AddNewItemToDiagram, QVariant());
            mUndoStack.push(new DiagramAddItemsCommand(this, newItem, (lastMouseEvent().modifiers() & Qt::ShiftModifier) ? PlaceStrict : PlaceLoose));
            newItem->changedEvent(DiagramItem::AddNewItemToDiagram, QVariant());
            emit numberOfItemsChanged(QString::number(numberOfItems()));
		}

        if (adjustPoints && mNewItem)
        {
            if (mNewItem->point(0)) mNewItem->point(0)->setPosition(0.0, 0.0);
            if (mNewItem->point(1)) mNewItem->point(1)->setPosition(0.0, 0.0);
            mNewItem->setPosition(roundToGrid(event.diagramPos()));
        }

	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::paint(DiagramPainter* painter)
{
	QList<DiagramItem*>::Iterator itemIter;

	DiagramBase::paint(painter);

	if (mode() == NewMode && mNewItem)
	{
		qreal itemScale = unitsScale(units(), mNewItem->units());

		painter->save();
        painter->translate(mNewItem->position());
        painter->scale(itemScale, itemScale);
        mNewItem->render(painter);
		painter->restore();
	}

	for(itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		drawItemForeground(painter, *itemIter);

	drawHotpoints(painter, mNewItem);
	for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
		drawHotpoints(painter, *itemIter);

	if (mMouseState == MouseRubberBand) drawRubberBand(painter);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::drawItems(DiagramPainter* painter, const QRectF& rect)
{
	Q_UNUSED(rect);

	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		if ((*itemIter)->isVisible())
		{
			qreal scaleFactor = unitsScale(units(), (*itemIter)->units());

			painter->save();
			painter->translate((*itemIter)->position());
			painter->scale(scaleFactor, scaleFactor);
			(*itemIter)->render(painter);
			painter->restore();
		}
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::drawItemForeground(DiagramPainter* painter, DiagramItem* item)
{
	if (item && item->isSelected())
	{
		QList<DiagramItemPoint*> points = item->points();
		DiagramItemPoint* point;
		QRectF rect;

		QPen pen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
		pen.setCosmetic(true);

		qreal itemScale;

		for(QList<DiagramItemPoint*>::Iterator pointIter = points.begin();
			pointIter != points.end(); pointIter++)
		{
			point = *pointIter;

			itemScale = unitsScale(units(), item->units());

			painter->save();
			painter->translate(item->position());
			painter->scale(itemScale, itemScale);
			painter->translate((*pointIter)->position());

			if (point->isControlPoint())
			{
				rect = point->boundingRect();

				painter->setPen(pen);
				painter->setBrush(painter->outputBrush(DiagramPainter::ResizePoint));

                painter->drawRect(rect);
				if (point->isConnectionPoint())
				{
					// Try to adjust for rendering errors with cosmetic pen
					rect.setRect(rect.left() + rect.width() / point->size().width(),
								 rect.top() + rect.height() / point->size().height(),
								 rect.width() - 2 * rect.width() / point->size().width(),
								 rect.height() - 2 * rect.height() / point->size().height());

					painter->drawLine(rect.topLeft(), rect.bottomRight());
					painter->drawLine(rect.bottomLeft(), rect.topRight());
				}
			}
            else if (point->isConnectionPoint())
			{
				QBrush brush = painter->outputBrush(DiagramPainter::Background);
                brush.setColor(QColor(255 - brush.color().red(),
                    255 - brush.color().green(), 255 - brush.color().blue(), 192));
                pen.setBrush(brush);

				rect = point->boundingRect();

				painter->setPen(pen);

				painter->drawLine(rect.topLeft(), rect.bottomRight());
				painter->drawLine(rect.bottomLeft(), rect.topRight());
			}

			painter->restore();
		}
	}
}
//--------------------------------------------------------------------------------------------------
void Diagram::drawHotpoints(DiagramPainter* painter, DiagramItem* item)
{
	if (item)
	{


        QList<DiagramItemPoint*> itemPoints, otherItemPoints;
        itemPoints = item->points();

        QList<DiagramItemPoint*>::Iterator pointIter, otherItemPointIter;
        QList<DiagramItem*>::Iterator otherItemIter;

        if(false && (item->uniqueKey() == "line"))
        {
            qDebug() << "item selected is: " << item->uniqueKey() << "index" << mItems.indexOf(item);
        }

        for(pointIter = itemPoints.begin(); pointIter != itemPoints.end(); pointIter++)
		{

			for(otherItemIter = mItems.begin(); otherItemIter != mItems.end(); otherItemIter++)
			{
                if ((item != (*otherItemIter)) && ((*otherItemIter)->uniqueKey()!="line") && ((*otherItemIter)->uniqueKey()!="arc") )
				{
					otherItemPoints = (*otherItemIter)->points();

					for(otherItemPointIter = otherItemPoints.begin();
						otherItemPointIter != otherItemPoints.end(); otherItemPointIter++)
					{
                        if ((*pointIter)->shouldConnect(*otherItemPointIter,
                            (lastMouseEvent().modifiers() & Qt::ShiftModifier) ? PlaceStrict : PlaceLoose))
                        {
							// Draw hotpoint
							QRectF rect = (*pointIter)->boundingRect();
							rect.adjust(-rect.width(), -rect.height(), rect.width(), rect.height());

							qreal itemScale = unitsScale(units(), item->units());

							painter->save();

							painter->translate(item->position());
							painter->scale(itemScale, itemScale);
							painter->translate((*pointIter)->position());
							painter->scale(1.0 / itemScale, 1.0 / itemScale);

							painter->setBrush(painter->outputBrush(DiagramPainter::Hotpoint));
							painter->setPen(QPen(painter->outputBrush(DiagramPainter::Hotpoint), 1));

                            painter->drawEllipse(rect);

							painter->restore();
						}
                        else if((*otherItemPointIter)->isConnectionPoint())    // highlight all connection points to the user; added by UT
                        {
                            // Draw point
                            QRectF rect = (*otherItemPointIter)->boundingRect();
                            rect.adjust(-rect.width(), -rect.height(), rect.width(), rect.height());

                            qreal itemScale = unitsScale(units(), (*otherItemIter)->units());

                            painter->save();

                            painter->translate((*otherItemIter)->position());
                            painter->scale(itemScale, itemScale);
                            painter->translate((*otherItemPointIter)->position());
                            painter->scale(0.25 / itemScale, 0.25 / itemScale);
                            if((*otherItemPointIter)->isInputPoint())
                            {
                                painter->setBrush(QBrush(QColor(0,0,255)));
                                painter->setPen(QPen(QBrush(QColor(0,0,255)),1));
                            }
                            else if((*otherItemPointIter)->isOutputPoint())
                            {
                                painter->setBrush(QBrush(QColor(255,0,0)));
                                painter->setPen(QPen(QBrush(QColor(255,0,0)),1));
                            }


                            painter->drawEllipse(rect);

                            painter->restore();
                        }
                    }
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::writeXmlAttributes(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeAttribute("width", QString::number(width()));
	xmlWriter.writeAttribute("height", QString::number(height()));
	xmlWriter.writeAttribute("units", QString::number((quint16)units()));

	xmlWriter.writeAttribute("borderWidth", QString::number(borderWidth()));
	xmlWriter.writeAttribute("grid", QString::number(grid()));

	xmlWriter.writeAttribute("alignment", QString::number((quint32)alignment()));
	xmlWriter.writeAttribute("viewportAnchor", QString::number((quint32)viewportAnchor()));
	xmlWriter.writeAttribute("selectionMode", QString::number((quint32)selectionMode()));
	xmlWriter.writeAttribute("forceItemsInside", (isForcingItemsInside()) ? "true" : "false");

	xmlWriter.writeAttribute("gridStyle", QString::number((quint16)gridStyle()));
	xmlWriter.writeAttribute("majorGridSpacing", QString::number(majorGridSpacing()));
	xmlWriter.writeAttribute("minorGridSpacing", QString::number(minorGridSpacing()));
}
//--------------------------------------------------------------------------------------------------
void Diagram::writeXmlChildElements(QXmlStreamWriter& xmlWriter)
{
	xmlWriter.writeStartElement("items");
	writeItemsToXml(xmlWriter, items());
	xmlWriter.writeEndElement();
}
//--------------------------------------------------------------------------------------------------
void Diagram::readXmlAttributes(QXmlStreamReader& xmlReader)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	if (attributes.hasAttribute("units"))
		setUnits((DiagramUnits)attributes.value("units").toString().toUShort());

	if (attributes.hasAttribute("width"))
		setSize(attributes.value("width").toString().toDouble(), height());
	if (attributes.hasAttribute("height"))
		setSize(width(), attributes.value("height").toString().toDouble());

	if (attributes.hasAttribute("borderWidth"))
		setBorderWidth(attributes.value("borderWidth").toString().toDouble());
	if (attributes.hasAttribute("grid"))
		setGrid(attributes.value("grid").toString().toDouble());

	if (attributes.hasAttribute("alignment"))
		setAlignment((Qt::Alignment)attributes.value("alignment").toString().toUInt());
	if (attributes.hasAttribute("viewportAnchor"))
		setViewportAnchor((ViewportAnchor)attributes.value("viewportAnchor").toString().toUInt());
	if (attributes.hasAttribute("selectionMode"))
		setSelectionMode((Qt::ItemSelectionMode)attributes.value("selectionMode").toString().toUInt());

	if (attributes.hasAttribute("forceItemsInside"))
		setForcingItemsInside(attributes.value("forceItemsInside").toString().toLower() == "true");

	if (attributes.hasAttribute("gridStyle"))
		setGridStyle((GridStyle)attributes.value("gridStyle").toString().toUShort());
	if (attributes.hasAttribute("majorGridSpacing"))
		setMajorGridSpacing(attributes.value("majorGridSpacing").toString().toInt());
	if (attributes.hasAttribute("minorGridSpacing"))
		setMinorGridSpacing(attributes.value("minorGridSpacing").toString().toInt());
}
//--------------------------------------------------------------------------------------------------
void Diagram::readXmlChildElement(QXmlStreamReader& xmlReader)
{
	if (xmlReader.name() == "items")
	{
		QList<DiagramItem*> items = readItemsFromXml(xmlReader);
		while (!items.isEmpty()) addItem(items.takeFirst());
	}
	else xmlReader.skipCurrentElement();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Diagram::singleItemProperties(DiagramItem* item)
{
	bool dialogAccepted = false;

	if (item)
	{
		DiagramItemPropertiesDialog* propertiesDialog = item->propertiesDialog(this);
		if (propertiesDialog)
		{
			QList<QPointF> originalPointPositions;
			for(int i = 0; i < item->numberOfPoints(); i++)
				originalPointPositions.append(item->mapToDiagram(item->point(i)->position()));

			propertiesDialog->setProperties(item->properties());
			propertiesDialog->setPosition(item->mapToDiagram(item->mapFromParent(item->position())));
			propertiesDialog->setPointPositions(originalPointPositions);

			dialogAccepted = (propertiesDialog->exec() == QDialog::Accepted);
			if (dialogAccepted)
			{
				QList<QPointF> newPointPositions = propertiesDialog->pointPositions();
				QHash<QString,QVariant> itemProperties = item->properties();

				QHash<QString,QVariant> dialogProperties = propertiesDialog->properties();
				for(QHash<QString,QVariant>::Iterator iter = dialogProperties.begin();
				iter != dialogProperties.end(); iter++)
				{
					itemProperties[iter.key()] = iter.value();
				}

				QUndoCommand* command = new DiagramItemPropertiesCommand(item, itemProperties);

				if (item->canResize())
				{
					for(int i = 0; i < originalPointPositions.size() && i < newPointPositions.size(); i++)
					{
						if (originalPointPositions[i] != newPointPositions[i])
						{
							new DiagramResizeItemCommand(item->point(i), newPointPositions[i],
														 PlaceLoose, true, command);
						}
					}
				}

				if (item->canMove())
				{
					new DiagramMoveItemsDeltaCommand(item,
						item->mapToParent(item->mapFromDiagram(propertiesDialog->position())) - item->position(),
						PlaceStrict, command);
				}

				mUndoStack.push(command);
			}

			delete propertiesDialog;
		}
	}

	return dialogAccepted;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::multipleItemProperties(const QList<DiagramItem*>& items)
{
	bool dialogAccepted = false;
	DiagramMultipleItemPropertiesDialog::Flags dialogFlags =
		DiagramMultipleItemPropertiesDialog::ItemsUsePalette;

	QList< QHash<QString, QVariant> > originalProperties;
	QList<DiagramItem*>::ConstIterator itemIter;

	for(itemIter = items.begin(); itemIter != items.end(); itemIter++)
	{
		originalProperties.append((*itemIter)->properties());

		if (((*itemIter)->flags() & DiagramItem::UsePalette) == 0)
			dialogFlags = DiagramMultipleItemPropertiesDialog::NoFlags;
	}

	DiagramMultipleItemPropertiesDialog dialog(this, dialogFlags);
	dialog.setProperties(originalProperties);

	dialogAccepted = (dialog.shouldExec() && dialog.exec() == QDialog::Accepted);
	if (dialogAccepted)
	{
		QHash<QString, QVariant> finalProperties = dialog.properties();
		QHash<QString, QVariant> itemProperties;

		QUndoCommand* command = new QUndoCommand("Change Item Properties");

		for(itemIter = items.begin(); itemIter != items.end(); itemIter++)
		{
			itemProperties = (*itemIter)->properties();

			for(QHash<QString, QVariant>::Iterator hIter = finalProperties.begin();
				hIter != finalProperties.end(); hIter++)
			{
				if (itemProperties.contains(hIter.key()))
					itemProperties[hIter.key()] = hIter.value();
			}

			new DiagramItemPropertiesCommand(*itemIter, itemProperties, command);
		}

		mUndoStack.push(command);
	}

	return dialogAccepted;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::diagramProperties()
{
	bool dialogAccept = false;

    DiagramPropertiesWidget* propertiesWidget = new DiagramPropertiesWidget();
    propertiesWidget->layout()->setContentsMargins(0, 0, 0, 0);

    DiagramTwoButtonDialog dialog(this);
    dialog.addWidget(propertiesWidget);
    dialog.addWidget(new QWidget(), 100);
    dialog.addButtonBox();
    dialog.setWindowTitle("Diagram Properties");
	dialog.resize(280, 10);

	propertiesWidget->setUnits(units());
	propertiesWidget->setSize(width(), height());
	propertiesWidget->setBackground(painter()->brushAt(DiagramPainter::Background));
	propertiesWidget->setBorder(borderWidth(), painter()->brushAt(DiagramPainter::Border));
	propertiesWidget->setGrid(grid(), gridStyle(), majorGridSpacing(), minorGridSpacing(),
							  painter()->brushAt(DiagramPainter::Grid));

	if (dialog.exec() == QDialog::Accepted)
	{
		DiagramPropertiesCommand* command = new DiagramPropertiesCommand(this);
		command->setNewUnits(propertiesWidget->units());
		command->setNewSize(propertiesWidget->width(), propertiesWidget->height());
		command->setNewBackground(propertiesWidget->backgroundBrush());
		command->setNewBorder(propertiesWidget->borderWidth(), propertiesWidget->borderBrush());
		command->setNewGrid(propertiesWidget->grid(), propertiesWidget->gridStyle(),
							propertiesWidget->majorGridSpacing(), propertiesWidget->minorGridSpacing(),
							propertiesWidget->gridBrush());

		mUndoStack.push(command);

		updateScrollBars();
		emit mousePositionChanged("");
		emit numberOfItemsChanged(QString::number(numberOfItems()));
		dialogAccept = true;
	}

	return dialogAccept;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::emitCleanChanged(bool clean)
{
	emit cleanChanged(clean ? "" : "Modified");
}
//--------------------------------------------------------------------------------------------------
void Diagram::emitFileStateChanged()
{
	emit fileStateChanged();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPointF Diagram::tryMove(const QList<DiagramItem*>& items, const QPointF& deltaPos)
{
	QPointF actualDeltaPos = deltaPos;

	if (mForcingItemsInside)
	{
		QRectF rect;
		qreal lGrid = grid();

		for(QList<DiagramItem*>::ConstIterator itemIter = items.begin();
			itemIter != items.end(); itemIter++)
		{
			(*itemIter)->setPosition((*itemIter)->position() + deltaPos);

			rect = (*itemIter)->mapToDiagram((*itemIter)->boundingRect());
			if (rect.left() < 0)
			{
				actualDeltaPos.setX(qMax(actualDeltaPos.x(),
					deltaPos.x() - qFloor(rect.left() / lGrid) * lGrid));
			}
			else if (rect.right() > width())
			{
				actualDeltaPos.setX(qMin(actualDeltaPos.x(),
					deltaPos.x() + width() - qCeil(rect.right() / lGrid) * lGrid));
			}

			if (rect.top() < 0)
			{
				actualDeltaPos.setY(qMax(actualDeltaPos.y(),
					deltaPos.y() - qFloor(rect.top() / lGrid) * lGrid));
			}
			else if (rect.bottom() > height())
			{
				actualDeltaPos.setY(qMin(actualDeltaPos.y(),
					deltaPos.y() + height() - qCeil(rect.bottom() / lGrid) * lGrid));
			}

			(*itemIter)->setPosition((*itemIter)->position() - deltaPos);
		}
	}

	return actualDeltaPos;
}
//--------------------------------------------------------------------------------------------------
QPointF Diagram::tryResize(DiagramItemPoint* itemPoint, const QPointF& diagramPos)
{
	QPointF actualDiagramPos = diagramPos;

	Q_UNUSED(itemPoint);

	if (mForcingItemsInside)
	{
		if (actualDiagramPos.x() < 0) actualDiagramPos.setX(0.0);
		else if (actualDiagramPos.x() > width()) actualDiagramPos.setX(roundToGrid(width()));

		if (actualDiagramPos.y() < 0) actualDiagramPos.setY(0.0);
		else if (actualDiagramPos.y() > height()) actualDiagramPos.setY(roundToGrid(height()));
	}

	return actualDiagramPos;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::keepItemsInside(const QList<DiagramItem*>& items)
{
	bool success = false;

	// Ensure that new items are placed inside diagram boundaries
	QRectF diagramRect = Diagram::diagramRect();
	QRectF itemsRect;
	QList<DiagramItem*>::ConstIterator itemIter;

	for(itemIter = items.begin(); itemIter != items.end(); itemIter++)
	{
		if (!itemsRect.isValid())
			itemsRect = (*itemIter)->boundingRect().translated((*itemIter)->position());
		else
		{
			itemsRect = itemsRect.united(
				(*itemIter)->boundingRect().translated((*itemIter)->position()));
		}
	}

	success = (itemsRect.width() <= diagramRect.width() && itemsRect.height() <= diagramRect.height());
	if (success)
	{
		qreal offset;

		offset = itemsRect.right() - diagramRect.right();
		if (offset > 0)
		{
			for(itemIter = items.begin(); itemIter != items.end(); itemIter++)
				(*itemIter)->setPosition((*itemIter)->positionX() - offset, (*itemIter)->positionY());
		}
		offset = itemsRect.left() - diagramRect.left();
		if (offset < 0)
		{
			for(itemIter = items.begin(); itemIter != items.end(); itemIter++)
				(*itemIter)->setPosition((*itemIter)->positionX() - offset, (*itemIter)->positionY());
		}

		offset = itemsRect.bottom() - diagramRect.bottom();
		if (offset > 0)
		{
			for(itemIter = items.begin(); itemIter != items.end(); itemIter++)
				(*itemIter)->setPosition((*itemIter)->positionX(), (*itemIter)->positionY() - offset);
		}
		offset = itemsRect.top() - diagramRect.top();
		if (offset < 0)
		{
			for(itemIter = items.begin(); itemIter != items.end(); itemIter++)
				(*itemIter)->setPosition((*itemIter)->positionX(), (*itemIter)->positionY() - offset);
		}
	}

	return success;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool Diagram::canRotateSelection()
{
	bool canRotate = false;
	QRectF diagramRect = Diagram::diagramRect();
	QRectF itemRect;
	QList<DiagramItem*>::Iterator itemIter;

	for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
		(*itemIter)->rotateEvent(selectionCenter());

	canRotate = true;
	for(itemIter = mSelectedItems.begin();
		canRotate && itemIter != mSelectedItems.end(); itemIter++)
	{
		itemRect = (*itemIter)->mapToDiagram((*itemIter)->boundingRect());
		if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
		if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);

		canRotate = ((*itemIter)->canRotate() &&
			(!mForcingItemsInside || diagramRect.contains(itemRect)));
	}

	for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
		(*itemIter)->rotateBackEvent(selectionCenter());

	return canRotate;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::canRotateBackSelection()
{
	bool canRotate = false;
	QRectF diagramRect = Diagram::diagramRect();
	QRectF itemRect;
	QList<DiagramItem*>::Iterator itemIter;

	for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
		(*itemIter)->rotateBackEvent(selectionCenter());

	canRotate = true;
	for(itemIter = mSelectedItems.begin();
		canRotate && itemIter != mSelectedItems.end(); itemIter++)
	{
		itemRect = (*itemIter)->mapToDiagram((*itemIter)->boundingRect());
		if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
		if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);

		canRotate = ((*itemIter)->canRotate() &&
			(!mForcingItemsInside || diagramRect.contains(itemRect)));
	}

	for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
		(*itemIter)->rotateEvent(selectionCenter());

	return canRotate;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::canFlipSelection()
{
	bool canFlip = false;
	QRectF diagramRect = Diagram::diagramRect();
	QRectF itemRect;
	QList<DiagramItem*>::Iterator itemIter;

	for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
		(*itemIter)->flipEvent(selectionCenter());

	canFlip = true;
	for(itemIter = mSelectedItems.begin(); canFlip && itemIter != mSelectedItems.end(); itemIter++)
	{
		itemRect = (*itemIter)->mapToDiagram((*itemIter)->boundingRect());
		if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
		if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);

		canFlip = ((*itemIter)->canFlip() &&
			(!mForcingItemsInside || diagramRect.contains(itemRect)));
	}

	for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
		(*itemIter)->flipEvent(selectionCenter());

	return canFlip;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::canAlignVerticalSelection()
{
    bool canFlip = false;
    QRectF diagramRect = Diagram::diagramRect();
    QRectF itemRect;
    QList<DiagramItem*>::Iterator itemIter;

    for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
        (*itemIter)->alignVerticalEvent(selectionCenter());

    canFlip = true;
    for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
    {
        itemRect = (*itemIter)->mapToDiagram((*itemIter)->boundingRect());
        if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
        if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);

        canFlip = ((*itemIter)->canFlip() &&
            (!mForcingItemsInside || diagramRect.contains(itemRect)));
    }

    for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
        (*itemIter)->alignVerticalEvent(selectionCenter());

    return canFlip;
}
//--------------------------------------------------------------------------------------------------
bool Diagram::canAlignHorizontalSelection()
{
    bool canFlip = false;
    QRectF diagramRect = Diagram::diagramRect();
    QRectF itemRect;
    QList<DiagramItem*>::Iterator itemIter;

    for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
        (*itemIter)->alignHorizontalEvent(selectionCenter());

    canFlip = true;
    for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
    {
        itemRect = (*itemIter)->mapToDiagram((*itemIter)->boundingRect());
        if (itemRect.width() == 0) itemRect.setWidth(kMinimumRectSize);
        if (itemRect.height() == 0) itemRect.setHeight(kMinimumRectSize);

        canFlip = ((*itemIter)->canFlip() &&
            (!mForcingItemsInside || diagramRect.contains(itemRect)));
    }

    for(itemIter = mSelectedItems.begin(); itemIter != mSelectedItems.end(); itemIter++)
        (*itemIter)->alignHorizontalEvent(selectionCenter());

    return canFlip;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::writeItemsToXml(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	for(QList<DiagramItem*>::ConstIterator itemIter = items.begin();
		itemIter != items.end(); itemIter++)
	{
		if ((*itemIter)->parent() == NULL)
		{
			xmlWriter.writeStartElement((*itemIter)->uniqueKey());
			(*itemIter)->writeXmlAttributes(xmlWriter, items);
			(*itemIter)->writeXmlChildElements(xmlWriter, items);
			xmlWriter.writeEndElement();
		}
	}
}
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> Diagram::readItemsFromXml(QXmlStreamReader& xmlReader)
{
	QList<DiagramItem*> items;
	DiagramItem* item;

	while (xmlReader.readNextStartElement())
	{
		item = itemFactory.create(xmlReader.name().toString());
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
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void Diagram::addActions()
{
	QList<QAction*> lActions;

	addAction("Undo", this, SLOT(undo()), ":/icons/oxygen/edit-undo.png", "Ctrl+Z");
	addAction("Redo", this, SLOT(redo()), ":/icons/oxygen/edit-redo.png", "Ctrl+Shift+Z");
	addAction("Cut", this, SLOT(cut()), ":/icons/oxygen/edit-cut.png", "Ctrl+X");
	addAction("Copy", this, SLOT(copy()), ":/icons/oxygen/edit-copy.png", "Ctrl+C");
	addAction("Paste", this, SLOT(paste()), ":/icons/oxygen/edit-paste.png", "Ctrl+V");
	addAction("Select All", this, SLOT(selectAll()), ":/icons/oxygen/edit-select-all.png", "Ctrl+A");
	addAction("Select None", this, SLOT(deselectAll()), "", "Ctrl+Shift+A");

	addAction("Rotate", this, SLOT(rotateSelection()), ":/icons/oxygen/object-rotate-right.png", "R");
	addAction("Rotate Back", this, SLOT(rotateBackSelection()), ":/icons/oxygen/object-rotate-left.png", "Shift+R");
    addAction("Flip", this, SLOT(flipSelection()), ":/icons/oxygen/object-flip-horizontal.png", "F");
    addAction("Align Vertical", this, SLOT(alignVerticalSelection()), ":/icons/oxygen/align-horizontal-center.png", "V");
    addAction("Align Horizontal", this, SLOT(alignHorizontalSelection()), ":/icons/oxygen/align-vertical-center.png", "H");
    addAction("Delete", this, SLOT(deleteSelection()), ":/icons/oxygen/edit-delete.png", "Delete");

	addAction("Group", this, SLOT(group()), ":/icons/oxygen/merge.png", "Ctrl+G");
	addAction("Ungroup", this, SLOT(ungroup()), ":/icons/oxygen/split.png", "Ctrl+Shift+G");

	addAction("Send Backward", this, SLOT(sendBackward()), ":/icons/oxygen/go-down.png");
	addAction("Bring Forward", this, SLOT(bringForward()), ":/icons/oxygen/go-up.png");
	addAction("Send to Back", this, SLOT(sendToBack()), ":/icons/oxygen/go-bottom.png");
	addAction("Bring to Front", this, SLOT(bringToFront()), ":/icons/oxygen/go-top.png");

	addAction("Insert Point", this, SLOT(insertItemPoint()), "");
	addAction("Remove Point", this, SLOT(removeItemPoint()), "");

	addAction("Zoom In", this, SLOT(zoomIn()), ":/icons/oxygen/zoom-in.png", ".");
	addAction("Zoom Out", this, SLOT(zoomOut()), ":/icons/oxygen/zoom-out.png", ",");
	addAction("Zoom Fit", this, SLOT(zoomFit()), ":/icons/oxygen/zoom-fit-best.png", "/");

	addAction("Properties...", this, SLOT(properties()), ":/icons/oxygen/games-config-board.png");

	lActions = actions();
	lActions[UndoAction]->setEnabled(canUndo());
	lActions[RedoAction]->setEnabled(canRedo());
	connect(&mUndoStack, SIGNAL(canUndoChanged(bool)), lActions[UndoAction], SLOT(setEnabled(bool)));
	connect(&mUndoStack, SIGNAL(canRedoChanged(bool)), lActions[RedoAction], SLOT(setEnabled(bool)));
}
//--------------------------------------------------------------------------------------------------
void Diagram::createContextMenus()
{
	QList<QAction*> lActions = actions();

	mSingleItemContextMenu.addAction(lActions[PropertiesAction]);
	mSingleItemContextMenu.addSeparator();
	mSingleItemContextMenu.addAction(lActions[RotateAction]);
	mSingleItemContextMenu.addAction(lActions[RotateBackAction]);
    mSingleItemContextMenu.addAction(lActions[FlipAction]);
    mSingleItemContextMenu.addAction(lActions[AlignVerticalAction]);
    mSingleItemContextMenu.addAction(lActions[AlignHorizontalAction]);
    mSingleItemContextMenu.addAction(lActions[DeleteAction]);
	mSingleItemContextMenu.addSeparator();
	mSingleItemContextMenu.addAction(lActions[CutAction]);
	mSingleItemContextMenu.addAction(lActions[CopyAction]);
	mSingleItemContextMenu.addAction(lActions[PasteAction]);

	mAlternateSingleItemContextMenu.addAction(lActions[PropertiesAction]);
	mAlternateSingleItemContextMenu.addSeparator();
	mAlternateSingleItemContextMenu.addAction(lActions[InsertPointAction]);
	mAlternateSingleItemContextMenu.addAction(lActions[RemovePointAction]);
	mAlternateSingleItemContextMenu.addSeparator();
	mAlternateSingleItemContextMenu.addAction(lActions[RotateAction]);
	mAlternateSingleItemContextMenu.addAction(lActions[RotateBackAction]);
    mAlternateSingleItemContextMenu.addAction(lActions[FlipAction]);
    mAlternateSingleItemContextMenu.addAction(lActions[AlignVerticalAction]);
    mAlternateSingleItemContextMenu.addAction(lActions[AlignHorizontalAction]);
    mAlternateSingleItemContextMenu.addAction(lActions[DeleteAction]);
	mAlternateSingleItemContextMenu.addSeparator();
	mAlternateSingleItemContextMenu.addAction(lActions[CutAction]);
	mAlternateSingleItemContextMenu.addAction(lActions[CopyAction]);
	mAlternateSingleItemContextMenu.addAction(lActions[PasteAction]);

	mDiagramContextMenu.addAction(lActions[PropertiesAction]);
	mDiagramContextMenu.addSeparator();
	mDiagramContextMenu.addAction(lActions[UndoAction]);
	mDiagramContextMenu.addAction(lActions[RedoAction]);
	mDiagramContextMenu.addSeparator();
	mDiagramContextMenu.addAction(lActions[CutAction]);
	mDiagramContextMenu.addAction(lActions[CopyAction]);
	mDiagramContextMenu.addAction(lActions[PasteAction]);
	mDiagramContextMenu.addSeparator();
	mDiagramContextMenu.addAction(lActions[ZoomInAction]);
	mDiagramContextMenu.addAction(lActions[ZoomOutAction]);
	mDiagramContextMenu.addAction(lActions[ZoomFitAction]);
}
//--------------------------------------------------------------------------------------------------
QAction* Diagram::addAction(const QString& text, QObject* slotObj,
	const char* slotFunction, const QString& iconPath, const QString& shortcut)
{
	QAction* action = new QAction(text, slotObj);
	connect(action, SIGNAL(triggered()), slotObj, slotFunction);

	if (!iconPath.isEmpty()) action->setIcon(QIcon(iconPath));
	if (!shortcut.isEmpty()) action->setShortcut(QKeySequence(shortcut));

	QAbstractScrollArea::addAction(action);
	return action;
}
//--------------------------------------------------------------------------------------------------
