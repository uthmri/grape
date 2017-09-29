/* ChartEllipseItem.cpp
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

#include <ChartEllipseItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

ChartEllipseItem::ChartEllipseItem() : DiagramEllipseItem()
{
	QFont font("Arial");
	font.setPointSizeF(100.0);

	addProperty("Caption", QVariant(QString("Label")));
	addProperty("Font", QVariant(font));
	addProperty("Text Brush", QBrush(QColor(255, 0, 255)));

	setFlags(CanMove | CanRotate | CanFlip | CanResize | MatchUnitsWithParent);
	setPlaceType(PlaceMouseUp);

	point(0)->setPosition(-400.0f, -200.0f);
	point(1)->setPosition(400.0f, 200.0f);
	point(2)->setPosition(400.0f, -200.0f);
	point(3)->setPosition(-400.0f, 200.0f);
	point(4)->setPosition(0.0f, -200.0f);
	point(5)->setPosition(400.0f, 0.0f);
	point(6)->setPosition(0.0f, 200.0f);
	point(7)->setPosition(-400.0f, 0.0f);

	point(8)->setPosition(-282.84, -141.42);
	point(9)->setPosition(282.84, -141.42);
	point(10)->setPosition(282.84, 141.42);
	point(11)->setPosition(-282.84, 141.42);
}
//--------------------------------------------------------------------------------------------------
ChartEllipseItem::ChartEllipseItem(const ChartEllipseItem& item) : DiagramEllipseItem(item) { }
//--------------------------------------------------------------------------------------------------
ChartEllipseItem::~ChartEllipseItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ChartEllipseItem::copy() const
{
	return new ChartEllipseItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString ChartEllipseItem::uniqueKey() const
{
	return "ChartEllipse";
}
//--------------------------------------------------------------------------------------------------
QString ChartEllipseItem::iconPath() const
{
	return ":/icons/custom/items/chartellipse.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::setCaption(const QString& caption)
{
	setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ChartEllipseItem::caption() const
{
	return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ChartEllipseItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ChartEllipseItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF ChartEllipseItem::boundingRect() const
{
	return DiagramEllipseItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ChartEllipseItem::shape() const
{
	QPainterPath shape = DiagramEllipseItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool ChartEllipseItem::isSuperfluous() const
{
	return (DiagramEllipseItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::render(DiagramPainter* painter)
{
	QFont lFont = font();
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);
	qreal deviceFactor = 1.0;

	DiagramEllipseItem::render(painter);

	if (painter->paintEngine()->paintDevice())
		deviceFactor = 96.0 / painter->paintEngine()->paintDevice()->logicalDpiX();

	lFont.setPointSizeF(lFont.pointSizeF() * 0.72 / scaleFactor);       // Scale to workspace
	lFont.setPointSizeF(lFont.pointSizeF() * deviceFactor);             // Scale to device

	if (!mBoundingRect.isValid()) updateLabel(lFont, painter->device());

	painter->setFont(lFont);
	painter->setForItem(QPen(textBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), textBrush());
	painter->translate(DiagramEllipseItem::boundingRect().center());
	painter->rotate(orientedTextAngle());
	painter->scale(scaleFactor, scaleFactor);
	painter->drawText(mTextRect, Qt::AlignCenter, caption());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramEllipseItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("caption", caption());
	xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramEllipseItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("caption"))
		setCaption(attributes.value("caption").toString());
	if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ChartEllipseItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
	return new ChartEllipseItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::rotateEvent(const QPointF& diagramPos)
{
	DiagramEllipseItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::rotateBackEvent(const QPointF& diagramPos)
{
	DiagramEllipseItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::flipEvent(const QPointF& diagramPos)
{
	DiagramEllipseItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
	DiagramEllipseItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant ChartEllipseItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	QVariant result = DiagramEllipseItem::aboutToChangeEvent(reason, value);

	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);
		QFont lFont = font();

		lFont.setPointSizeF(lFont.pointSizeF() * scaleFactor);
		setFont(lFont);
	}

	return result;
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItem::updateLabel(const QFont& font, QPaintDevice* device)
{
	QSizeF textSizeF = textSize(caption(), font, device);
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);

	// Update mTextRect
	mTextRect = QRectF(QPointF(-textSizeF.width() / 2, -textSizeF.height() / 2), textSizeF);

	// Update mBoundingRect
	mBoundingRect = rotateRect(QRectF(
		QPointF(mTextRect.left() * scaleFactor, mTextRect.top() * scaleFactor),
		QPointF(mTextRect.right() * scaleFactor, mTextRect.bottom() * scaleFactor)),
		rotationAngle()).translated(DiagramEllipseItem::boundingRect().center());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
qreal ChartEllipseItem::orientedTextAngle() const
{
	qreal textAngle = rotationAngle();

	if (isFlipped())
	{
		if (textAngle == 90) textAngle = 270;
		else if (textAngle == 270) textAngle = 90;
	}

	return textAngle;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
ChartEllipseItemDialog::ChartEllipseItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
	captionEdit = new QTextEdit();

	topLeftLayout = new DiagramPositionLayout(diagramSize);
	bottomRightLayout = new DiagramPositionLayout(diagramSize);
	penGroup = new DiagramPenGroup(DiagramPenGroup::ShowAll, diagramSize);
	brushGroup = new DiagramBrushGroup(DiagramBrushGroup::ShowColor);

	QGroupBox* positionGroup = new QGroupBox("Position");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Top Left: ", topLeftLayout);
	fLayout->addRow("Bottom Right: ", bottomRightLayout);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	positionGroup->setLayout(fLayout);

	QGroupBox* textGroup = new QGroupBox("Text");
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(fontToolBar);
	vLayout->addWidget(captionEdit, 100);
	textGroup->setLayout(vLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

	setWindowTitle("Chart Box Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ChartEllipseItemDialog::~ChartEllipseItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ChartEllipseItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
	captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void ChartEllipseItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ChartEllipseItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ChartEllipseItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartEllipseItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
