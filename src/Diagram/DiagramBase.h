/* DiagramBase.h
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


#ifndef DIAGRAMBASE_H
#define DIAGRAMBASE_H

#include <DiagramTypes.h>

class DiagramBase : public QAbstractScrollArea
{
	Q_OBJECT

public:
    enum Mode { DefaultMode, ScrollMode, ZoomMode, NewMode, NonInteractiveMode, RunPipelineMode,StopPipelineMode, UserMode = 0x100 };
	enum ViewportAnchor { NoAnchor, AnchorViewCenter, AnchorUnderMouse };
	enum GridStyle { GridNone, GridDotted, GridLined, GridGraphPaper };

	const static QVector<qreal> kZoomLevels;

private:
	qreal mWidth, mHeight;
	DiagramUnits mUnits;
	qreal mGrid;
	qreal mBorderWidth;

	DiagramPainter* mPainter;
	Qt::Alignment mAlignment;
	ViewportAnchor mViewportAnchor;

	qreal mZoomLevel;

	Mode mMode;

	GridStyle mGridStyle;
	int mGridSpacingMajor;
	int mGridSpacingMinor;

	QPointF mViewCenter;
	QPointF mMouseDiagramPos;
	DiagramMouseEvent mMouseEvent;

public:
	DiagramBase();
	virtual ~DiagramBase();

	void setSize(qreal width, qreal height);
	void setUnits(DiagramUnits units);
	void setGrid(qreal grid);
	void setBorderWidth(qreal width);
	qreal width() const;
	qreal height() const;
	DiagramUnits units() const;
	qreal grid() const;
	qreal borderWidth() const;
	QRectF diagramRect() const;
	QRectF diagramRectWithBorder() const;

	void setPainter(DiagramPainter* painter);
	void setAlignment(Qt::Alignment alignment);
	void setViewportAnchor(ViewportAnchor anchor);
	DiagramPainter* painter() const;
	Qt::Alignment alignment() const;
	ViewportAnchor viewportAnchor() const;

	void setZoomLevel(qreal level);
	qreal zoomLevel() const;

	void setMode(Mode mode);
	Mode mode() const;

	void setGridStyle(GridStyle style);
	void setGridSpacing(int major, int minor);
	void setMajorGridSpacing(int major);
	void setMinorGridSpacing(int minor);
	GridStyle gridStyle() const;
	int majorGridSpacing() const;
	int minorGridSpacing() const;

	QPointF mapToDiagram(const QPoint& screenPos) const;
	QRectF mapToDiagram(const QRect& screenRect) const;
	QPoint mapFromDiagram(const QPointF& diagramPos) const;
	QRect mapFromDiagram(const QRectF& diagramRect) const;
	QRectF visibleRect() const;

	qreal roundToGrid(qreal value) const;
	QPointF roundToGrid(const QPointF& diagramPos) const;
	DiagramMouseEvent lastMouseEvent() const;

	void render();
	virtual void render(DiagramPainter* painter, const QRectF& rect);

public slots:
	virtual void update();
	virtual void updateScrollBars();

	virtual void zoomIn();
	virtual void zoomOut();
	virtual void zoomFit(const QRectF& rect = QRectF());
	virtual void zoomToLevel(qreal level);

	virtual void centerOn(const QPointF& diagramPos);

	virtual void setDefaultMode();
    virtual void setZoomMode();
    virtual void setRunPipelineMode();
    virtual void setStopPipelineMode();
    virtual void setScrollMode();
	virtual void setNonInteractiveMode();

signals:
	void mousePositionChanged(const QString& text);
	void zoomLevelChanged(qreal level);

protected:
	virtual void contextMenuEvent(QContextMenuEvent* event);
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void keyPressEvent(QKeyEvent* event);
	virtual void keyReleaseEvent(QKeyEvent* event);
	virtual void wheelEvent(QWheelEvent* event);

	virtual void defaultContextMenuEvent(DiagramMouseEvent& event);
	virtual void defaultMouseDoubleClickEvent(DiagramMouseEvent& event);
	virtual void defaultMousePressEvent(DiagramMouseEvent& event);
	virtual void defaultMouseMoveEvent(DiagramMouseEvent& event);
	virtual void defaultMouseReleaseEvent(DiagramMouseEvent& event);

	virtual void newMousePressEvent(DiagramMouseEvent& event);
	virtual void newMouseMoveEvent(DiagramMouseEvent& event);
	virtual void newMouseReleaseEvent(DiagramMouseEvent& event);

	virtual void resizeEvent(QResizeEvent* event);

	virtual void paintEvent(QPaintEvent* event);
	virtual void paint(DiagramPainter* painter);

	virtual void drawBackground(DiagramPainter* painter, const QRectF& rect);
	virtual void drawBorder(DiagramPainter* painter, const QRectF& rect);
	virtual void drawGrid(DiagramPainter* painter, const QRectF& rect);
	virtual void drawForeground(DiagramPainter* painter, const QRectF& rect);
	virtual void drawRubberBand(DiagramPainter* painter, const QRectF& rect = QRectF());

private:
	void setScale(qreal scale);
	qreal scale() const;

	void beginScrollUpdate();
	void endScrollUpdate(bool adjustAnchor);
};

#endif
