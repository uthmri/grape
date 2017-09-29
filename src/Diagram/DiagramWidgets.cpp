/* DiagramWidgets.cpp
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

#include <DiagramWidgets.h>

DiagramArrowStyleCombo::DiagramArrowStyleCombo(QWidget* parent) : QComboBox(parent)
{
	refreshItems(false, true);
}
//--------------------------------------------------------------------------------------------------
DiagramArrowStyleCombo::~DiagramArrowStyleCombo() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramArrowStyleCombo::setArrowStyle(DiagramArrow::Style style)
{
	switch (style)
	{
	case DiagramArrow::Normal: setCurrentIndex(1); break;
	case DiagramArrow::Reverse: setCurrentIndex(2); break;
	case DiagramArrow::Triangle: setCurrentIndex(3); break;
	case DiagramArrow::TriangleFilled: setCurrentIndex(4); break;
	case DiagramArrow::Concave: setCurrentIndex(5); break;
	case DiagramArrow::ConcaveFilled: setCurrentIndex(6); break;
	case DiagramArrow::Circle: setCurrentIndex(7); break;
	case DiagramArrow::CircleFilled: setCurrentIndex(8); break;
	case DiagramArrow::Diamond: setCurrentIndex(9); break;
	case DiagramArrow::DiamondFilled: setCurrentIndex(10); break;
	case DiagramArrow::Harpoon: setCurrentIndex(11); break;
	case DiagramArrow::HarpoonMirrored: setCurrentIndex(12); break;
	case DiagramArrow::XArrow: setCurrentIndex(13); break;
	default: setCurrentIndex(0); break;
	}
}
//--------------------------------------------------------------------------------------------------
DiagramArrow::Style DiagramArrowStyleCombo::arrowStyle() const
{
	DiagramArrow::Style style = DiagramArrow::None;

	switch (currentIndex())
	{
	case 1: style = DiagramArrow::Normal; break;
	case 2: style = DiagramArrow::Reverse; break;
	case 3: style = DiagramArrow::Triangle; break;
	case 4: style = DiagramArrow::TriangleFilled; break;
	case 5: style = DiagramArrow::Concave; break;
	case 6: style = DiagramArrow::ConcaveFilled; break;
	case 7: style = DiagramArrow::Circle; break;
	case 8: style = DiagramArrow::CircleFilled; break;
	case 9: style = DiagramArrow::Diamond; break;
	case 10: style = DiagramArrow::DiamondFilled; break;
	case 11: style = DiagramArrow::Harpoon; break;
	case 12: style = DiagramArrow::HarpoonMirrored; break;
	case 13: style = DiagramArrow::XArrow; break;
	default: break;
	}

	return style;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramArrowStyleCombo::setLabelsVisible(bool visible)
{
	refreshItems(isArrowDirectionRight(), visible);
}
//--------------------------------------------------------------------------------------------------
void DiagramArrowStyleCombo::setArrowDirectionRight(bool right)
{
	refreshItems(right, areLabelsVisible());
}
//--------------------------------------------------------------------------------------------------
bool DiagramArrowStyleCombo::areLabelsVisible() const
{
	return !itemText(0).isEmpty();
}
//--------------------------------------------------------------------------------------------------
bool DiagramArrowStyleCombo::isArrowDirectionRight() const
{
	return mArrowDirectionRight;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramArrowStyleCombo::refreshItems(bool arrowsRight, bool labelsVisible)
{
	int index = currentIndex();

	clear();
	mArrowDirectionRight = arrowsRight;

	if (arrowsRight)
	{
		if (labelsVisible)
		{
			addItem(QIcon(":/icons/custom/arrow/right_arrow_none.png"), "None");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_normal.png"), "Normal");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_reverse.png"), "Reverse");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_triangle.png"), "Triangle");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_triangle_filled.png"), "Triangle Filled");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_concave.png"), "Concave");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_concave_filled.png"), "Concave Filled");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_circle.png"), "Circle");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_circle_filled.png"), "Circle Filled");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_diamond.png"), "Diamond");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_diamond_filled.png"), "Diamond Filled");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_harpoon.png"), "Harpoon");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_harpoon_mirrored.png"), "Harpoon Mirrored");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_x.png"), "X");
		}
		else
		{
			addItem(QIcon(":/icons/custom/arrow/right_arrow_none.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_normal.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_reverse.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_triangle.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_triangle_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_concave.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_concave_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_circle.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_circle_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_diamond.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_diamond_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_harpoon.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_harpoon_mirrored.png"), "");
			addItem(QIcon(":/icons/custom/arrow/right_arrow_x.png"), "");
		}
	}
	else
	{
		if (labelsVisible)
		{
			addItem(QIcon(":/icons/custom/arrow/left_arrow_none.png"), "None");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_normal.png"), "Normal");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_reverse.png"), "Reverse");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_triangle.png"), "Triangle");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_triangle_filled.png"), "Triangle Filled");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_concave.png"), "Concave");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_concave_filled.png"), "Concave Filled");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_circle.png"), "Circle");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_circle_filled.png"), "Circle Filled");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_diamond.png"), "Diamond");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_diamond_filled.png"), "Diamond Filled");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_harpoon.png"), "Harpoon");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_harpoon_mirrored.png"), "Harpoon Mirrored");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_x.png"), "X");
		}
		else
		{
			addItem(QIcon(":/icons/custom/arrow/left_arrow_none.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_normal.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_reverse.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_triangle.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_triangle_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_concave.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_concave_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_circle.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_circle_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_diamond.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_diamond_filled.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_harpoon.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_harpoon_mirrored.png"), "");
			addItem(QIcon(":/icons/custom/arrow/left_arrow_x.png"), "");
		}
	}

	setCurrentIndex(index);
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramBrushGroup::DiagramBrushGroup(Capabilities capabilities, QWidget* parent) : QGroupBox("Brush", parent)
{
	mBrush = Qt::white;
	mCapabilities = capabilities;

	colorWidget = new DiagramColorWidget();
	filledCheck = new QCheckBox();

	QFormLayout* fLayout = new QFormLayout();
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->addRow("Color: ", colorWidget);
	fLayout->addRow("Filled: ", filledCheck);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	fLayout->itemAt(1, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	setLayout(fLayout);

	if ((mCapabilities & ShowColor) == 0)
	{
		colorWidget->setVisible(false);
		fLayout->labelForField(colorWidget)->setVisible(false);
	}
	if ((mCapabilities & ShowFilled) == 0)
	{
		filledCheck->setVisible(false);
		fLayout->labelForField(filledCheck)->setVisible(false);
	}

	setBrush(Qt::white);
}
//--------------------------------------------------------------------------------------------------
DiagramBrushGroup::~DiagramBrushGroup() { }
//--------------------------------------------------------------------------------------------------
void DiagramBrushGroup::setBrush(const QBrush& brush)
{
	mBrush = brush;

	colorWidget->setCurrentColor(brush.color());
	filledCheck->setChecked(brush.color().alpha() != 0);
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramBrushGroup::brush() const
{
	QBrush brush = mBrush;

	if (mCapabilities & ShowColor) brush = QBrush(colorWidget->currentColor());
	else if (mCapabilities & ShowFilled)
	{
		QColor color = brush.color();
		color.setAlpha(filledCheck->isChecked() ? 255 : 0);
		brush.setColor(color);
	}

	return brush;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramColorWidget::DiagramColorWidget(QWidget* parent) : QPushButton(parent)
{
	mImage = QImage(14, 14, QImage::Format_RGB32);

	connect(this, SIGNAL(clicked()), this, SLOT(clickedEvent()));
	setCurrentColor(QColor(0, 0, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramColorWidget::~DiagramColorWidget() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramColorWidget::setCurrentColor(const QColor& color)
{
	mColor = color;

	fillImage(mImage, mColor);
	setText(colorToString(mColor));
	setIcon(QIcon(QPixmap::fromImage(mImage)));
}
//--------------------------------------------------------------------------------------------------
QColor DiagramColorWidget::currentColor() const
{
	return mColor;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramColorWidget::clickedEvent()
{
	QColor color = QColorDialog::getColor(mColor, this, "Select Color", QColorDialog::ShowAlphaChannel);
	if (color.isValid()) setCurrentColor(color);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramColorWidget::fillImage(QImage& image, const QColor& color)
{
	for(int y = 0; y < image.height(); y++)
	for(int x = 0; x < image.width(); x++)
	{
		if (x == 0 || x == image.width() - 1 || y == 0 || y == image.height() - 1)
			image.setPixel(x, y, 0xFF222222);
		else
			image.setPixel(x, y, color.rgb());
	}
}
//--------------------------------------------------------------------------------------------------
QString DiagramColorWidget::colorToString(const QColor& color)
{
	QString result;

	if (color.alpha() == 0) result = "Transparent";
	else
	{

		result = "#";
		if (color.red() < 16) result += "0";
		result += QString::number(color.red(), 16);
		if (color.green() < 16) result += "0";
		result += QString::number(color.green(), 16);
		if (color.blue() < 16) result += "0";
		result += QString::number(color.blue(), 16);
		if (color.alpha() != 0xFF)
		{
			if (color.alpha() < 16) result += "0";
			result += QString::number(color.alpha(), 16);
		}
		result = result.toUpper();
	}

	return result;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramFontToolBar::DiagramFontToolBar(Capabilities capabilities, const QSizeF& diagramSize,
	QWidget* parent) : QWidget(parent)
{
	qreal maxSize;
	if (diagramSize.width() < 0 && diagramSize.height() < 0) maxSize = INT_MAX;
	else maxSize = qMax(diagramSize.width(), diagramSize.height());

	mCapabilities = capabilities;

	fontCombo = new QFontComboBox();
	sizeEdit = new QLineEdit();
	colorWidget = new DiagramColorWidget();

	boldButton = new QToolButton();
	italicButton = new QToolButton();
	underlineButton = new QToolButton();
	strikeThroughButton = new QToolButton();
	horizontalAlignmentButton = new QToolButton();
	verticalAlignmentButton = new QToolButton();
	boldButton->setIcon(QIcon(":/icons/oxygen/format-text-bold.png"));
	italicButton->setIcon(QIcon(":/icons/oxygen/format-text-italic.png"));
	underlineButton->setIcon(QIcon(":/icons/oxygen/format-text-underline.png"));
	strikeThroughButton->setIcon(QIcon(":/icons/oxygen/format-text-strikethrough.png"));
	horizontalAlignmentButton->setIcon(QIcon(":/icons/oxygen/align-horizontal-center.png"));
	verticalAlignmentButton->setIcon(QIcon(":/icons/oxygen/align-vertical-center.png"));
	boldButton->setCheckable(true);
	italicButton->setCheckable(true);
	underlineButton->setCheckable(true);
	strikeThroughButton->setCheckable(true);
	horizontalAlignmentButton->setMenu(&horizontalAlignmentMenu);
	verticalAlignmentButton->setMenu(&verticalAlignmentMenu);
	horizontalAlignmentButton->setPopupMode(QToolButton::InstantPopup);
	verticalAlignmentButton->setPopupMode(QToolButton::InstantPopup);

	fontCombo->setMinimumWidth(180);
    sizeEdit->setValidator(new QDoubleValidator(0., (double)maxSize, 10, this));

	horizontalAlignmentActionGroup = new QActionGroup(this);
	horizontalAlignmentMenu.addAction(QIcon(":/icons/oxygen/align-horizontal-left.png"), "Align Left")->setCheckable(true);
	horizontalAlignmentMenu.addAction(QIcon(":/icons/oxygen/align-horizontal-center.png"), "Align Center")->setCheckable(true);
	horizontalAlignmentMenu.addAction(QIcon(":/icons/oxygen/align-horizontal-right.png"), "Align Right")->setCheckable(true);
	horizontalAlignmentActionGroup->addAction(horizontalAlignmentMenu.actions()[0]);
	horizontalAlignmentActionGroup->addAction(horizontalAlignmentMenu.actions()[1]);
	horizontalAlignmentActionGroup->addAction(horizontalAlignmentMenu.actions()[2]);
	connect(horizontalAlignmentActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(horizontalAlignmentChanged(QAction*)));
	horizontalAlignmentMenu.actions()[1]->setChecked(true);

	verticalAlignmentActionGroup = new QActionGroup(this);
	verticalAlignmentMenu.addAction(QIcon(":/icons/oxygen/align-vertical-top.png"), "Align Top")->setCheckable(true);
	verticalAlignmentMenu.addAction(QIcon(":/icons/oxygen/align-vertical-center.png"), "Align Center")->setCheckable(true);
	verticalAlignmentMenu.addAction(QIcon(":/icons/oxygen/align-vertical-bottom.png"), "Align Bottom")->setCheckable(true);
	verticalAlignmentActionGroup->addAction(verticalAlignmentMenu.actions()[0]);
	verticalAlignmentActionGroup->addAction(verticalAlignmentMenu.actions()[1]);
	verticalAlignmentActionGroup->addAction(verticalAlignmentMenu.actions()[2]);
	connect(verticalAlignmentActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(verticalAlignmentChanged(QAction*)));
	verticalAlignmentMenu.actions()[1]->setChecked(true);

	QFrame* vSeparator1 = new QFrame();
	QFrame* vSeparator2 = new QFrame();
	vSeparator1->setFrameStyle(QFrame::Sunken | QFrame::VLine);
	vSeparator2->setFrameStyle(QFrame::Sunken | QFrame::VLine);

	QHBoxLayout* topLayout = new QHBoxLayout();
	topLayout->addWidget(fontCombo, 10);
	topLayout->addWidget(sizeEdit, 4);

	QHBoxLayout* bottomLayout = new QHBoxLayout();
	bottomLayout->addWidget(boldButton);
	bottomLayout->addWidget(italicButton);
	bottomLayout->addWidget(underlineButton);
	bottomLayout->addWidget(strikeThroughButton);
	bottomLayout->addWidget(vSeparator1);
	bottomLayout->addWidget(colorWidget, 10);
	bottomLayout->addWidget(vSeparator2);
	bottomLayout->addWidget(horizontalAlignmentButton);
	bottomLayout->addWidget(verticalAlignmentButton);
	bottomLayout->addWidget(new QWidget(), 100);

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addLayout(topLayout);
	vLayout->addLayout(bottomLayout);
	vLayout->setSpacing(0);
	vLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(vLayout);

	setFont(QFont());
	setBrush(QBrush());
	setAlignment(Qt::AlignCenter);

	if ((mCapabilities & ShowFont) == 0)
	{
		fontCombo->setEnabled(false);
		sizeEdit->setEnabled(false);
		boldButton->setEnabled(false);
		italicButton->setEnabled(false);
		underlineButton->setEnabled(false);
		strikeThroughButton->setEnabled(false);
	}
	if ((mCapabilities & ShowBrush) == 0)
	{
		vSeparator1->setVisible(false);
		colorWidget->setVisible(false);
	}
	if ((mCapabilities & ShowAlignment) == 0)
	{
		vSeparator2->setVisible(false);
		horizontalAlignmentButton->setVisible(false);
		verticalAlignmentButton->setVisible(false);
	}
}
//--------------------------------------------------------------------------------------------------
DiagramFontToolBar::~DiagramFontToolBar() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramFontToolBar::setFont(const QFont& f)
{
	mFont = f;

	fontCombo->setCurrentFont(mFont);
	sizeEdit->setText(QString::number(mFont.pointSizeF()));
	boldButton->setChecked(mFont.bold());
	italicButton->setChecked(mFont.italic());
	underlineButton->setChecked(mFont.underline());
	strikeThroughButton->setChecked(mFont.strikeOut());
}
//--------------------------------------------------------------------------------------------------
void DiagramFontToolBar::setBrush(const QBrush& brush)
{
	mBrush = brush;
	colorWidget->setCurrentColor(mBrush.color());
}
//--------------------------------------------------------------------------------------------------
void DiagramFontToolBar::setAlignment(Qt::Alignment alignment)
{
	QList<QAction*> horizontalActions = horizontalAlignmentMenu.actions();
	QList<QAction*> verticalActions = verticalAlignmentMenu.actions();

	mAlignment = alignment;

	if (mAlignment & Qt::AlignLeft)
	{
		horizontalActions[0]->setChecked(true);
		horizontalAlignmentChanged(horizontalActions[0]);
	}
	else if (mAlignment & Qt::AlignRight)
	{
		horizontalActions[2]->setChecked(true);
		horizontalAlignmentChanged(horizontalActions[2]);
	}
	else
	{
		horizontalActions[1]->setChecked(true);
		horizontalAlignmentChanged(horizontalActions[1]);
	}

	if (mAlignment & Qt::AlignTop)
	{
		verticalActions[0]->setChecked(true);
		verticalAlignmentChanged(verticalActions[0]);
	}
	else if (mAlignment & Qt::AlignBottom)
	{
		verticalActions[2]->setChecked(true);
		verticalAlignmentChanged(verticalActions[2]);
	}
	else
	{
		verticalActions[1]->setChecked(true);
		verticalAlignmentChanged(verticalActions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QFont DiagramFontToolBar::font() const
{
	QFont font = mFont;

	font.setFamily(fontCombo->currentFont().family());
	font.setPointSizeF(sizeEdit->text().toDouble());
	font.setBold(boldButton->isChecked());
	font.setItalic(italicButton->isChecked());
	font.setUnderline(underlineButton->isChecked());
	font.setStrikeOut(strikeThroughButton->isChecked());

	return font;
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramFontToolBar::brush() const
{
	QBrush brush = mBrush;

	brush.setColor(colorWidget->currentColor());

	return brush;
}
//--------------------------------------------------------------------------------------------------
Qt::Alignment DiagramFontToolBar::alignment() const
{
	Qt::Alignment alignment = 0;

	QList<QAction*> horizontalActions = horizontalAlignmentMenu.actions();
	QList<QAction*> verticalActions = verticalAlignmentMenu.actions();

	if (horizontalActions[0]->isChecked()) alignment |= Qt::AlignLeft;
	else if (horizontalActions[2]->isChecked()) alignment |= Qt::AlignRight;
	else alignment |= Qt::AlignHCenter;

	if (verticalActions[0]->isChecked()) alignment |= Qt::AlignTop;
	else if (verticalActions[2]->isChecked()) alignment |= Qt::AlignBottom;
	else alignment |= Qt::AlignVCenter;

	return alignment;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramFontToolBar::horizontalAlignmentChanged(QAction* action)
{
	horizontalAlignmentButton->setIcon(action->icon());
}
//--------------------------------------------------------------------------------------------------
void DiagramFontToolBar::verticalAlignmentChanged(QAction* action)
{
	verticalAlignmentButton->setIcon(action->icon());
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemPointGroup::DiagramItemPointGroup(const QString& title, Capabilities capabilities,
	const QSizeF& diagramSize, QWidget* parent) : QGroupBox(title, parent)
{
	qreal maxSize;
	if (diagramSize.width() < 0 && diagramSize.height() < 0) maxSize = INT_MAX;
	else maxSize = qMax(diagramSize.width(), diagramSize.height());

	mCapabilities = capabilities;

	positionLayout = new DiagramPositionLayout(diagramSize);
	arrowStyleCombo = new DiagramArrowStyleCombo();
	arrowSizeEdit = new QLineEdit();
    arrowSizeEdit->setValidator(new QDoubleValidator((double)0, (double)maxSize, 10, this));

	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Position: ", positionLayout);
	fLayout->addRow("Arrow Style: ", arrowStyleCombo);
	fLayout->addRow("Arrow Size: ", arrowSizeEdit);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	fLayout->itemAt(1, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	setLayout(fLayout);

	if ((mCapabilities & ShowPosition) == 0)
	{
		positionLayout->setVisible(false);
		fLayout->labelForField(positionLayout)->setVisible(false);
	}
	if ((mCapabilities & ShowArrow) == 0)
	{
		arrowStyleCombo->setVisible(false);
		fLayout->labelForField(arrowStyleCombo)->setVisible(false);
		arrowSizeEdit->setVisible(false);
		fLayout->labelForField(arrowSizeEdit)->setVisible(false);
	}
}
//--------------------------------------------------------------------------------------------------
DiagramItemPointGroup::~DiagramItemPointGroup() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemPointGroup::setPosition(const QPointF& pos)
{
	positionLayout->setPosition(pos);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemPointGroup::setArrow(const DiagramArrow& arrow)
{
	arrowStyleCombo->setArrowStyle(arrow.style());
	arrowSizeEdit->setText(QString::number(arrow.size()));
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramItemPointGroup::position() const
{
	return positionLayout->position();
}
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramItemPointGroup::arrow() const
{
	return DiagramArrow(arrowStyleCombo->arrowStyle(), arrowSizeEdit->text().toDouble());
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPenGroup::DiagramPenGroup(Capabilities capabilities, const QSizeF& diagramSize,
	QWidget* parent) : QGroupBox("Pen", parent)
{
	qreal maxSize;
	if (diagramSize.width() < 0 && diagramSize.height() < 0) maxSize = INT_MAX;
	else maxSize = qMax(diagramSize.width(), diagramSize.height());

	mPen = QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	mCapabilities = capabilities;

	colorWidget = new DiagramColorWidget();

	widthEdit = new QLineEdit();
    widthEdit->setValidator(new QDoubleValidator(0., (double)maxSize, 10, this));

	styleCombo = new QComboBox();
	styleCombo->addItem("No Pen");
	styleCombo->addItem("Solid Line");
	styleCombo->addItem("Dash Line");
	styleCombo->addItem("Dot Line");
	styleCombo->addItem("Dash-Dot Line");
	styleCombo->addItem("Dash-Dot-Dot Line");

	QFormLayout* fLayout = new QFormLayout();
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->addRow("Style: ", styleCombo);
	fLayout->addRow("Line Width: ", widthEdit);
	fLayout->addRow("Color: ", colorWidget);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	fLayout->itemAt(1, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	fLayout->itemAt(2, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	setLayout(fLayout);

	setPen(mPen);

	if ((mCapabilities & ShowBrush) == 0)
	{
		colorWidget->setVisible(false);
		fLayout->labelForField(colorWidget)->setVisible(false);
	}
	if ((mCapabilities & ShowWidth) == 0)
	{
		widthEdit->setVisible(false);
		fLayout->labelForField(widthEdit)->setVisible(false);
	}
	if ((mCapabilities & ShowStyle) == 0)
	{
		styleCombo->setVisible(false);
		fLayout->labelForField(styleCombo)->setVisible(false);
	}
}
//--------------------------------------------------------------------------------------------------
DiagramPenGroup::~DiagramPenGroup() { }
//--------------------------------------------------------------------------------------------------
void DiagramPenGroup::setPen(const QPen& pen)
{
	mPen = pen;

	colorWidget->setCurrentColor(pen.brush().color());
	widthEdit->setText(QString::number(pen.widthF()));
	styleCombo->setCurrentIndex((int)pen.style());
}
//--------------------------------------------------------------------------------------------------
QPen DiagramPenGroup::pen() const
{
	QPen pen = mPen;

	if (mCapabilities & ShowBrush) pen.setBrush(QBrush(colorWidget->currentColor()));
	if (mCapabilities & ShowWidth) pen.setWidthF(widthEdit->text().toDouble());
	if (mCapabilities & ShowStyle) pen.setStyle((Qt::PenStyle)styleCombo->currentIndex());

	return pen;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramPositionLayout::DiagramPositionLayout(const QSizeF& diagramSize, QWidget* parent) : QHBoxLayout(parent)
{
	xEdit = new QLineEdit();
	yEdit = new QLineEdit();
    xEdit->setValidator(new QDoubleValidator((double)0, (double)((diagramSize.width() > 0) ? diagramSize.width() : INT_MAX), 10, this));
    yEdit->setValidator(new QDoubleValidator((double)0, (double)((diagramSize.height() > 0) ? diagramSize.height() : INT_MAX), 10, this));

	addWidget(xEdit);
	addWidget(yEdit);
}
//--------------------------------------------------------------------------------------------------
DiagramPositionLayout::~DiagramPositionLayout() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPositionLayout::setPosition(const QPointF& position)
{
	setPositionX(position.x());
	setPositionY(position.y());
}
//--------------------------------------------------------------------------------------------------
void DiagramPositionLayout::setPosition(qreal x, qreal y)
{
	setPositionX(x);
	setPositionY(y);
}
//--------------------------------------------------------------------------------------------------
void DiagramPositionLayout::setPositionX(qreal x)
{
	xEdit->setText(QString::number(x));
}
//--------------------------------------------------------------------------------------------------
void DiagramPositionLayout::setPositionY(qreal y)
{
	yEdit->setText(QString::number(y));
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramPositionLayout::position() const
{
	return QPointF(positionX(), positionY());
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPositionLayout::positionX() const
{
	return xEdit->text().toDouble();
}
//--------------------------------------------------------------------------------------------------
qreal DiagramPositionLayout::positionY() const
{
	return yEdit->text().toDouble();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramPositionLayout::setVisible(bool visible)
{
	xEdit->setVisible(visible);
	yEdit->setVisible(visible);
}
//--------------------------------------------------------------------------------------------------
bool DiagramPositionLayout::isVisible() const
{
	return (xEdit->isVisible() || yEdit->isVisible());
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramTwoButtonDialog::DiagramTwoButtonDialog(QWidget* parent) : QDialog(parent)
{
    mLayout = new QVBoxLayout();
    setLayout(mLayout);
}
//--------------------------------------------------------------------------------------------------
DiagramTwoButtonDialog::~DiagramTwoButtonDialog() { }
//--------------------------------------------------------------------------------------------------
void DiagramTwoButtonDialog::addWidget(QWidget* widget, int stretch, Qt::Alignment alignment)
{
    mLayout->addWidget(widget, stretch, alignment);
}
//--------------------------------------------------------------------------------------------------
void DiagramTwoButtonDialog::addButtonBox()
{
    QDialogButtonBox* buttonBox = new QDialogButtonBox(Qt::Horizontal);
    QPushButton* okButton = buttonBox->addButton("OK", QDialogButtonBox::AcceptRole);
    QPushButton* cancelButton = buttonBox->addButton("Cancel", QDialogButtonBox::RejectRole);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    okButton->setMinimumSize(72, 28);
    cancelButton->setMinimumSize(72, 28);

    buttonBox->setCenterButtons(true);
    mLayout->addWidget(buttonBox);
}
//--------------------------------------------------------------------------------------------------
