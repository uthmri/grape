/* SystemcallItem.cpp
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

#include <SystemcallItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
SystemcallItem::SystemcallItem(const NodeInterface &NI) : NodeItem(NI)
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
    addProperty("Output3", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
SystemcallItem::SystemcallItem(const SystemcallItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
SystemcallItem::~SystemcallItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* SystemcallItem::copy() const
{
    return new SystemcallItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::uniqueKey() const
{
    return "Systemcall";
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::iconPath() const
{
    return "";//":icons/oxygen/sc.png";
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setCommand(const QString& command)
{
    setPropertyValue("Command", QVariant(command));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg1Port(const QString& arg1Port)
{
    setPropertyValue("Arg1Port", QVariant(arg1Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg1(const QString& arg1)
{
    setPropertyValue("Arg1", QVariant(arg1));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg2Port(const QString& arg2Port)
{
    setPropertyValue("Arg2Port", QVariant(arg2Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg2(const QString& arg2)
{
    setPropertyValue("Arg2", QVariant(arg2));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg3Port(const QString& arg3Port)
{
    setPropertyValue("Arg3Port", QVariant(arg3Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg3(const QString& arg3)
{
    setPropertyValue("Arg3", QVariant(arg3));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg4Port(const QString& arg4Port)
{
    setPropertyValue("Arg4Port", QVariant(arg4Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg4(const QString& arg4)
{
    setPropertyValue("Arg4", QVariant(arg4));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg5Port(const QString& arg5Port)
{
    setPropertyValue("Arg5Port", QVariant(arg5Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setArg5(const QString& arg5)
{
    setPropertyValue("Arg5", QVariant(arg5));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setOutput1Port(const QString& output1Port)
{
    setPropertyValue("Output1Port", QVariant(output1Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setOutput1(const QString& output1)
{
    setPropertyValue("Output1", QVariant(output1));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setOutput2Port(const QString& output2Port)
{
    setPropertyValue("Output2Port", QVariant(output2Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setOutput2(const QString& output2)
{
    setPropertyValue("Output2", QVariant(output2));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setOutput3Port(const QString& output3Port)
{
    setPropertyValue("Output3Port", QVariant(output3Port));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setOutput3(const QString& output3)
{
    setPropertyValue("Output3", QVariant(output3));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::command() const
{
    return propertyValue("Command").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Arg1Port() const
{
    return propertyValue("Arg1Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::arg1() const
{
    return propertyValue("Arg1").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Arg2Port() const
{
    return propertyValue("Arg2Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::arg2() const
{
    return propertyValue("Arg2").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Arg3Port() const
{
    return propertyValue("Arg3Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::arg3() const
{
    return propertyValue("Arg3").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Arg4Port() const
{
    return propertyValue("Arg4Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::arg4() const
{
    return propertyValue("Arg4").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Arg5Port() const
{
    return propertyValue("Arg5Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::arg5() const
{
    return propertyValue("Arg5").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Output1Port() const
{
    return propertyValue("Output1Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::output1() const
{
    return propertyValue("Output1").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Output2Port() const
{
    return propertyValue("Output2Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::output2() const
{
    return propertyValue("Output2").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::Output3Port() const
{
    return propertyValue("Output3Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::output3() const
{
    return propertyValue("Output3").toString();
}
//--------------------------------------------------------------------------------------------------
QString SystemcallItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString SystemcallItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont SystemcallItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush SystemcallItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF SystemcallItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath SystemcallItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();
	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);
	return shape;
}
//--------------------------------------------------------------------------------------------------
bool SystemcallItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::render(DiagramPainter* painter)
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
void SystemcallItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
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
void SystemcallItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
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
DiagramItemPropertiesDialog* SystemcallItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new SystemcallItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant SystemcallItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void SystemcallItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void SystemcallItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal SystemcallItem::orientedTextAngle() const
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
SystemcallItemDialog::SystemcallItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
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
    Arg1PortCombo->addItem("none");
    Arg1PortCombo->addItem("I1");
    Arg1PortCombo->addItem("I2");
    Arg1PortCombo->addItem("I3");
    Arg1PortCombo->addItem("I4");
    Arg1PortCombo->addItem("I5");
    Arg1PortCombo->addItem("fixed");

    Arg2PortCombo = new QComboBox();
    Arg2PortCombo->addItem("none");
    Arg2PortCombo->addItem("I1");
    Arg2PortCombo->addItem("I2");
    Arg2PortCombo->addItem("I3");
    Arg2PortCombo->addItem("I4");
    Arg2PortCombo->addItem("I5");
    Arg2PortCombo->addItem("fixed");

    Arg3PortCombo = new QComboBox();
    Arg3PortCombo->addItem("none");
    Arg3PortCombo->addItem("I1");
    Arg3PortCombo->addItem("I2");
    Arg3PortCombo->addItem("I3");
    Arg3PortCombo->addItem("I4");
    Arg3PortCombo->addItem("I5");
    Arg3PortCombo->addItem("fixed");

    Arg4PortCombo = new QComboBox();
    Arg4PortCombo->addItem("none");
    Arg4PortCombo->addItem("I1");
    Arg4PortCombo->addItem("I2");
    Arg4PortCombo->addItem("I3");
    Arg4PortCombo->addItem("I4");
    Arg4PortCombo->addItem("I5");
    Arg4PortCombo->addItem("fixed");

    Arg5PortCombo = new QComboBox();
    Arg5PortCombo->addItem("none");
    Arg5PortCombo->addItem("I1");
    Arg5PortCombo->addItem("I2");
    Arg5PortCombo->addItem("I3");
    Arg5PortCombo->addItem("I4");
    Arg5PortCombo->addItem("I5");
    Arg5PortCombo->addItem("fixed");


    Output1PortCombo = new QComboBox();
    Output1PortCombo->addItem("none");
    Output1PortCombo->addItem("I1");
    Output1PortCombo->addItem("I2");
    Output1PortCombo->addItem("I3");
    Output1PortCombo->addItem("I4");
    Output1PortCombo->addItem("I5");
    Output1PortCombo->addItem("fixed");

    Output2PortCombo = new QComboBox();
    Output2PortCombo->addItem("none");
    Output2PortCombo->addItem("I1");
    Output2PortCombo->addItem("I2");
    Output2PortCombo->addItem("I3");
    Output2PortCombo->addItem("I4");
    Output2PortCombo->addItem("I5");
    Output2PortCombo->addItem("fixed");

    Output3PortCombo = new QComboBox();
    Output3PortCombo->addItem("none");
    Output3PortCombo->addItem("I1");
    Output3PortCombo->addItem("I2");
    Output3PortCombo->addItem("I3");
    Output3PortCombo->addItem("I4");
    Output3PortCombo->addItem("I5");
    Output3PortCombo->addItem("fixed");

    QGroupBox* SystemcallGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Command: ", commandEdit);
    pLayout->addRow("Arg1 Port: ", Arg1PortCombo);
    pLayout->addRow("Arg1", arg1Edit);
    pLayout->addRow("Arg2 Port: ", Arg2PortCombo);
    pLayout->addRow("Arg2", arg2Edit);
    pLayout->addRow("Arg3 Port: ", Arg3PortCombo);
    pLayout->addRow("Arg3", arg3Edit);
    pLayout->addRow("Arg4 Port: ", Arg4PortCombo);
    pLayout->addRow("Arg4", arg4Edit);
    pLayout->addRow("Arg5 Port: ", Arg5PortCombo);
    pLayout->addRow("Arg5", arg5Edit);
    pLayout->addRow("Output1 Port: ", Output1PortCombo);
    pLayout->addRow("Output1", output1Edit);
    pLayout->addRow("Output2 Port: ", Output2PortCombo);
    pLayout->addRow("Output2", output2Edit);
    pLayout->addRow("Output3 Port: ", Output3PortCombo);
    pLayout->addRow("Output3", output3Edit);
    pLayout->addRow(arrayCheck);
    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    SystemcallGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(SystemcallGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("Systemcall  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
SystemcallItemDialog::~SystemcallItemDialog() { }
//--------------------------------------------------------------------------------------------------
void SystemcallItemDialog::setProperties(const QHash<QString, QVariant>& properties)
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
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);

        QString combo_text = properties["Arg1Port"].toString();
        Arg1PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Arg1")) arg1Edit->setText(properties["Arg1"].toString());
    if (properties.contains("Arg2Port")) //Arg2PortCombo->setCurrentText(properties["Arg2Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);

        QString combo_text = properties["Arg2Port"].toString();
        Arg2PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Arg2")) arg2Edit->setText(properties["Arg2"].toString());
    if (properties.contains("Arg3Port")) //Arg3PortCombo->setCurrentText(properties["Arg3Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);
        QString combo_text = properties["Arg3Port"].toString();
        Arg3PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Arg3")) arg3Edit->setText(properties["Arg3"].toString());
    if (properties.contains("Arg4Port")) //Arg4PortCombo->setCurrentText(properties["Arg4Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);
        QString combo_text = properties["Arg4Port"].toString();
        Arg4PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Arg4")) arg4Edit->setText(properties["Arg4"].toString());
    if (properties.contains("Arg5Port")) //Arg5PortCombo->setCurrentText(properties["Arg5Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);
        QString combo_text = properties["Arg5Port"].toString();
        Arg5PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Arg5")) arg5Edit->setText(properties["Arg5"].toString());
    if (properties.contains("Output1Port")) //Output1PortCombo->setCurrentText(properties["Output1Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);
        QString combo_text = properties["Output1Port"].toString();
        Output1PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Output1")) output1Edit->setText(properties["Output1"].toString());
    if (properties.contains("Output2Port")) //Output2PortCombo->setCurrentText(properties["Output2Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);
        QString combo_text = properties["Output2Port"].toString();
        Output2PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Output2")) output2Edit->setText(properties["Output2"].toString());
    if (properties.contains("Output3Port")) //Output3PortCombo->setCurrentText(properties["Output3Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("none",0);
        combo_text_hash.insert("I1",1);
        combo_text_hash.insert("I2",2);
        combo_text_hash.insert("I3",3);
        combo_text_hash.insert("I4",4);
        combo_text_hash.insert("I5",5);
        combo_text_hash.insert("fixed",6);
        QString combo_text = properties["Output3Port"].toString();
        Output3PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
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
void SystemcallItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> SystemcallItemDialog::properties() const
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
    properties["Arg2Port"] = QVariant(Arg2PortCombo->currentText());
    properties["Arg2"] = QVariant(arg2Edit->text());
    properties["Arg3Port"] = QVariant(Arg3PortCombo->currentText());
    properties["Arg3"] = QVariant(arg3Edit->text());
    properties["Arg4Port"] = QVariant(Arg4PortCombo->currentText());
    properties["Arg4"] = QVariant(arg4Edit->text());
    properties["Arg5Port"] = QVariant(Arg5PortCombo->currentText());
    properties["Arg5"] = QVariant(arg5Edit->text());
    properties["Output1Port"] = QVariant(Output1PortCombo->currentText());
    properties["Output1"] = QVariant(output1Edit->text());
    properties["Output2Port"] = QVariant(Output2PortCombo->currentText());
    properties["Output2"] = QVariant(output2Edit->text());
    properties["Output3Port"] = QVariant(Output3PortCombo->currentText());
    properties["Output3"] = QVariant(output3Edit->text());
    properties["DoubleQuote"] = QVariant(arrayCheck->isChecked());
	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> SystemcallItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void SystemcallItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void SystemcallItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
