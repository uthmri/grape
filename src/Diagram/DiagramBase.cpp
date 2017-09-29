/* DiagramBase.cpp
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


#include <DiagramBase.h>

const QVector<qreal> DiagramBase::kZoomLevels = QVector<qreal>() << 0.1 << 0.25 << 0.33 << 0.5 <<
	0.67 << 0.75 << 1.0 << 1.5 << 2.0 << 3.0 << 4.0 << 6.0 << 8.0 << 10.0 << 12.0 << 16.0;

DiagramBase::DiagramBase() : QAbstractScrollArea()
{
	setMouseTracking(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	mWidth = 10000;
	mHeight = 7500;
	mUnits = UnitsMils;
	mGrid = 100;
	mBorderWidth = 24;

	mPainter = new DiagramPainter();
	mAlignment = Qt::AlignCenter;
	mViewportAnchor = AnchorUnderMouse;

	mZoomLevel = 1.0;

	mMode = DefaultMode;

	mGridStyle = GridGraphPaper;
	mGridSpacingMajor = 8;
	mGridSpacingMinor = 2;
}
//--------------------------------------------------------------------------------------------------
DiagramBase::~DiagramBase()
{
	setDefaultMode();

	delete mPainter;
	mPainter = NULL;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::setSize(qreal width, qreal height)
{
	mWidth = width;
	mHeight = height;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setUnits(DiagramUnits units)
{
	mUnits = units;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setGrid(qreal grid)
{
	mGrid = grid;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setBorderWidth(qreal width)
{
	mBorderWidth = width;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramBase::width() const
{
	return mWidth;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramBase::height() const
{
	return mHeight;
}
//--------------------------------------------------------------------------------------------------
DiagramUnits DiagramBase::units() const
{
	return mUnits;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramBase::grid() const
{
	return mGrid;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramBase::borderWidth() const
{
	return mBorderWidth;
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramBase::diagramRect() const
{
	return QRectF(0, 0, mWidth, mHeight);
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramBase::diagramRectWithBorder() const
{
	return QRectF(-mBorderWidth/2, -mBorderWidth/2, mWidth + mBorderWidth, mHeight + mBorderWidth);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::setPainter(DiagramPainter* painter)
{
	delete mPainter;
	mPainter = painter;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setAlignment(Qt::Alignment alignment)
{
	mAlignment = alignment;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setViewportAnchor(ViewportAnchor anchor)
{
	mViewportAnchor = anchor;
}
//--------------------------------------------------------------------------------------------------
DiagramPainter* DiagramBase::painter() const
{
	return mPainter;
}
//--------------------------------------------------------------------------------------------------
Qt::Alignment DiagramBase::alignment() const
{
	return mAlignment;
}
//--------------------------------------------------------------------------------------------------
DiagramBase::ViewportAnchor DiagramBase::viewportAnchor() const
{
	return mViewportAnchor;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::setZoomLevel(qreal level)
{
	mZoomLevel = level;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramBase::zoomLevel() const
{
	return mZoomLevel;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::setMode(Mode mode)
{
	mMode = mode;
}
//--------------------------------------------------------------------------------------------------
DiagramBase::Mode DiagramBase::mode() const
{
	return mMode;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::setGridStyle(GridStyle style)
{
	mGridStyle = style;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setGridSpacing(int major, int minor)
{
	mGridSpacingMajor = major;
	mGridSpacingMinor = minor;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setMajorGridSpacing(int major)
{
	mGridSpacingMajor = major;
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setMinorGridSpacing(int minor)
{
	mGridSpacingMinor = minor;
}
//--------------------------------------------------------------------------------------------------
DiagramBase::GridStyle DiagramBase::gridStyle() const
{
	return mGridStyle;
}
//--------------------------------------------------------------------------------------------------
int DiagramBase::majorGridSpacing() const
{
	return mGridSpacingMajor;
}
//--------------------------------------------------------------------------------------------------
int DiagramBase::minorGridSpacing() const
{
	return mGridSpacingMinor;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPointF DiagramBase::mapToDiagram(const QPoint& screenPos) const
{
	QPointF diagramPos;
	QRectF viewRect = visibleRect();

	diagramPos.setX(screenPos.x() / (qreal)maximumViewportSize().width() * viewRect.width() +
		viewRect.left());
	diagramPos.setY(screenPos.y() / (qreal)maximumViewportSize().height() * viewRect.height() +
		viewRect.top());

	return diagramPos;
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramBase::mapToDiagram(const QRect& screenRect) const
{
	return QRectF(mapToDiagram(screenRect.topLeft()), mapToDiagram(screenRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
QPoint DiagramBase::mapFromDiagram(const QPointF& diagramPos) const
{
	QPoint screenPos;
	QRectF viewRect = visibleRect();

	screenPos.setX((int)(((diagramPos.x() - viewRect.left()) / viewRect.width() *
		(qreal)maximumViewportSize().width()) + 0.5));
	screenPos.setY((int)(((diagramPos.y() - viewRect.top()) / viewRect.height() *
		(qreal)maximumViewportSize().height()) + 0.5));

	return screenPos;
}
//--------------------------------------------------------------------------------------------------
QRect DiagramBase::mapFromDiagram(const QRectF& diagramRect) const
{
	return QRect(mapFromDiagram(diagramRect.topLeft()), mapFromDiagram(diagramRect.bottomRight()));
}
//--------------------------------------------------------------------------------------------------
QRectF DiagramBase::visibleRect() const
{
	QScrollBar* horizontalScroll = horizontalScrollBar();
	QScrollBar* verticalScroll = verticalScrollBar();

	qreal lScale = scale();
	QRectF lDiagramRect = diagramRectWithBorder();

	qreal width = maximumViewportSize().width() / lScale;
	qreal height = maximumViewportSize().height() / lScale;

	qreal left = horizontalScroll->value() / lScale + lDiagramRect.left();
	qreal top = verticalScroll->value() / lScale + lDiagramRect.top();
	if (horizontalScroll->maximum() == 0)
	{
		if (mAlignment & Qt::AlignHCenter) left += (lDiagramRect.width() - width) / 2;
		else if (mAlignment & Qt::AlignRight) left += (lDiagramRect.width() - width);
	}
	if (verticalScroll->maximum() == 0)
	{
		if (mAlignment & Qt::AlignVCenter) top += (lDiagramRect.height() - height) / 2;
		else if (mAlignment & Qt::AlignBottom) top += (lDiagramRect.height() - height);
	}

	return QRectF(left, top, width, height);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
qreal DiagramBase::roundToGrid(qreal value) const
{
	qreal temp;

	temp = value - (fmod(value, mGrid));
	if (fmod(value, mGrid) >= mGrid/2) temp += mGrid;

	return temp;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramBase::roundToGrid(const QPointF& diagramPos) const
{
	return QPointF(roundToGrid(diagramPos.x()), roundToGrid(diagramPos.y()));
}
//--------------------------------------------------------------------------------------------------
DiagramMouseEvent DiagramBase::lastMouseEvent() const
{
	return mMouseEvent;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::render()
{
	render(mPainter, visibleRect());
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::render(DiagramPainter* painter, const QRectF& rect)
{
	drawBackground(painter, rect);
	drawForeground(painter, rect);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::update()
{
	if (viewport()) viewport()->update();
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::updateScrollBars()
{
	endScrollUpdate(false);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::zoomIn()
{
	if (mMode != NonInteractiveMode)
	{
		int zoomLevelIndex = 0;

		while (zoomLevelIndex < kZoomLevels.size() && mZoomLevel >= kZoomLevels[zoomLevelIndex])
			zoomLevelIndex++;
		if (zoomLevelIndex >= kZoomLevels.size()) zoomLevelIndex = kZoomLevels.size() - 1;

		beginScrollUpdate();
		setZoomLevel(kZoomLevels[zoomLevelIndex]);
		endScrollUpdate(true);
		update();

		emit zoomLevelChanged(mZoomLevel);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::zoomOut()
{
	if (mMode != NonInteractiveMode)
	{
		int zoomLevelIndex = kZoomLevels.size() - 1;

		while (zoomLevelIndex >= 0 && mZoomLevel <= kZoomLevels[zoomLevelIndex])
			zoomLevelIndex--;
		if (zoomLevelIndex < 0) zoomLevelIndex = 0;

		beginScrollUpdate();
		setZoomLevel(kZoomLevels[zoomLevelIndex]);
		endScrollUpdate(true);
		update();

		emit zoomLevelChanged(mZoomLevel);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::zoomFit(const QRectF& rect)
{
	if (mMode != NonInteractiveMode)
	{
		QRectF actualRect = rect;
		if (!actualRect.isValid()) actualRect = diagramRectWithBorder();

		qreal scaleX = maximumViewportSize().width() / actualRect.width();
		qreal scaleY = maximumViewportSize().height() / actualRect.height();
		qreal finalScale = qMin(scaleX, scaleY);

		beginScrollUpdate();
		setScale(finalScale);
		endScrollUpdate(false);

		horizontalScrollBar()->setValue((int)(finalScale * actualRect.left()));
		verticalScrollBar()->setValue((int)(finalScale * actualRect.top()));

		update();

		emit zoomLevelChanged(mZoomLevel);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::zoomToLevel(qreal level)
{
	if (mMode != NonInteractiveMode)
	{
		beginScrollUpdate();
		setZoomLevel(level);
		endScrollUpdate(false);
		update();

		emit zoomLevelChanged(mZoomLevel);
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::centerOn(const QPointF& pos)
{
	QRectF lVisibleRect = visibleRect();
	qreal lScale = scale();

	horizontalScrollBar()->setValue(
		(int)(horizontalScrollBar()->value() + (pos.x() - lVisibleRect.center().x()) * lScale));
	verticalScrollBar()->setValue(
		(int)(verticalScrollBar()->value() + (pos.y() - lVisibleRect.center().y()) * lScale));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::setDefaultMode()
{
	setMode(DefaultMode);
	setCursor(Qt::ArrowCursor);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setZoomMode()
{
    setMode(ZoomMode);
    setCursor(Qt::CrossCursor);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setRunPipelineMode()
{
    setMode(RunPipelineMode);
    //setCursor(Qt::WaitCursor);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setStopPipelineMode()
{
    setMode(StopPipelineMode);
    //setCursor(Qt::WaitCursor);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setScrollMode()
{
	setMode(ScrollMode);
	setCursor(Qt::OpenHandCursor);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::setNonInteractiveMode()
{
	setMode(NonInteractiveMode);
	setCursor(Qt::ArrowCursor);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::contextMenuEvent(QContextMenuEvent* event)
{
	if (event)
	{
		mMouseEvent.update(this, event);
		mMouseEvent.ignore();
		event->ignore();

		switch (mMode)
		{
		case ScrollMode:
		case ZoomMode:
		case NewMode:
			setDefaultMode();
			event->accept();
			break;

		case DefaultMode:
			defaultContextMenuEvent(mMouseEvent);
			if (mMouseEvent.isAccepted()) event->accept();
			break;

		case NonInteractiveMode:
			event->accept();
			break;

		default: break;
		}

		// If !event->isAccepted(), run local context menu event
	}

	update();
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (event && event->button() == Qt::LeftButton)
	{
		mMouseEvent.update(this, event);
		mMouseEvent.ignore();
		event->ignore();

		switch (mMode)
		{
		case ScrollMode:
		case ZoomMode:
		case NewMode:
			setDefaultMode();
			event->accept();
			break;

		case DefaultMode:
			defaultMouseDoubleClickEvent(mMouseEvent);
			if (mMouseEvent.isAccepted()) event->accept();
			break;

		case NonInteractiveMode:
			event->accept();
			break;

		default: break;
		}
	}

	update();
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::mousePressEvent(QMouseEvent* event)
{
	if (event)
	{
		mMouseEvent.update(this, event);
		mMouseEvent.ignore();

		if (event->button() == Qt::LeftButton)
		{
			switch (mMode)
			{
			case ScrollMode:
				setCursor(Qt::ClosedHandCursor);
				event->accept();
				break;

			case ZoomMode:
				event->accept();
				break;

			case NewMode:
				newMousePressEvent(mMouseEvent);
				if (mMouseEvent.isAccepted()) event->accept();
				break;

			case DefaultMode:
				defaultMousePressEvent(mMouseEvent);
				if (mMouseEvent.isAccepted()) event->accept();
				break;

			case NonInteractiveMode:
				event->accept();
				break;

			default: break;
			}
		}
	}

	update();
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::mouseMoveEvent(QMouseEvent* event)
{
	if (event)
	{
		mMouseEvent.update(this, event);
		mMouseEvent.ignore();

		switch (mMode)
		{
		case ScrollMode:
			if (mMouseEvent.isDragged())
			{
				horizontalScrollBar()->setValue(mMouseEvent.buttonDownScrollValue().x() -
					(mMouseEvent.screenPos().x() - mMouseEvent.buttonDownScreenPos().x()));
				verticalScrollBar()->setValue(mMouseEvent.buttonDownScrollValue().y() -
					(mMouseEvent.screenPos().y() - mMouseEvent.buttonDownScreenPos().y()));
			}
			event->accept();
			break;

		case ZoomMode:
			event->accept();
			break;

		case NewMode:
			newMouseMoveEvent(mMouseEvent);
			if (mMouseEvent.isAccepted()) event->accept();
			break;

		case DefaultMode:
			defaultMouseMoveEvent(mMouseEvent);
			if (mMouseEvent.isAccepted()) event->accept();
			break;

		case NonInteractiveMode:
			event->accept();
			break;

		default: break;
		}

        // Rounded to grid
        /*if (mMouseEvent.isDragged())
        {
            if (mMode != ScrollMode)
            {
                emit mousePositionChanged(
                    pointToString(roundToGrid(mMouseEvent.buttonDownDiagramPos())) + " - " +
                    pointToString(roundToGrid(mMouseEvent.diagramPos())) + "; " + QString(QChar(0x394)) + " = " +
                    pointToString(roundToGrid(mMouseEvent.diagramPos() - mMouseEvent.buttonDownDiagramPos())));
            }
        }
        else emit mousePositionChanged(pointToString(roundToGrid(mMouseEvent.diagramPos())));*/

        // Not rounded to grid
        if (mMouseEvent.isDragged())
		{
			if (mMode != ScrollMode)
			{
                emit mousePositionChanged(
					pointToString(mMouseEvent.buttonDownDiagramPos(), mUnits) + " - " +
					pointToString(mMouseEvent.diagramPos(), mUnits) + "; " + QString(QChar(0x394)) + " = " +
					pointToString(mMouseEvent.diagramPos() - mMouseEvent.buttonDownDiagramPos(), mUnits));
            }
		}
		else emit mousePositionChanged(pointToString(mMouseEvent.diagramPos(), mUnits));
	}

	update();
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::mouseReleaseEvent(QMouseEvent* event)
{
	if (event)
	{
		mMouseEvent.update(this, event);
		mMouseEvent.ignore();

		if (event->button() == Qt::LeftButton)
		{
			switch (mMode)
			{
			case ScrollMode:
				setCursor(Qt::OpenHandCursor);
				event->accept();
				break;

			case ZoomMode:
				if (mMouseEvent.isDragged())
				{
					zoomFit(rectFromPoints(
						mMouseEvent.diagramPos(), mMouseEvent.buttonDownDiagramPos()));
					setDefaultMode();
					emit zoomLevelChanged(mZoomLevel);
				}
				event->accept();
				break;

			case NewMode:
				newMouseReleaseEvent(mMouseEvent);
				if (mMouseEvent.isAccepted()) event->accept();
				break;

			case DefaultMode:
				defaultMouseReleaseEvent(mMouseEvent);
				if (mMouseEvent.isAccepted()) event->accept();
				break;

			case NonInteractiveMode:
				event->accept();
				break;

			default: break;
			}
		}
	}

	//emit mousePositionChanged(pointToString(roundToGrid(mMouseEvent.diagramPos()), mUnits));
	emit mousePositionChanged(pointToString(mMouseEvent.diagramPos(), mUnits));
	update();
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::keyPressEvent(QKeyEvent* event)
{
	// Base implementation ignores key press events
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::keyReleaseEvent(QKeyEvent* event)
{
	// Base implementation ignores key press events
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::wheelEvent(QWheelEvent* event)
{
	if (event && (event->modifiers() & Qt::ControlModifier))
	{
		if (event->delta() > 0) zoomIn();
		else if (event->delta() < 0) zoomOut();
	}
	else QAbstractScrollArea::wheelEvent(event);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::defaultContextMenuEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::defaultMouseDoubleClickEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::defaultMousePressEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::defaultMouseMoveEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::defaultMouseReleaseEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::newMousePressEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::newMouseMoveEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::newMouseReleaseEvent(DiagramMouseEvent& event)
{
	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::resizeEvent(QResizeEvent* event)
{
	Q_UNUSED(event);
	endScrollUpdate(false);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::paintEvent(QPaintEvent* event)
{
	// This code works the same speed in Windows and Linux
	// Try to avoid using an alpha channel as that slows things down considerably

	if (mPainter)
	{
		QImage image(viewport()->width(), viewport()->height(), QImage::Format_RGB32);
		mPainter->begin(&image);

		qreal lScale = scale();
		QRectF lRect = diagramRectWithBorder();

		//if (event)
		//{
		//    mPainter->setClipRect(event->rect());
		//    mPainter->setClipping(true);
		//}

		mPainter->translate(-horizontalScrollBar()->value(), -verticalScrollBar()->value());
		mPainter->scale(lScale, lScale);
		mPainter->translate(-lRect.topLeft());

		// Adjust for alignment
		if (horizontalScrollBar()->maximum() == 0)
		{
			if (mAlignment & Qt::AlignHCenter)
				mPainter->translate(-(lRect.width() - maximumViewportSize().width() / lScale) / 2, 0);
			else if (mAlignment & Qt::AlignRight)
				mPainter->translate(-(lRect.width() - maximumViewportSize().width() / lScale), 0);
		}
		if (verticalScrollBar()->maximum() == 0)
		{
			if (mAlignment & Qt::AlignVCenter)
				mPainter->translate(0, -(lRect.height() - maximumViewportSize().height() / lScale) / 2);
			else if (mAlignment & Qt::AlignBottom)
				mPainter->translate(0, -(lRect.height() - maximumViewportSize().height() / lScale));
		}

		paint(mPainter);
		mPainter->end();

		QPainter widgetPainter(viewport());
		widgetPainter.drawImage(0, 0, image);
	}


	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::paint(DiagramPainter* painter)
{
	QRectF lVisibleRect = visibleRect();

	render(painter, lVisibleRect);

	if (mMode == ZoomMode && mMouseEvent.isDragged()) drawRubberBand(painter);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::drawBackground(DiagramPainter* painter, const QRectF& rect)
{
	if (painter->shouldDrawBackground())
	{
		painter->setBrush(painter->outputBrush(DiagramPainter::Background));
		painter->setPen(QPen(painter->outputBrush(DiagramPainter::Background), 1));
		painter->drawRect(rect);
	}

	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	if (painter->shouldDrawGrid() && mGrid > 0)
		drawGrid(painter, rect);
	if (painter->shouldDrawBorder() && mBorderWidth > 0.0)
		drawBorder(painter, rect);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::drawBorder(DiagramPainter* painter, const QRectF& rect)
{
	QPen borderPen(painter->outputBrush(DiagramPainter::Border),
		mBorderWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

	painter->setBrush(Qt::transparent);
	painter->setPen(borderPen);
	painter->drawRect(diagramRect());

	Q_UNUSED(rect);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::drawGrid(DiagramPainter* painter,	const QRectF& rect)
{
	QPainter::RenderHints hints = painter->renderHints();
	qreal actualGridSpacing;
	qreal lGrid = grid();

	QPen gridPen(painter->outputBrush(DiagramPainter::Grid), 1);
	gridPen.setCosmetic(true);

	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
							QPainter::SmoothPixmapTransform, false);
	painter->setPen(gridPen);

	switch (mGridStyle)
	{
	case GridDotted:
		if (mGridSpacingMajor > 0)
		{
			actualGridSpacing = lGrid * mGridSpacingMajor;
			for(qreal y = ceil(rect.top() / actualGridSpacing) * actualGridSpacing;
				y < rect.bottom(); y += actualGridSpacing)
			{
				for(qreal x = ceil(rect.left() / actualGridSpacing) * actualGridSpacing;
					x < rect.right(); x += actualGridSpacing)
				{
					painter->drawPoint(QPointF(x, y));
				}
			}
		}
		break;

	case GridLined:
		gridPen.setStyle(Qt::DotLine);
		painter->setPen(gridPen);
		if (mGridSpacingMajor > 0)
		{
			actualGridSpacing = lGrid * mGridSpacingMajor;
			for(qreal y = ceil(rect.top() / actualGridSpacing) * actualGridSpacing;
				y < rect.bottom(); y += actualGridSpacing)
			{
				painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
			}
			for(qreal x = ceil(rect.left() / actualGridSpacing) * actualGridSpacing;
				x < rect.right(); x += actualGridSpacing)
			{
				painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
			}
		}
		break;

	case GridGraphPaper:
		gridPen.setStyle(Qt::DotLine);
		painter->setPen(gridPen);
		if (mGridSpacingMinor > 0)
		{
			actualGridSpacing = lGrid * mGridSpacingMinor;
			for(qreal y = ceil(rect.top() / actualGridSpacing) * actualGridSpacing;
				y < rect.bottom(); y += actualGridSpacing)
			{
				painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
			}
			for(qreal x = ceil(rect.left() / actualGridSpacing) * actualGridSpacing;
				x < rect.right(); x += actualGridSpacing)
			{
				painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
			}
		}

		gridPen.setStyle(Qt::SolidLine);
		painter->setPen(gridPen);
		if (mGridSpacingMajor > 0)
		{
			actualGridSpacing = lGrid * mGridSpacingMajor;
			for(qreal y = ceil(rect.top() / actualGridSpacing) * actualGridSpacing;
				y < rect.bottom(); y += actualGridSpacing)
			{
				painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
			}
			for(qreal x = ceil(rect.left() / actualGridSpacing) * actualGridSpacing;
				x < rect.right(); x += actualGridSpacing)
			{
				painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
			}
		}
		break;

	default:
		break;
	}

	painter->setRenderHints(hints);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::drawForeground(DiagramPainter* painter, const QRectF& rect)
{
	Q_UNUSED(painter);
	Q_UNUSED(rect);
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::drawRubberBand(DiagramPainter* painter, const QRectF& rect)
{
	QRectF rubberBandRect = rect;
	if (!rubberBandRect.isValid())
		rubberBandRect = rectFromPoints(mMouseEvent.screenPos(), mMouseEvent.buttonDownScreenPos());

	painter->save();
	painter->resetTransform();
	painter->setRenderHints(QPainter::Antialiasing |
		QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, false);

	painter->setBrush(painter->outputBrush(DiagramPainter::RubberBand));
	painter->setPen(QPen(painter->outputBrush(DiagramPainter::RubberBand), 1));
	painter->drawRect(rubberBandRect);
	painter->restore();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::setScale(qreal scl)
{
	qreal lZoomLevel = scl * 8.0;

	switch (mUnits)
	{
	case UnitsSimpleMM:
	case UnitsMM: lZoomLevel = scl * 0.2; break;
	default: break;
	}

	setZoomLevel(lZoomLevel);
}
//--------------------------------------------------------------------------------------------------
qreal DiagramBase::scale() const
{
	qreal lScale = mZoomLevel / 8.0;

	switch (mUnits)
	{
	case UnitsSimpleMM:
	case UnitsMM: lScale = mZoomLevel / 0.2; break;
	default: break;
	}

	return lScale;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramBase::beginScrollUpdate()
{
	mMouseDiagramPos = mapToDiagram(mapFromGlobal(QCursor::pos()));
	mViewCenter = visibleRect().center();
}
//--------------------------------------------------------------------------------------------------
void DiagramBase::endScrollUpdate(bool adjustAnchor)
{
	QScrollBar* horizontalScroll = horizontalScrollBar();
	QScrollBar* verticalScroll = verticalScrollBar();
	qreal lScale = scale();
	QRectF lDiagramRect = diagramRectWithBorder();

	int innerWidth = maximumViewportSize().width();
	int innerHeight = maximumViewportSize().height();
	int widgetWidth = (int)((lScale * lDiagramRect.width()) + 0.5);
	int widgetHeight = (int)((lScale * lDiagramRect.height()) + 0.5);
	QPoint mousePos = mapFromGlobal(QCursor::pos());

	// Adjust the horizontal scroll bar to reflect the new size of viewport()
	if (widgetWidth - innerWidth > 0)
	{
		horizontalScroll->setRange(0, widgetWidth - innerWidth + 1);
		horizontalScroll->setSingleStep(innerWidth / 40);
		horizontalScroll->setPageStep(innerWidth);
	}
	else horizontalScroll->setRange(0, 0);

	// Adjust the vertical scroll bar to reflect the new size of viewport()
	if (widgetHeight - innerHeight > 0)
	{
		verticalScroll->setRange(0, widgetHeight - innerHeight + 1);
		verticalScroll->setSingleStep(innerHeight / 40);
		verticalScroll->setPageStep(innerHeight);
	}
	else verticalScroll->setRange(0, 0);

	if (adjustAnchor)
	{
		switch (mViewportAnchor)
		{
		case AnchorViewCenter:
			centerOn(mViewCenter);
			break;

		case AnchorUnderMouse:
			// Try to keep oldMousePos mapping to the same screen position
			if (rect().contains(mousePos))
			{
				QPointF diagramPos = mapToDiagram(mousePos);
				horizontalScroll->setValue((int)(horizontalScroll->value() +
					(mMouseDiagramPos.x() - diagramPos.x()) * lScale));
				verticalScroll->setValue((int)(verticalScroll->value() +
					(mMouseDiagramPos.y() - diagramPos.y()) * lScale));
			}
			else centerOn(mViewCenter);
			break;

		default:
			break;
		}
	}

	emit mousePositionChanged("");
}
//--------------------------------------------------------------------------------------------------
