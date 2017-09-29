/* ImageSegmentationItem.cpp
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

#include <ImageSegmentationItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
ImageSegmentationItem::ImageSegmentationItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("ImageSegmentationType", QVariant(QString("../Atropos.exe")));
    addProperty("CommandPath", QVariant(QString("../Atropos.exe")));
    addProperty("Arg1", QVariant(QString("../Atropos.exe")));
    addProperty("Arg2", QVariant(QString("../Atropos.exe")));
    addProperty("Output1", QVariant(QString("../Atropos.exe")));
    addProperty("Output2", QVariant(QString("../Atropos.exe")));
}
//--------------------------------------------------------------------------------------------------
ImageSegmentationItem::ImageSegmentationItem(const ImageSegmentationItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ImageSegmentationItem::~ImageSegmentationItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ImageSegmentationItem::copy() const
{
    return new ImageSegmentationItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::uniqueKey() const
{
    return "ImageSegmentation";
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::iconPath() const
{
    return "";//":/icons/oxygen/seg.png";
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setImageSegmentationType(const QString& imagesegmentationType)
{
    setPropertyValue("ImageSegmentationType", QVariant(imagesegmentationType));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setArg1(const QString& arg1)
{
    setPropertyValue("Arg1", QVariant(arg1));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setCommandPath(const QString& commandPath)
{
    setPropertyValue("CommandPath", QVariant(commandPath));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setArg2(const QString& arg2)
{
    setPropertyValue("Arg2", QVariant(arg2));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setOutput1(const QString& output1)
{
    setPropertyValue("Output1", QVariant(output1));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setOutput2(const QString& output2)
{
    setPropertyValue("Output2", QVariant(output2));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::imagesegmentationType() const
{
    return propertyValue("ImageSegmentationType").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::commandPath() const
{
    return propertyValue("CommandPath").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::arg1() const
{
    return propertyValue("Arg1").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::arg2() const
{
    return propertyValue("Arg2").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::output1() const
{
    return propertyValue("Output1").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::output2() const
{
    return propertyValue("Output2").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageSegmentationItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString ImageSegmentationItem::caption1()
{
   return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ImageSegmentationItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ImageSegmentationItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ImageSegmentationItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ImageSegmentationItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);
	return shape;
}
//--------------------------------------------------------------------------------------------------
bool ImageSegmentationItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::render(DiagramPainter* painter)
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
void ImageSegmentationItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("ImageSegmentationType", imagesegmentationType());
    xmlWriter.writeAttribute("CommandPath", commandPath());
    xmlWriter.writeAttribute("Arg1", arg1());
    xmlWriter.writeAttribute("Arg2", arg2());
    xmlWriter.writeAttribute("Output1", output1());
    xmlWriter.writeAttribute("Output2", output2());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("ImageSegmentationType"))
        setImageSegmentationType(attributes.value("ImageSegmentationType").toString());
    if (attributes.hasAttribute("CommandPath"))
        setCommandPath(attributes.value("CommandPath").toString());
    if (attributes.hasAttribute("Arg1"))
        setArg1(attributes.value("Arg1").toString());
    if (attributes.hasAttribute("Arg2"))
        setArg2(attributes.value("Arg2").toString());
    if (attributes.hasAttribute("Output1"))
        setOutput1(attributes.value("Output1").toString());
    if (attributes.hasAttribute("Output2"))
        setOutput2(attributes.value("Output2").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ImageSegmentationItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new ImageSegmentationItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ImageSegmentationItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ImageSegmentationItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ImageSegmentationItem::orientedTextAngle() const
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
ImageSegmentationItemDialog::ImageSegmentationItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    commandPathEdit = new QLineEdit();
    arg1Edit = new QLineEdit();
    arg2Edit = new QLineEdit();
    output1Edit = new QLineEdit();
    output2Edit = new QLineEdit();

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

    imagesegmentationTypeCombo = new QComboBox();
    imagesegmentationTypeCombo->addItem("Atropos");
    QGroupBox* imagesegmentationGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("ImageSegmentationType: ", imagesegmentationTypeCombo);
    pLayout->addRow("CommandPath: ", commandPathEdit);
    pLayout->addRow("Arg1: ", arg1Edit);
    pLayout->addRow("Arg2: ", arg2Edit);
    pLayout->addRow("Output1 : ", output1Edit);
    pLayout->addRow("Output2 : ", output2Edit);

    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    imagesegmentationGroup->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(imagesegmentationGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("ImageSegmentation  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ImageSegmentationItemDialog::~ImageSegmentationItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("ImageSegmentationType")) //imagesegmentationTypeCombo->setCurrentText(properties["ImageSegmentationType"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("Atropos",0);
        QString combo_text = properties["ImageSegmentationType"].toString();

         imagesegmentationTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("CommandPath")) commandPathEdit->setText(properties["CommandPath"].toString());
    if (properties.contains("Arg1")) arg1Edit->setText(properties["Arg1"].toString());
    if (properties.contains("Arg2")) arg2Edit->setText(properties["Arg2"].toString());
    if (properties.contains("Output1")) output1Edit->setText(properties["Output1"].toString());
    if (properties.contains("Output2")) output2Edit->setText(properties["Output2"].toString());

    captionEdit->selectAll();
    commandPathEdit->selectAll();
    arg1Edit->selectAll();
    arg2Edit->selectAll();
    output1Edit->selectAll();
    output2Edit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ImageSegmentationItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());

    properties["ImageSegmentationType"] = QVariant(imagesegmentationTypeCombo->currentText());
    properties["CommandPath"] = QVariant(commandPathEdit->text());
    properties["Arg1"] = QVariant(arg1Edit->text());
    properties["Arg2"] = QVariant(arg2Edit->text());
    properties["Output1"] = QVariant(output1Edit->text());
    properties["Output2"] = QVariant(output2Edit->text());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ImageSegmentationItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ImageSegmentationItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
