/* DiagramItemDefaultsWidget.cpp
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

#include <DiagramItemDefaultsWidget.h>
#include <Diagram.h>
#include <DiagramWidgets.h>

DiagramItemDefaultsWidget::DiagramItemDefaultsWidget() : QFrame()
{
	QVBoxLayout* buttonLayout = new QVBoxLayout();
	QHBoxLayout* hLayout = new QHBoxLayout();
	QVBoxLayout* vLayout = new QVBoxLayout();

	mPreviewWidget = new DiagramItemDefaultsPreviewWidget();

	mPenButton = new QPushButton(QIcon(":/icons/oxygen/pen.png"), "");
	mPenButton->setIconSize(QSize(16, 16));
	mPenButton->setToolTip("Pen");
	mPenButton->setFixedSize(28, 26);
	connect(mPenButton, SIGNAL(clicked()), this, SLOT(runPenDialog()));

	mBrushButton = new QPushButton(QIcon(":/icons/oxygen/paintbrush.png"), "");
	mBrushButton->setIconSize(QSize(16, 16));
	mBrushButton->setToolTip("Brush");
	mBrushButton->setFixedSize(28, 26);
	connect(mBrushButton, SIGNAL(clicked()), this, SLOT(runBrushDialog()));

	mTextButton = new QPushButton(QIcon(":/icons/oxygen/text.png"), "");
	mTextButton->setIconSize(QSize(16, 16));
	mTextButton->setToolTip("Text");
	mTextButton->setFixedSize(28, 26);
	connect(mTextButton, SIGNAL(clicked()), this, SLOT(runTextDialog()));

	buttonLayout->addWidget(mPenButton);
	buttonLayout->addWidget(mBrushButton);
	buttonLayout->addWidget(mTextButton);
	buttonLayout->setSpacing(0);
	buttonLayout->setContentsMargins(0, 0, 0, 0);

    hLayout->addWidget(mPreviewWidget);//, 100);
	hLayout->addLayout(buttonLayout);
	hLayout->setSpacing(2);
	hLayout->setContentsMargins(2, 2, 2, 2);
	vLayout->addLayout(hLayout);
    vLayout->addWidget(new QWidget());//, 100);
	vLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(vLayout);

	Diagram::itemFactory.setDefaultProperties(properties());
	connect(this, SIGNAL(itemPropertiesChanged(const QHash<QString,QVariant>&)),
		&Diagram::itemFactory, SLOT(setDefaultProperties(const QHash<QString,QVariant>&)));
}
//--------------------------------------------------------------------------------------------------
DiagramItemDefaultsWidget::~DiagramItemDefaultsWidget() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::setPen(const QPen& pen)
{
	return mPreviewWidget->setPen(pen);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::setStartArrow(const DiagramArrow& arrow)
{
	return mPreviewWidget->setStartArrow(arrow);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::setEndArrow(const DiagramArrow& arrow)
{
	return mPreviewWidget->setEndArrow(arrow);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::setBrush(const QBrush& brush)
{
	return mPreviewWidget->setBrush(brush);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::setFont(const QFont& font)
{
	return mPreviewWidget->setFont(font);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::setTextBrush(const QBrush& brush)
{
	return mPreviewWidget->setTextBrush(brush);
}
//--------------------------------------------------------------------------------------------------
QPen DiagramItemDefaultsWidget::pen() const
{
	return mPreviewWidget->pen();
}
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramItemDefaultsWidget::startArrow() const
{
	return mPreviewWidget->startArrow();
}
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramItemDefaultsWidget::endArrow() const
{
	return mPreviewWidget->endArrow();
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramItemDefaultsWidget::brush() const
{
	return mPreviewWidget->brush();
}
//--------------------------------------------------------------------------------------------------
QFont DiagramItemDefaultsWidget::font() const
{
	return mPreviewWidget->font();
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramItemDefaultsWidget::textBrush() const
{
	return mPreviewWidget->textBrush();
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramItemDefaultsWidget::properties() const
{
	QHash<QString, QVariant> properties;
	QVariant startArrow, endArrow;

	startArrow.setValue(mPreviewWidget->startArrow());
	endArrow.setValue(mPreviewWidget->endArrow());

	properties["Pen"] = mPreviewWidget->pen();
	properties["Brush"] = mPreviewWidget->brush();
	properties["Font"] = mPreviewWidget->font();
	properties["Text Brush"] = mPreviewWidget->textBrush();
	properties["Start Arrow"] = startArrow;
	properties["End Arrow"] = endArrow;

	return properties;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::runPenDialog()
{
	DiagramTwoButtonDialog dialog(this);
	DiagramPenGroup* penGroup = new DiagramPenGroup();
	DiagramItemPointGroup* startArrowGroup = new DiagramItemPointGroup("Start Arrow", DiagramItemPointGroup::ShowArrow);
	DiagramItemPointGroup* endArrowGroup = new DiagramItemPointGroup("End Arrow", DiagramItemPointGroup::ShowArrow);

    dialog.addWidget(penGroup);
    dialog.addWidget(startArrowGroup);
    dialog.addWidget(endArrowGroup);
    dialog.addWidget(new QWidget(), 100);
    dialog.addButtonBox();
	dialog.setWindowTitle("Default Item Pen Settings");
    dialog.resize(280, 10);

	penGroup->setPen(mPreviewWidget->pen());
	startArrowGroup->setArrow(mPreviewWidget->startArrow());
	endArrowGroup->setArrow(mPreviewWidget->endArrow());

	if (dialog.exec() == QDialog::Accepted)
	{
		mPreviewWidget->setPen(penGroup->pen());
		mPreviewWidget->setStartArrow(startArrowGroup->arrow());
		mPreviewWidget->setEndArrow(endArrowGroup->arrow());

		mPreviewWidget->update();
		emit itemPropertiesChanged(properties());
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::runBrushDialog()
{
	DiagramTwoButtonDialog dialog(this);
	DiagramBrushGroup* brushGroup = new DiagramBrushGroup();

    dialog.addWidget(brushGroup);
    dialog.addWidget(new QWidget(), 100);
    dialog.addButtonBox();
    dialog.setWindowTitle("Default Item Brush Settings");
    dialog.resize(280, 10);

	brushGroup->setBrush(mPreviewWidget->brush());

	if (dialog.exec() == QDialog::Accepted)
	{
		mPreviewWidget->setBrush(brushGroup->brush());

		mPreviewWidget->update();
		emit itemPropertiesChanged(properties());
	}
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsWidget::runTextDialog()
{
	DiagramTwoButtonDialog dialog(this);
	DiagramFontToolBar* fontToolBar = new DiagramFontToolBar(DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush);

	QGroupBox* fontGroup = new QGroupBox("Font");
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(fontToolBar);
	fontGroup->setLayout(vLayout);

    dialog.addWidget(fontGroup);
    dialog.addWidget(new QWidget(), 100);
    dialog.addButtonBox();
	dialog.setWindowTitle("Default Item Text Settings");
    dialog.resize(10, 10);

	fontToolBar->setFont(mPreviewWidget->font());
	fontToolBar->setBrush(mPreviewWidget->textBrush());

	if (dialog.exec() == QDialog::Accepted)
	{
		mPreviewWidget->setFont(fontToolBar->font());
		mPreviewWidget->setTextBrush(fontToolBar->brush());

		mPreviewWidget->update();
		emit itemPropertiesChanged(properties());
	}
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramItemDefaultsPreviewWidget::DiagramItemDefaultsPreviewWidget() : QFrame()
{
	mPen = QPen(QBrush(Qt::black), 12, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	mBrush = QBrush(Qt::white);
    mFont = QFont("Arial", 150);
    mTextBrush = mPen.brush();

	QImage image(8, 8, QImage::Format_RGB32);
    image.fill(0xFFFFFFFF);
	for(int i = 0; i < 64; i++)
	{
		if ((i % 8 >= 4 && i / 8 >= 4) || (i % 8 <= 3 && i / 8 <= 3))
			image.setPixel(i % 8, i / 8, 0xFFC0C0C0);
	}
	mTransparentBrush.setTextureImage(image);

	setFrameStyle(Sunken | NoFrame);
	setMinimumSize(84, 52);
}
//--------------------------------------------------------------------------------------------------
DiagramItemDefaultsPreviewWidget::~DiagramItemDefaultsPreviewWidget() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::setPen(const QPen& pen)
{
	mPen = pen;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::setStartArrow(const DiagramArrow& arrow)
{
	mStartArrow = arrow;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::setEndArrow(const DiagramArrow& arrow)
{
	mEndArrow = arrow;
}
//--------------------------------------------------------------------------------------------------
QPen DiagramItemDefaultsPreviewWidget::pen() const
{
	return mPen;
}
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramItemDefaultsPreviewWidget::startArrow() const
{
	return mStartArrow;
}
//--------------------------------------------------------------------------------------------------
DiagramArrow DiagramItemDefaultsPreviewWidget::endArrow() const
{
	return mEndArrow;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::setBrush(const QBrush& brush)
{
	mBrush = brush;
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramItemDefaultsPreviewWidget::brush() const
{
	return mBrush;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::setFont(const QFont& font)
{
	mFont = font;
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::setTextBrush(const QBrush& brush)
{
	mTextBrush = brush;
}
//--------------------------------------------------------------------------------------------------
QFont DiagramItemDefaultsPreviewWidget::font() const
{
	return mFont;
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramItemDefaultsPreviewWidget::textBrush() const
{
	return mTextBrush;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen lPen = mPen;
	QBrush lBrush = mBrush;
	QFont lFont = mFont;
	QBrush lTextBrush = mTextBrush;

	if (!isEnabled())
	{
		lPen.setBrush(palette().brush(QPalette::WindowText));
		lBrush = palette().brush(QPalette::Window);
		lTextBrush = palette().brush(QPalette::WindowText);
	}
	else
	{
		if (lPen.color().alpha() == 0) lPen.setBrush(lBrush);
		if (lBrush.color().alpha() == 0) lBrush.setColor(QColor(255, 255, 255, 255));
		if (lTextBrush.color().alpha() == 0) lTextBrush = lBrush;
	}
	lPen.setWidth(2);
	lFont.setPointSize(QWidget::font().pointSize());

	painter.setPen(lPen);
	painter.setBrush(lBrush);
	painter.setFont(lFont);
	painter.setRenderHints(QPainter::TextAntialiasing);

	drawLine(painter);
	drawTextBox(painter, lTextBrush);

	Q_UNUSED(event);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::drawLine(QPainter& painter)
{
	QPoint startPoint(7, 12), endPoint(width() - 8, 12);
    DiagramArrow startArrow(mStartArrow.style(), 12);
    DiagramArrow endArrow(mEndArrow.style(), 12);

	painter.drawLine(startPoint, endPoint);
	painter.setRenderHints(QPainter::Antialiasing, true);
	startArrow.render(&painter, startPoint, 0);
	endArrow.render(&painter, endPoint, 180);
	painter.setRenderHints(QPainter::Antialiasing, false);
}
//--------------------------------------------------------------------------------------------------
void DiagramItemDefaultsPreviewWidget::drawTextBox(QPainter& painter, const QBrush& textBrush)
{
	QRect textBoxRect = rect().adjusted(1, 25, -2, -2);
	QRect textRect = textBoxRect.adjusted(4, 4, -4, -4);

	painter.drawRect(textBoxRect);

	painter.setPen(QPen(textBrush, 1));
	painter.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, painter.font().family());
}
//--------------------------------------------------------------------------------------------------
