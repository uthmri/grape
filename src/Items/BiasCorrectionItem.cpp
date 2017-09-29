/* BiasCorrectionItem.cpp*/


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
* along with GRAPE.  If not, see <http://www.gnu.org/licenses/>
*/

#include <BiasCorrectionItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//SourceTypeCombo::SourceTypeCombo(QWidget* parent) : QComboBox(parent)
//{
//    refreshItems(false, true);
//}
//--------------------------------------------------------------------------------------------------
BiasCorrectionItem::BiasCorrectionItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("BiasCorrectionType", QVariant(QString("string")));
    addProperty("CommandPath", QVariant(QString("../ANTS/")));
    addProperty("OutputPrefix", QVariant(QString("bc_")));
    addProperty("Dimension", QVariant(QString("3 ")));
    addProperty("ShrinkFactor", QVariant(QString(" 2")));
    addProperty("Convergence", QVariant(QString("[50x50x50x50,0.000001]")));
    addProperty("BSplineFitting", QVariant(QString("200")));
}
//--------------------------------------------------------------------------------------------------
BiasCorrectionItem::BiasCorrectionItem(const BiasCorrectionItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
BiasCorrectionItem::~BiasCorrectionItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* BiasCorrectionItem::copy() const
{
    return new BiasCorrectionItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::uniqueKey() const
{
    return "BiasCorrection";
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::iconPath() const
{
    return "";//":/icons/oxygen/bc.png";
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setBiasCorrectionType(const QString& biasCorrectionType)
{
    setPropertyValue("BiasCorrectionType", QVariant(biasCorrectionType));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setCommandPath(const QString& commandPath)
{
    setPropertyValue("CommandPath", QVariant(commandPath));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setOutputPrefix(const QString& OutputPrefix)
{
    setPropertyValue("OutputPrefix", QVariant(OutputPrefix));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setShrinkFactor(const QString& shrinkFactor)
{
    setPropertyValue("ShrinkFactor", QVariant(shrinkFactor));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setConvergence(const QString& convergence)
{
    setPropertyValue("Convergence", QVariant(convergence));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setBSplineFitting(const QString& bSplineFitting)
{
    setPropertyValue("BSplineFitting", QVariant(bSplineFitting));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setDimension(const QString& dimension)
{
    setPropertyValue("Dimension", QVariant(dimension));
}
//--------------------------------------------------------------------------------------------------

void BiasCorrectionItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::BiasCorrectionType() const
{
    return propertyValue("BiasCorrectionType").toString();
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::commandPath() const
{
    return propertyValue("CommandPath").toString();
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::outputPrefix() const
{
    return propertyValue("OutputPrefix").toString();
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::bSplineFitting() const
{
    return propertyValue("BSplineFitting").toString();
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::convergence() const
{
    return propertyValue("Convergence").toString();
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::dimension() const
{
    return propertyValue("Dimension").toString();
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::arg2() const
{
    return propertyValue("ShrinkFactor").toString();
}
//--------------------------------------------------------------------------------------------------
QString BiasCorrectionItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString BiasCorrectionItem::caption1()
{
     return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont BiasCorrectionItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush BiasCorrectionItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF BiasCorrectionItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath BiasCorrectionItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool BiasCorrectionItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::render(DiagramPainter* painter)
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
void BiasCorrectionItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("BiasCorrectionType", BiasCorrectionType());
    xmlWriter.writeAttribute("CommandPath", commandPath());
    xmlWriter.writeAttribute("OutputPrefix", outputPrefix());
    xmlWriter.writeAttribute("Dimension", dimension());
    xmlWriter.writeAttribute("ShrinkFactor", arg2());
    xmlWriter.writeAttribute("Convergence", convergence());
    xmlWriter.writeAttribute("BSplineFitting", bSplineFitting());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("BiasCorrectionType"))
        setBiasCorrectionType(attributes.value("BiasCorrectionType").toString());
    if (attributes.hasAttribute("CommandPath"))
        setCommandPath(attributes.value("CommandPath").toString());
    if (attributes.hasAttribute("OutputPrefix"))
        setOutputPrefix(attributes.value("OutputPrefix").toString());
    if (attributes.hasAttribute("Dimension"))
        setDimension(attributes.value("Dimension").toString());
    if (attributes.hasAttribute("ShrinkFactor"))
        setShrinkFactor(attributes.value("ShrinkFactor").toString());
    if (attributes.hasAttribute("Convergence"))
        setConvergence(attributes.value("Convergence").toString());
    if (attributes.hasAttribute("BSplineFitting"))
        setBSplineFitting(attributes.value("BSplineFitting").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* BiasCorrectionItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new BiasCorrectionItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant BiasCorrectionItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void BiasCorrectionItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal BiasCorrectionItem::orientedTextAngle() const
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
BiasCorrectionItemDialog::BiasCorrectionItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();

    commandPathEdit = new QLineEdit();        
    outputPrefixEdit = new QLineEdit();

    spinDimension = new QSpinBox();
    spinDimension->setMinimum(2); spinDimension->setMaximum(4);

    spinShrink = new QSpinBox();

    convergenceEdit = new QLineEdit();

    bSplineFittingEdit = new QDoubleSpinBox();
    bSplineFittingEdit->setMinimum(0.0); bSplineFittingEdit->setMaximum(500.0);

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

    biasCorrectionTypeCombo = new QComboBox();
    biasCorrectionTypeCombo->addItem("N3");
    biasCorrectionTypeCombo->addItem("N4");

    QGroupBox* biasCorrectionGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Bias Field Correction: ", biasCorrectionTypeCombo);
    pLayout->addRow("Command Path: ", commandPathEdit);
    pLayout->addRow("Output Prefix: ", outputPrefixEdit);
    pLayout->addRow("Dimension: ", spinDimension);
    pLayout->addRow("Shrink Factor: ", spinShrink);
    pLayout->addRow("Convergence: ", convergenceEdit);
    pLayout->addRow("B-Spline Fitting: ", bSplineFittingEdit);

    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    biasCorrectionGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(biasCorrectionGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("Biascorrection  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
BiasCorrectionItemDialog::~BiasCorrectionItemDialog() { }
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("BiasCorrectionType"))
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("N3",0);
        combo_text_hash.insert("N4",1);

        QString combo_text = properties["BiasCorrectionType"].toString();
        biasCorrectionTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    if (properties.contains("CommandPath")) commandPathEdit->setText(properties["CommandPath"].toString());
    if (properties.contains("OutputPrefix")) outputPrefixEdit->setText(properties["OutputPrefix"].toString());
    if (properties.contains("Dimension")) spinDimension->setValue(properties["Dimension"].toInt());
    if (properties.contains("ShrinkFactor")) spinShrink->setValue(properties["ShrinkFactor"].toInt());
    if (properties.contains("Convergence")) convergenceEdit->setText(properties["Convergence"].toString());
    if (properties.contains("BSplineFitting")) bSplineFittingEdit->setValue(properties["BSplineFitting"].toInt());

    captionEdit->selectAll();
    commandPathEdit->selectAll();
    outputPrefixEdit->selectAll();
    spinDimension->selectAll();
    spinShrink->selectAll();
    convergenceEdit->selectAll();
    bSplineFittingEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> BiasCorrectionItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());

    properties["BiasCorrectionType"] = QVariant(biasCorrectionTypeCombo->currentText());
    properties["CommandPath"] = QVariant(commandPathEdit->text());
    properties["OutputPrefix"] = QVariant(outputPrefixEdit->text());
    properties["Dimension"] = QVariant(spinDimension->value());
    properties["ShrinkFactor"] = QVariant(spinShrink->value());
    properties["Convergence"] = QVariant(convergenceEdit->text());
    properties["BSplineFitting"] = QVariant(bSplineFittingEdit->value());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> BiasCorrectionItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void BiasCorrectionItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
