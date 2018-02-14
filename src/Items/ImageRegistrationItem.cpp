/* ImageRegistrationItem.cpp
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

#include <ImageRegistrationItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
ImageRegistrationItem::ImageRegistrationItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("Work Model", QVariant(QString("Estimate & Reslice")));//<<<<<<<<<<<<<<
    addProperty("Registration Method", QVariant(QString("Affine")));
    addProperty("Similarity Measure", QVariant(QString("MI")));//<<<<<<<<<<<<<<
    addProperty("CommandPath", QVariant(QString("/ANTS/")));
//    addProperty("CommandPath2", QVariant(QString("/ANTS/WarpImageMultiTransform")));
    addProperty("Param1", QVariant(QString("")));
    addProperty("Param2", QVariant(QString("")));
    addProperty("OutputPath", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
ImageRegistrationItem::ImageRegistrationItem(const ImageRegistrationItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ImageRegistrationItem::~ImageRegistrationItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ImageRegistrationItem::copy() const
{
    return new ImageRegistrationItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::uniqueKey() const
{
    return "ImageRegistration";
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::iconPath() const
{
    return "";//":/icons/oxygen/reg.png";
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setImageRegistrationType(const QString& imageRegistrationType)
{
    setPropertyValue("Registration Method", QVariant(imageRegistrationType));
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setImageSimilarityMeasure(const QString& imageSimilarityMeasure)
{
    setPropertyValue("Similarity Measure", QVariant(imageSimilarityMeasure));
}
//--------------------------------------------------------------------------------------------------

void ImageRegistrationItem::setRegistrationWorkModel(const QString& registrationWorkModel)
{
    setPropertyValue("Work Model", QVariant(registrationWorkModel));
}

//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setCommandPath(const QString& commandPath1)
{
    setPropertyValue("CommandPath", QVariant(commandPath1));
}
//--------------------------------------------------------------------------------------------------
//void ImageRegistrationItem::setCommandPath2(const QString& commandPath2)
//{
//    setPropertyValue("CommandPath2", QVariant(commandPath2));
//}
//--------------------------------------------------------------------------------------------------
//void ImageRegistrationItem::setFixedImagePort(const QString& fixedImagePort)
//{
//    setPropertyValue("FixedImagePort", QVariant(fixedImagePort));
//}
//--------------------------------------------------------------------------------------------------
//void ImageRegistrationItem::setMovingImagePort(const QString& movingImagePort)
//{
//    setPropertyValue("MovingImagePort", QVariant(movingImagePort));
//}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setParam1(const QString& param1)
{
    setPropertyValue("Param1", QVariant(param1));
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setParam2(const QString& param2)
{
    setPropertyValue("Param2", QVariant(param2));
}
////--------------------------------------------------------------------------------------------------
//void ImageRegistrationItem::setFixedImage(const QString& fixedimage)
//{
//    setPropertyValue("FixedImage", QVariant(fixedimage));
//}
////--------------------------------------------------------------------------------------------------
//void ImageRegistrationItem::setMovingImage(const QString& movingimage)
//{
//    setPropertyValue("MovingImage", QVariant(movingimage));
//}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setOutputPath(const QString& outputPath)
{
    setPropertyValue("OutputPath", QVariant(outputPath));
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::imageRegistrationType() const
{
    return propertyValue("Registration Method").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::imageSimilarityMeasure() const
{
    return propertyValue("Similarity Measure").toString();
}//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::registrationWorkModel() const
{
    return propertyValue("Work Model").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::commandPath() const
{
    return propertyValue("CommandPath").toString();
}
//--------------------------------------------------------------------------------------------------
//QString ImageRegistrationItem::commandPath2() const
//{
//    return propertyValue("CommandPath2").toString();
//}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::param1() const
{
    return propertyValue("Param1").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::param2() const
{
    return propertyValue("Param2").toString();
}
//--------------------------------------------------------------------------------------------------
//QString ImageRegistrationItem::fixedImagePort() const
//{
//    return propertyValue("FixedImagePort").toString();
//}
//--------------------------------------------------------------------------------------------------
//QString ImageRegistrationItem::fixedimage() const
//{
//    return propertyValue("FixedImage").toString();
//}
//--------------------------------------------------------------------------------------------------
//QString ImageRegistrationItem::movingImagePort() const
//{
//    return propertyValue("MovingImagePort").toString();
//}
//--------------------------------------------------------------------------------------------------
//QString ImageRegistrationItem::movingimage() const
//{
//    return propertyValue("MovingImage").toString();
//}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::outputPath() const
{
    return propertyValue("OutputPath").toString();
}
//--------------------------------------------------------------------------------------------------
QString ImageRegistrationItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString ImageRegistrationItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ImageRegistrationItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ImageRegistrationItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ImageRegistrationItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ImageRegistrationItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);
	return shape;
}
//--------------------------------------------------------------------------------------------------
bool ImageRegistrationItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::render(DiagramPainter* painter)
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
void ImageRegistrationItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("WorkModel", registrationWorkModel());
    xmlWriter.writeAttribute("RegistrationMethod", imageRegistrationType());
    xmlWriter.writeAttribute("SimilarityMeasure", imageSimilarityMeasure());
    xmlWriter.writeAttribute("CommandPath", commandPath());
//    xmlWriter.writeAttribute("CommandPath2", commandPath2());
    xmlWriter.writeAttribute("Param1", param1());
    xmlWriter.writeAttribute("Param2", param2());
    xmlWriter.writeAttribute("OutputPath", outputPath());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("WorkModel"))
        setRegistrationWorkModel(attributes.value("WorkModel").toString());
    if (attributes.hasAttribute("RegistrationMethod"))
        setImageRegistrationType(attributes.value("RegistrationMethod").toString());
    if (attributes.hasAttribute("SimilarityMeasure"))
        setImageSimilarityMeasure(attributes.value("SimilarityMeasure").toString());
    if (attributes.hasAttribute("CommandPath"))
        setCommandPath(attributes.value("CommandPath").toString());
//    if (attributes.hasAttribute("CommandPath2"))
//        setCommandPath2(attributes.value("CommandPath2").toString());
    if (attributes.hasAttribute("Param1"))
        setParam1(attributes.value("Param1").toString());
    if (attributes.hasAttribute("Param2"))
        setParam2(attributes.value("Param2").toString());
    //if (attributes.hasAttribute("FixedImagePort"))
    //        setFixedImagePort(attributes.value("FixedImagePort").toString());
    //if (attributes.hasAttribute("FixedImage"))
    //    setFixedImage(attributes.value("FixedImage").toString());
    //if (attributes.hasAttribute("MovingImagePort"))
    //   setMovingImagePort(attributes.value("MovingImagePort").toString());
    //if (attributes.hasAttribute("MovingImage"))
    //    setMovingImage(attributes.value("MovingImage").toString());
    if (attributes.hasAttribute("OutputPath"))
        setOutputPath(attributes.value("OutputPath").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ImageRegistrationItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new ImageRegistrationItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ImageRegistrationItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ImageRegistrationItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ImageRegistrationItem::orientedTextAngle() const
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
ImageRegistrationItemDialog::ImageRegistrationItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
//    inputPathEdit = new QTextEdit();
    commandPathEdit = new QLineEdit();
//    commandPath2Edit = new QLineEdit();
    param1Edit = new QLineEdit();
    param2Edit = new QLineEdit();
    fixedimageEdit = new QLineEdit();
    movingimageEdit = new QLineEdit();
    outputPathEdit = new QLineEdit();

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

    workModelCombo = new QComboBox();
    workModelCombo->addItem("Estimate");
    workModelCombo->addItem("Reslice");
    workModelCombo->addItem("Estimate & Reslice");

    imageregistrationTypeCombo = new QComboBox();
    imageregistrationTypeCombo->addItem("Affine");
    imageregistrationTypeCombo->addItem("Rigid-Body");
    imageregistrationTypeCombo->addItem("Elastic");
    imageregistrationTypeCombo->addItem("Diffeomorphic");

    similarityMeasureCombo = new QComboBox();
    similarityMeasureCombo->addItem("MI");
    similarityMeasureCombo->addItem("PR");
    similarityMeasureCombo->addItem("CC");
    similarityMeasureCombo->addItem("MSQ");
    similarityMeasureCombo->addItem("PSE");


//    fixedImagePortCombo = new QComboBox();
//    fixedImagePortCombo->addItem("I1");
//    fixedImagePortCombo->addItem("I2");
//    fixedImagePortCombo->addItem("fixed");

//    movingImagePortCombo = new QComboBox();
//    movingImagePortCombo->addItem("I1");
//    movingImagePortCombo->addItem("I2");
//    movingImagePortCombo->addItem("fixed");


    QGroupBox* imageregistrationGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Work Model: ", workModelCombo);
    pLayout->addRow("Registration Method: ", imageregistrationTypeCombo);
    pLayout->addRow("Similarity Fcn: ", similarityMeasureCombo);
    pLayout->addRow("CommandPath: ", commandPathEdit);
    //pLayout->addRow("FixedImagePort: ", fixedImagePortCombo);
    //pLayout->addRow("FixedImage: ", fixedimageEdit);
    //pLayout->addRow("MovingImagePort: ", movingImagePortCombo);
    //pLayout->addRow("MovingImage: ", movingimageEdit);
    pLayout->addRow("Param1: ", param1Edit);
    pLayout->addRow("Param2: ", param2Edit);
    pLayout->addRow("OutputPath: ", outputPathEdit);

    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    imageregistrationGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(imageregistrationGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("ImageRegistration  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ImageRegistrationItemDialog::~ImageRegistrationItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
    QHash<QString, int> combo_text_hash;
    QString combo_text;

    if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("Work Model"))
    {
        combo_text_hash.clear();
        combo_text_hash.insert("Estimate",0);
        combo_text_hash.insert("Reslice",1);
        combo_text_hash.insert("Estimate & Reslice",2);
        combo_text = properties["Work Model"].toString();
        workModelCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    if (properties.contains("Registration Method"))
    {
        combo_text_hash.clear();
        combo_text_hash.insert("Affine",0);
        combo_text_hash.insert("Rigid-Body",1);
        combo_text_hash.insert("Elastic",2);
        combo_text_hash.insert("Diffeomorphic",3);
        combo_text = properties["Registration Method"].toString();
        imageregistrationTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    if (properties.contains("Similarity Measure"))
    {
        combo_text_hash.clear();
        combo_text_hash.insert("MI",0);
        combo_text_hash.insert("PR",1);
        combo_text_hash.insert("CC",2);
        combo_text_hash.insert("MSQ",3);
        combo_text_hash.insert("PSE",4);
        combo_text = properties["Similarity Measure"].toString();
        similarityMeasureCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    if (properties.contains("CommandPath"))
        commandPathEdit->setText(properties["CommandPath"].toString());

//    if (properties.contains("CommandPath2"))
//        commandPath2Edit->setText(properties["CommandPath2"].toString());

//    if (properties.contains("MovingImagePort"))
//    {
//        combo_text_hash.clear();
//        combo_text_hash.insert("I1",0);
//        combo_text_hash.insert("I2",1);
//        combo_text_hash.insert("fixed",2);
//        combo_text = properties["MovingImagePort"].toString();
//        movingImagePortCombo->setCurrentIndex(combo_text_hash[combo_text]);
//    }
//    if (properties.contains("MovingImage"))
//        movingimageEdit->setText(properties["MovingImage"].toString());

//    if (properties.contains("FixedImagePort"))
//    {
//        combo_text_hash.clear();
//        combo_text_hash.insert("I1",0);
//        combo_text_hash.insert("I2",1);
//        combo_text_hash.insert("fixed",2);
//        combo_text = properties["FixedImagePort"].toString();
//        fixedImagePortCombo->setCurrentIndex(combo_text_hash[combo_text]);
//    }
//    if (properties.contains("FixedImage"))
//        fixedimageEdit->setText(properties["FixedImage"].toString());
    if (properties.contains("Param1"))
        param1Edit->setText(properties["Param1"].toString());
    if (properties.contains("Param2"))
        param2Edit->setText(properties["Param2"].toString());
    if (properties.contains("OutputPath"))
        outputPathEdit->setText(properties["OutputPath"].toString());

    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ImageRegistrationItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["Work Model"] = QVariant(workModelCombo->currentText());
    properties["Registration Method"] = QVariant(imageregistrationTypeCombo->currentText());
    properties["Similarity Measure"] = QVariant(similarityMeasureCombo->currentText());
    properties["CommandPath"] = QVariant(commandPathEdit->text());
//    properties["CommandPath2"] = QVariant(commandPath2Edit->text());
    //properties["FixedImagePort"] = QVariant(fixedImagePortCombo->currentText());
    //properties["MovingImagePort"] = QVariant(movingImagePortCombo->currentText());
    properties["Param1"] = QVariant(param1Edit->text());
    properties["Param2"] = QVariant(param2Edit->text());
    //properties["FixedImage"] = QVariant(fixedimageEdit->text());
    //properties["MovingImage"] = QVariant(movingimageEdit->text());
    properties["OutputPath"] = QVariant(outputPathEdit->text());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ImageRegistrationItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ImageRegistrationItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
