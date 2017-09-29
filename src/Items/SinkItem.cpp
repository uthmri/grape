/* SinkSINKItem.cpp
 *
/* SelectItem.cpp
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



#include <SinkItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
SinkItem::SinkItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("OutputType", QVariant(QString("ImageDisplay")));
    addProperty("FilePath", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
SinkItem::SinkItem(const SinkItem& item) : NodeItem(item)
{ }
//--------------------------------------------------------------------------------------------------
SinkItem::~SinkItem()
{ }
//--------------------------------------------------------------------------------------------------
DiagramItem* SinkItem::copy() const
{
    return new SinkItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString SinkItem::uniqueKey() const
{
    return "Sink";
}
//--------------------------------------------------------------------------------------------------
QString SinkItem::iconPath() const
{
      return "";//":/icons/oxygen/sink.png";
}
//--------------------------------------------------------------------------------------------------
void SinkItem::setOutputType(const QString& outputType)
{
    setPropertyValue("OutputType", QVariant(outputType));
}
//--------------------------------------------------------------------------------------------------
void SinkItem::setFilePath(const QString& filePath)
{
    setPropertyValue("FilePath", QVariant(filePath));
}
//--------------------------------------------------------------------------------------------------
void SinkItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void SinkItem::setFont(const QFont& font)
{
    setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void SinkItem::setTextBrush(const QBrush& brush)
{
    setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString SinkItem::outputType() const
{
    return propertyValue("OutputType").toString();
}
//--------------------------------------------------------------------------------------------------
QString SinkItem::filePath() const
{
    return propertyValue("FilePath").toString();
}
//--------------------------------------------------------------------------------------------------
QString SinkItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString SinkItem::caption1()
{
   return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont SinkItem::font() const
{
    return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush SinkItem::textBrush() const
{
    return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void SinkItem::markDirty()
{
    mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF SinkItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath SinkItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

    if (brush().color().alpha() == 0)
        shape.addRect(mBoundingRect);
    return shape;
}
//--------------------------------------------------------------------------------------------------
bool SinkItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void SinkItem::render(DiagramPainter* painter)
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
void SinkItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("OutputType", outputType());
    xmlWriter.writeAttribute("FilePath", filePath());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
    xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void SinkItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
    QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("OutputType"))
        setOutputType(attributes.value("OutputType").toString());
    if (attributes.hasAttribute("FilePath"))
        setFilePath(attributes.value("FilePath").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
        setFont(fontFromString(attributes.value("font").toString()));
    if (attributes.hasAttribute("textBrush"))
        setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
void SinkItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void SinkItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void SinkItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void SinkItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant SinkItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void SinkItem::changedEvent(Reason reason, const QVariant& value)
{
    Q_UNUSED(value);

    if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void SinkItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal SinkItem::orientedTextAngle() const
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
DiagramItemPropertiesDialog* SinkItem::propertiesDialog(QWidget* parent) const
{
    if (parent == NULL) parent = diagram();
    return new SinkItemDialog(parent);
}

//--------------------------------------------------------------------------------------------------
SinkItemDialog::SinkItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
    QSizeF diagramSize = diagramSizeFromParent(parent);

    fontToolBar = new DiagramFontToolBar(
        DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    filePathEdit = new QLineEdit();

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

    outputTypeCombo = new QComboBox();
    outputTypeCombo->addItem("ImageDisplay");
    outputTypeCombo->addItem("PrintText");
    outputTypeCombo->addItem("WritetoFile");

    QGroupBox* SinkGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Output Type: ", outputTypeCombo);
    pLayout->addRow("Output File Path: ", filePathEdit);

    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    SinkGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(SinkGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
    captionEdit->setFocus();
    setWindowTitle("Sink  Properties");
    resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
SinkItemDialog::~SinkItemDialog() { }
//--------------------------------------------------------------------------------------------------
void SinkItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
    if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
    if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

    if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
    if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
    if (properties.contains("Text Brush"))
        fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("OutputType")) //outputTypeCombo->setCurrentText(properties["OutputType"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("ImageDisplay",0);
        combo_text_hash.insert("PrintText",1);
        combo_text_hash.insert("WritetoFile",2);
        QString combo_text = properties["OutputType"].toString();
        outputTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }
    if (properties.contains("FilePath")) filePathEdit->setText(properties["FilePath"].toString());
    captionEdit->selectAll();
    filePathEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void SinkItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
    if (pointPositions.size() >= 2)
    {
        topLeftLayout->setPosition(pointPositions[0]);
        bottomRightLayout->setPosition(pointPositions[1]);
    }
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> SinkItemDialog::properties() const
{
    QHash<QString, QVariant> properties;

    properties["Pen"] = QVariant(penGroup->pen());
    properties["Brush"] = QVariant(brushGroup->brush());
    properties["Caption"] = QVariant(captionEdit->toPlainText());
    properties["Font"] = QVariant(fontToolBar->font());
    properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["OutputType"] = QVariant(outputTypeCombo->currentText());
    properties["FilePath"] = QVariant(filePathEdit->text());
    return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> SinkItemDialog::pointPositions() const
{
    QList<QPointF> pointPositions;

    pointPositions.append(topLeftLayout->position());
    pointPositions.append(bottomRightLayout->position());

    return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void SinkItemDialog::accept()
{
    if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void SinkItemDialog::hideEvent(QHideEvent* event)
{
    QDialog::hideEvent(event);
    captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
