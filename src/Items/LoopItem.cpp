/* LoopItem.cpp
 *
 Copyright (C) 2016 UTHealth MRI Research
*
* This file is part of the GRAPE Diagram Editor.
*
* GRAPE is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GRAPE is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GRAPE.  If not, see <http://www.gnu.org/licenses/>
 */

#include <LoopItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

LoopItem::LoopItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("StartValue", QVariant(QString("")));
    addProperty("Step", QVariant(QString("")));
    addProperty("EndValue", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
LoopItem::LoopItem(const LoopItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
LoopItem::~LoopItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* LoopItem::copy() const
{
    return new LoopItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString LoopItem::uniqueKey() const
{
    return "Loop";
}
//--------------------------------------------------------------------------------------------------
QString LoopItem::iconPath() const
{
    return "";//":/icons/oxygen/loop.png";
}
//--------------------------------------------------------------------------------------------------
void LoopItem::setStartValue(const QString& path)
{
    setPropertyValue("StartValue", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void LoopItem::setStep(const QString& path)
{
    setPropertyValue("Step", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void LoopItem::setEndValue(const QString& path)
{
    setPropertyValue("EndValue", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void LoopItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void LoopItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void LoopItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString LoopItem::startValue() const
{
    return propertyValue("StartValue").toString();
}
//--------------------------------------------------------------------------------------------------
QString LoopItem::step() const
{
    return propertyValue("Step").toString();
}
//--------------------------------------------------------------------------------------------------
QString LoopItem::endValue() const
{
    return propertyValue("EndValue").toString();
}
//--------------------------------------------------------------------------------------------------
QString LoopItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString LoopItem::caption1()
{
   return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont LoopItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush LoopItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void LoopItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF LoopItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath LoopItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool LoopItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void LoopItem::render(DiagramPainter* painter)
{
    QFont lFont = font();
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);
	qreal deviceFactor = 1.0;

    NodeItem::render(painter);

	if (painter->paintEngine()->paintDevice())
		deviceFactor = 96.0 / painter->paintEngine()->paintDevice()->logicalDpiX();

	lFont.setPointSizeF(lFont.pointSizeF() * 0.72 / scaleFactor);       // Scale to workspace
	lFont.setPointSizeF(lFont.pointSizeF() * deviceFactor);             // Scale to device

	if (!mBoundingRect.isValid()) updateLabel(lFont, painter->device());

	painter->setFont(lFont);
	painter->setForItem(QPen(textBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), textBrush());
    painter->translate(DiagramRoundItem::boundingRect().center());
	painter->rotate(orientedTextAngle());
	painter->scale(scaleFactor, scaleFactor);
    painter->drawText(mTextRect, Qt::AlignCenter, caption());
}
//--------------------------------------------------------------------------------------------------
void LoopItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("StartValue", startValue());
    xmlWriter.writeAttribute("Step", step());
    xmlWriter.writeAttribute("EndValue", endValue());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void LoopItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("StartValue"))
        setStartValue(attributes.value("StartValue").toString());
    if (attributes.hasAttribute("Step"))
        setStep(attributes.value("Step").toString());
    if (attributes.hasAttribute("EndValue"))
        setEndValue(attributes.value("EndValue").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* LoopItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new LoopItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void LoopItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void LoopItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void LoopItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void LoopItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant LoopItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
    QVariant result = DiagramRoundItem::aboutToChangeEvent(reason, value);

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
void LoopItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void LoopItem::updateLabel(const QFont& font, QPaintDevice* device)
{
	QSizeF textSizeF = textSize(caption(), font, device);
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);

	// Update mTextRect
	mTextRect = QRectF(QPointF(-textSizeF.width() / 2, -textSizeF.height() / 2), textSizeF);

	// Update mBoundingRect
	mBoundingRect = rotateRect(QRectF(
		QPointF(mTextRect.left() * scaleFactor, mTextRect.top() * scaleFactor),
		QPointF(mTextRect.right() * scaleFactor, mTextRect.bottom() * scaleFactor)),
        rotationAngle()).translated(DiagramRoundItem::boundingRect().center());
}
//--------------------------------------------------------------------------------------------------
qreal LoopItem::orientedTextAngle() const
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
LoopItemDialog::LoopItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    startValueEdit = new QDoubleSpinBox();
    stepEdit = new QDoubleSpinBox();
    endValueEdit = new QDoubleSpinBox();
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
    QGroupBox* Loop1Group = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("StartValue: ", startValueEdit);
    pLayout->addRow("Step: ", stepEdit);
    pLayout->addRow("EndValue: ", endValueEdit);
    Loop1Group->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(Loop1Group);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();
    setWindowTitle("Loop  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
LoopItemDialog::~LoopItemDialog() { }
//--------------------------------------------------------------------------------------------------
void LoopItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    //if (properties.contains("Parameter")) Loop1Group->setPen(properties["Parameter"].value<QPen>());
    if (properties.contains("StartValue")) startValueEdit->setValue(properties["StartValue"].toDouble());
    if (properties.contains("Step")) stepEdit->setValue(properties["Step"].toDouble());
    if (properties.contains("EndValue")) endValueEdit->setValue(properties["EndValue"].toDouble());

    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void LoopItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> LoopItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["StartValue"] = QVariant(startValueEdit->text());
    properties["Step"] = QVariant(stepEdit->text());
    properties["EndValue"] = QVariant(endValueEdit->text());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> LoopItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());
	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void LoopItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void LoopItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
