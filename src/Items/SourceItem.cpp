/* SourceItem.cpp
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


#include <SourceItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
SourceItem::SourceItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("DataType", QVariant(QString("")));
    addProperty("DataValue", QVariant(QString("")));
    addProperty("Array", QVariant(QString("")));
    addProperty("Dimension1", QVariant(QString("")));
    addProperty("Dimension2", QVariant(QString("")));
    addProperty("Dimension3", QVariant(QString("")));
    addProperty("Dimension4", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
SourceItem::SourceItem(const SourceItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
SourceItem::~SourceItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* SourceItem::copy() const
{
    return new SourceItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::uniqueKey() const
{
    return "Source";
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::iconPath() const
{
    return "";//":/icons/oxygen/source.png";
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setDataType(const QString& dataType)
{
    setPropertyValue("DataType", QVariant(dataType));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setDataValue(const QString& dataValue)
{
    setPropertyValue("DataValue", QVariant(dataValue));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setArray(const QString& v)
{
    setPropertyValue("Array", QVariant(v));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setDimension1(const QString& dim1)
{
    setPropertyValue("Dimension1", QVariant(dim1));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setDimension2(const QString& dim2)
{
    setPropertyValue("Dimension2", QVariant(dim2));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setDimension3(const QString& dim3)
{
    setPropertyValue("Dimension3", QVariant(dim3));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setDimension4(const QString& dim4)
{
    setPropertyValue("Dimension4", QVariant(dim4));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::dataType() const
{
    return propertyValue("DataType").toString();
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::dataValue() const
{
    return propertyValue("DataValue").toString();
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::Array() const
{
    return propertyValue("Array").toString();
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::dim1() const
{
    return propertyValue("Dimension1").toString();
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::dim2() const
{
    return propertyValue("Dimension2").toString();
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::dim3() const
{
    return propertyValue("Dimension3").toString();
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::dim4() const
{
    return propertyValue("Dimension4").toString();
}
//--------------------------------------------------------------------------------------------------
QString SourceItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString SourceItem::caption1()
{
   return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont SourceItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush SourceItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void SourceItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF SourceItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath SourceItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
        shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool SourceItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void SourceItem::render(DiagramPainter* painter)
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
void SourceItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("DataType", dataType());
    xmlWriter.writeAttribute("DataValue", dataValue());
    xmlWriter.writeAttribute("Array", Array());
    xmlWriter.writeAttribute("Dimension1", dim1());
    xmlWriter.writeAttribute("Dimension2", dim2());
    xmlWriter.writeAttribute("Dimension3", dim3());
    xmlWriter.writeAttribute("Dimension4", dim4());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void SourceItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("DataType"))
        setDataType(attributes.value("DataType").toString());
    if (attributes.hasAttribute("DataValue"))
        setDataValue(attributes.value("DataValue").toString());
    if (attributes.hasAttribute("Array"))
        setArray(attributes.value("Array").toString());
    if (attributes.hasAttribute("Dimension1"))
        setDimension1(attributes.value("Dimension1").toString());
    if (attributes.hasAttribute("Dimension2"))
        setDimension2(attributes.value("Dimension2").toString());
    if (attributes.hasAttribute("Dimension3"))
        setDimension3(attributes.value("Dimension3").toString());
    if (attributes.hasAttribute("Dimension4"))
        setDimension4(attributes.value("Dimension4").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* SourceItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new SourceItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void SourceItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SourceItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SourceItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SourceItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant SourceItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void SourceItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void SourceItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal SourceItem::orientedTextAngle() const
{
	qreal textAngle = rotationAngle();

	if (isFlipped())
	{
		if (textAngle == 90) textAngle = 270;
		else if (textAngle == 270) textAngle = 90;
	}
	return textAngle;
}

//==================================================================================================

SourceItemDialog::SourceItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    dataValueEdit = new QLineEdit();
    dim1Edit = new QSpinBox();
    dim2Edit = new QSpinBox();
    dim3Edit = new QSpinBox();
    dim4Edit = new QSpinBox();
	topLeftLayout = new DiagramPositionLayout(diagramSize);
	bottomRightLayout = new DiagramPositionLayout(diagramSize);
	penGroup = new DiagramPenGroup(DiagramPenGroup::ShowAll, diagramSize);
	brushGroup = new DiagramBrushGroup(DiagramBrushGroup::ShowColor);
    arrayCheck = new QCheckBox();
    arrayCheck->setText("Array");

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

    dataTypeCombo = new QComboBox();
    dataTypeCombo->addItem("string");
    dataTypeCombo->addItem("byte");
    dataTypeCombo->addItem("int");
    dataTypeCombo->addItem("float");
    dataTypeCombo->addItem("double");
    dataTypeCombo->addItem("boolean");
    dataTypeCombo->addItem("File");
    dataTypeCombo->addItem("rand");
    dataTypeCombo->addItem("randn");

    QLabel* dimLabel = new QLabel("Array dimensions:");
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(dimLabel);
    hLayout->addWidget(dim1Edit);
    hLayout->addWidget(dim2Edit);
    hLayout->addWidget(dim3Edit);
    hLayout->addWidget(dim4Edit);
    QGroupBox* sourceGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Data Type: ", dataTypeCombo);
    pLayout->addRow("Data Value: ", dataValueEdit);
    pLayout->addRow(arrayCheck);
    pLayout->addRow(hLayout);
    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    sourceGroup->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(sourceGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();
    setWindowTitle("Source  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
SourceItemDialog::~SourceItemDialog() { }
//--------------------------------------------------------------------------------------------------
void SourceItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("DataType")) //dataTypeCombo->setCurrentText(properties["DataType"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("string",0);
        combo_text_hash.insert("byte",1);
        combo_text_hash.insert("int",2);
        combo_text_hash.insert("float",3);
        combo_text_hash.insert("double",4);
        combo_text_hash.insert("boolean",5);
        combo_text_hash.insert("File",6);
        combo_text_hash.insert("rand",7);
        combo_text_hash.insert("randn",8);
        QString combo_text = properties["DataType"].toString();
        dataTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("DataValue")) dataValueEdit->setText(properties["DataValue"].toString());
    if (properties.contains("Array"))
        arrayCheck->setChecked(properties["Array"].toBool());
    if (properties.contains("Dimension1")) dim1Edit->setValue(properties["Dimension1"].toInt());
    if (properties.contains("Dimension2")) dim2Edit->setValue(properties["Dimension2"].toInt());
    if (properties.contains("Dimension3")) dim3Edit->setValue(properties["Dimension3"].toInt());
    if (properties.contains("Dimension4")) dim4Edit->setValue(properties["Dimension4"].toInt());
    captionEdit->selectAll();
    dataValueEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void SourceItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> SourceItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["DataType"] = QVariant(dataTypeCombo->currentText());
    properties["DataValue"] = QVariant(dataValueEdit->text());
    properties["Array"] = QVariant(arrayCheck->isChecked());
    properties["Dimension1"] = QVariant(dim1Edit->value());
    properties["Dimension2"] = QVariant(dim2Edit->value());
    properties["Dimension3"] = QVariant(dim3Edit->value());
    properties["Dimension4"] = QVariant(dim4Edit->value());
	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> SourceItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void SourceItemDialog::setPrompts(bool dimension)
{
    //promptCloseUnsavedCheck->setChecked(closingUnsaved);
    promptDimensionCheck->setChecked(dimension);
}
//--------------------------------------------------------------------------------------------------

bool SourceItemDialog::shouldPromptOnDimension() const
{
    return promptDimensionCheck->isChecked();
}
//--------------------------------------------------------------------------------------------------
void SourceItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void SourceItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
