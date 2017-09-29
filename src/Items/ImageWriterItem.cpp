/* ImageWriterItem.cpp
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

#include <ImageWriterItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//}
//--------------------------------------------------------------------------------------------------
ImageWriterItem::ImageWriterItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("OutputImageType", QVariant(QString("")));
    addProperty("OutputfilePath", QVariant(QString("")));
    addProperty("Anonymous", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
ImageWriterItem::ImageWriterItem(const ImageWriterItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ImageWriterItem::~ImageWriterItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ImageWriterItem::copy() const
{
    return new ImageWriterItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ImageWriterItem::uniqueKey() const
{
    return "ImageWriter";
}
//--------------------------------------------------------------------------------------------------
QString ImageWriterItem::iconPath() const
{
    return "";//":icons/oxygen/IW.png";
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::setOutputfilePath(const QString& path)
{
    setPropertyValue("OutputfilePath", QVariant(path));
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::setOutputImageType(const QString& arg1Port)
{
    setPropertyValue("OutputImageType", QVariant(arg1Port));
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::setAnonymous(const QString& v)
{
    setPropertyValue("Anonymous", QVariant(v));
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::setFont(const QFont& font)
{
    setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::setTextBrush(const QBrush& brush)
{
    setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ImageWriterItem::outputImageType() const
{
    return propertyValue("OutputImageType").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageWriterItem::filePath() const
{
    return propertyValue("OutputfilePath").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageWriterItem::Anonymous() const
{
    return propertyValue("Anonymous").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageWriterItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageWriterItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ImageWriterItem::font() const
{
    return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ImageWriterItem::textBrush() const
{
    return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::markDirty()
{
    mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ImageWriterItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ImageWriterItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

    if (brush().color().alpha() == 0)
        shape.addRect(mBoundingRect);

    return shape;
}
//--------------------------------------------------------------------------------------------------
bool ImageWriterItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::render(DiagramPainter* painter)
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
void ImageWriterItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("OutputImageType", outputImageType());
    xmlWriter.writeAttribute("OutputfilePath", filePath());
    xmlWriter.writeAttribute("Anonymous", Anonymous());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
    xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
    QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("OutputImageType"))
        setOutputImageType(attributes.value("OutputImageType").toString());
    if (attributes.hasAttribute("OutputfilePath"))
        setOutputfilePath(attributes.value("OutputfilePath").toString());
    if (attributes.hasAttribute("Anonymous"))
        setAnonymous(attributes.value("Anonymous").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
        setFont(fontFromString(attributes.value("font").toString()));
    if (attributes.hasAttribute("textBrush"))
        setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ImageWriterItem::propertiesDialog(QWidget* parent) const
{
    if (parent == NULL) parent = diagram();
    return new ImageWriterItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------

void ImageWriterItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
    markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ImageWriterItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ImageWriterItem::changedEvent(Reason reason, const QVariant& value)
{
    Q_UNUSED(value);

    if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ImageWriterItem::orientedTextAngle() const
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
ImageWriterItemDialog::ImageWriterItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
    QSizeF diagramSize = diagramSizeFromParent(parent);

    fontToolBar = new DiagramFontToolBar(
                DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();

    outputfilePathEdit = new QLineEdit();
    anonymousCheck = new QCheckBox();
    anonymousCheck->setText("Anonymous");
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
    OutputImageCombo = new QComboBox();
    OutputImageCombo->addItem("Analyze");
    OutputImageCombo->addItem("DICOM");
    OutputImageCombo->addItem("NIfTI");
    OutputImageCombo->addItem("XMLREC");
    QGroupBox* ImageWriterGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Output Image Type: ", OutputImageCombo);
    pLayout->addRow("Output File Path: ", outputfilePathEdit);
    pLayout->addRow(anonymousCheck);
    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    ImageWriterGroup->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(ImageWriterGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
    captionEdit->setFocus();
    setWindowTitle("ImageWriter  Properties");
    resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ImageWriterItemDialog::~ImageWriterItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ImageWriterItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
    if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
    if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

    if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
    if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
    if (properties.contains("Text Brush"))
        fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("OutputImageType"))//OutputImageCombo->setCurrentText(properties["OutputImageType"].toString());
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("Analyze",0);
        combo_text_hash.insert("DICOM",1);
        combo_text_hash.insert("NIfTI",2);
        combo_text_hash.insert("XMLREC",3);
        QString combo_text = properties["OutputImageType"].toString();
        OutputImageCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    if (properties.contains("OutputfilePath")) outputfilePathEdit->setText(properties["OutputfilePath"].toString());
    if (properties.contains("Anonymous"))
        anonymousCheck->setChecked(properties["Anonymous"].toBool());
    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
    if (pointPositions.size() >= 2)
    {
        topLeftLayout->setPosition(pointPositions[0]);
        bottomRightLayout->setPosition(pointPositions[1]);
    }
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ImageWriterItemDialog::properties() const
{
    QHash<QString, QVariant> properties;
    properties["Pen"] = QVariant(penGroup->pen());
    properties["Brush"] = QVariant(brushGroup->brush());
    properties["Caption"] = QVariant(captionEdit->toPlainText());
    properties["Font"] = QVariant(fontToolBar->font());
    properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["OutputfilePath"] = QVariant(outputfilePathEdit->text());
    properties["OutputImageType"] = QVariant(OutputImageCombo->currentText());
    properties["Anonymous"] = QVariant(anonymousCheck->isChecked());
    return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ImageWriterItemDialog::pointPositions() const
{
    QList<QPointF> pointPositions;

    pointPositions.append(topLeftLayout->position());
    pointPositions.append(bottomRightLayout->position());

    return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItemDialog::accept()
{
    if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ImageWriterItemDialog::hideEvent(QHideEvent* event)
{
    QDialog::hideEvent(event);
    captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
