/* DiagramUndo.cpp
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

#include <DiagramUndo.h>
#include <Diagram.h>
#include <DiagramItem.h>
#include <DiagramItemPoint.h>

DiagramUndoCommand::DiagramUndoCommand(Diagram* diagram, QUndoCommand* parent)
	: QUndoCommand(parent)
{
	mDiagram = diagram;
}
//--------------------------------------------------------------------------------------------------
DiagramUndoCommand::DiagramUndoCommand(Diagram* diagram, const QString& title,
	QUndoCommand* parent) : QUndoCommand(title, parent)
{
	mDiagram = diagram;
}
//--------------------------------------------------------------------------------------------------
DiagramUndoCommand::~DiagramUndoCommand()
{
	mDiagram = NULL;
}
//--------------------------------------------------------------------------------------------------
Diagram* DiagramUndoCommand::diagram() const
{
	return mDiagram;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemsUndoCommand::DiagramItemsUndoCommand(DiagramItem* item,
	const QString& title, QUndoCommand* parent) : DiagramUndoCommand(item->diagram(), title, parent)
{
	mItems.append(item);
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
DiagramItemsUndoCommand::DiagramItemsUndoCommand(QList<DiagramItem*> items,
	const QString& title, QUndoCommand* parent) :
	DiagramUndoCommand(items.first()->diagram(), title, parent)
{
	mItems = items;
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
DiagramItemsUndoCommand::DiagramItemsUndoCommand(Diagram* diagram, DiagramItem* item,
	const QString& title, QUndoCommand* parent) : DiagramUndoCommand(diagram, title, parent)
{
	mItems.append(item);
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
DiagramItemsUndoCommand::DiagramItemsUndoCommand(Diagram* diagram, QList<DiagramItem*> items,
	const QString& title, QUndoCommand* parent) : DiagramUndoCommand(diagram, title, parent)
{
	mItems = items;
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
DiagramItemsUndoCommand::~DiagramItemsUndoCommand()
{
	mItems.clear();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QList<DiagramItem*> DiagramItemsUndoCommand::items() const
{
	return mItems;
}
//--------------------------------------------------------------------------------------------------
bool DiagramItemsUndoCommand::isUndone() const
{
	return mUndone;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::redo()
{
	mUndone = false;
	redoOperation();
	QUndoCommand::redo();
}
//--------------------------------------------------------------------------------------------------
#include <typeinfo>
void DiagramItemsUndoCommand::undo()
{
	mUndone = true;
	QUndoCommand::undo();
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QList<QUndoCommand*> DiagramItemsUndoCommand::placeItems(DiagramItemPlaceMode placeMode)
{
	QList<QUndoCommand*> newCommandsAdded;

	Diagram* diagram = mItems.first()->diagram();
	if (diagram)
	{
		QList<DiagramItem*> diagramItems = diagram->items();
		QList<DiagramItemPoint*> itemPoints;
		QList<DiagramItemPoint*> otherItemPoints;
		QList<DiagramItem*>::Iterator itemIter, otherItemIter;
		QList<DiagramItemPoint*>::Iterator itemPointIter, otherItemPointIter;

		for(itemIter = mItems.begin(); 	itemIter != mItems.end(); itemIter++)
		{
			for(otherItemIter = diagramItems.begin();
				otherItemIter != diagramItems.end(); otherItemIter++)
			{
				if (!mItems.contains(*otherItemIter))
				{
					itemPoints = (*itemIter)->points();
					otherItemPoints = (*otherItemIter)->points();

					for(itemPointIter = itemPoints.begin();
						itemPointIter != itemPoints.end(); itemPointIter++)
					{
						for(otherItemPointIter = otherItemPoints.begin();
							otherItemPointIter != otherItemPoints.end(); otherItemPointIter++)
						{
							if ((*itemPointIter)->shouldConnect(*otherItemPointIter, placeMode))
							{
								newCommandsAdded.append(new DiagramItemPointConnectCommand(
									*itemPointIter, *otherItemPointIter, this));
							}
						}
					}
				}
			}
		}
	}

	return newCommandsAdded;
}
//--------------------------------------------------------------------------------------------------
QList<QUndoCommand*> DiagramItemsUndoCommand::unplaceItems()
{
	QList<QUndoCommand*> newCommandsAdded;
	QList<DiagramItem*>::Iterator itemIter;
	QList<DiagramItemPoint*>::Iterator itemPointIter, targetPointIter;
	QList<DiagramItemPoint*> itemPoints, targetPoints;
	DiagramItem* item;
	DiagramItemPoint* itemPoint;

	for(itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
	{
		item = *itemIter;
		itemPoints = item->points();
		for(itemPointIter = itemPoints.begin(); itemPointIter != itemPoints.end(); itemPointIter++)
		{
			itemPoint = *itemPointIter;
			targetPoints = itemPoint->targets();
			for(targetPointIter = targetPoints.begin();
				targetPointIter != targetPoints.end(); targetPointIter++)
			{
				if (!mItems.contains((*targetPointIter)->item()))
				{
					newCommandsAdded.append(new DiagramItemPointDisconnectCommand(
						itemPoint, *targetPointIter, this));
				}
			}
		}
	}

	return newCommandsAdded;
}
//--------------------------------------------------------------------------------------------------
QList<QUndoCommand*> DiagramItemsUndoCommand::disconnectAll(DiagramItemPoint* disconnectAllPoint)
{
	QList<QUndoCommand*> newCommandsAdded;
	QList<DiagramItemPoint*> targetPoints;

	if (disconnectAllPoint)
	{
		targetPoints = disconnectAllPoint->targets();
		for(QList<DiagramItemPoint*>::Iterator targetPointIter = targetPoints.begin();
			targetPointIter != targetPoints.end(); targetPointIter++)
		{
			newCommandsAdded.append(new DiagramItemPointDisconnectCommand(
				disconnectAllPoint, *targetPointIter, this));
		}
	}

	return newCommandsAdded;
}
//--------------------------------------------------------------------------------------------------
QList<QUndoCommand*> DiagramItemsUndoCommand::tryToMaintainConnections(bool allowResize,
	bool checkControlPoints, DiagramItemPoint* pointToSkip)
{
	QList<QUndoCommand*> newCommandsAdded;
	QList<DiagramItem*>::Iterator itemIter;
	QList<DiagramItemPoint*>::Iterator itemPointIter, targetPointIter;
	QList<DiagramItemPoint*> itemPoints, targetPoints;
	DiagramItem* item;
	DiagramItem* targetItem;
	DiagramItemPoint* itemPoint;
	DiagramItemPoint* targetItemPoint;

	for(itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
	{
		item = *itemIter;
		itemPoints = item->points();
		for(itemPointIter = itemPoints.begin(); itemPointIter != itemPoints.end(); itemPointIter++)
		{
			itemPoint = *itemPointIter;
			if (itemPoint != pointToSkip && (checkControlPoints || !itemPoint->isControlPoint()))
			{
				targetPoints = itemPoint->targets();
				for(targetPointIter = targetPoints.begin();
					targetPointIter != targetPoints.end(); targetPointIter++)
				{
					targetItemPoint = *targetPointIter;
					targetItem = targetItemPoint->item();

					if (item->mapToDiagram(itemPoint->position()) !=
						targetItem->mapToDiagram(targetItemPoint->position()))
					{
						// Try to maintain the connection by resizing targetPoint if possible
						if (allowResize && targetItemPoint->isFree() &&
							!itemPoint->shouldDisconnect(targetItemPoint))
						{
							newCommandsAdded.append(new DiagramResizeItemCommand(targetItemPoint,
								item->mapToDiagram(itemPoint->position()), DoNotPlace, false, this));
						}
						else
						{
							newCommandsAdded.append(new DiagramItemPointDisconnectCommand(
								itemPoint, targetItemPoint, this));
						}
					}
				}
			}
		}
	}

	return newCommandsAdded;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::mergeChildren(QList<QUndoCommand*>& children,
	const QList<QUndoCommand*>& otherChildren)
{
	bool mergeSuccess;

	for(QList<QUndoCommand*>::ConstIterator otherIter = otherChildren.begin();
		otherIter != otherChildren.end(); otherIter++)
	{
		mergeSuccess = false;

		for(QList<QUndoCommand*>::Iterator iter = children.begin(); iter != children.end(); iter++)
			mergeSuccess = ((*iter)->mergeWith(*otherIter) && mergeSuccess);

		if (!mergeSuccess)
		{
			if ((*otherIter)->id() == PointConnectType)
			{
				children.append(new DiagramItemPointConnectCommand(
					*static_cast<DiagramItemPointConnectCommand*>(*otherIter), this));
				mergeSuccess = true;
			}
			else if ((*otherIter)->id() == PointDisconnectType)
			{
				children.append(new DiagramItemPointDisconnectCommand(
					*static_cast<DiagramItemPointDisconnectCommand*>(*otherIter), this));
				mergeSuccess = true;
			}
			else if ((*otherIter)->id() == ItemResizeType)
			{
				children.append(new DiagramResizeItemCommand(
					*static_cast<DiagramResizeItemCommand*>(*otherIter), this));
				mergeSuccess = true;
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::moveItem(DiagramItem* item, const QPointF& deltaPos)
{
	if (item) item->moveEvent(item->position() + deltaPos);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::resizeItem(DiagramItemPoint* point, const QPointF& diagramPos)
{
	if (point && point->item())
	{
		point->item()->resizeEvent(point,
			point->item()->mapToParent(point->item()->mapFromDiagram(diagramPos)));
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::rotateItem(DiagramItem* item, const QPointF& diagramPos)
{
	if (item) item->rotateEvent(item->mapToParent(item->mapFromDiagram(diagramPos)));
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::rotateBackItem(DiagramItem* item, const QPointF& diagramPos)
{
	if (item) item->rotateBackEvent(item->mapToParent(item->mapFromDiagram(diagramPos)));
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::flipItem(DiagramItem* item, const QPointF& diagramPos)
{
    if (item) item->flipEvent(item->mapToParent(item->mapFromDiagram(diagramPos)));
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::alignVerticalItem(DiagramItem* item, const QPointF& diagramPos)
{
    if (item) item->alignVerticalEvent(item->mapToParent(item->mapFromDiagram(diagramPos)));
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::alignHorizontalItem(DiagramItem* item, const QPointF& diagramPos)
{
    if (item) item->alignHorizontalEvent(item->mapToParent(item->mapFromDiagram(diagramPos)));
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::insertItemPoint(DiagramItem* item, DiagramItemPoint* point, int index)
{
	if (item) item->insertPointEvent(index, point);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemsUndoCommand::removeItemPoint(DiagramItem* item, DiagramItemPoint* point)
{
	if (item) item->removePointEvent(point);
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPropertiesCommand::DiagramPropertiesCommand(Diagram* diagram,
	QUndoCommand* parent) : QUndoCommand("Change Diagram Properties", parent)
{
	mDiagram = diagram;
	mOriginalUnits = mDiagram->units();
	mOriginalWidth = mDiagram->width();
	mOriginalHeight = mDiagram->height();
	mOriginalBackgroundBrush = mDiagram->painter()->brushAt(DiagramPainter::Background);
	mOriginalBorderWidth = mDiagram->borderWidth();
	mOriginalBorderBrush = mDiagram->painter()->brushAt(DiagramPainter::Border);
	mOriginalGrid = mDiagram->grid();
	mOriginalGridStyle = mDiagram->gridStyle();
	mOriginalMajorGridSpacing = mDiagram->majorGridSpacing();
	mOriginalMinorGridSpacing = mDiagram->minorGridSpacing();
	mOriginalGridBrush = mDiagram->painter()->brushAt(DiagramPainter::Grid);
}
//--------------------------------------------------------------------------------------------------
DiagramPropertiesCommand::~DiagramPropertiesCommand() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::setNewUnits(DiagramUnits units)
{
	mUnits = units;
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::setNewSize(qreal width, qreal height)
{
	mWidth = width;
	mHeight = height;

	// Prepare to delete any items that now fall outside the diagram boundaries
	QList<DiagramItem*> itemsToDelete;

	QList<DiagramItem*> diagramItems = mDiagram->items();
	QRectF diagramRect(0, 0, mWidth, mHeight);
	QRectF itemRect;
	DiagramUnits originalDiagramUnits;

	originalDiagramUnits = mDiagram->units();
	mDiagram->setUnits(mUnits);
	adjustDiagramForNewUnits(originalDiagramUnits, mUnits);
	for(QList<DiagramItem*>::Iterator iter = diagramItems.begin(); iter != diagramItems.end(); iter++)
	{
		itemRect = (*iter)->mapToDiagram((*iter)->boundingRect());
		if (itemRect.width() == 0) itemRect.setWidth(Diagram::kMinimumRectSize);
		if (itemRect.height() == 0) itemRect.setHeight(Diagram::kMinimumRectSize);

		if (!diagramRect.contains(itemRect)) itemsToDelete.append(*iter);
	}
	mDiagram->setUnits(originalDiagramUnits);
	adjustDiagramForNewUnits(mUnits, originalDiagramUnits);

	if (!itemsToDelete.isEmpty())
		new DiagramRemoveItemsCommand(mDiagram, itemsToDelete, this);
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::setNewBackground(const QBrush& brush)
{
	mBackgroundBrush = brush;
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::setNewBorder(qreal width, const QBrush& brush)
{
	mBorderWidth = width;
	mBorderBrush = brush;
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::setNewGrid(qreal grid, DiagramBase::GridStyle style,
	int spacingMajor, int spacingMinor, const QBrush& brush)
{
	mGrid = grid;
	mGridStyle = style;
	mMajorGridSpacing = spacingMajor;
	mMinorGridSpacing = spacingMinor;
	mGridBrush = brush;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::redo()
{
	mDiagram->setUnits(mUnits);
	adjustDiagramForNewUnits(mOriginalUnits, mUnits);
	mDiagram->setSize(mWidth, mHeight);
	mDiagram->setBorderWidth(mBorderWidth);
	mDiagram->setGrid(mGrid);
	mDiagram->setGridStyle(mGridStyle);
	mDiagram->setGridSpacing(mMajorGridSpacing, mMinorGridSpacing);

	mDiagram->painter()->setBrushAt(DiagramPainter::Background, mBackgroundBrush);
	mDiagram->painter()->setBrushAt(DiagramPainter::Border, mBorderBrush);
	mDiagram->painter()->setBrushAt(DiagramPainter::Grid, mGridBrush);

	QUndoCommand::redo();
}
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::undo()
{
	QUndoCommand::undo();

	mDiagram->setUnits(mOriginalUnits);
	adjustDiagramForNewUnits(mUnits, mOriginalUnits);
	mDiagram->setSize(mOriginalWidth, mOriginalHeight);
	mDiagram->setBorderWidth(mOriginalBorderWidth);
	mDiagram->setGrid(mOriginalGrid);
	mDiagram->setGridStyle(mOriginalGridStyle);
	mDiagram->setGridSpacing(mOriginalMajorGridSpacing, mOriginalMinorGridSpacing);

	mDiagram->painter()->setBrushAt(DiagramPainter::Background, mOriginalBackgroundBrush);
	mDiagram->painter()->setBrushAt(DiagramPainter::Border, mOriginalBorderBrush);
	mDiagram->painter()->setBrushAt(DiagramPainter::Grid, mOriginalGridBrush);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
int DiagramPropertiesCommand::id() const
{
	return DiagramUndoCommand::UserUndoType;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPropertiesCommand::adjustDiagramForNewUnits(DiagramUnits oldUnits, DiagramUnits newUnits)
{
	qreal scaleFactor = unitsScale(oldUnits, newUnits);

	if (scaleFactor != 1.0)
	{
		QList<DiagramItem*> lItems = mDiagram->items();

		mDiagram->setSize(mDiagram->width() * scaleFactor, mDiagram->height() * scaleFactor);
		mDiagram->setGrid(mDiagram->grid() * scaleFactor);
		mDiagram->setBorderWidth(mDiagram->borderWidth() * scaleFactor);

		for(QList<DiagramItem*>::Iterator itemIter = lItems.begin();
			itemIter != lItems.end(); itemIter++)
		{
			(*itemIter)->setPosition((*itemIter)->position() * scaleFactor);
			if ((*itemIter)->shouldMatchUnitsWithParent()) (*itemIter)->setUnits(newUnits);
		}
	}
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramAddItemsCommand::DiagramAddItemsCommand(Diagram* diagram, DiagramItem* item,
	DiagramItemPlaceMode placeMode, QUndoCommand* parent) : DiagramItemsUndoCommand(diagram, item, "Add Item", parent)
{
	mPlaceMode = placeMode;
	placeItems(mPlaceMode);
}
//--------------------------------------------------------------------------------------------------
DiagramAddItemsCommand::DiagramAddItemsCommand(Diagram* diagram, const QList<DiagramItem*>& items,
	DiagramItemPlaceMode placeMode, QUndoCommand* parent) : DiagramItemsUndoCommand(diagram, items, "Add Items", parent)
{
	mPlaceMode = placeMode;
	placeItems(mPlaceMode);
}
//--------------------------------------------------------------------------------------------------
DiagramAddItemsCommand::~DiagramAddItemsCommand()
{
	if (isUndone())
	{
		DiagramItem* item;
		while (!mItems.isEmpty())
		{
			item = mItems.takeFirst();
			if (!mDiagram->containsItem(item)) delete item;
		}
	}

	mDiagram = NULL;
}
//--------------------------------------------------------------------------------------------------
void DiagramAddItemsCommand::redoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		mDiagram->addItem(*itemIter);
}
//--------------------------------------------------------------------------------------------------
void DiagramAddItemsCommand::undoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		mDiagram->removeItem(*itemIter);
}
//--------------------------------------------------------------------------------------------------
int DiagramAddItemsCommand::id() const
{
	return AddItemsType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramRemoveItemsCommand::DiagramRemoveItemsCommand(Diagram* diagram, DiagramItem* item,
	QUndoCommand* parent) : DiagramItemsUndoCommand(diagram, item, "Remove Item", parent)
{
	unplaceItems();
}
//--------------------------------------------------------------------------------------------------
DiagramRemoveItemsCommand::DiagramRemoveItemsCommand(Diagram* diagram, const QList<DiagramItem*>& items,
	QUndoCommand* parent) : DiagramItemsUndoCommand(diagram, items, "Remove Items", parent)
{
	unplaceItems();
}
//--------------------------------------------------------------------------------------------------
DiagramRemoveItemsCommand::~DiagramRemoveItemsCommand()
{
	if (!isUndone())
	{
		DiagramItem* item;
		while (!mItems.isEmpty())
		{
			item = mItems.takeFirst();
			if (!mDiagram->containsItem(item)) delete item;
		}
	}

	mDiagram = NULL;
}
//--------------------------------------------------------------------------------------------------
void DiagramRemoveItemsCommand::redoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
	{
		mItemIndex[*itemIter] = mDiagram->itemIndex(*itemIter);
		mDiagram->removeItem(*itemIter);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramRemoveItemsCommand::undoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		mDiagram->insertItem(mItemIndex[*itemIter], *itemIter);
}
//--------------------------------------------------------------------------------------------------
int DiagramRemoveItemsCommand::id() const
{
	return RemoveItemsType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramMoveItemsDeltaCommand::DiagramMoveItemsDeltaCommand(DiagramItem* item,
	const QPointF& deltaPosition, DiagramItemPlaceMode placeMode, QUndoCommand* parent) :
	DiagramItemsUndoCommand(item, "Move Item", parent)
{
	mDeltaPosition = deltaPosition;
	mPlaceMode = placeMode;

	redoOperation();
	mChildren.append(tryToMaintainConnections(true, true, NULL));
	mChildren.append(placeItems(mPlaceMode));
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramMoveItemsDeltaCommand::DiagramMoveItemsDeltaCommand(const QList<DiagramItem*>& items,
	const QPointF& deltaPosition, DiagramItemPlaceMode placeMode, QUndoCommand* parent) :
	DiagramItemsUndoCommand(items, "Move Items", parent)
{
	mDeltaPosition = deltaPosition;
	mPlaceMode = placeMode;

	redoOperation();
	mChildren.append(tryToMaintainConnections(true, true, NULL));
	mChildren.append(placeItems(mPlaceMode));
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramMoveItemsDeltaCommand::~DiagramMoveItemsDeltaCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramMoveItemsDeltaCommand::redoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		moveItem(*itemIter, mDeltaPosition);
}
//--------------------------------------------------------------------------------------------------
void DiagramMoveItemsDeltaCommand::undoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin(); itemIter != mItems.end(); itemIter++)
		moveItem(*itemIter, -mDeltaPosition);
}
//--------------------------------------------------------------------------------------------------
int DiagramMoveItemsDeltaCommand::id() const
{
	return MoveItemsDeltaType;
}
//--------------------------------------------------------------------------------------------------
bool DiagramMoveItemsDeltaCommand::mergeWith(const QUndoCommand* command)
{
	bool mergeSuccess = false;

	if (command && command->id() == MoveItemsDeltaType)
	{
		const DiagramMoveItemsDeltaCommand* moveCommand =
			static_cast<const DiagramMoveItemsDeltaCommand*>(command);
		if (moveCommand && mItems == moveCommand->mItems && mPlaceMode == DoNotPlace)
		{
			mDeltaPosition += moveCommand->mDeltaPosition;
			mPlaceMode = moveCommand->mPlaceMode;

			mergeChildren(mChildren, moveCommand->mChildren);

			mergeSuccess = true;
		}
	}

	return mergeSuccess;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramResizeItemCommand::DiagramResizeItemCommand(DiagramItemPoint* point, const QPointF& diagramPos,
	DiagramItemPlaceMode placeMode, bool disconnect, QUndoCommand* parent) :
	DiagramItemsUndoCommand(point->item(), "Resize Item", parent)
{
	mPoint = point;
	mDiagramPos = diagramPos;
	mOriginalDiagramPos = mPoint->item()->mapToDiagram(mPoint->position());
	mPlaceMode = placeMode;

	redoOperation();
	if (disconnect) mChildren.append(disconnectAll(mPoint));
	mChildren.append(tryToMaintainConnections(true, !mPoint->isFree(), mPoint));

	mChildren.append(placeItems(mPlaceMode));
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramResizeItemCommand::DiagramResizeItemCommand(const DiagramResizeItemCommand& command,
	QUndoCommand* parent) : DiagramItemsUndoCommand(command.mItems, "Resize Item", parent)
{
	mPoint = command.mPoint;
	mDiagramPos = command.mDiagramPos;
	mOriginalDiagramPos = command.mOriginalDiagramPos;
	mPlaceMode = command.mPlaceMode;

	for(QList<QUndoCommand*>::ConstIterator childIter = command.mChildren.begin();
		childIter != command.mChildren.end(); childIter++)
	{
		switch ((*childIter)->id())
		{
		case ItemResizeType:
			mChildren.append(new DiagramResizeItemCommand(
				*static_cast<DiagramResizeItemCommand*>(*childIter), this));
			break;
		case PointConnectType:
			mChildren.append(new DiagramItemPointConnectCommand(
				*static_cast<DiagramItemPointConnectCommand*>(*childIter), this));
			break;
		case PointDisconnectType:
			mChildren.append(new DiagramItemPointDisconnectCommand(
				*static_cast<DiagramItemPointDisconnectCommand*>(*childIter), this));
			break;
		default:
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------
DiagramResizeItemCommand::~DiagramResizeItemCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramResizeItemCommand::redoOperation()
{
	resizeItem(mPoint, mDiagramPos);
}
//--------------------------------------------------------------------------------------------------
void DiagramResizeItemCommand::undoOperation()
{
	resizeItem(mPoint, mOriginalDiagramPos);
}
//--------------------------------------------------------------------------------------------------
int DiagramResizeItemCommand::id() const
{
	return ItemResizeType;
}
//--------------------------------------------------------------------------------------------------
bool DiagramResizeItemCommand::mergeWith(const QUndoCommand* command)
{
	bool mergeSuccess = false;

	if (command && command->id() == ItemResizeType)
	{
		const DiagramResizeItemCommand* resizeCommand =
			static_cast<const DiagramResizeItemCommand*>(command);
		if (resizeCommand && mPoint == resizeCommand->mPoint && mPlaceMode == DoNotPlace)
		{
			mDiagramPos = resizeCommand->mDiagramPos;
			mPlaceMode = resizeCommand->mPlaceMode;

			mergeChildren(mChildren, resizeCommand->mChildren);

			mergeSuccess = true;
		}
	}

	return mergeSuccess;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramRotateItemsCommand::DiagramRotateItemsCommand(DiagramItem* item,
	const QPointF& diagramPos, QUndoCommand* parent) :
	DiagramItemsUndoCommand(item, "Rotate Item", parent)
{
	mDiagramPos = diagramPos;

	redoOperation();
	tryToMaintainConnections(true, true, NULL);
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramRotateItemsCommand::DiagramRotateItemsCommand(const QList<DiagramItem*>& items,
	const QPointF& diagramPos, QUndoCommand* parent) :
	DiagramItemsUndoCommand(items, "Rotate Items", parent)
{
	mDiagramPos = diagramPos;

	redoOperation();
	tryToMaintainConnections(true, true, NULL);
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramRotateItemsCommand::~DiagramRotateItemsCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramRotateItemsCommand::redoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		rotateItem(*itemIter, mDiagramPos);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramRotateItemsCommand::undoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		rotateBackItem(*itemIter, mDiagramPos);
	}
}
//--------------------------------------------------------------------------------------------------
int DiagramRotateItemsCommand::id() const
{
	return RotateItemsType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramRotateBackItemsCommand::DiagramRotateBackItemsCommand(DiagramItem* item,
	const QPointF& diagramPos, QUndoCommand* parent) :
	DiagramItemsUndoCommand(item, "Rotate Back Item", parent)
{
	mDiagramPos = diagramPos;

	redoOperation();
	tryToMaintainConnections(true, true, NULL);
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramRotateBackItemsCommand::DiagramRotateBackItemsCommand(const QList<DiagramItem*>& items,
	const QPointF& diagramPos, QUndoCommand* parent) :
	DiagramItemsUndoCommand(items, "Rotate Back Items", parent)
{
	mDiagramPos = diagramPos;

	redoOperation();
	tryToMaintainConnections(true, true, NULL);
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramRotateBackItemsCommand::~DiagramRotateBackItemsCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramRotateBackItemsCommand::redoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		rotateBackItem(*itemIter, mDiagramPos);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramRotateBackItemsCommand::undoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		rotateItem(*itemIter, mDiagramPos);
	}
}
//--------------------------------------------------------------------------------------------------
int DiagramRotateBackItemsCommand::id() const
{
	return RotateBackItemsType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramFlipItemsCommand::DiagramFlipItemsCommand(DiagramItem* item,
	const QPointF& diagramPos, QUndoCommand* parent) :
	DiagramItemsUndoCommand(item, "Flip Item", parent)
{
	mDiagramPos = diagramPos;

	redoOperation();
	tryToMaintainConnections(true, true, NULL);
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramFlipItemsCommand::DiagramFlipItemsCommand(const QList<DiagramItem*>& items,
	const QPointF& diagramPos, QUndoCommand* parent) :
	DiagramItemsUndoCommand(items, "Flip Items", parent)
{
	mDiagramPos = diagramPos;

	redoOperation();
	tryToMaintainConnections(true, true, NULL);
	undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramFlipItemsCommand::~DiagramFlipItemsCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramFlipItemsCommand::redoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		flipItem(*itemIter, mDiagramPos);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramFlipItemsCommand::undoOperation()
{
	for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
		itemIter != mItems.end(); itemIter++)
	{
		flipItem(*itemIter, mDiagramPos);
	}
}
//--------------------------------------------------------------------------------------------------
int DiagramFlipItemsCommand::id() const
{
	return FlipItemsType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramAlignVerticalItemsCommand::DiagramAlignVerticalItemsCommand(DiagramItem* item,
    const QPointF& diagramPos, QUndoCommand* parent) :
    DiagramItemsUndoCommand(item, "Flip Item", parent)
{
    mDiagramPos = diagramPos;

    redoOperation();
    tryToMaintainConnections(true, true, NULL);
    undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramAlignVerticalItemsCommand::DiagramAlignVerticalItemsCommand(const QList<DiagramItem*>& items,
    const QPointF& diagramPos, QUndoCommand* parent) :
    DiagramItemsUndoCommand(items, "Vertically align Items", parent)
{
    mDiagramPos = diagramPos;

    redoOperation();
    tryToMaintainConnections(true, true, NULL);
    undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramAlignVerticalItemsCommand::~DiagramAlignVerticalItemsCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramAlignVerticalItemsCommand::redoOperation()
{
    for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
        itemIter != mItems.end(); itemIter++)
    {
        alignVerticalItem(*itemIter, mDiagramPos);
    }
}
//--------------------------------------------------------------------------------------------------
void DiagramAlignVerticalItemsCommand::undoOperation()
{
    for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
        itemIter != mItems.end(); itemIter++)
    {
        alignVerticalItem(*itemIter, mDiagramPos);
    }
}
//--------------------------------------------------------------------------------------------------
int DiagramAlignVerticalItemsCommand::id() const
{
    return AlignVerticalItemsType;
}



//--------------------------------------------------------------------------------------------------
DiagramAlignHorizontalItemsCommand::DiagramAlignHorizontalItemsCommand(DiagramItem* item,
    const QPointF& diagramPos, QUndoCommand* parent) :
    DiagramItemsUndoCommand(item, "Flip Item", parent)
{
    mDiagramPos = diagramPos;

    redoOperation();
    tryToMaintainConnections(true, true, NULL);
    undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramAlignHorizontalItemsCommand::DiagramAlignHorizontalItemsCommand(const QList<DiagramItem*>& items,
    const QPointF& diagramPos, QUndoCommand* parent) :
    DiagramItemsUndoCommand(items, "Horizontally align Items", parent)
{
    mDiagramPos = diagramPos;

    redoOperation();
    tryToMaintainConnections(true, true, NULL);
    undoOperation();
}
//--------------------------------------------------------------------------------------------------
DiagramAlignHorizontalItemsCommand::~DiagramAlignHorizontalItemsCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramAlignHorizontalItemsCommand::redoOperation()
{
    for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
        itemIter != mItems.end(); itemIter++)
    {
        alignHorizontalItem(*itemIter, mDiagramPos);
    }
}
//--------------------------------------------------------------------------------------------------
void DiagramAlignHorizontalItemsCommand::undoOperation()
{
    for(QList<DiagramItem*>::Iterator itemIter = mItems.begin();
        itemIter != mItems.end(); itemIter++)
    {
        alignHorizontalItem(*itemIter, mDiagramPos);
    }
}
//--------------------------------------------------------------------------------------------------
int DiagramAlignHorizontalItemsCommand::id() const
{
    return AlignHorizontalItemsType;
}



//--------------------------------------------------------------------------------------------------
DiagramReorderItemsCommand::DiagramReorderItemsCommand(Diagram* diagram,
	const QList<DiagramItem*>& newItemOrder, QUndoCommand* parent) :
	DiagramItemsUndoCommand(newItemOrder, "Reorder Items", parent)
{
	mDiagram = diagram;
	mOriginalItems = mDiagram->items();
}
//--------------------------------------------------------------------------------------------------
DiagramReorderItemsCommand::~DiagramReorderItemsCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramReorderItemsCommand::redoOperation()
{
	mDiagram->reorderItems(mItems);
}
//--------------------------------------------------------------------------------------------------
void DiagramReorderItemsCommand::undoOperation()
{
	mDiagram->reorderItems(mOriginalItems);
}
//--------------------------------------------------------------------------------------------------
int DiagramReorderItemsCommand::id() const
{
	return ReorderItemsType;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemInsertPointCommand::DiagramItemInsertPointCommand(DiagramItem* item, DiagramItemPoint* point,
	int pointIndex, QUndoCommand* parent) : DiagramItemsUndoCommand(item, "Insert Point", parent)
{
	mPoint = point;
	mPointIndex = pointIndex;
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
DiagramItemInsertPointCommand::~DiagramItemInsertPointCommand()
{
	if (mUndone) delete mPoint;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemInsertPointCommand::redoOperation()
{
	insertItemPoint(mItems.first(), mPoint, mPointIndex);
	mUndone = false;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemInsertPointCommand::undoOperation()
{
	mPointIndex = mItems.first()->points().indexOf(mPoint);
	removeItemPoint(mItems.first(), mPoint);
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
int DiagramItemInsertPointCommand::id() const
{
	return InsertItemPointType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemRemovePointCommand::DiagramItemRemovePointCommand(DiagramItem* item, DiagramItemPoint* point,
	QUndoCommand* parent) : DiagramItemsUndoCommand(item, "Remove Point", parent)
{
	mPoint = point;
	mPointIndex = mItems.first()->points().indexOf(mPoint);
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
DiagramItemRemovePointCommand::~DiagramItemRemovePointCommand()
{
	if (!mUndone) delete mPoint;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemRemovePointCommand::redoOperation()
{
	mPointIndex = mItems.first()->points().indexOf(mPoint);
	removeItemPoint(mItems.first(), mPoint);
	mUndone = false;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemRemovePointCommand::undoOperation()
{
	insertItemPoint(mItems.first(), mPoint, mPointIndex);
	mUndone = true;
}
//--------------------------------------------------------------------------------------------------
int DiagramItemRemovePointCommand::id() const
{
	return RemoveItemPointType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesCommand::DiagramItemPropertiesCommand(DiagramItem* item,
	const QHash<QString, QVariant>& newProperties, QUndoCommand* parent) :
	DiagramItemsUndoCommand(item, "Change Item Properties", parent)
{
	mOriginalProperties = item->properties();
	mNewProperties = newProperties;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesCommand::~DiagramItemPropertiesCommand() { }
//--------------------------------------------------------------------------------------------------
void DiagramItemPropertiesCommand::redoOperation()
{
	mItems.first()->setProperties(mNewProperties);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPropertiesCommand::undoOperation()
{
	mItems.first()->setProperties(mOriginalProperties);
}
//--------------------------------------------------------------------------------------------------
int DiagramItemPropertiesCommand::id() const
{
	return ItemPropertiesType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemPointConnectCommand::DiagramItemPointConnectCommand(DiagramItemPoint* point1,
	DiagramItemPoint* point2, QUndoCommand* parent) :
	DiagramUndoCommand(point1->item()->diagram(), "Connect Points", parent)
{
	mPoint1 = point1;
	mPoint2 = point2;

	if (mPoint1->item()->mapToDiagram(mPoint1->position()) !=
		mPoint2->item()->mapToDiagram(mPoint2->position()))
	{
		if (mPoint2->item()->canResize() && mPoint2->isControlPoint())
		{
			mChildren.append(new DiagramResizeItemCommand(mPoint2,
				mPoint1->item()->mapToDiagram(mPoint1->position()), DoNotPlace, true, this));
		}
		else if (mPoint1->item()->canResize() && mPoint1->isControlPoint())
		{
			mChildren.append(new DiagramResizeItemCommand(
				mPoint1, mPoint2->item()->mapToDiagram(mPoint2->position()), DoNotPlace, true, this));
		}
	}
}
//--------------------------------------------------------------------------------------------------
DiagramItemPointConnectCommand::DiagramItemPointConnectCommand(
	const DiagramItemPointConnectCommand& command, QUndoCommand* parent) :
	DiagramUndoCommand(command.mPoint1->item()->diagram(), "Connect Points", parent)
{
	mPoint1 = command.mPoint1;
	mPoint2 = command.mPoint2;

	for(QList<QUndoCommand*>::ConstIterator childIter = command.mChildren.begin();
		childIter != command.mChildren.end(); childIter++)
	{
		if ((*childIter)->id() == DiagramUndoCommand::ItemResizeType)
		{
			mChildren.append(new DiagramResizeItemCommand(
				*static_cast<DiagramResizeItemCommand*>(*childIter), this));
		}
	}
}
//--------------------------------------------------------------------------------------------------
DiagramItemPointConnectCommand::~DiagramItemPointConnectCommand()
{
	mPoint1 = NULL;
	mPoint2 = NULL;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPointConnectCommand::redo()
{
	mPoint1->addTarget(mPoint2);
	mPoint2->addTarget(mPoint1);
	QUndoCommand::redo();
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPointConnectCommand::undo()
{
	QUndoCommand::undo();
	mPoint1->removeTarget(mPoint2);
	mPoint2->removeTarget(mPoint1);
}
//--------------------------------------------------------------------------------------------------
int DiagramItemPointConnectCommand::id() const
{
	return PointConnectType;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemPointDisconnectCommand::DiagramItemPointDisconnectCommand(DiagramItemPoint* point1,
	DiagramItemPoint* point2, QUndoCommand* parent) :
	DiagramUndoCommand(point1->item()->diagram(), "Disconnect Points", parent)
{
	mPoint1 = point1;
	mPoint2 = point2;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPointDisconnectCommand::DiagramItemPointDisconnectCommand(
	const DiagramItemPointDisconnectCommand& command, QUndoCommand* parent) :
	DiagramUndoCommand(command.mPoint1->item()->diagram(), "Disconnect Points", parent)
{
	mPoint1 = command.mPoint1;
	mPoint2 = command.mPoint2;
}
//--------------------------------------------------------------------------------------------------
DiagramItemPointDisconnectCommand::~DiagramItemPointDisconnectCommand()
{
	mPoint1 = NULL;
	mPoint2 = NULL;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPointDisconnectCommand::redo()
{
	mPoint1->removeTarget(mPoint2);
	mPoint2->removeTarget(mPoint1);
	QUndoCommand::redo();
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPointDisconnectCommand::undo()
{
	QUndoCommand::undo();
	mPoint1->addTarget(mPoint2);
	mPoint2->addTarget(mPoint1);
}
//--------------------------------------------------------------------------------------------------
int DiagramItemPointDisconnectCommand::id() const
{
	return PointDisconnectType;
}
//--------------------------------------------------------------------------------------------------
