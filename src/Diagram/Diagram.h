/* Diagram.h
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


#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <DiagramBase.h>
#include <DiagramItemFactory.h>
#include <qthread.h>
#include <Pipeline.h>

class DiagramItem;
class DiagramItemPoint;

class Diagram : public DiagramBase
{
	Q_OBJECT

public:
	enum MouseState { MouseReady, MouseSelect, MouseMoveItems, MouseResizeItem, MouseRubberBand };
	enum ActionIndex { UndoAction, RedoAction, CutAction, CopyAction, PasteAction,
        SelectAllAction, SelectNoneAction, RotateAction, RotateBackAction, FlipAction, AlignVerticalAction, AlignHorizontalAction,DeleteAction,
		GroupAction, UngroupAction, SendBackwardAction, BringForwardAction, SendToBackAction, BringToFrontAction,
		InsertPointAction, RemovePointAction, ZoomInAction, ZoomOutAction, ZoomFitAction,
        PropertiesAction, NumberOfActions };

	static const qreal kMinimumRectSize;

private:
	Qt::ItemSelectionMode mSelectionMode;
	bool mForcingItemsInside;

	QList<DiagramItem*> mItems;
	QList<DiagramItem*> mSelectedItems;
    QPointF mSelectionCenter;
    QPointF mSelectionVertical;
    QPointF mSelectionHorizontal;
    DiagramItem* mNewItem;

	QUndoStack mUndoStack;

	MouseState mMouseState;
	DiagramItem* mMouseDownItem;
	QPointF mMouseDownItemPos;

	int mNewClickCount;
	int mConsecutivePastes;

	QMenu mSingleItemContextMenu;
	QMenu mAlternateSingleItemContextMenu;
	QMenu mDiagramContextMenu;

    QThread mRunThread;

public:
    Pipeline mainPipeline;
    bool mPipelineRunning;


public:
	Diagram();
	virtual ~Diagram();

	void setSelectionMode(Qt::ItemSelectionMode mode);
	void setForcingItemsInside(bool inside);
	Qt::ItemSelectionMode selectionMode() const;
	bool isForcingItemsInside() const;

	// Items
	void addItem(DiagramItem* item);
	void insertItem(int index, DiagramItem* item);
	void removeItem(DiagramItem* item);
	void clearItems();
	QList<DiagramItem*> items() const;
	int numberOfItems() const;
	int itemIndex(DiagramItem* item) const;
	bool containsItem(DiagramItem* item) const;
	void reorderItems(const QList<DiagramItem*>& items);

	QList<DiagramItem*> items(const QRectF& diagramRect) const;
	QList<DiagramItem*> childItems(const QRectF& diagramRect) const;
	DiagramItem* itemAt(const QPointF& diagramPos) const;
	DiagramItem* childAt(const QPointF& diagramPos) const;
	QRectF itemsRect() const;
	QRectF itemsShapeRect() const;

	void selectItem(DiagramItem* item);
	void deselectItem(DiagramItem* item);
	void selectItems(const QRectF& diagramRect);
	QList<DiagramItem*> selectedItems() const;
	int numberOfSelectedItems() const;

	void updateSelectionCenter();
	QPointF selectionCenter() const;

	void setNewItem(DiagramItem* item);
	DiagramItem* newItem() const;

	void jumpToItem(DiagramItem* item);

	// Undo
	bool canUndo() const;
	bool canRedo() const;
	void setClean();
	bool isClean() const;

	// Render
	void render(DiagramPainter* painter, const QRectF& rect);

public slots:
	void setDefaultMode();
    void setZoomMode();
    void setRunPipelineMode();
    void setStopPipelineMode();
    void setScrollMode();
	void setNewMode(DiagramItem* item);
	void setNewMode(const QString& key);
	void setNonInteractiveMode();

	void selectAll();
	void deselectAll();

	void undo();
	void redo();

	void cut();
	void copy();
	void paste();

	void rotateSelection();
	void rotateBackSelection();
	void flipSelection();
	void deleteSelection();

    void alignVerticalSelection();
    void alignHorizontalSelection();

	void sendBackward();
	void bringForward();
	void sendToBack();
	void bringToFront();

	void insertItemPoint();
	void removeItemPoint();

	void group();
	void ungroup();

    bool save(const QString& path);
    bool load(const QString& path);
    void clear();

	bool properties();

signals:
	void fileStateChanged();

	void modeChanged(const QString& text);
    void modeReset();

	void cleanChanged(const QString& text);
	void numberOfItemsChanged(const QString& text);

protected:
	virtual void defaultContextMenuEvent(DiagramMouseEvent& event);
	virtual void defaultMouseDoubleClickEvent(DiagramMouseEvent& event);
	virtual void defaultMousePressEvent(DiagramMouseEvent& event);
	virtual void defaultMouseMoveEvent(DiagramMouseEvent& event);
	virtual void defaultMouseReleaseEvent(DiagramMouseEvent& event);

	virtual void newMousePressEvent(DiagramMouseEvent& event);
	virtual void newMouseMoveEvent(DiagramMouseEvent& event);
	virtual void newMouseReleaseEvent(DiagramMouseEvent& event);

	virtual void paint(DiagramPainter* painter);

	virtual void drawItems(DiagramPainter* painter, const QRectF& rect);
	virtual void drawItemForeground(DiagramPainter* painter, DiagramItem* item);
	virtual void drawHotpoints(DiagramPainter* painter, DiagramItem* item);

	virtual void writeXmlAttributes(QXmlStreamWriter& xmlWriter);
	virtual void writeXmlChildElements(QXmlStreamWriter& xmlWriter);
	virtual void readXmlAttributes(QXmlStreamReader& xmlReader);
	virtual void readXmlChildElement(QXmlStreamReader& xmlReader);

	virtual bool singleItemProperties(DiagramItem* item);
	virtual bool multipleItemProperties(const QList<DiagramItem*>& items);
	virtual bool diagramProperties();

private slots:
	void emitCleanChanged(bool clean);
	void emitFileStateChanged();

private:
	QPointF tryMove(const QList<DiagramItem*>& items, const QPointF& deltaPos);
	QPointF tryResize(DiagramItemPoint* itemPoint, const QPointF& diagramPos);
	bool keepItemsInside(const QList<DiagramItem*>& items);

	bool canRotateSelection();
	bool canRotateBackSelection();
    bool canFlipSelection();
    bool canAlignVerticalSelection();
    bool canAlignHorizontalSelection();

	void writeItemsToXml(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	QList<DiagramItem*> readItemsFromXml(QXmlStreamReader& xmlReader);

	void addActions();
	void createContextMenus();
	QAction* addAction(const QString& text, QObject* slotObj, const char* slotFunction,
		const QString& iconPath = QString(), const QString& shortcut = QString());

public:
	static DiagramItemFactory itemFactory;
};

#endif
