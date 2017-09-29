/* DiagramItemDefaultsWidget.h
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


#ifndef DIAGRAMITEMDEFAULTSWIDGET_H
#define DIAGRAMITEMDEFAULTSWIDGET_H

#include <DiagramTypes.h>

class DiagramItemDefaultsPreviewWidget;

class DiagramItemDefaultsWidget : public QFrame
{
	Q_OBJECT

private:
	DiagramItemDefaultsPreviewWidget* mPreviewWidget;
	QPushButton* mPenButton;
	QPushButton* mBrushButton;
	QPushButton* mTextButton;

public:
	DiagramItemDefaultsWidget();
	~DiagramItemDefaultsWidget();

	void setPen(const QPen& pen);
    void setStartArrow(const DiagramArrow& arrow);
    void setEndArrow(const DiagramArrow& arrow);
	void setBrush(const QBrush& brush);
	void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
	QPen pen() const;
    DiagramArrow startArrow() const;
    DiagramArrow endArrow() const;
	QBrush brush() const;
	QFont font() const;
	QBrush textBrush() const;
	QHash<QString, QVariant> properties() const;

signals:
	void itemPropertiesChanged(const QHash<QString, QVariant>& properties);

private slots:
	void runPenDialog();
	void runBrushDialog();
	void runTextDialog();
};

//==================================================================================================

class DiagramItemDefaultsPreviewWidget : public QFrame
{
	Q_OBJECT

private:
	QPen mPen;
    DiagramArrow mStartArrow;
    DiagramArrow mEndArrow;
	QBrush mBrush;
	QFont mFont;
	QBrush mTextBrush;

	QBrush mTransparentBrush;

public:
	DiagramItemDefaultsPreviewWidget();
	~DiagramItemDefaultsPreviewWidget();

	void setPen(const QPen& pen);
    void setStartArrow(const DiagramArrow& arrow);
    void setEndArrow(const DiagramArrow& arrow);
	QPen pen() const;
    DiagramArrow startArrow() const;
    DiagramArrow endArrow() const;

	void setBrush(const QBrush& brush);
	QBrush brush() const;

	void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
	QFont font() const;
	QBrush textBrush() const;

private:
	void paintEvent(QPaintEvent* event);
	void drawLine(QPainter& painter);
	void drawTextBox(QPainter& painter, const QBrush& textBrush);
};

#endif
