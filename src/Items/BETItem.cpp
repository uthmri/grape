/* BETItem.cpp
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
/* Copyright (C) 2016 UTHealth MRI Research
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
* along with GRAPE.  If not, see <http://www.gnu.org/licenses/>*/

#include <BETItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

BETItem::BETItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("CommandPath", QVariant(QString("C:/Users/gtefera/Desktop/mricron/bet.exe")));
    addProperty("OutputFile", QVariant(QString("bet_")));    
    addProperty("BinaryBrainMask", QVariant(QString("1")));
    addProperty("GenerateBrainSurfaceOutline", QVariant(QString("0")));
    addProperty("ApproximateSkullImage", QVariant(QString("0")));
    addProperty("SegmentedBrainImage", QVariant(QString("0")));
    addProperty("FractionalIntensityThreshold", QVariant(QString("0.5")));
    addProperty("VerticaGradientInFractionalIintensityThreshold", QVariant(QString("0.0")));
    addProperty("ApplyThresholdingToSegmentedBrainImageAndMask", QVariant(QString("0")));
}
//--------------------------------------------------------------------------------------------------
BETItem::BETItem(const BETItem& item) : NodeItem(item)
{ }
//--------------------------------------------------------------------------------------------------
BETItem::~BETItem()
{ }
//--------------------------------------------------------------------------------------------------
DiagramItem* BETItem::copy() const
{
    return new BETItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString BETItem::uniqueKey() const
{
    return "BET";
}
//--------------------------------------------------------------------------------------------------
QString BETItem::iconPath() const
{
    return "";// ":/icons/oxygen/bet.png";
}
//--------------------------------------------------------------------------------------------------
void BETItem::setCommandPath(const QString& path)
{
    setPropertyValue("CommandPath", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setOutputFile(const QString& path)
{
    setPropertyValue("OutputFile", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setArg1(const QString& path)
{
    setPropertyValue("BinaryBrainMask", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setArg7(const QString& path)
{
    setPropertyValue("GenerateBrainSurfaceOutline", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setArg2(const QString& path)
{
    setPropertyValue("ApproximateSkullImage", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setArg3(const QString& path)
{
    setPropertyValue("SegmentedBrainImage", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setArg4(const QString& path)
{
    setPropertyValue("FractionalIntensityThreshold", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setArg5(const QString& path)
{
    setPropertyValue("VerticaGradientInFractionalIntensityThreshold", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setArg6(const QString& path)
{
    setPropertyValue("ApplyThresholdingToSegmentedBrainImageAndMask", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
//void BETItem::setArgPath(const QString& path)
//{
//    setPropertyValue("Arg", QVariant(path));
//}
//--------------------------------------------------------------------------------------------------
void BETItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void BETItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString BETItem::commandPath() const
{
    return propertyValue("CommandPath").toString();
}
//--------------------------------------------------------------------------------------------------
QString BETItem::outputFile() const
{
    return propertyValue("OutputFile").toString();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QString BETItem::arg1() const
{
    return propertyValue("BinaryBrainMask").toString();
}
//--------------------------------------------------------------------------------------------------
QString BETItem::arg7() const
{
    return propertyValue("GenerateBrainSurfaceOutline").toString();
}
//--------------------------------------------------------------------------------------------------
QString BETItem::arg2() const
{
    return propertyValue("ApproximateSkullImage").toString();
}
//--------------------------------------------------------------------------------------------------
QString BETItem::arg3() const
{
    return propertyValue("SegmentedBrainImage").toString();
}
//--------------------------------------------------------------------------------------------------
QString BETItem::arg4() const
{
    return propertyValue("FractionalIntensityThreshold").toString();
}
//----------------------------------------------------------------------------------------------
QString BETItem::arg5() const
{
    return propertyValue("VerticaGradientInFractionalIintensityThreshold").toString();
}
//--------------------------------------------------------------------------------------------------
QString BETItem::arg6() const
{
    return propertyValue("ApplyThresholdingToSegmentedBrainImageAndMask").toString();
}
//----------------------------------------------------------------------------------------------
QString BETItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString BETItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont BETItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush BETItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void BETItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF BETItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath BETItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool BETItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void BETItem::render(DiagramPainter* painter)
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
void BETItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("CommandPath", commandPath());
    xmlWriter.writeAttribute("OutputFile", outputFile());
    xmlWriter.writeAttribute("GenerateBrainSurfaceOutline", arg7());
    xmlWriter.writeAttribute("BinaryBrainMask", arg1());
    xmlWriter.writeAttribute("ApproximateSkullImage", arg2());
    xmlWriter.writeAttribute("SegmentedBrainImage", arg3());
    xmlWriter.writeAttribute("FractionalIntensityThreshold", arg4());
    xmlWriter.writeAttribute("VerticaGradientInFractionalIintensityThreshold", arg5());
    xmlWriter.writeAttribute("ApplyThresholdingToSegmentedBrainImageAndMask", arg6());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void BETItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("CommandPath"))
        setCommandPath(attributes.value("CommandPath").toString());
    if (attributes.hasAttribute("OutputFile"))
        setOutputFile(attributes.value("OutputFile").toString());
    if (attributes.hasAttribute("GenerateBrainSurfaceOutline"))
        setArg7(attributes.value("GenerateBrainSurfaceOutline").toString());
    if (attributes.hasAttribute("BinaryBrainMask"))
        setArg1(attributes.value("BinaryBrainMask").toString());
    if(attributes.hasAttribute("ApproximateSkullImage"))
        setArg2(attributes.value("ApproximateSkullImage").toString());
    if (attributes.hasAttribute("SegmentedBrainImage"))
        setArg3(attributes.value("SegmentedBrainImage").toString());
    if (attributes.hasAttribute("FractionalIntensityThreshold"))
        setArg4(attributes.value("FractionalIntensityThreshold").toString());
    if (attributes.hasAttribute("VerticaGradientInFractionalIintensityThreshold"))
        setArg5(attributes.value("VerticaGradientInFractionalIintensityThreshold").toString());
    if (attributes.hasAttribute("ApplyThresholdingToSegmentedBrainImageAndMask"))
        setArg6(attributes.value("ApplyThresholdingToSegmentedBrainImageAndMask").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* BETItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new BETItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void BETItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void BETItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void BETItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void BETItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant BETItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void BETItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void BETItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal BETItem::orientedTextAngle() const
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
BETItemDialog::BETItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    commandPathEdit = new QLineEdit();
    outputFileEdit = new QLineEdit();
    //arg1Edit = new QLineEdit();
    arg2Edit = new QLineEdit();

    //arg4Edit = new QLineEdit();
    fractionalIntensityThreshold = new QDoubleSpinBox();
    //arg5Edit = new QLineEdit();
    VGFractionalIntensityThreshold = new QDoubleSpinBox();

    arg6Edit = new QLineEdit();
    generateBrainSurfaceOutlineCheck = new QCheckBox();
    generateBrainSurfaceOutlineCheck->setText("GenerateBrainSurfaceOutline");
    binaryBrainMaskCheck = new QCheckBox();
    binaryBrainMaskCheck->setText("BinaryBrainMask");
    approximateSkullImageCheck = new QCheckBox();
    approximateSkullImageCheck->setText("ApproximateSkullImage");
    segmentedBrainImageCheck = new QCheckBox();
    segmentedBrainImageCheck->setText("SegmentedBrainImage");
    applyThresholdingToSegmentedBrainImageAndMaskCheck = new QCheckBox();
    applyThresholdingToSegmentedBrainImageAndMaskCheck->setText("ApplyThresholdingToSegmentedBrainImageAndMask");

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

    QGroupBox* betGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Command Path: ", commandPathEdit);
    pLayout->addRow("Output File Pefix: ", outputFileEdit);
    pLayout->addRow("FractionalIntensityThreshold: ", fractionalIntensityThreshold);
    pLayout->addRow("VGinFractionalIintensityThreshold: ",  VGFractionalIntensityThreshold);
    pLayout->addRow(generateBrainSurfaceOutlineCheck);
    pLayout->addRow(binaryBrainMaskCheck);
    pLayout->addRow(segmentedBrainImageCheck);
    pLayout->addRow(approximateSkullImageCheck);
    pLayout->addRow(applyThresholdingToSegmentedBrainImageAndMaskCheck);

    betGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(betGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("BET  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
BETItemDialog::~BETItemDialog() { }
//--------------------------------------------------------------------------------------------------
void BETItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
    if (properties.contains("GenerateBrainSurfaceOutline"))
        generateBrainSurfaceOutlineCheck->setChecked(properties["GenerateBrainSurfaceOutline"].toBool());
    if (properties.contains("BinaryBrainMask"))
        binaryBrainMaskCheck->setChecked(properties["BinaryBrainMask"].toBool());
    if (properties.contains("SegmentedBrainImage"))
        segmentedBrainImageCheck->setChecked(properties["SegmentedBrainImage"].toBool());
    if (properties.contains("ApproximateSkullImage"))
        segmentedBrainImageCheck->setChecked(properties["ApproximateSkullImage"].toBool());
    //if (properties.contains("Parameter")) betGroup->setPen(properties["Parameter"].value<QPen>());
    if (properties.contains("CommandPath")) commandPathEdit->setText(properties["CommandPath"].toString());
    if (properties.contains("OutputFile")) outputFileEdit->setText(properties["OutputFile"].toString());
   // if (properties.contains("Input2 Path")) input2PathEdit->setText(properties["Input2 Path"].toString());
   // if (properties.contains("BinaryBrainMask")) arg1Edit->setText(properties["BinaryBrainMask"].toString());
   // if (properties.contains("ApproximateSkullImage")) arg2Edit->setText(properties["ApproximateSkullImage"].toString());
    //if (properties.contains("SegmentedBrainImage")) arg3Edit->setText(properties["SegmentedBrainImage"].toString());
    if (properties.contains("FractionalIntensityThreshold")) fractionalIntensityThreshold->setValue(properties["FractionalIntensityThreshold"].toDouble());
    if (properties.contains("VerticaGradientInFractionalIintensityThreshold")) VGFractionalIntensityThreshold->setValue(properties["VerticaGradientInFractionalIintensityThreshold"].toDouble());
    //if (properties.contains("ApplyThresholdingToSegmentedBrainImageAndMask")) arg6Edit->setText(properties["ApplyThresholdingToSegmentedBrainImageAndMask"].toString());
    if (properties.contains("ApplyThresholdingToSegmentedBrainImageAndMask"))
        applyThresholdingToSegmentedBrainImageAndMaskCheck->setChecked(properties["ApplyThresholdingToSegmentedBrainImageAndMask"].toBool());
    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void BETItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> BETItemDialog::properties() const
{
	QHash<QString, QVariant> properties;
 //properties["Array"] = QVariant(arrayCheck->isChecked());
	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["CommandPath"] = QVariant(commandPathEdit->text());
    properties["OutputFile"] = QVariant(outputFileEdit->text());
    properties["GenerateBrainSurfaceOutline"] = QVariant(generateBrainSurfaceOutlineCheck->isChecked());
    properties["BinaryBrainMask"] = QVariant(binaryBrainMaskCheck->isChecked());
    properties["ApproximateSkullImage"] = QVariant(approximateSkullImageCheck->isChecked());
    properties["SegmentedBrainImage"] = QVariant(segmentedBrainImageCheck->isChecked());
    properties["FractionalIntensityThreshold"] = QVariant(fractionalIntensityThreshold->value());
    properties["VerticaGradientInFractionalIintensityThreshold"] = QVariant(VGFractionalIntensityThreshold->value());
    properties["ApplyThresholdingToSegmentedBrainImageAndMask"] = QVariant(applyThresholdingToSegmentedBrainImageAndMaskCheck->isChecked());
    return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> BETItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void BETItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void BETItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
