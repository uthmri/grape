/* ImageStatisticsItem.cpp
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

#include <ImageStatisticsItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
ImageStatisticsItem::ImageStatisticsItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("SourceImage", QVariant(QString("")));
    addProperty("MaskImage", QVariant(QString("")));
    addProperty("Percentile", QVariant(QString("")));
    addProperty("ImageStatistics", QVariant(QString("")));
    addProperty("SetRound", QVariant(QString("0")));
}
//--------------------------------------------------------------------------------------------------
ImageStatisticsItem::ImageStatisticsItem(const ImageStatisticsItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ImageStatisticsItem::~ImageStatisticsItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ImageStatisticsItem::copy() const
{
    return new ImageStatisticsItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::uniqueKey() const
{
    return "ImageStatistics";
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::iconPath() const
{
    return "";//":/icons/oxygen/IS.png";
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setArg1(const QString& path)
{
    setPropertyValue("SetRound", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::arg1() const
{
    return propertyValue("SetRound").toString();
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setStatistics(const QString& ImageStatistics)
{
    setPropertyValue("ImageStatistics", QVariant(ImageStatistics));
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setSourceImage(const QString& sourceImage)
{
    setPropertyValue("SourceImage", QVariant(sourceImage));
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setMaskImage(const QString& maskImage)
{
    setPropertyValue("MaskImage", QVariant(maskImage));
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setPercentile(const QString& percentile)
{
    setPropertyValue("Percentile", QVariant(percentile));
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::statistics() const
{
    return propertyValue("ImageStatistics").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::sourceImage() const
{
    return propertyValue("SourceImage").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::percentile() const
{
    return propertyValue("Percentile").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::maskImage() const
{
    return propertyValue("MaskImage").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageStatisticsItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ImageStatisticsItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ImageStatisticsItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ImageStatisticsItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ImageStatisticsItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool ImageStatisticsItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::render(DiagramPainter* painter)
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
void ImageStatisticsItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("SourceImage", sourceImage());
    xmlWriter.writeAttribute("MaskImage", maskImage());
    xmlWriter.writeAttribute("Percentile", percentile());
    xmlWriter.writeAttribute("ImageStatistics", statistics());
    xmlWriter.writeAttribute("SetRound", arg1());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("SourceImage"))
        setSourceImage(attributes.value("SourceImage").toString());
    if (attributes.hasAttribute("MaskImage"))
        setMaskImage(attributes.value("MaskImage").toString());
    if (attributes.hasAttribute("Percentile"))
        setPercentile(attributes.value("Percentile").toString());
    if (attributes.hasAttribute("ImageStatistics"))
        setStatistics(attributes.value("ImageStatistics").toString());
    if (attributes.hasAttribute("SetRound"))
        setArg1(attributes.value("SetRound").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ImageStatisticsItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new ImageStatisticsItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------

void ImageStatisticsItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ImageStatisticsItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ImageStatisticsItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ImageStatisticsItem::orientedTextAngle() const
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
ImageStatisticsItemDialog::ImageStatisticsItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    sourceImageEdit = new QLineEdit();
    maskImageEdit = new QLineEdit();
    percentileSpinBox = new QDoubleSpinBox();
    percentileSpinBox->setMinimum(percentileSpinBox->maximum()*-1.0);
    setRoundCheck = new QCheckBox();
    setRoundCheck->setText("Round to integer");
    QFormLayout* pLayout = new QFormLayout();
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
    StatisticsCombo = new QComboBox();
    StatisticsCombo->addItem("Min");
    StatisticsCombo->addItem("Max");
    StatisticsCombo->addItem("Mean");
    StatisticsCombo->addItem("Median");
    StatisticsCombo->addItem("stdDev");
    StatisticsCombo->addItem("Variance");
    StatisticsCombo->addItem("Percentile");
    QGroupBox* ImageStatisticsGroup = new QGroupBox("Parameter");
    pLayout->addRow("Image Statistics: ", StatisticsCombo);
    pLayout->addRow("Percentile Value: ", percentileSpinBox);
    pLayout->addRow(setRoundCheck);

    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    ImageStatisticsGroup->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(ImageStatisticsGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();
    setWindowTitle("ImageStatistics  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ImageStatisticsItemDialog::~ImageStatisticsItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
    if (properties.contains("SourceImage")) sourceImageEdit->setText(properties["SourceImage"].toString());
    if (properties.contains("MaskImage")) maskImageEdit->setText(properties["MaskImage"].toString());
    if (properties.contains("Percentile")) percentileSpinBox->setValue(properties["Percentile"].toDouble());
    if (properties.contains("SetRound")) setRoundCheck->setChecked(properties["SetRound"].toBool());

    if (properties.contains("ImageStatistics"))// StatisticsCombo->setCurrentText(properties["ImageStatistics"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("Min",0);
        combo_text_hash.insert("Max",1);
        combo_text_hash.insert("Mean",2);
        combo_text_hash.insert("Median",3);
        combo_text_hash.insert("stdDev",4);
        combo_text_hash.insert("Variance",5);
        combo_text_hash.insert("Percentile",6);
        QString combo_text = properties["ImageStatistics"].toString();
        StatisticsCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    captionEdit->selectAll();
    sourceImageEdit->selectAll();
    maskImageEdit->selectAll();

}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ImageStatisticsItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["SourceImage"] = QVariant(sourceImageEdit->text());
    properties["MaskImage"] = QVariant(maskImageEdit->text());
    properties["Percentile"] = QVariant(percentileSpinBox->value());
    properties["ImageStatistics"] = QVariant(StatisticsCombo->currentText());
    properties["SetRound"] = QVariant(setRoundCheck->isChecked());
    return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ImageStatisticsItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;
	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());
	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ImageStatisticsItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
