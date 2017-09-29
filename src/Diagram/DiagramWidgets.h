/* DiagramWidgets.h
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

#ifndef DIAGRAMWIDGETS_H
#define DIAGRAMWIDGETS_H

#include <DiagramTypes.h>

class DiagramColorWidget;
class DiagramPositionLayout;

class DiagramArrowStyleCombo : public QComboBox
{
	Q_OBJECT

private:
	bool mArrowDirectionRight;

public:
	DiagramArrowStyleCombo(QWidget* parent = NULL);
	~DiagramArrowStyleCombo();

	void setArrowStyle(DiagramArrow::Style style);
	DiagramArrow::Style arrowStyle() const;

	void setLabelsVisible(bool visible);
	void setArrowDirectionRight(bool right);
	bool areLabelsVisible() const;
	bool isArrowDirectionRight() const;

private:
	void refreshItems(bool arrowsRight, bool labelsVisible);
};

//==================================================================================================

class DiagramBrushGroup : public QGroupBox
{
	Q_OBJECT

public:
	enum Capability { NoCapabilities = 0x00, ShowColor = 0x01, ShowFilled = 0x02 };
	Q_DECLARE_FLAGS(Capabilities, Capability)

private:
	QBrush mBrush;
	Capabilities mCapabilities;

	DiagramColorWidget* colorWidget;
	QCheckBox* filledCheck;

public:
	DiagramBrushGroup(Capabilities capabilities = ShowColor, QWidget* parent = NULL);
	~DiagramBrushGroup();

public slots:
	void setBrush(const QBrush& brush);

public:
	QBrush brush() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramBrushGroup::Capabilities)

//==================================================================================================

class DiagramColorWidget : public QPushButton
{
	Q_OBJECT

private:
	QColor mColor;
	QImage mImage;

public:
	DiagramColorWidget(QWidget* parent = NULL);
	~DiagramColorWidget();

public slots:
	void setCurrentColor(const QColor& color);

public:
	QColor currentColor() const;

private slots:
	void clickedEvent();

private:
	void fillImage(QImage& image, const QColor& color);
	QString colorToString(const QColor& color);
};

//==================================================================================================

class DiagramFontToolBar : public QWidget
{
	Q_OBJECT

public:
	enum Capability { NoCapabilities = 0x00, ShowFont = 0x01, ShowBrush = 0x02,
		ShowAlignment = 0x04, ShowAll = 0xFF };
	Q_DECLARE_FLAGS(Capabilities, Capability)

private:
	QFont mFont;
	QBrush mBrush;
	Qt::Alignment mAlignment;
	Capabilities mCapabilities;

	QFontComboBox* fontCombo;
	QLineEdit* sizeEdit;
	DiagramColorWidget* colorWidget;
	QToolButton* boldButton;
	QToolButton* italicButton;
	QToolButton* underlineButton;
	QToolButton* strikeThroughButton;
	QToolButton* horizontalAlignmentButton;
	QToolButton* verticalAlignmentButton;

	QMenu horizontalAlignmentMenu;
	QMenu verticalAlignmentMenu;
	QActionGroup* horizontalAlignmentActionGroup;
	QActionGroup* verticalAlignmentActionGroup;

public:
	DiagramFontToolBar(Capabilities capabilities = ShowAll, const QSizeF& diagramSize = QSizeF(-1, -1),
		QWidget* parent = NULL);
	~DiagramFontToolBar();

public slots:
	void setFont(const QFont& f);
	void setBrush(const QBrush& brush);
	void setAlignment(Qt::Alignment alignment);

public:
	QFont font() const;
	QBrush brush() const;
	Qt::Alignment alignment() const;

private slots:
	void horizontalAlignmentChanged(QAction* action);
	void verticalAlignmentChanged(QAction* action);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramFontToolBar::Capabilities)

//==================================================================================================

class DiagramItemPointGroup : public QGroupBox
{
	Q_OBJECT

public:
	enum Capability { NoCapabilities = 0x00, ShowPosition = 0x01, ShowArrow = 0x02, ShowAll = 0xFF };
	Q_DECLARE_FLAGS(Capabilities, Capability)

private:
	Capabilities mCapabilities;

	DiagramPositionLayout* positionLayout;
	DiagramArrowStyleCombo* arrowStyleCombo;
	QLineEdit* arrowSizeEdit;

public:
	DiagramItemPointGroup(const QString& title = "Point", Capabilities capabilities = ShowAll,
		const QSizeF& diagramSize = QSizeF(-1, -1), QWidget* parent = NULL);
	~DiagramItemPointGroup();

public slots:
	void setPosition(const QPointF& pos);

public:
	void setArrow(const DiagramArrow& arrow);
	QPointF position() const;
	DiagramArrow arrow() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramItemPointGroup::Capabilities)

//==================================================================================================

class DiagramPenGroup : public QGroupBox
{
	Q_OBJECT

public:
	enum Capability { NoCapabilities = 0x00, ShowBrush = 0x01, ShowWidth = 0x02, ShowStyle = 0x04,
		ShowAll = 0xFF };
	Q_DECLARE_FLAGS(Capabilities, Capability)

private:
	QPen mPen;
	Capabilities mCapabilities;

	DiagramColorWidget* colorWidget;
	QLineEdit* widthEdit;
	QComboBox* styleCombo;

public:
	DiagramPenGroup(Capabilities capabilities = ShowAll, const QSizeF& diagramSize = QSizeF(-1, -1),
		QWidget* parent = NULL);
	~DiagramPenGroup();

public slots:
	void setPen(const QPen& pen);

public:
	QPen pen() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramPenGroup::Capabilities)

//==================================================================================================

class DiagramPositionLayout : public QHBoxLayout
{
	Q_OBJECT

private:
	QLineEdit* xEdit;
	QLineEdit* yEdit;

public:
	DiagramPositionLayout(const QSizeF& diagramSize = QSizeF(-1, -1), QWidget* parent = NULL);
	~DiagramPositionLayout();

public slots:
	void setPosition(const QPointF& position);
	void setPosition(qreal x, qreal y);
	void setPositionX(qreal x);
	void setPositionY(qreal y);

public:
	QPointF position() const;
	qreal positionX() const;
	qreal positionY() const;

	void setVisible(bool visible);
	bool isVisible() const;
};

//==================================================================================================

class DiagramTwoButtonDialog : public QDialog
{
    Q_OBJECT

private:
    QVBoxLayout* mLayout;

public:
    DiagramTwoButtonDialog(QWidget* parent = NULL);
    virtual ~DiagramTwoButtonDialog();

    void addWidget(QWidget* widget, int stretch = 0, Qt::Alignment alignment = 0);
    void addButtonBox();
};

#endif
