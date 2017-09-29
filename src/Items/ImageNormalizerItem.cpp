/* ImageNormalizerItem.cpp
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

#include <ImageNormalizerItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
ImageNormalizerItem::ImageNormalizerItem(const NodeInterface &NI) : NodeItem(NI)
{  
    addProperty("Max", QVariant(QString("")));
    addProperty("Min", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
ImageNormalizerItem::ImageNormalizerItem(const ImageNormalizerItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ImageNormalizerItem::~ImageNormalizerItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ImageNormalizerItem::copy() const
{
    return new ImageNormalizerItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ImageNormalizerItem::uniqueKey() const
{
    return "ImageNormalizer";
}
//--------------------------------------------------------------------------------------------------
QString ImageNormalizerItem::iconPath() const
{
    return "";//":icons/oxygen/normalizer.png";
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::setMin(const QString& s)
{
    setPropertyValue("Min", QVariant(s));
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::setMax(const QString& s)
{
    setPropertyValue("Max", QVariant(s));
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::setFont(const QFont& font)
{
    setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::setTextBrush(const QBrush& brush)
{
    setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ImageNormalizerItem::max() const
{
    return propertyValue("Max").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageNormalizerItem::min() const
{
    return propertyValue("Min").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageNormalizerItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageNormalizerItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ImageNormalizerItem::font() const
{
    return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ImageNormalizerItem::textBrush() const
{
    return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::markDirty()
{
    mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ImageNormalizerItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ImageNormalizerItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

    if (brush().color().alpha() == 0)
        shape.addRect(mBoundingRect);

    return shape;
}
//--------------------------------------------------------------------------------------------------
bool ImageNormalizerItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::render(DiagramPainter* painter)
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
void ImageNormalizerItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("Max", max());
    xmlWriter.writeAttribute("Min", min());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
    xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
    QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("Max"))
        setMax(attributes.value("Max").toString());
    if (attributes.hasAttribute("Min"))
        setMin(attributes.value("Min").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
        setFont(fontFromString(attributes.value("font").toString()));
    if (attributes.hasAttribute("textBrush"))
        setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ImageNormalizerItem::propertiesDialog(QWidget* parent) const
{
    if (parent == NULL) parent = diagram();
    return new ImageNormalizerItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ImageNormalizerItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ImageNormalizerItem::changedEvent(Reason reason, const QVariant& value)
{
    Q_UNUSED(value);

    if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ImageNormalizerItem::orientedTextAngle() const
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
ImageNormalizerItemDialog::ImageNormalizerItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
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

    QGroupBox* ImageNormalizerGroup = new QGroupBox("Parameter");

    QFormLayout* pLayout = new QFormLayout();
    MinSpinBox = new QDoubleSpinBox();
    MaxSpinBox = new QDoubleSpinBox();

    MinSpinBox->setMinimum(MinSpinBox->maximum()*-1.0);
    MaxSpinBox->setMinimum(MaxSpinBox->maximum()*-1.0);

    MinSpinBox->setValue(0.0);
    MaxSpinBox->setValue(1.0);

    pLayout->addRow("Min Value: ", MinSpinBox);
    pLayout->addRow("Maxt Value: ", MaxSpinBox);

    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    ImageNormalizerGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(ImageNormalizerGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

    captionEdit->setFocus();

    setWindowTitle("ImageNormalizer  Properties");
    resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ImageNormalizerItemDialog::~ImageNormalizerItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
    if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
    if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

    if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
    if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
    if (properties.contains("Text Brush"))
        fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
    if (properties.contains("Max")) MaxSpinBox->setValue(properties["Max"].toDouble());
    if (properties.contains("Min")) MinSpinBox->setValue(properties["Min"].toDouble());
    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
    if (pointPositions.size() >= 2)
    {
        topLeftLayout->setPosition(pointPositions[0]);
        bottomRightLayout->setPosition(pointPositions[1]);
    }
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ImageNormalizerItemDialog::properties() const
{
    QHash<QString, QVariant> properties;
    properties["Pen"] = QVariant(penGroup->pen());
    properties["Brush"] = QVariant(brushGroup->brush());
    properties["Caption"] = QVariant(captionEdit->toPlainText());
    properties["Font"] = QVariant(fontToolBar->font());
    properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["Min"] = QVariant(MinSpinBox->value());
    properties["Max"] = QVariant(MaxSpinBox->value());
    return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ImageNormalizerItemDialog::pointPositions() const
{
    QList<QPointF> pointPositions;

    pointPositions.append(topLeftLayout->position());
    pointPositions.append(bottomRightLayout->position());
    return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItemDialog::accept()
{
    if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ImageNormalizerItemDialog::hideEvent(QHideEvent* event)
{
    QDialog::hideEvent(event);
    captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
