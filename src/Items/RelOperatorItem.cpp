/* RelOperatorItem.cpp
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

#include <RelOperatorItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
RelOperatorItem::RelOperatorItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("RelOperatorType", QVariant(QString("")));
    addProperty("Arg1Port", QVariant(QString("")));
    addProperty("Arg2Port", QVariant(QString("")));
    addProperty("Output1", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
RelOperatorItem::RelOperatorItem(const RelOperatorItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
RelOperatorItem::~RelOperatorItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* RelOperatorItem::copy() const
{
    return new RelOperatorItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString RelOperatorItem::uniqueKey() const
{
    return "RelOperator";
}
//--------------------------------------------------------------------------------------------------
QString RelOperatorItem::iconPath() const
{
    return "";//":icons/oxygen/reloperator.png";
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::setRelOperatorType(const QString& RelOperatorType)
{
    setPropertyValue("RelOperatorType", QVariant(RelOperatorType));
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::setArg1Port(const QString& arg1Port)
{
    setPropertyValue("Arg1Port", QVariant(arg1Port));
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::setArg2Port(const QString& arg2Port)
{
    setPropertyValue("Arg2Port", QVariant(arg2Port));
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::setOutput1(const QString& output1)
{
    setPropertyValue("Output1", QVariant(output1));
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------

QString RelOperatorItem::RelOperatorType() const
{
    return propertyValue("RelOperatorType").toString();
}
//--------------------------------------------------------------------------------------------------
QString RelOperatorItem::arg1Port() const
{
    return propertyValue("Arg1Port").toString();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QString RelOperatorItem::arg2Port() const
{
    return propertyValue("Arg2Port").toString();
}
//--------------------------------------------------------------------------------------------------
QString RelOperatorItem::output1() const
{
    return propertyValue("Output1").toString();
}
//--------------------------------------------------------------------------------------------------
QString RelOperatorItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString RelOperatorItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont RelOperatorItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush RelOperatorItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF RelOperatorItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath RelOperatorItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);
	return shape;
}
//--------------------------------------------------------------------------------------------------
bool RelOperatorItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::render(DiagramPainter* painter)
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
void RelOperatorItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("RelOperatorType", RelOperatorType());
    xmlWriter.writeAttribute("Arg1Port", arg1Port());
    xmlWriter.writeAttribute("Arg2Port", arg2Port());
    xmlWriter.writeAttribute("Output1", output1());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("RelOperatorType"))
        setRelOperatorType(attributes.value("RelOperatorType").toString());
    if (attributes.hasAttribute("Arg1Port"))
        setArg1Port(attributes.value("Arg1Port").toString());
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
DiagramItemPropertiesDialog* RelOperatorItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new RelOperatorItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------

void RelOperatorItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant RelOperatorItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void RelOperatorItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal RelOperatorItem::orientedTextAngle() const
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
RelOperatorItemDialog::RelOperatorItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
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
    RelOperatorTypeCombo = new QComboBox();
    RelOperatorTypeCombo->addItem("=");
    RelOperatorTypeCombo->addItem(">");
    RelOperatorTypeCombo->addItem("<");
    RelOperatorTypeCombo->addItem(">=");
    RelOperatorTypeCombo->addItem("<=");
    RelOperatorTypeCombo->addItem("<>");
    Arg1PortCombo = new QComboBox();
    Arg1PortCombo->addItem("I1");
    Arg1PortCombo->addItem("I3");
    Arg2PortCombo = new QComboBox();
    Arg2PortCombo->addItem("I1");
    Arg2PortCombo->addItem("I3");
    QGroupBox* RelOperatorGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Arg1Port: ", Arg1PortCombo);
    pLayout->addRow("RelOperatorType: ", RelOperatorTypeCombo);
    pLayout->addRow("Arg2Port: ", Arg2PortCombo);
    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    RelOperatorGroup->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(RelOperatorGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();
    setWindowTitle("RelOperator  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
RelOperatorItemDialog::~RelOperatorItemDialog() { }
//--------------------------------------------------------------------------------------------------
void RelOperatorItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("Arg1Port"))//Arg1PortCombo->setCurrentText(properties["Arg1Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("I1",0);
        combo_text_hash.insert("I3",1);
        QString combo_text = properties["Arg1Port"].toString();
        Arg1PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("RelOperatorType")) //RelOperatorTypeCombo->setCurrentText(properties["RelOperatorType"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("=",0);
        combo_text_hash.insert(">",1);
        combo_text_hash.insert("<",2);
        combo_text_hash.insert(">=",3);
        combo_text_hash.insert("<=",4);
        combo_text_hash.insert("<>",5);
        QString combo_text = properties["RelOperatorType"].toString();
        RelOperatorTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Arg2Port")) //Arg2PortCombo->setCurrentText(properties["Arg2Port"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("I1",0);
        combo_text_hash.insert("I3",1);
        QString combo_text = properties["Arg2Port"].toString();
        Arg2PortCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("Output1")) output1Edit->setText(properties["Output1"].toString());
    captionEdit->selectAll();
    output1Edit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> RelOperatorItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["Arg1Port"] = QVariant(Arg1PortCombo->currentText());
    properties["RelOperatorType"] = QVariant(RelOperatorTypeCombo->currentText());
    properties["Arg2Port"] = QVariant(Arg2PortCombo->currentText());
    properties["Output1"] = QVariant(output1Edit->text());
	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> RelOperatorItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void RelOperatorItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
