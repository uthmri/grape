/* DevelopItem.cpp
 *
* Copyright (C) 2016 UTHealth MRI Research
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

#include <DevelopItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
DevelopItem::DevelopItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("Command", QVariant(QString("")));
    addProperty("Arg1Port", QVariant(QString("")));
    addProperty("Arg1", QVariant(QString("")));
    addProperty("Arg2Port", QVariant(QString("")));
    addProperty("Arg2", QVariant(QString("")));
    addProperty("Arg3Port", QVariant(QString("")));
    addProperty("Arg3", QVariant(QString("")));
    addProperty("Arg4Port", QVariant(QString("")));
    addProperty("Arg4", QVariant(QString("")));
    addProperty("Arg5Port", QVariant(QString("")));
    addProperty("Arg5", QVariant(QString("")));
    addProperty("Output1Port", QVariant(QString("")));
    addProperty("Output1", QVariant(QString("")));
    addProperty("Output2Port", QVariant(QString("")));
    addProperty("Output2", QVariant(QString("")));
    addProperty("Output3Port", QVariant(QString("")));
    addProperty("Output4", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
DevelopItem::DevelopItem(const DevelopItem& item) : NodeItem(item)
{ }
//--------------------------------------------------------------------------------------------------
DevelopItem::~DevelopItem()
{ }
//--------------------------------------------------------------------------------------------------
DiagramItem* DevelopItem::copy() const
{
    return new DevelopItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::uniqueKey() const
{
    return "Develop";
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::iconPath() const
{
    return "";//":icons/oxygen/sc.png";
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setCommand(const QString& command)
{
    setPropertyValue("Command", QVariant(command));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg1Port(const QString& arg1Port)
{
    setPropertyValue("Arg1Port", QVariant(arg1Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg1(const QString& arg1)
{
    setPropertyValue("Arg1", QVariant(arg1));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg2Port(const QString& arg2Port)
{
    setPropertyValue("Arg2Port", QVariant(arg2Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg2(const QString& arg2)
{
    setPropertyValue("Arg2", QVariant(arg2));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg3Port(const QString& arg3Port)
{
    setPropertyValue("Arg3Port", QVariant(arg3Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg3(const QString& arg3)
{
    setPropertyValue("Arg3", QVariant(arg3));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg4Port(const QString& arg4Port)
{
    setPropertyValue("Arg4Port", QVariant(arg4Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg4(const QString& arg4)
{
    setPropertyValue("Arg4", QVariant(arg4));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg5Port(const QString& arg5Port)
{
    setPropertyValue("Arg5Port", QVariant(arg5Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setArg5(const QString& arg5)
{
    setPropertyValue("Arg5", QVariant(arg5));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setOutput1Port(const QString& output1Port)
{
    setPropertyValue("Output1Port", QVariant(output1Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setOutput1(const QString& output1)
{
    setPropertyValue("Output1", QVariant(output1));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setOutput2Port(const QString& output2Port)
{
    setPropertyValue("Output2Port", QVariant(output2Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setOutput2(const QString& output2)
{
    setPropertyValue("Output2", QVariant(output2));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setOutput3Port(const QString& output3Port)
{
    setPropertyValue("Output3Port", QVariant(output3Port));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setOutput3(const QString& output3)
{
    setPropertyValue("Output3", QVariant(output3));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::command() const
{
    return propertyValue("Command").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Arg1Port() const
{
    return propertyValue("Arg1Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::arg1() const
{
    return propertyValue("Arg1").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Arg2Port() const
{
    return propertyValue("Arg2Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::arg2() const
{
    return propertyValue("Arg2").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Arg3Port() const
{
    return propertyValue("Arg3Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::arg3() const
{
    return propertyValue("Arg3").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Arg4Port() const
{
    return propertyValue("Arg4Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::arg4() const
{
    return propertyValue("Arg4").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Arg5Port() const
{
    return propertyValue("Arg5Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::arg5() const
{
    return propertyValue("Arg5").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Output1Port() const
{
    return propertyValue("Output1Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::output1() const
{
    return propertyValue("Output1").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Output2Port() const
{
    return propertyValue("Output2Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::output2() const
{
    return propertyValue("Output2").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::Output3Port() const
{
    return propertyValue("Output3Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::output3() const
{
    return propertyValue("Output3").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString DevelopItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont DevelopItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush DevelopItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF DevelopItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath DevelopItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool DevelopItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::render(DiagramPainter* painter)
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
void DevelopItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);
    xmlWriter.writeAttribute("Command", command());
    xmlWriter.writeAttribute("Arg1Port", Arg1Port());
    xmlWriter.writeAttribute("Arg1", arg1());
    xmlWriter.writeAttribute("Arg2Port", Arg2Port());
    xmlWriter.writeAttribute("Arg2", arg2());
    xmlWriter.writeAttribute("Arg3Port", Arg3Port());
    xmlWriter.writeAttribute("Arg3", arg3());
    xmlWriter.writeAttribute("Arg4Port", Arg4Port());
    xmlWriter.writeAttribute("Arg4", arg4());
    xmlWriter.writeAttribute("Arg5Port", Arg5Port());
    xmlWriter.writeAttribute("Arg5", arg5());
    xmlWriter.writeAttribute("Output1Port", Output1Port());
    xmlWriter.writeAttribute("Output1", output1());
    xmlWriter.writeAttribute("Output2Port", Output2Port());
    xmlWriter.writeAttribute("Output2", output2());
    xmlWriter.writeAttribute("Output3Port", Output3Port());
    xmlWriter.writeAttribute("Output3", output3());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();
    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("Command"))
        setCommand(attributes.value("Command").toString());
    if (attributes.hasAttribute("Arg1Port"))
        setArg1Port(attributes.value("Arg1Port").toString());
    if (attributes.hasAttribute("Arg1"))
        setArg1(attributes.value("Arg1").toString());
    if (attributes.hasAttribute("Arg2Port"))
        setArg2Port(attributes.value("Arg2Port").toString());
    if (attributes.hasAttribute("Arg2"))
        setArg2(attributes.value("Arg2").toString());
    if (attributes.hasAttribute("Arg3Port"))
        setArg3Port(attributes.value("Arg3Port").toString());
    if (attributes.hasAttribute("Arg3"))
        setArg3(attributes.value("Arg3").toString());
    if (attributes.hasAttribute("Arg4Port"))
        setArg4Port(attributes.value("Arg4Port").toString());
    if (attributes.hasAttribute("Arg4"))
        setArg4(attributes.value("Arg4").toString());
    if (attributes.hasAttribute("Arg5Port"))
        setArg5Port(attributes.value("Arg5Port").toString());
    if (attributes.hasAttribute("Arg5"))
        setArg5(attributes.value("Arg5").toString());
    if (attributes.hasAttribute("Output1Port"))
        setOutput1Port(attributes.value("Output1Port").toString());
    if (attributes.hasAttribute("Output1"))
        setOutput1(attributes.value("Output1").toString());
    if (attributes.hasAttribute("Output2Port"))
        setOutput2Port(attributes.value("Output2Port").toString());
    if (attributes.hasAttribute("Output2"))
        setOutput2(attributes.value("Output2").toString());
    if (attributes.hasAttribute("Output3Port"))
        setOutput3Port(attributes.value("Output3Port").toString());
    if (attributes.hasAttribute("Output3"))
        setOutput3(attributes.value("Output3").toString());

    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DevelopItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new DevelopItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant DevelopItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void DevelopItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void DevelopItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal DevelopItem::orientedTextAngle() const
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
DevelopItemDialog::DevelopItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
    DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    commandEdit = new QLineEdit();
    arg1Edit = new QLineEdit();
    arg2Edit = new QLineEdit();
    arg3Edit = new QLineEdit();
    arg4Edit = new QLineEdit();
    arg5Edit = new QLineEdit();

    output1Edit = new QLineEdit();
    output2Edit = new QLineEdit();
    output3Edit = new QLineEdit();
    arrayCheck = new QCheckBox();
    arrayCheck->setText("Double Quote");

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

    Arg1PortCombo = new QComboBox();
    Arg1PortCombo->addItem("f0");
    Arg1PortCombo->addItem("f1");
    Arg1PortCombo->addItem("f2");
    Arg1PortCombo->addItem("f3");
    Arg1PortCombo->addItem("f4");
    Arg1PortCombo->addItem("f5");
    Arg1PortCombo->addItem("f6");

    Output1PortCombo = new QComboBox();
    Output1PortCombo->addItem("f0");
    Output1PortCombo->addItem("f1");
    Output1PortCombo->addItem("f2");
    Output1PortCombo->addItem("f3");
    Output1PortCombo->addItem("f4");
    Output1PortCombo->addItem("f5");
    Output1PortCombo->addItem("f6");

    QGroupBox* DevelopGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Command: ", commandEdit);
    pLayout->addRow("Function: ", Arg1PortCombo);
    pLayout->addRow("Arg1", arg1Edit);
   // pLayout->addRow("Arg2 Port: ", Arg2PortCombo);
    pLayout->addRow("Arg2", arg2Edit);
   // pLayout->addRow("Arg3 Port: ", Arg3PortCombo);
    pLayout->addRow("Arg3", arg3Edit);
   // pLayout->addRow("Arg4 Port: ", Arg4PortCombo);
    pLayout->addRow("Arg4", arg4Edit);
  //  pLayout->addRow("Arg5 Port: ", Arg5PortCombo);
    pLayout->addRow("Arg5", arg5Edit);
    pLayout->addRow("Output1 Port: ", Output1PortCombo);
    pLayout->addRow("Output1", output1Edit);
   // pLayout->addRow("Output2 Port: ", Output2PortCombo);
    pLayout->addRow("Output2", output2Edit);
   // pLayout->addRow("Output3 Port: ", Output3PortCombo);
    pLayout->addRow("Output3", output3Edit);
    pLayout->addRow(arrayCheck);
//    pLayout->addRow("Output4Port: ", Output4PortCombo);
//    pLayout->addRow("Output4", output4Edit);
    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    DevelopGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(DevelopGroup);
   // addWidget(Arg2Group);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("Develop  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DevelopItemDialog::~DevelopItemDialog() { }
//--------------------------------------------------------------------------------------------------
void DevelopItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
    if (properties.contains("Command")) commandEdit->setText(properties["Command"].toString());
    if (properties.contains("Arg1Port")) //Arg1PortCombo->setCurrentText(properties["Arg1Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("f0",0);
        combo_text_hash.insert("f1",1);
        combo_text_hash.insert("f2",2);
        combo_text_hash.insert("f3",3);
        combo_text_hash.insert("f4",4);
        combo_text_hash.insert("f5",5);
        combo_text_hash.insert("f6",6);

        QString combo_text = properties["Arg1Port"].toString();
        Arg1PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Arg1")) arg1Edit->setText(properties["Arg1"].toString());
    if (properties.contains("Arg2")) arg2Edit->setText(properties["Arg2"].toString());
    if (properties.contains("Arg3")) arg3Edit->setText(properties["Arg3"].toString());
    if (properties.contains("Arg4")) arg4Edit->setText(properties["Arg4"].toString());
    if (properties.contains("Arg5")) arg5Edit->setText(properties["Arg5"].toString());
    if (properties.contains("Output1Port")) //Output1PortCombo->setCurrentText(properties["Output1Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("f0",0);
        combo_text_hash.insert("f1",1);
        combo_text_hash.insert("f2",2);
        combo_text_hash.insert("f3",3);
        combo_text_hash.insert("f4",4);
        combo_text_hash.insert("f5",5);
        combo_text_hash.insert("f6",6);

        QString combo_text = properties["Output1Port"].toString();
        Output1PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Output1")) output1Edit->setText(properties["Output1"].toString());
    if (properties.contains("Output3")) output3Edit->setText(properties["Output3"].toString());
    if (properties.contains("DoubleQuote")) //dim1Edit->setText(properties["Dimension1"].toString());
        arrayCheck->setChecked(properties["DoubleQuote"].toBool());

    captionEdit->selectAll();
    commandEdit->selectAll();
    arg1Edit->selectAll();
    arg2Edit->selectAll();
    arg3Edit->selectAll();
    arg4Edit->selectAll();
    arg5Edit->selectAll();
    output1Edit->selectAll();
    output2Edit->selectAll();
    output3Edit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void DevelopItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DevelopItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());

    properties["Command"] = QVariant(commandEdit->text());
    properties["Arg1Port"] = QVariant(Arg1PortCombo->currentText());
    properties["Arg1"] = QVariant(arg1Edit->text());
   //  properties["Arg2Port"] = QVariant(Arg2PortCombo->currentText());
    properties["Arg2"] = QVariant(arg2Edit->text());
   // properties["Arg3Port"] = QVariant(Arg3PortCombo->currentText());
    properties["Arg3"] = QVariant(arg3Edit->text());
  //  properties["Arg4Port"] = QVariant(Arg4PortCombo->currentText());
    properties["Arg4"] = QVariant(arg4Edit->text());
  //  properties["Arg5Port"] = QVariant(Arg5PortCombo->currentText());
    properties["Arg5"] = QVariant(arg5Edit->text());
  //  properties["Output1Port"] = QVariant(Output1PortCombo->currentText());
    properties["Output1"] = QVariant(output1Edit->text());
  //  properties["Output2Port"] = QVariant(Output2PortCombo->currentText());
    properties["Output2"] = QVariant(output2Edit->text());
  //  properties["Output3Port"] = QVariant(Output3PortCombo->currentText());
    properties["Output3"] = QVariant(output3Edit->text());
  //  properties["DoubleQuote"] = QVariant(arrayCheck->isChecked());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> DevelopItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void DevelopItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void DevelopItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
