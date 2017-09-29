/* DiagramTypes.cpp
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

#include <DiagramTypes.h>
#include <DiagramBase.h>

DiagramArrow::DiagramArrow(Style style, qreal size)
{
	mStyle = style;
	mSize = size;
}
//--------------------------------------------------------------------------------------------------
DiagramArrow::DiagramArrow(const DiagramArrow& arrow)
{
	mStyle = arrow.mStyle;
	mSize = arrow.mSize;
}
//--------------------------------------------------------------------------------------------------
DiagramArrow::~DiagramArrow() { }
//--------------------------------------------------------------------------------------------------
DiagramArrow& DiagramArrow::operator=(const DiagramArrow& arrow)
{
	mStyle = arrow.mStyle;
	mSize = arrow.mSize;
	return *this;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramArrow::setArrow(Style style, qreal size)
{
	mStyle = style;
	mSize = size;
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::setStyle(Style style)
{
	mStyle = style;
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::setSize(qreal size)
{
	mSize = size;
}
//--------------------------------------------------------------------------------------------------
DiagramArrow::Style DiagramArrow::style() const
{
	return mStyle;
}
//--------------------------------------------------------------------------------------------------
qreal DiagramArrow::size() const
{
	return mSize;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QString DiagramArrow::toString() const
{
	return QString::number((quint32)mStyle) + "," + QString::number(mSize);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramArrow::render(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		direction *= 3.141592654 / 180.0;

		switch (mStyle)
		{
		case Normal: drawNormal(painter, position, direction); break;

		case TriangleFilled:
		case Triangle: drawTriangle(painter, position, direction); break;

		case DiamondFilled:
		case Diamond: drawDiamond(painter, position, direction); break;
			break;

		case CircleFilled:
		case Circle: drawCircle(painter, position, direction); break;
			break;

		case Harpoon:
		case HarpoonMirrored: drawHarpoon(painter, position, direction); break;
			break;

		case Concave:
		case ConcaveFilled: drawConcave(painter, position, direction); break;
			break;

		case Reverse: drawReverse(painter, position, direction); break;
			break;

		case XArrow: drawX(painter, position, direction); break;
			break;

		default:
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawNormal(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		qreal angle = 3.14159 / 6;
		qreal sqrt2 = qSqrt(2);
		QPointF tipPoint1(position.x() + mSize / sqrt2 * cos(direction - angle),
						  position.y() + mSize / sqrt2 * sin(direction - angle));
		QPointF tipPoint2(position.x() + mSize / sqrt2 * cos(direction + angle),
						  position.y() + mSize / sqrt2 * sin(direction + angle));

		painter->drawLine(position, tipPoint1);
		painter->drawLine(position, tipPoint2);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawTriangle(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		QPolygonF polygon;
		qreal angle = 3.14159 / 6;
		qreal sqrt2 = qSqrt(2);

		QBrush originalBrush = painter->brush();
		if (mStyle == TriangleFilled) painter->setBrush(painter->pen().brush());

		polygon.append(position);
		polygon.append(QPointF(position.x() + mSize / sqrt2 * cos(direction - angle),
							   position.y() + mSize / sqrt2 * sin(direction - angle)));
		polygon.append(QPointF(position.x() + mSize / sqrt2 * cos(direction + angle),
							   position.y() + mSize / sqrt2 * sin(direction + angle)));
		polygon.append(polygon[0]);
		painter->drawPolygon(polygon);

		if (mStyle == TriangleFilled) painter->setBrush(originalBrush);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawCircle(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		QBrush originalBrush = painter->brush();
		if (mStyle == CircleFilled) painter->setBrush(painter->pen().brush());

		Q_UNUSED(direction);
		painter->drawEllipse(QRectF(position.x() - mSize / 2, position.y() - mSize / 2, mSize, mSize));

		if (mStyle == CircleFilled) painter->setBrush(originalBrush);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawDiamond(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		QPolygonF polygon;
		qreal pi = 3.141592654;

		QBrush originalBrush = painter->brush();
		if (mStyle == DiamondFilled) painter->setBrush(painter->pen().brush());

		polygon.append(QPointF(position.x() + mSize / 2 * qCos(direction),
							   position.y() + mSize / 2 * qSin(direction)));
		polygon.append(QPointF(position.x() + mSize / 2 * qCos(direction - pi / 2),
							   position.y() + mSize / 2 * qSin(direction - pi / 2)));
		polygon.append(QPointF(position.x() + mSize / 2 * qCos(direction - pi),
							   position.y() + mSize / 2 * qSin(direction - pi)));
		polygon.append(QPointF(position.x() + mSize / 2 * qCos(direction + pi / 2),
							   position.y() + mSize / 2 * qSin(direction + pi / 2)));
		polygon.append(polygon[0]);
		painter->drawPolygon(polygon);

		if (mStyle == DiamondFilled) painter->setBrush(originalBrush);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawHarpoon(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		qreal angle = 3.14159 / 6;
		qreal sqrt2 = qSqrt(2);
		QPointF tipPoint1(position.x() + mSize / sqrt2 * cos(direction - angle),
						  position.y() + mSize / sqrt2 * sin(direction - angle));
		QPointF tipPoint2(position.x() + mSize / sqrt2 * cos(direction + angle),
						  position.y() + mSize / sqrt2 * sin(direction + angle));

		if (mStyle == HarpoonMirrored) painter->drawLine(position, tipPoint1);
		else painter->drawLine(position, tipPoint2);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawConcave(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		QPolygonF polygon;
		qreal angle = 3.14159 / 6;
		qreal sqrt2 = qSqrt(2);

		QBrush originalBrush = painter->brush();
		if (mStyle == ConcaveFilled) painter->setBrush(painter->pen().brush());

		polygon.append(position);
		polygon.append(QPointF(position.x() + mSize / sqrt2 * cos(direction - angle),
							   position.y() + mSize / sqrt2 * sin(direction - angle)));
		polygon.append(QPointF(position.x() + mSize / sqrt2 / 2 * cos(direction),
							   position.y() + mSize / sqrt2 / 2 * sin(direction)));
		polygon.append(QPointF(position.x() + mSize / sqrt2 * cos(direction + angle),
							   position.y() + mSize / sqrt2 * sin(direction + angle)));
		polygon.append(polygon[0]);
		painter->drawPolygon(polygon);

		if (mStyle == ConcaveFilled) painter->setBrush(originalBrush);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawReverse(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		qreal angle = 3.14159 / 6;
		qreal sqrt2 = qSqrt(2);
		QPointF headPoint(position.x() + mSize / sqrt2 * cos(direction),
						  position.y() + mSize / sqrt2 * sin(direction));
		QPointF tipPoint1(headPoint.x() - mSize / sqrt2 * cos(direction - angle),
						  headPoint.y() - mSize / sqrt2 * sin(direction - angle));
		QPointF tipPoint2(headPoint.x() - mSize / sqrt2 * cos(direction + angle),
						  headPoint.y() - mSize / sqrt2 * sin(direction + angle));

		painter->drawLine(headPoint, tipPoint1);
		painter->drawLine(headPoint, tipPoint2);
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramArrow::drawX(QPainter* painter, const QPointF& position, qreal direction)
{
	if (painter)
	{
		qreal sqrt2 = qSqrt(2);
		QPointF tipPoint1(position.x() + mSize / sqrt2, position.y() + mSize / sqrt2);
		QPointF tipPoint2(position.x() - mSize / sqrt2, position.y() - mSize / sqrt2);
		QPointF tipPoint3(position.x() + mSize / sqrt2, position.y() - mSize / sqrt2);
		QPointF tipPoint4(position.x() - mSize / sqrt2, position.y() + mSize / sqrt2);

		painter->drawLine(tipPoint1, tipPoint2);
		painter->drawLine(tipPoint3, tipPoint4);
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramArrow::operator==(const DiagramArrow& arrow) const
{
	return (mSize == arrow.mSize && mStyle == arrow.mStyle);
}
//--------------------------------------------------------------------------------------------------
bool DiagramArrow::operator!=(const DiagramArrow& arrow) const
{
	return (mSize != arrow.mSize || mStyle != arrow.mStyle);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramArrow::fromString(const QString& string, bool* ok)
{
	DiagramArrow arrow;
	bool conversionOk = true;
	QStringList data = string.split(",", QString::SkipEmptyParts);

	if (data.isEmpty()) conversionOk = false;
	else
	{
		if (data.size() >= 1)
		{
			bool styleOk = true;
			quint32 styleValue = data[0].toInt(&styleOk);

			if (styleOk) arrow.setStyle((DiagramArrow::Style)styleValue);

			conversionOk = (conversionOk && styleOk);
		}
		if (data.size() >= 2)
		{
			bool sizeOk = true;
			qreal sizeValue = data[1].toDouble(&sizeOk);

			if (sizeOk) arrow.setSize(sizeValue);

			conversionOk = (conversionOk && sizeOk);
		}
	}

	if (ok) *ok = conversionOk;
	return arrow;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramMouseEvent::DiagramMouseEvent() : QEvent(None)
{
	mButton = Qt::NoButton;
	mButtons = Qt::NoButton;
	mModifiers = Qt::NoModifier;
	mDragged = false;
}
//--------------------------------------------------------------------------------------------------
DiagramMouseEvent::~DiagramMouseEvent() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramMouseEvent::setType(QEvent::Type type)
{
	t = type;
}
//--------------------------------------------------------------------------------------------------
void DiagramMouseEvent::update(DiagramBase* diagram, QMouseEvent* event)
{
	if (diagram && event)
	{
		mButton = event->button();
		mButtons = event->buttons();
		mModifiers = event->modifiers();

		mPreviousDiagramPos = mDiagramPos;
		mPreviousScreenPos = mScreenPos;

		mScreenPos = event->pos();
		mDiagramPos = diagram->mapToDiagram(mScreenPos);

		if (event->type() == QEvent::MouseButtonPress ||
			event->type() == QEvent::MouseButtonDblClick)
		{
			mButtonDownScreenPos = event->pos();
			mButtonDownDiagramPos = diagram->mapToDiagram(mButtonDownScreenPos);

			mButtonDownScrollValue.setX(diagram->horizontalScrollBar()->value());
			mButtonDownScrollValue.setY(diagram->verticalScrollBar()->value());
		}

		if (event->type() == QEvent::MouseMove && (mButtons & Qt::LeftButton))
			mDragged = (mDragged || magnitude(mScreenPos - mButtonDownScreenPos) > 4);
		else if (event->type() != QEvent::MouseButtonRelease &&
			event->type() != QEvent::MouseButtonDblClick)
			mDragged = false;
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramMouseEvent::update(DiagramBase* diagram, QContextMenuEvent* event)
{
	if (diagram && event)
	{
		mButton = Qt::RightButton;
		mButtons = Qt::RightButton;
		mModifiers = event->modifiers();

		mPreviousDiagramPos = mDiagramPos;
		mPreviousScreenPos = mScreenPos;

		mScreenPos = event->pos();
		mDiagramPos = diagram->mapToDiagram(mScreenPos);

		mButtonDownScreenPos = event->pos();
		mButtonDownDiagramPos = diagram->mapToDiagram(mButtonDownScreenPos);

		mButtonDownScrollValue.setX(diagram->horizontalScrollBar()->value());
		mButtonDownScrollValue.setY(diagram->verticalScrollBar()->value());
		mDragged = false;
	}
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
Qt::MouseButton DiagramMouseEvent::button() const
{
	return mButton;
}
//--------------------------------------------------------------------------------------------------
Qt::MouseButtons DiagramMouseEvent::buttons() const
{
	return mButtons;
}
//--------------------------------------------------------------------------------------------------
Qt::KeyboardModifiers DiagramMouseEvent::modifiers() const
{
	return mModifiers;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPointF DiagramMouseEvent::diagramPos() const
{
	return mDiagramPos;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramMouseEvent::previousDiagramPos() const
{
	return mPreviousDiagramPos;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramMouseEvent::buttonDownDiagramPos() const
{
	return mButtonDownDiagramPos;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPoint DiagramMouseEvent::screenPos() const
{
	return mScreenPos;
}
//--------------------------------------------------------------------------------------------------
QPoint DiagramMouseEvent::previousScreenPos() const
{
	return mPreviousScreenPos;
}
//--------------------------------------------------------------------------------------------------
QPoint DiagramMouseEvent::buttonDownScreenPos() const
{
	return mButtonDownScreenPos;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPoint DiagramMouseEvent::buttonDownScrollValue() const
{
	return mButtonDownScrollValue;
}
//--------------------------------------------------------------------------------------------------
bool DiagramMouseEvent::isDragged() const
{
	return mDragged;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPainter::DiagramPainter() : QPainter()
{
	mMode = Normal;

	mDefaultFont = QFont("Arial");
	mDefaultFont.setPointSizeF(100.0);
	mDefaultPen = QPen(Qt::magenta, 12.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

	mBrushes.append(Qt::white);                             // Background
	mBrushes.append(QBrush(QColor(0, 0, 0)));               // Border
	mBrushes.append(QBrush(QColor(0, 128, 128)));           // Grid
	mBrushes.append(QBrush(QColor(144, 160, 172, 128)));    // RubberBand
	mBrushes.append(QBrush(QColor(0, 224, 0)));             // ResizePoint
	mBrushes.append(QBrush(QColor(255, 128, 0, 128)));      // Hotpoint
	mBrushes.append(QBrush(QColor(255, 0, 0)));             // Item0
	mBrushes.append(QBrush(QColor(255, 255, 0)));           // Item1
	mBrushes.append(QBrush(QColor(255, 255, 255)));         // UserDefined

	mDrawBackground = true;
	mDrawBorder = true;
	mDrawGrid = true;
}
//--------------------------------------------------------------------------------------------------
DiagramPainter::DiagramPainter(QPaintDevice* device) : QPainter(device)
{
	mMode = Normal;

	mDefaultFont = QFont("Arial");
	mDefaultFont.setPointSizeF(100.0);
	mDefaultPen = QPen(Qt::magenta, 12.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

	mBrushes.append(Qt::white);                             // Background
	mBrushes.append(QBrush(QColor(0, 0, 0)));               // Border
	mBrushes.append(QBrush(QColor(0, 128, 128)));           // Grid
	mBrushes.append(QBrush(QColor(144, 160, 172, 128)));    // RubberBand
	mBrushes.append(QBrush(QColor(0, 224, 0)));             // ResizePoint
	mBrushes.append(QBrush(QColor(255, 128, 0, 128)));      // Hotpoint
	mBrushes.append(QBrush(QColor(255, 0, 255)));           // Item0
	mBrushes.append(QBrush(QColor(255, 0, 0)));             // Item1
	mBrushes.append(QBrush(QColor(255, 255, 255)));         // UserDefined

	mDrawBackground = true;
	mDrawBorder = true;
	mDrawGrid = true;
}
//--------------------------------------------------------------------------------------------------
DiagramPainter::~DiagramPainter() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setMode(Mode mode)
{
	mMode = mode;
}
//--------------------------------------------------------------------------------------------------
DiagramPainter::Mode DiagramPainter::mode() const
{
	return mMode;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setDefaultFont(const QFont& font)
{
	mDefaultFont = font;
}
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setDefaultPen(const QPen& pen)
{
	mDefaultPen = pen;
}
//--------------------------------------------------------------------------------------------------
QFont DiagramPainter::defaultFont() const
{
	return mDefaultFont;
}
//--------------------------------------------------------------------------------------------------
QPen DiagramPainter::defaultPen() const
{
	return mDefaultPen;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPainter::addBrush(const QBrush& brush)
{
	mBrushes.append(brush);
}
//--------------------------------------------------------------------------------------------------
void DiagramPainter::insertBrush(int index, const QBrush& brush)
{
	if (index > UserDefined) mBrushes.insert(index, brush);
}
//--------------------------------------------------------------------------------------------------
void DiagramPainter::removeBrush(int index)
{
	if (index > UserDefined) mBrushes.removeAt(index);
}
//--------------------------------------------------------------------------------------------------
void DiagramPainter::clearBrushes()
{
	while (mBrushes.size() > UserDefined+1) mBrushes.takeLast();
}
//--------------------------------------------------------------------------------------------------
int DiagramPainter::numberOfBrushes() const
{
	return mBrushes.size();
}
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setBrushAt(int index, const QBrush& brush)
{
	if (0 <= index && index < mBrushes.size()) mBrushes.replace(index, brush);
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramPainter::brushAt(int index) const
{
	QBrush brush;
	if (0 <= index && index < mBrushes.size()) brush = mBrushes[index];
	return brush;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setDrawBackground(bool draw)
{
	mDrawBackground = draw;
}
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setDrawBorder(bool draw)
{
	mDrawBorder = draw;
}
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setDrawGrid(bool draw)
{
	mDrawGrid = draw;
}
//--------------------------------------------------------------------------------------------------
bool DiagramPainter::shouldDrawBackground() const
{
	return mDrawBackground;
}
//--------------------------------------------------------------------------------------------------
bool DiagramPainter::shouldDrawBorder() const
{
	return mDrawBorder;
}
//--------------------------------------------------------------------------------------------------
bool DiagramPainter::shouldDrawGrid() const
{
	return mDrawGrid;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPainter::setForItem(const QPen& itemPen, const QBrush& itemBrush)
{
	QBrush lBrush = itemBrush;
	QPen lPen = itemPen;
	QBrush lPenBrush = lPen.brush();

	switch (mMode)
	{
	case BlackOnWhite:
		if (lBrush.color().alpha() != 0) lBrush.setColor(QColor(255, 255, 255));
		lPenBrush.setColor(QColor(0, 0 ,0));
		break;

	case WhiteOnBlack:
		if (lBrush.color().alpha() != 0) lBrush.setColor(QColor(0, 0, 0));
		lPenBrush.setColor(QColor(255, 255, 255));
		break;

	default: break;
	}

	setBrush(lBrush);

	lPen.setBrush(lPenBrush);
	setPen(lPen);
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramPainter::outputBrush(int index) const
{
	QBrush lBrush;

	switch (mMode)
	{
	case BlackOnWhite:
		if (index == Background) lBrush = Qt::white;
		else lBrush = Qt::black;
		break;

	case WhiteOnBlack:
		if (index == Background) lBrush = Qt::black;
		else lBrush = Qt::white;
		break;

	default:
		lBrush = brushAt(index);
		break;
	}

	return lBrush;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
qreal unitsScale(DiagramUnits units, DiagramUnits newUnits)
{
	qreal result = 1.0;

	switch (units)
	{
	case UnitsSimpleMM:
		switch (newUnits)
		{
		case UnitsSimpleMM:     break;
		case UnitsMM:           result = 25.4 / 25.0; break;
		default:                result = 1000.0 / 25.0; break;
		};
		break;

	case UnitsMM:
		switch (newUnits)
		{
		case UnitsSimpleMM:     result = 25.0 / 25.4; break;
		case UnitsMM:           break;
		default:                result = 1000.0 / 25.4; break;
		};
		break;

	default:
		switch (newUnits)
		{
		case UnitsSimpleMM:     result = 25.0 / 1000.0; break;
		case UnitsMM:           result = 25.4 / 1000.0; break;
		default:                break;
		};
		break;
	}

	return result;
}
//----------------------------------------------------------------------------------------------------------------------
QRectF rotateRect(const QRectF& rect, qreal rotationAngle)
{
	qreal l = rect.left();
	qreal t = rect.top();
	qreal r = rect.right();
	qreal b = rect.bottom();
	qreal deltaX, deltaY;

	while (rotationAngle >= 360.0) rotationAngle -= 360.0;
	while (rotationAngle < 0.0) rotationAngle += 360.0;

	if (rotationAngle == 90)
	{
		deltaX = l; deltaY = t;
		l = -deltaY; t = deltaX;
		deltaX = r; deltaY = b;
		r = -deltaY; b = deltaX;
	}
	else if (rotationAngle == 180)
	{
		l *= -1; t *= -1; r *= -1; b *= -1;
	}
	else if (rotationAngle == 270)
	{
		deltaX = l; deltaY = t;
		l = deltaY; t = -deltaX;
		deltaX = r; deltaY = b;
		r = deltaY; b = -deltaX;
	}

	return QRectF(qMin(l, r), qMin(t, b), qAbs(r - l), qAbs(b - t));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QPolygonF outlineLineShape(const QPointF& startPoint, const QPointF& endPoint, qreal outlineWidth)
{
	const qreal kPi = 3.14159265358;

	QPolygonF polygon;
	qreal angle = qAtan2(endPoint.y() - startPoint.y(), endPoint.x() - startPoint.x());

	outlineWidth *= qSqrt(2) / 2;

	polygon.append(QPointF(startPoint.x() + outlineWidth * qCos(angle + 3 * kPi / 4),
						   startPoint.y() + outlineWidth * qSin(angle + 3 * kPi / 4)));
	polygon.append(QPointF(startPoint.x() + outlineWidth * qCos(angle + 5 * kPi / 4),
						   startPoint.y() + outlineWidth * qSin(angle + 5 * kPi / 4)));
	polygon.append(QPointF(endPoint.x() + outlineWidth * qCos(angle - kPi / 4),
						   endPoint.y() + outlineWidth * qSin(angle - kPi / 4)));
	polygon.append(QPointF(endPoint.x() + outlineWidth * qCos(angle + kPi / 4),
						   endPoint.y() + outlineWidth * qSin(angle + kPi / 4)));

	return polygon;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QSizeF textSize(const QString& text, const QFont& font, QPaintDevice* device)
{
	QFontMetricsF fontMetrics(font, device);
	QStringList lines = text.split("\n");
	QSizeF textSize(0, (lines.size() - 1) * fontMetrics.leading());
	QString line;
	QRectF tempRect;

	while (!lines.isEmpty())
	{
		line = lines.takeFirst();

		if (line.isEmpty()) tempRect = QRectF(0, 0, 0, fontMetrics.height());
		else tempRect = fontMetrics.boundingRect(line);

		textSize.setWidth(qMax(textSize.width(), tempRect.width()));
		textSize.setHeight(textSize.height() + tempRect.height());
	}

	return textSize;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
qreal distanceFromPointToLineSegment(const QPointF& point, const QLineF& line)
{
	qreal distance = 1E10;
	qreal dotAbBc, dotBaAc, crossABC, distanceAB, distanceAC, distanceBC;

	// Let A = line point 0, B = line point 1, and C = point
	dotAbBc = (line.x2() - line.x1()) * (point.x() - line.x2()) +
			  (line.y2() - line.y1()) * (point.y() - line.y2());
	dotBaAc = (line.x1() - line.x2()) * (point.x() - line.x1()) +
			  (line.y1() - line.y2()) * (point.y() - line.y1());
	crossABC = (line.x2() - line.x1()) * (point.y() - line.y1()) -
			   (line.y2() - line.y1()) * (point.x() - line.x1());
	distanceAB = qSqrt( (line.x2() - line.x1()) * (line.x2() - line.x1()) +
						(line.y2() - line.y1()) * (line.y2() - line.y1()) );
	distanceAC = qSqrt( (point.x() - line.x1()) * (point.x() - line.x1()) +
						(point.y() - line.y1()) * (point.y() - line.y1()) );
	distanceBC = qSqrt( (point.x() - line.x2()) * (point.x() - line.x2()) +
						(point.y() - line.y2()) * (point.y() - line.y2()) );

	if (distanceAB != 0)
	{
		if (dotAbBc > 0) distance = distanceBC;
		else if (dotBaAc > 0) distance = distanceAC;
		else distance = qAbs(crossABC) / distanceAB;
	}

	return distance;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QString penToString(const QPen& pen)
{
	return (brushToString(pen.brush()) + "," + QString::number(pen.widthF()) + "," +
		QString::number((quint32)pen.style()) + "," +
		QString::number((quint32)pen.capStyle()) + "," + QString::number((quint32)pen.joinStyle()));
}
//--------------------------------------------------------------------------------------------------
QString brushToString(const QBrush& brush)
{
	QString value = QString::number(brush.color().rgba(), 16);
	value = "#" + QString(8 - value.size(), '0') + value;
	return value;
}
//--------------------------------------------------------------------------------------------------
QString fontToString(const QFont& font)
{
	return font.family() + "," + QString::number(font.pointSizeF()) + "," +
		((font.bold()) ? "true" : "false") + ","  +
		((font.italic()) ? "true" : "false") + ","  +
		((font.underline()) ? "true" : "false") + ","  +
		((font.overline()) ? "true" : "false") + ","  +
		((font.strikeOut()) ? "true" : "false") + ","  +
		QString::number((quint32)font.capitalization());
}
//--------------------------------------------------------------------------------------------------
QString pathToString(const QPainterPath& path)
{
	QString value;
	QList<QPointF> curvePoints;
	QPainterPath::Element element;

	for(int i = 0; i < path.elementCount(); i++)
	{
		element = path.elementAt(i);

		switch (element.type)
		{
		case QPainterPath::MoveToElement:
			value += "M " + QString::number(element.x) + " " + QString::number(element.y) + " ";
			break;

		case QPainterPath::LineToElement:
			value += "L " + QString::number(element.x) + " " + QString::number(element.y) + " ";
			break;

		case QPainterPath::CurveToElement:
			curvePoints.append(QPointF(element.x, element.y));
			break;

		case QPainterPath::CurveToDataElement:
			if (i + 1 >= path.elementCount() || path.elementAt(i+1).type != QPainterPath::CurveToDataElement)
			{
				if (curvePoints.size() == 1)
				{
					value += "S " + QString::number(curvePoints[0].x()) + " " + QString::number(curvePoints[0].y()) + " ";
					value += QString::number(element.x) + " " + QString::number(element.y) + " ";
				}
				else if (curvePoints.size() == 2)
				{
					value += "C " + QString::number(curvePoints[0].x()) + " " + QString::number(curvePoints[0].y()) + " ";
					value += QString::number(curvePoints[1].x()) + " " + QString::number(curvePoints[1].y()) + " ";
					value += QString::number(element.x) + " " + QString::number(element.y) + " ";
				}

				curvePoints.clear();
			}
			else curvePoints.append(QPointF(element.x, element.y));
			break;

		default:
			break;
		}
	}

	return value.trimmed();
}
//--------------------------------------------------------------------------------------------------
QPen penFromString(const QString& string, bool* ok)
{
	QPen pen;
	bool conversionOk = true;
	QStringList data = string.split(",", QString::SkipEmptyParts);

	if (data.isEmpty()) conversionOk = false;
	else
	{
		if (data.size() >= 1)
		{
			bool brushOk = true;
			QBrush brush = brushFromString(data[0], &brushOk);

			if (brushOk) pen.setBrush(brush);

			conversionOk = (conversionOk && brushOk);
		}
		if (data.size() >= 2)
		{
			bool widthOk = true;
			qreal widthValue = data[1].toDouble(&widthOk);

			if (widthOk) pen.setWidthF(widthValue);

			conversionOk = (conversionOk && widthOk);
		}
		if (data.size() >= 3)
		{
			bool styleOk = true;
			Qt::PenStyle styleValue = (Qt::PenStyle)data[2].toUInt(&styleOk);

			if (styleOk) pen.setStyle(styleValue);

			conversionOk = (conversionOk && styleOk);
		}
		if (data.size() >= 4)
		{
			bool capStyleOk = true;
			Qt::PenCapStyle capStyleValue = (Qt::PenCapStyle)data[3].toUInt(&capStyleOk);

			if (capStyleOk) pen.setCapStyle(capStyleValue);

			conversionOk = (conversionOk && capStyleOk);
		}
		if (data.size() >= 5)
		{
			bool joinStyleOk = true;
			Qt::PenJoinStyle joinStyleValue = (Qt::PenJoinStyle)data[4].toUInt(&joinStyleOk);

			if (joinStyleOk) pen.setJoinStyle(joinStyleValue);

			conversionOk = (conversionOk && joinStyleOk);
		}
	}

	if (ok) *ok = conversionOk;
	return pen;
}
//--------------------------------------------------------------------------------------------------
QBrush brushFromString(const QString& string, bool* ok)
{
	QBrush brush = Qt::black;
	bool conversionOk = true;

	if (string.startsWith("#"))
	{
		bool rgbaOk = true;
		QRgb rgba = string.right(string.size() - 1).toUInt(&rgbaOk, 16);

		if (rgbaOk)
		{
			QColor color;
			color.setRgba(rgba);
			brush.setColor(color);
		}

		conversionOk = (conversionOk && rgbaOk);
	}
	else conversionOk = false;

	if (ok) *ok = conversionOk;
	return brush;
}
//--------------------------------------------------------------------------------------------------
QFont fontFromString(const QString& string, bool* ok)
{
	QFont font;
	bool conversionOk = true;
	QStringList data = string.split(",", QString::SkipEmptyParts);

	if (data.isEmpty()) conversionOk = false;
	else
	{
		if (data.size() >= 1) font.setFamily(data[0]);
		if (data.size() >= 2)
		{
			bool sizeOk = true;
			qreal sizeValue = data[1].toDouble(&sizeOk);

			if (sizeOk) font.setPointSizeF(sizeValue);

			conversionOk = (conversionOk && sizeOk);
		}
		if (data.size() >= 3) font.setBold(data[2].toLower() == "true");
		if (data.size() >= 4) font.setItalic(data[3].toLower() == "true");
		if (data.size() >= 5) font.setUnderline(data[4].toLower() == "true");
		if (data.size() >= 6) font.setOverline(data[5].toLower() == "true");
		if (data.size() >= 7) font.setStrikeOut(data[6].toLower() == "true");
		if (data.size() >= 8)
		{
			bool capitalizationOk = true;
			QFont::Capitalization capitialization = (QFont::Capitalization)data[7].toUInt(&capitalizationOk);

			if (capitalizationOk) font.setCapitalization(capitialization);

			conversionOk = (conversionOk && capitalizationOk);
		}
	}

	if (ok) *ok = conversionOk;
	return font;
}
//--------------------------------------------------------------------------------------------------
QPainterPath pathFromString(const QString& string, bool* ok)
{
	QPainterPath path;
	bool conversionOk = true;

	QStringList data = string.split(" ", QString::SkipEmptyParts);
	int index = 0;
	qreal positionX, positionY;
	qreal curvePosition0X, curvePosition0Y;
	qreal curvePosition1X, curvePosition1Y;

	if (data.isEmpty()) conversionOk = false;
	else
	{
		while (conversionOk && index < data.size())
		{
			if (data[index] == "M" && index+2 < data.size())
			{
				positionX = data[index+1].toDouble(&conversionOk);
				if (conversionOk) positionY = data[index+2].toDouble(&conversionOk);

				if (conversionOk) path.moveTo(positionX, positionY);
				index += 3;
			}
			else if (data[index] == "L" && index+2 < data.size())
			{
				positionX = data[index+1].toDouble(&conversionOk);
				if (conversionOk) positionY = data[index+2].toDouble(&conversionOk);

				if (conversionOk) path.lineTo(positionX, positionY);
				index += 3;
			}
			else if (data[index] == "C" && index+6 < data.size())
			{
				curvePosition0X = data[index+1].toDouble(&conversionOk);
				if (conversionOk) curvePosition0Y = data[index+2].toDouble(&conversionOk);
				if (conversionOk) curvePosition1X = data[index+3].toDouble(&conversionOk);
				if (conversionOk) curvePosition1Y = data[index+4].toDouble(&conversionOk);
				if (conversionOk) positionX = data[index+5].toDouble(&conversionOk);
				if (conversionOk) positionY = data[index+6].toDouble(&conversionOk);

				if (conversionOk) path.cubicTo(curvePosition0X, curvePosition0Y,
					curvePosition1X, curvePosition1Y, positionX, positionY);
				index += 7;
			}
			else if (data[index] == "S" && index+4 < data.size())
			{
				curvePosition0X = data[index+1].toDouble(&conversionOk);
				if (conversionOk) curvePosition0Y = data[index+2].toDouble(&conversionOk);
				if (conversionOk) positionX = data[index+3].toDouble(&conversionOk);
				if (conversionOk) positionY = data[index+4].toDouble(&conversionOk);

				if (conversionOk) path.quadTo(curvePosition0X, curvePosition0Y, positionX, positionY);
				index += 5;
			}
			else conversionOk = false;
		}
	}

	if (ok) *ok = conversionOk;
	return path;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QString pointToString(const QPointF& point, DiagramUnits units)
{
	QString result;

	switch (units)
	{
	case UnitsSimpleMM:
	case UnitsMM:
		result = "(" + QString::number(point.x(), 'f', 1) + "," + QString::number(point.y(), 'f', 1) + ")";
		break;
	default:
		result = "(" + QString::number(point.x(), 'f', 0) + "," + QString::number(point.y(), 'f', 0) + ")";
		break;
	}

	return result;
}
//--------------------------------------------------------------------------------------------------
