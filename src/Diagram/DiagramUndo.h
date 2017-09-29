/* DiagramUndo.h
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

#ifndef DIAGRAMUNDO_H
#define DIAGRAMUNDO_H

#include <DiagramTypes.h>
#include <DiagramBase.h>

class Diagram;
class DiagramItem;
class DiagramItemPoint;

class DiagramUndoCommand : public QUndoCommand
{
public:
	enum DiagramUndoCommandType { AddItemsType, RemoveItemsType, MoveItemsDeltaType,
        RotateItemsType, RotateBackItemsType, FlipItemsType, AlignVerticalItemsType,AlignHorizontalItemsType, ItemResizeType,
		ItemPropertiesType, ReorderItemsType, InsertItemPointType, RemoveItemPointType,
		PointConnectType, PointDisconnectType, NumberOfCommands, UserUndoType = 0x10000 };

protected:
	Diagram* mDiagram;

public:
	DiagramUndoCommand(Diagram* diagram, QUndoCommand* parent = NULL);
	DiagramUndoCommand(Diagram* diagram, const QString& title, QUndoCommand* parent = NULL);
	virtual ~DiagramUndoCommand();

	Diagram* diagram() const;
};

//==================================================================================================

class DiagramItemsUndoCommand : public DiagramUndoCommand
{
protected:
	QList<DiagramItem*> mItems;
	QHash<DiagramItem*,int> mItemIndex;
	bool mUndone;

public:
	DiagramItemsUndoCommand(DiagramItem* item,
		const QString& title, QUndoCommand* parent = NULL);
	DiagramItemsUndoCommand(QList<DiagramItem*> items,
		const QString& title, QUndoCommand* parent = NULL);
	DiagramItemsUndoCommand(Diagram* diagram, DiagramItem* item,
		const QString& title, QUndoCommand* parent = NULL);
	DiagramItemsUndoCommand(Diagram* diagram, QList<DiagramItem*> items,
		const QString& title, QUndoCommand* parent = NULL);
	virtual ~DiagramItemsUndoCommand();

	QList<DiagramItem*> items() const;
	bool isUndone() const;

	void redo();
	void undo();

	virtual void redoOperation() = 0;
	virtual void undoOperation() = 0;
	virtual int id() const = 0;

protected:
	virtual QList<QUndoCommand*> placeItems(DiagramItemPlaceMode placeMode);
	virtual QList<QUndoCommand*> unplaceItems();
	virtual QList<QUndoCommand*> disconnectAll(DiagramItemPoint* itemPoint);
	virtual QList<QUndoCommand*> tryToMaintainConnections(bool allowResize,
		bool checkControlPoints, DiagramItemPoint* pointToSkip);
	virtual void mergeChildren(QList<QUndoCommand*>& children,
		const QList<QUndoCommand*>& otherChildren);

	void moveItem(DiagramItem* item, const QPointF& deltaPos);
	void resizeItem(DiagramItemPoint* point, const QPointF& diagramPos);
	void rotateItem(DiagramItem* item, const QPointF& diagramPos);
	void rotateBackItem(DiagramItem* item, const QPointF& diagramPos);
    void flipItem(DiagramItem* item, const QPointF& diagramPos);
    void alignVerticalItem(DiagramItem* item, const QPointF& diagramPos);
    void alignHorizontalItem(DiagramItem* item, const QPointF& diagramPos);
    void insertItemPoint(DiagramItem* item, DiagramItemPoint* point, int index);
	void removeItemPoint(DiagramItem* item, DiagramItemPoint* point);
};


//==================================================================================================

class DiagramPropertiesCommand : public QUndoCommand
{
private:
	Diagram* mDiagram;

	DiagramUnits mUnits, mOriginalUnits;
	qreal mWidth, mOriginalWidth;
	qreal mHeight, mOriginalHeight;
	QBrush mBackgroundBrush, mOriginalBackgroundBrush;
	qreal mBorderWidth, mOriginalBorderWidth;
	QBrush mBorderBrush, mOriginalBorderBrush;
	qreal mGrid, mOriginalGrid;
	DiagramBase::GridStyle mGridStyle, mOriginalGridStyle;
	int mMajorGridSpacing, mOriginalMajorGridSpacing;
	int mMinorGridSpacing, mOriginalMinorGridSpacing;
	QBrush mGridBrush, mOriginalGridBrush;

public:
	DiagramPropertiesCommand(Diagram* diagram, QUndoCommand* parent = NULL);
	~DiagramPropertiesCommand();

	void setNewUnits(DiagramUnits units);
	void setNewSize(qreal width, qreal height);
	void setNewBackground(const QBrush& brush);
	void setNewBorder(qreal width, const QBrush& brush);
	void setNewGrid(qreal grid, DiagramBase::GridStyle style, int spacingMajor,
		int spacingMinor, const QBrush& brush);

	void redo();
	void undo();

	int id() const;

private:
	void adjustDiagramForNewUnits(DiagramUnits oldUnits, DiagramUnits newUnits);
};

//==================================================================================================

class DiagramAddItemsCommand : public DiagramItemsUndoCommand
{
private:
	DiagramItemPlaceMode mPlaceMode;

public:
	DiagramAddItemsCommand(Diagram* diagram, DiagramItem* item,
		DiagramItemPlaceMode placeMode, QUndoCommand* parent = NULL);
	DiagramAddItemsCommand(Diagram* diagram, const QList<DiagramItem*>& items,
		DiagramItemPlaceMode placeMode, QUndoCommand* parent = NULL);
	~DiagramAddItemsCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramRemoveItemsCommand : public DiagramItemsUndoCommand
{
public:
	DiagramRemoveItemsCommand(Diagram* diagram, DiagramItem* item,
		QUndoCommand* parent = NULL);
	DiagramRemoveItemsCommand(Diagram* diagram, const QList<DiagramItem*>& items,
		QUndoCommand* parent = NULL);
	~DiagramRemoveItemsCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramMoveItemsDeltaCommand : public DiagramItemsUndoCommand
{
private:
	QPointF mDeltaPosition;
	DiagramItemPlaceMode mPlaceMode;

	QList<QUndoCommand*> mChildren;

public:
	DiagramMoveItemsDeltaCommand(DiagramItem* item, const QPointF& deltaPosition,
		DiagramItemPlaceMode placeMode, QUndoCommand* parent = NULL);
	DiagramMoveItemsDeltaCommand(const QList<DiagramItem*>& items, const QPointF& deltaPosition,
		DiagramItemPlaceMode placeMode, QUndoCommand* parent = NULL);
	~DiagramMoveItemsDeltaCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
	bool mergeWith(const QUndoCommand* command);
};

//==================================================================================================

class DiagramResizeItemCommand : public DiagramItemsUndoCommand
{
private:
	DiagramItemPoint* mPoint;
	QPointF mDiagramPos;
	QPointF mOriginalDiagramPos;
	DiagramItemPlaceMode mPlaceMode;

	QList<QUndoCommand*> mChildren;

public:
	DiagramResizeItemCommand(DiagramItemPoint* point, const QPointF& diagramPos,
		DiagramItemPlaceMode placeMode, bool disconnect, QUndoCommand* parent = NULL);
	DiagramResizeItemCommand(const DiagramResizeItemCommand& command, QUndoCommand* parent = NULL);
	~DiagramResizeItemCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
	bool mergeWith(const QUndoCommand* command);
};

//==================================================================================================

class DiagramRotateItemsCommand : public DiagramItemsUndoCommand
{
private:
	QPointF mDiagramPos;

public:
	DiagramRotateItemsCommand(DiagramItem* item,
		const QPointF& diagramPos, QUndoCommand* parent = NULL);
	DiagramRotateItemsCommand(const QList<DiagramItem*>& items,
		const QPointF& diagramPos, QUndoCommand* parent = NULL);
	~DiagramRotateItemsCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramRotateBackItemsCommand : public DiagramItemsUndoCommand
{
private:
	QPointF mDiagramPos;

public:
	DiagramRotateBackItemsCommand(DiagramItem* item,
		const QPointF& diagramPos, QUndoCommand* parent = NULL);
	DiagramRotateBackItemsCommand(const QList<DiagramItem*>& items,
		const QPointF& diagramPos, QUndoCommand* parent = NULL);
	~DiagramRotateBackItemsCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramFlipItemsCommand : public DiagramItemsUndoCommand
{
private:
    QPointF mDiagramPos;

public:
    DiagramFlipItemsCommand(DiagramItem* item,
        const QPointF& diagramPos, QUndoCommand* parent = NULL);
    DiagramFlipItemsCommand(const QList<DiagramItem*>& items,
        const QPointF& diagramPos, QUndoCommand* parent = NULL);
    ~DiagramFlipItemsCommand();

    void redoOperation();
    void undoOperation();

    int id() const;
};

//==================================================================================================
class DiagramAlignVerticalItemsCommand : public DiagramItemsUndoCommand
{
private:
    QPointF mDiagramPos;

public:
    DiagramAlignVerticalItemsCommand(DiagramItem* item,
        const QPointF& diagramPos, QUndoCommand* parent = NULL);
    DiagramAlignVerticalItemsCommand(const QList<DiagramItem*>& items,
        const QPointF& diagramPos, QUndoCommand* parent = NULL);
    ~DiagramAlignVerticalItemsCommand();

    void redoOperation();
    void undoOperation();

    int id() const;
};
//==================================================================================================
class DiagramAlignHorizontalItemsCommand : public DiagramItemsUndoCommand
{
private:
    QPointF mDiagramPos;

public:
    DiagramAlignHorizontalItemsCommand(DiagramItem* item,
        const QPointF& diagramPos, QUndoCommand* parent = NULL);
    DiagramAlignHorizontalItemsCommand(const QList<DiagramItem*>& items,
        const QPointF& diagramPos, QUndoCommand* parent = NULL);
    ~DiagramAlignHorizontalItemsCommand();

    void redoOperation();
    void undoOperation();

    int id() const;
};
//==================================================================================================

class DiagramReorderItemsCommand : public DiagramItemsUndoCommand
{
private:
	QList<DiagramItem*> mOriginalItems;

public:
	DiagramReorderItemsCommand(Diagram* diagram,
		const QList<DiagramItem*>& newItemOrder, QUndoCommand* parent = NULL);
	~DiagramReorderItemsCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramItemInsertPointCommand : public DiagramItemsUndoCommand
{
private:
	DiagramItemPoint* mPoint;
	int mPointIndex;
	bool mUndone;

public:
	DiagramItemInsertPointCommand(DiagramItem* item, DiagramItemPoint* point,
		int pointIndex, QUndoCommand* parent = NULL);
	~DiagramItemInsertPointCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramItemRemovePointCommand : public DiagramItemsUndoCommand
{
private:
	DiagramItemPoint* mPoint;
	int mPointIndex;
	bool mUndone;

public:
	DiagramItemRemovePointCommand(DiagramItem* item, DiagramItemPoint* point,
		QUndoCommand* parent = NULL);
	~DiagramItemRemovePointCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramItemPropertiesCommand : public DiagramItemsUndoCommand
{
private:
	QHash<QString, QVariant> mOriginalProperties;
	QHash<QString, QVariant> mNewProperties;

public:
	DiagramItemPropertiesCommand(DiagramItem* item, const QHash<QString,
		QVariant>& newProperties, QUndoCommand* parent = NULL);
	~DiagramItemPropertiesCommand();

	void redoOperation();
	void undoOperation();

	int id() const;
};

//==================================================================================================

class DiagramItemPointConnectCommand : public DiagramUndoCommand
{
private:
	DiagramItemPoint* mPoint1;
	DiagramItemPoint* mPoint2;

	QList<QUndoCommand*> mChildren;

public:
	DiagramItemPointConnectCommand(DiagramItemPoint* point1,
		DiagramItemPoint* point2, QUndoCommand* parent = NULL);
	DiagramItemPointConnectCommand(const DiagramItemPointConnectCommand& command,
		QUndoCommand* parent = NULL);
	~DiagramItemPointConnectCommand();

	void redo();
	void undo();

	int id() const;
};

//==================================================================================================

class DiagramItemPointDisconnectCommand : public DiagramUndoCommand
{
private:
	DiagramItemPoint* mPoint1;
	DiagramItemPoint* mPoint2;

public:
	DiagramItemPointDisconnectCommand(DiagramItemPoint* point1,
		DiagramItemPoint* point2, QUndoCommand* parent = NULL);
	DiagramItemPointDisconnectCommand(const DiagramItemPointDisconnectCommand& command,
		QUndoCommand* parent = NULL);
	~DiagramItemPointDisconnectCommand();

	void redo();
	void undo();

	int id() const;
};

#endif
