/* DiagramTypes.h
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

#ifndef DIAGRAMTYPES_H
#define DIAGRAMTYPES_H

#include <QtCore>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui>
#else
#include <QtWidgets>
#endif

enum DiagramUnits { UnitsMils, UnitsSimpleMM, UnitsMM };
enum DiagramItemPlaceMode { DoNotPlace, PlaceStrict, PlaceLoose };

//==================================================================================================

class DiagramPainter;

class DiagramArrow
{
public:
	enum Style { None, Normal, Triangle, TriangleFilled,
		Circle, CircleFilled, Diamond, DiamondFilled,
		Harpoon, HarpoonMirrored, Concave, ConcaveFilled, Reverse, XArrow };

private:
	Style mStyle;
	qreal mSize;

public:
	DiagramArrow(Style style = None, qreal size = 100.0);
	DiagramArrow(const DiagramArrow& arrow);
	virtual ~DiagramArrow();

	DiagramArrow& operator=(const DiagramArrow& arrow);

	void setArrow(Style style, qreal size);
	void setStyle(Style style);
	void setSize(qreal size);
	Style style() const;
	qreal size() const;

	QString toString() const;

	void render(QPainter* painter, const QPointF& position, qreal direction);

	void drawNormal(QPainter* painter, const QPointF& position, qreal direction);
	void drawTriangle(QPainter* painter, const QPointF& position, qreal direction);
	void drawCircle(QPainter* painter, const QPointF& position, qreal direction);
	void drawDiamond(QPainter* painter, const QPointF& position, qreal direction);
	void drawHarpoon(QPainter* painter, const QPointF& position, qreal direction);
	void drawConcave(QPainter* painter, const QPointF& position, qreal direction);
	void drawReverse(QPainter* painter, const QPointF& position, qreal direction);
	void drawX(QPainter* painter, const QPointF& position, qreal direction);

	bool operator==(const DiagramArrow& arrow) const;
	bool operator!=(const DiagramArrow& arrow) const;

public:
	static DiagramArrow fromString(const QString& string, bool* ok = NULL);
};

Q_DECLARE_METATYPE(DiagramArrow)

//==================================================================================================

class DiagramBase;

class DiagramMouseEvent : public QEvent
{
private:
	Qt::MouseButton mButton;
	Qt::MouseButtons mButtons;
	Qt::KeyboardModifiers mModifiers;

	QPointF mDiagramPos;
	QPointF mPreviousDiagramPos;
	QPointF mButtonDownDiagramPos;

	QPoint mScreenPos;
	QPoint mPreviousScreenPos;
	QPoint mButtonDownScreenPos;

	QPoint mButtonDownScrollValue;
	bool mDragged;

public:
	DiagramMouseEvent();
	~DiagramMouseEvent();

	void setType(QEvent::Type type);
	void update(DiagramBase* diagram, QMouseEvent* event);
	void update(DiagramBase* diagram, QContextMenuEvent* event);

	Qt::MouseButton button() const;
	Qt::MouseButtons buttons() const;
	Qt::KeyboardModifiers modifiers() const;

	QPointF diagramPos() const;
	QPointF previousDiagramPos() const;
	QPointF buttonDownDiagramPos() const;

	QPoint screenPos() const;
	QPoint previousScreenPos() const;
	QPoint buttonDownScreenPos() const;

	QPoint buttonDownScrollValue() const;
	bool isDragged() const;
};

//==================================================================================================

class DiagramPainter : public QPainter
{
public:
	enum Mode { Normal, BlackOnWhite, WhiteOnBlack };
	enum BrushType { Background, Border, Grid, RubberBand, ResizePoint, Hotpoint, Item0, Item1, UserDefined };

private:
	Mode mMode;

	QFont mDefaultFont;
	QPen mDefaultPen;

	QList<QBrush> mBrushes;     // Always will have enough to cover BrushType enums

	bool mDrawBackground;
	bool mDrawBorder;
	bool mDrawGrid;

public:
	DiagramPainter();
	DiagramPainter(QPaintDevice* device);
	~DiagramPainter();

	void setMode(Mode mode);
	Mode mode() const;

	void setDefaultFont(const QFont& font);
	void setDefaultPen(const QPen& pen);
	QFont defaultFont() const;
	QPen defaultPen() const;

	void addBrush(const QBrush& brush);
	void insertBrush(int index, const QBrush& brush);
	void removeBrush(int index);
	void clearBrushes();
	int numberOfBrushes() const;
	void setBrushAt(int index, const QBrush& brush);
	QBrush brushAt(int index) const;

	void setDrawBackground(bool draw);
	void setDrawBorder(bool draw);
	void setDrawGrid(bool draw);
	bool shouldDrawBackground() const;
	bool shouldDrawBorder() const;
	bool shouldDrawGrid() const;

	void setForItem(const QPen& itemPen, const QBrush& itemBrush = Qt::transparent);
	QBrush outputBrush(int index) const;
};

//==================================================================================================

qreal unitsScale(DiagramUnits units, DiagramUnits newUnits);
QRectF rotateRect(const QRectF& rect, qreal rotationAngle);

QPolygonF outlineLineShape(const QPointF& startPoint, const QPointF& endPoint, qreal outlineWidth);

QSizeF textSize(const QString& text, const QFont& font, QPaintDevice* device);

qreal distanceFromPointToLineSegment(const QPointF& point, const QLineF& line);

QString penToString(const QPen& pen);
QString brushToString(const QBrush& brush);
QString fontToString(const QFont& font);
QString pathToString(const QPainterPath& path);
QPen penFromString(const QString& string, bool* ok = NULL);
QBrush brushFromString(const QString& string, bool* ok = NULL);
QFont fontFromString(const QString& string, bool* ok = NULL);
QPainterPath pathFromString(const QString& string, bool* ok = NULL);

inline QRectF rectFromPoints(const QPointF& point0, const QPointF& point1)
{
	return QRectF(qMin(point0.x(), point1.x()), qMin(point0.y(), point1.y()),
			qAbs(point0.x() - point1.x()), qAbs(point0.y() - point1.y()));
}

QString pointToString(const QPointF& point, DiagramUnits units);

inline qreal magnitude(const QPointF &vec) { return qSqrt(vec.x() * vec.x() + vec.y() * vec.y()); }
inline qreal magnitude(const QPoint &vec) { return qSqrt(vec.x() * vec.x() + vec.y() * vec.y()); }

#endif
