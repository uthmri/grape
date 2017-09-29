/* SynchItem.cpp
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
* */

#include <SynchItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

SynchItem::SynchItem(const NodeInterface &NI) : NodeItem(NI)
{
}
//--------------------------------------------------------------------------------------------------
SynchItem::SynchItem(const SynchItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
SynchItem::~SynchItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* SynchItem::copy() const
{
    return new SynchItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString SynchItem::uniqueKey() const
{
    return "Synch";
}
//--------------------------------------------------------------------------------------------------
QString SynchItem::iconPath() const
{
    return "";//":/icons/oxygen/split.png";
}
//--------------------------------------------------------------------------------------------------

void SynchItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void SynchItem::setFont(const QFont& font)
{
    setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void SynchItem::setTextBrush(const QBrush& brush)
{
    setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString SynchItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString SynchItem::caption1()
{
   return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont SynchItem::font() const
{
    return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush SynchItem::textBrush() const
{
    return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void SynchItem::markDirty()
{
    mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF SynchItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath SynchItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

    if (brush().color().alpha() == 0)
        shape.addRect(mBoundingRect);

    return shape;
}
//--------------------------------------------------------------------------------------------------
bool SynchItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void SynchItem::render(DiagramPainter* painter)
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

void SynchItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);
    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
    xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void SynchItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
    QXmlStreamAttributes attributes = xmlReader.attributes();
    DiagramRoundItem::readXmlAttributes(xmlReader, items);
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
        setFont(fontFromString(attributes.value("font").toString()));
    if (attributes.hasAttribute("textBrush"))
        setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* SynchItem::propertiesDialog(QWidget* parent) const
{
    if (parent == NULL) parent = diagram();
    return new SynchItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void SynchItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void SynchItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void SynchItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void SynchItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant SynchItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void SynchItem::changedEvent(Reason reason, const QVariant& value)
{
    Q_UNUSED(value);
    if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void SynchItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal SynchItem::orientedTextAngle() const
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

SynchItemDialog::SynchItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
    QSizeF diagramSize = diagramSizeFromParent(parent);

    fontToolBar = new DiagramFontToolBar(
        DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
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
    QGroupBox* Loop1Group = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    Loop1Group->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(Loop1Group);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
    captionEdit->setFocus();
    setWindowTitle("Synch  Properties");
    resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
SynchItemDialog::~SynchItemDialog() { }
//--------------------------------------------------------------------------------------------------
void SynchItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
    if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
    if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());
    if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
    if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
    if (properties.contains("Text Brush"))
        fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void SynchItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
    if (pointPositions.size() >= 2)
    {
        topLeftLayout->setPosition(pointPositions[0]);
        bottomRightLayout->setPosition(pointPositions[1]);
    }
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> SynchItemDialog::properties() const
{
    QHash<QString, QVariant> properties;

    properties["Pen"] = QVariant(penGroup->pen());
    properties["Brush"] = QVariant(brushGroup->brush());
    properties["Caption"] = QVariant(captionEdit->toPlainText());
    properties["Font"] = QVariant(fontToolBar->font());
    properties["Text Brush"] = QVariant(fontToolBar->brush());
    return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> SynchItemDialog::pointPositions() const
{
    QList<QPointF> pointPositions;

    pointPositions.append(topLeftLayout->position());
    pointPositions.append(bottomRightLayout->position());
    return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void SynchItemDialog::accept()
{
    if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void SynchItemDialog::hideEvent(QHideEvent* event)
{
    QDialog::hideEvent(event);
    captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
