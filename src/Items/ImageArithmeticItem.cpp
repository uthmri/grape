/* ImageArithmeticItem.cpp
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

#include <ImageArithmeticItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
ImageArithmeticItem::ImageArithmeticItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("ImageArithmeticType", QVariant(QString("")));
    addProperty("Arg1Port", QVariant(QString("")));
    addProperty("Arg1DataValue", QVariant(QString("")));
    addProperty("Arg2Port", QVariant(QString("")));
    addProperty("DataValue", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
ImageArithmeticItem::ImageArithmeticItem(const ImageArithmeticItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ImageArithmeticItem::~ImageArithmeticItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ImageArithmeticItem::copy() const
{
    return new ImageArithmeticItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ImageArithmeticItem::uniqueKey() const
{
    return "ImageArithmetic";
}
//--------------------------------------------------------------------------------------------------
QString ImageArithmeticItem::iconPath() const
{
    return "";//":/icons/oxygen/IA.png";
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setImageArithmeticType(const QString& ImageArithmeticType)
{
    setPropertyValue("ImageArithmeticType", QVariant(ImageArithmeticType));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setArg1Port(const QString& arg1Port)
{
    setPropertyValue("Arg1Port", QVariant(arg1Port));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setArg2Port(const QString& arg2Port)
{
    setPropertyValue("Arg2Port", QVariant(arg2Port));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setArg1DataValue(const QString& dataValue)
{
    setPropertyValue("Arg1DataValue", QVariant(dataValue));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setDataValue(const QString& dataValue)
{
    setPropertyValue("DataValue", QVariant(dataValue));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------

QString ImageArithmeticItem::ImageArithmeticType() const
{
    return propertyValue("ImageArithmeticType").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageArithmeticItem::arg1Port() const
{
    return propertyValue("Arg1Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageArithmeticItem::arg2Port() const
{
    return propertyValue("Arg2Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageArithmeticItem::DataValue() const
{
    return propertyValue("DataValue").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageArithmeticItem::caption() const
{
    return propertyValue("Caption").toString();
}
QString ImageArithmeticItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ImageArithmeticItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ImageArithmeticItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ImageArithmeticItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ImageArithmeticItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool ImageArithmeticItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::render(DiagramPainter* painter)
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
void ImageArithmeticItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("ImageArithmeticType", ImageArithmeticType());
    xmlWriter.writeAttribute("Arg1Port", arg1Port());
    xmlWriter.writeAttribute("Arg1DataValue", DataValue());
    xmlWriter.writeAttribute("Arg2Port", arg2Port());
    xmlWriter.writeAttribute("DataValue", DataValue());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("Arg1Port"))
        setArg1Port(attributes.value("Arg1Port").toString());
    if (attributes.hasAttribute("Arg1DataValue"))
        setDataValue(attributes.value("DataValue").toString());
    if (attributes.hasAttribute("ImageArithmeticType"))
        setImageArithmeticType(attributes.value("ImageArithmeticType").toString());
    if (attributes.hasAttribute("Arg2Port"))
        setArg2Port(attributes.value("Arg2Port").toString());
    if (attributes.hasAttribute("DataValue"))
        setDataValue(attributes.value("DataValue").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ImageArithmeticItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new ImageArithmeticItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ImageArithmeticItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ImageArithmeticItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ImageArithmeticItem::orientedTextAngle() const
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
ImageArithmeticItemDialog::ImageArithmeticItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();

    dataValueEdit = new QLineEdit();
    arg1dataValueEdit = new QLineEdit();

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

    ImageArithmeticTypeCombo = new QComboBox();
    ImageArithmeticTypeCombo->addItem("+");
    ImageArithmeticTypeCombo->addItem("-");
    ImageArithmeticTypeCombo->addItem("*");
    ImageArithmeticTypeCombo->addItem("/");
    ImageArithmeticTypeCombo->addItem("pow");
    ImageArithmeticTypeCombo->addItem("==");
    ImageArithmeticTypeCombo->addItem(">=");
    ImageArithmeticTypeCombo->addItem("<=");
    ImageArithmeticTypeCombo->addItem(">");
    ImageArithmeticTypeCombo->addItem("<");
    ImageArithmeticTypeCombo->addItem("&&");
    ImageArithmeticTypeCombo->addItem("||");
    ImageArithmeticTypeCombo->addItem("~");

    QGroupBox* ImageArithmeticGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("ImageArithmeticType: ", ImageArithmeticTypeCombo);

    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    ImageArithmeticGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(ImageArithmeticGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("ImageArithmetic  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ImageArithmeticItemDialog::~ImageArithmeticItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("Arg1DataValue")) dataValueEdit->setText(properties["Arg1DataValue"].toString());
    if (properties.contains("ImageArithmeticType")) //ImageArithmeticTypeCombo->setCurrentText(properties["ImageArithmeticType"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("+",0);
        combo_text_hash.insert("-",1);
        combo_text_hash.insert("*",2);
        combo_text_hash.insert("/",3);
        combo_text_hash.insert("pow",4);
        combo_text_hash.insert("==",5);
        combo_text_hash.insert(">=",6);
        combo_text_hash.insert("<=",7);
        QString combo_text = properties["ImageArithmeticType"].toString();
        ImageArithmeticTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("DataValue")) dataValueEdit->setText(properties["DataValue"].toString());

    captionEdit->selectAll();
    dataValueEdit->selectAll();

}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ImageArithmeticItemDialog::properties() const
{
	QHash<QString, QVariant> properties;
	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["Arg1DataValue"] = QVariant(dataValueEdit->text());
    properties["ImageArithmeticType"] = QVariant(ImageArithmeticTypeCombo->currentText());
    properties["DataValue"] = QVariant(dataValueEdit->text());
	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ImageArithmeticItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ImageArithmeticItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
