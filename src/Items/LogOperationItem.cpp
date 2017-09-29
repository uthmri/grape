/* LogOperationItem.cpp
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

#include <LogOperationItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
LogOperationItem::LogOperationItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("Arg1Port", QVariant(QString("")));
    addProperty("LogOperationType", QVariant(QString("")));
    addProperty("Arg2Port", QVariant(QString("")));
    addProperty("Output1", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
LogOperationItem::LogOperationItem(const LogOperationItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
LogOperationItem::~LogOperationItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* LogOperationItem::copy() const
{
    return new LogOperationItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString LogOperationItem::uniqueKey() const
{
    return "LogOperation";
}
//--------------------------------------------------------------------------------------------------
QString LogOperationItem::iconPath() const
{
    return "";//":/icons/oxygen/logic.png";
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::setLogOperationType(const QString& LogOperationType)
{
    setPropertyValue("LogOperationType", QVariant(LogOperationType));
}
void LogOperationItem::setArg1Port(const QString& arg1Port)
{
    setPropertyValue("Arg1Port", QVariant(arg1Port));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void LogOperationItem::setArg2Port(const QString& arg2Port)
{
    setPropertyValue("Arg2Port", QVariant(arg2Port));
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::setOutput1(const QString& output1)
{
    setPropertyValue("Output1", QVariant(output1));
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString LogOperationItem::LogOperationType() const
{
    return propertyValue("LogOperationType").toString();
}
//--------------------------------------------------------------------------------------------------
QString LogOperationItem::arg1Port() const
{
    return propertyValue("Arg1Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString LogOperationItem::arg2Port() const
{
    return propertyValue("Arg2Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString LogOperationItem::output1() const
{
    return propertyValue("Output1").toString();
}
//--------------------------------------------------------------------------------------------------
QString LogOperationItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString LogOperationItem::caption1()
{
   return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont LogOperationItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush LogOperationItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF LogOperationItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath LogOperationItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool LogOperationItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::render(DiagramPainter* painter)
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
void LogOperationItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("Arg1Port", arg1Port());
    xmlWriter.writeAttribute("LogOperationType", LogOperationType());
    xmlWriter.writeAttribute("Arg2Port", arg2Port());
    xmlWriter.writeAttribute("Output1", output1());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("Arg1Port"))
        setArg1Port(attributes.value("Arg1Port").toString());
    if (attributes.hasAttribute("LogOperationType"))
        setLogOperationType(attributes.value("LogOperationType").toString());
    if (attributes.hasAttribute("Arg2Port"))
        setArg2Port(attributes.value("Arg2Port").toString());
    if (attributes.hasAttribute("Output1"))
        setOutput1(attributes.value("Output1").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* LogOperationItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new LogOperationItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant LogOperationItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void LogOperationItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void LogOperationItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal LogOperationItem::orientedTextAngle() const
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
LogOperationItemDialog::LogOperationItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    output1Edit = new QLineEdit();
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
    LogOperationTypeCombo = new QComboBox();
    LogOperationTypeCombo->addItem("&&");
    LogOperationTypeCombo->addItem("||");
    LogOperationTypeCombo->addItem("~");
    Arg1PortCombo = new QComboBox();
    Arg1PortCombo->addItem("I1");
    Arg1PortCombo->addItem("I3");
    Arg2PortCombo = new QComboBox();
    Arg2PortCombo->addItem("I1");
    Arg2PortCombo->addItem("I3");
    QGroupBox* LogOperationGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Arg1Port: ", Arg1PortCombo);
    pLayout->addRow("LogOperationType: ", LogOperationTypeCombo);
    pLayout->addRow("Arg2Port: ", Arg2PortCombo);
    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    LogOperationGroup->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(LogOperationGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();
    setWindowTitle("LogOperation  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
LogOperationItemDialog::~LogOperationItemDialog() { }
//--------------------------------------------------------------------------------------------------
void LogOperationItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("Arg1Port")) //Arg1PortCombo->setCurrentText(properties["Arg1Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("I1",0);
        combo_text_hash.insert("I2",1);
        combo_text_hash.insert("I3",2);
        QString combo_text = properties["Arg1Port"].toString();
        Arg1PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    if (properties.contains("LogOperationType")) //LogOperationTypeCombo->setCurrentText(properties["LogOperationType"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("&&",0);
        combo_text_hash.insert("||",1);
        combo_text_hash.insert("~",2);
        QString combo_text = properties["LogOperationType"].toString();
        LogOperationTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
   if (properties.contains("Arg2Port")) //Arg2PortCombo->setCurrentText(properties["Arg2Port"].toString());
   {
       QHash<QString, int> combo_text_hash;
       combo_text_hash.insert("I1",0);
       combo_text_hash.insert("I2",1);
       combo_text_hash.insert("I3",2);
       QString combo_text = properties["Arg2Port"].toString();
       Arg2PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    if (properties.contains("Output1")) output1Edit->setText(properties["Output1"].toString());
    captionEdit->selectAll();
    output1Edit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void LogOperationItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> LogOperationItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["Arg1Port"] = QVariant(Arg1PortCombo->currentText());
    properties["LogOperationType"] = QVariant(LogOperationTypeCombo->currentText());
    properties["Arg2Port"] = QVariant(Arg2PortCombo->currentText());
    properties["Output1"] = QVariant(output1Edit->text());
	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> LogOperationItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void LogOperationItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void LogOperationItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
