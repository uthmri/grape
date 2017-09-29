/* ImageReformatItem.cpp
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

#include <ImageReformatItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
ImageReformatItem::ImageReformatItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("SlicerType", QVariant(QString("")));
    for(int j=0; j<MAX_DIMENTION_NUM; j++)
        addProperty("Arg"+QString::number(j), QVariant(QString("")));

}
//--------------------------------------------------------------------------------------------------
ImageReformatItem::ImageReformatItem(const ImageReformatItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ImageReformatItem::~ImageReformatItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ImageReformatItem::copy() const
{
    return new ImageReformatItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ImageReformatItem::uniqueKey() const
{
    return "ImageReformat";
}
//--------------------------------------------------------------------------------------------------
QString ImageReformatItem::iconPath() const
{
    return "";//":/icons/oxygen/slicer.png";
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::setSlicerType(const QString& arg1Port)
{
    setPropertyValue("SlicerType", QVariant(arg1Port));
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ImageReformatItem::slicer() const
{
    return propertyValue("SlicerType").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageReformatItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageReformatItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ImageReformatItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ImageReformatItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ImageReformatItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ImageReformatItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
QString ImageReformatItem::ArgValue(int j ) const
{
    return propertyValue("Arg"+QString::number(j)).toString();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::setArgValue(const QString& Arg, int j)
{
    setPropertyValue("Arg"+QString::number(j), QVariant(Arg));
}
//--------------------------------------------------------------------------------------------------
bool ImageReformatItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::render(DiagramPainter* painter)
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
void ImageReformatItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("SlicerType", slicer());
    for(int j=0; j<MAX_DIMENTION_NUM; j++)
        xmlWriter.writeAttribute("Arg"+QString::number(j), ArgValue(j));

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
    xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    for(int j=0; j<MAX_DIMENTION_NUM; j++)
    {
        if (attributes.hasAttribute("Arg"+QString::number(j)) )
        {
            //qDebug()<<" $$$ in reading,  "<<"Arg"+QString::number(j)<<" : "<<attributes.value("Arg"+QString::number(j)).toString();
            setArgValue(attributes.value("Arg"+QString::number(j)).toString(), j);
        }
    }

    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ImageReformatItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new ImageReformatItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------

void ImageReformatItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ImageReformatItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ImageReformatItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ImageReformatItem::orientedTextAngle() const
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
ImageReformatItemDialog::ImageReformatItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
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
//    SlicerCombo = new QComboBox();
//    SlicerCombo->addItem("4Dto2D");
//    SlicerCombo->addItem("3Dto2D");
//    SlicerCombo->addItem("4Dto3D");
    QGroupBox* ImageReformatGroup = new QGroupBox("Parameter");

    QFormLayout* pLabel = new QFormLayout();
    QHBoxLayout* hLabel[2];
    hLabel[0] = new QHBoxLayout();
    hLabel[1] = new QHBoxLayout();
    for(int j=0; j<MAX_DIMENTION_NUM; j++)
    {
        ArgLabel[j] = new QLabel();
        ArgValueEdit[j] = new QLineEdit();
        ArgValueEdit[j]->setText(QString::number(j+1));
        ArgLabel[j]->setText("A" + QString::number(j+1));
        hLabel[0]->addWidget(ArgLabel[j]);
        hLabel[1]->addWidget(ArgValueEdit[j]);
    }
    pLabel->addRow(hLabel[0]);
    ImageReformatGroup->setLayout(pLabel);
    pLabel->addRow(hLabel[1]);
    ImageReformatGroup->setLayout(pLabel);


    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(ImageReformatGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();
    setWindowTitle("ImageReformat  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ImageReformatItemDialog::~ImageReformatItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ImageReformatItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    QString arg;
    for(int j=0; j<MAX_DIMENTION_NUM; j++)
    {
        arg = "Arg" + QString::number(j);
        if (properties.contains(arg)) ArgValueEdit[j]->setText(properties[arg].toString());
    }

    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ImageReformatItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    for(int j=0; j<MAX_DIMENTION_NUM; j++)
        properties["Arg"+QString::number(j)]= QVariant( ArgValueEdit[j]->text() );
    return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ImageReformatItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());
	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ImageReformatItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
