//--------------------------------------------------------------------------------------------------
/* CurveFittingItem.cpp*/


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

#include <CurveFittingItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
CurveFittingItem::CurveFittingItem(const NodeInterface &NI) : NodeItem(NI)
{    

    addProperty("CurveFittingType", QVariant(QString("T2")));
    for(int j=0; j<MAX_CURVEFITTING_ARG; j++)
        addProperty("Arg"+QString::number(j), QVariant(QString("")));

    addProperty("xData", QVariant(QString("1 2 3 4")));
    addProperty("parValue", QVariant(QString("2")));
}
//--------------------------------------------------------------------------------------------------
CurveFittingItem::CurveFittingItem(const CurveFittingItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
CurveFittingItem::~CurveFittingItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* CurveFittingItem::copy() const
{
    return new CurveFittingItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString CurveFittingItem::uniqueKey() const
{
    return "CurveFitting";
}
//--------------------------------------------------------------------------------------------------
QString CurveFittingItem::iconPath() const
{
    return "";//":/icons/oxygen/bc.png";
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::setCurveFittingType(const QString& CurveFittingType)
{
    setPropertyValue("CurveFittingType", QVariant(CurveFittingType));
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::setArgValue(const QString& Arg, int j)
{
    setPropertyValue("Arg"+QString::number(j), QVariant(Arg));
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString CurveFittingItem::CurveFittingType() const
{
    return propertyValue("CurveFittingType").toString();
}
//--------------------------------------------------------------------------------------------------
QString CurveFittingItem::ArgValue(int j ) const
{
    return propertyValue("Arg"+QString::number(j)).toString();
}
//--------------------------------------------------------------------------------------------------
QString CurveFittingItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString CurveFittingItem::caption1()
{
     return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont CurveFittingItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush CurveFittingItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF CurveFittingItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath CurveFittingItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool CurveFittingItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::render(DiagramPainter* painter)
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
void CurveFittingItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);
    xmlWriter.writeAttribute("CurveFittingType", CurveFittingType());
    for(int j=0; j<MAX_CURVEFITTING_ARG; j++)
        xmlWriter.writeAttribute("Arg"+QString::number(j), ArgValue(j));

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
    xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

    if (attributes.hasAttribute("CurveFittingType"))
        setCurveFittingType(attributes.value("CurveFittingType").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));

    for(int j=0; j<MAX_CURVEFITTING_ARG; j++)
    {
        if (attributes.hasAttribute("Arg"+QString::number(j)) )
        {
            //qDebug()<<" $$$ in reading,  "<<"Arg"+QString::number(j)<<" : "<<attributes.value("Arg"+QString::number(j)).toString();
            setArgValue(attributes.value("Arg"+QString::number(j)).toString(), j);
        }
    }
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* CurveFittingItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new CurveFittingItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant CurveFittingItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void CurveFittingItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal CurveFittingItem::orientedTextAngle() const
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
CurveFittingItemDialog::CurveFittingItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();

    for(int j=0; j<MAX_CURVEFITTING_ARG; j++) ArgValueEdit[j] = new QLineEdit();
    for(int j=0; j<MAX_CURVEFITTING_ARG; j++) ArgLabel[j] = new QLabel();

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

    CurveFittingTypeCombo = new QComboBox();
    CurveFittingTypeCombo->addItem("T2");
    CurveFittingTypeCombo->addItem("T1-IR");
    CurveFittingTypeCombo->addItem("T1-LookLocker");

    connect(this->CurveFittingTypeCombo,SIGNAL(currentIndexChanged(const QString&)),
            this,SLOT(FitModelChange(const QString&)));

    QGroupBox* CurveFittingGroup = new QGroupBox("Parameter");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("Fitting Model: ", CurveFittingTypeCombo);
    QHBoxLayout* hLayout[MAX_CURVEFITTING_ARG/2];
    for(int j=0; j<MAX_CURVEFITTING_ARG/2; j++)
    {
        hLayout[j] = new QHBoxLayout();

        ArgLabel[2*j]->setText("Arg " + QString::number(2*j));
        ArgLabel[2*j+1]->setText("Arg " + QString::number(2*j+1));

        hLayout[j]->addWidget(ArgLabel[2*j]);
        hLayout[j]->addWidget(ArgValueEdit[j*2]);
        hLayout[j]->addWidget(ArgLabel[j*2+1]);
        hLayout[j]->addWidget(ArgValueEdit[j*2+1]);

        pLayout->addRow(hLayout[j]);
    }


    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    pLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    CurveFittingGroup->setLayout(pLayout);

    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(CurveFittingGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();

	captionEdit->setFocus();

    setWindowTitle("CurveFitting  Properties");
	resize(280, 10);

    ArgLabel[0]->setText("TEs [ms]");
    ArgValueEdit[0]->setText("9.5, 90");
    ArgLabel[1]->setText("Model Order");
    ArgValueEdit[1]->setText("2");
    for(int j=2; j<MAX_CURVEFITTING_ARG; j++)
    {
        ArgLabel[j]->setText("-");
        ArgValueEdit[j]->setEnabled(false);
    }



}
//--------------------------------------------------------------------------------------------------
CurveFittingItemDialog::~CurveFittingItemDialog() { }
//--------------------------------------------------------------------------------------------------
void CurveFittingItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());

    if (properties.contains("CurveFittingType"))
    {
        QHash<QString, int> combo_text_hash;
        combo_text_hash.insert("T2",0);
        combo_text_hash.insert("T1-IR",1);
        combo_text_hash.insert("T1-LookLocker",3);

        QString combo_text = properties["CurveFittingType"].toString();
        CurveFittingTypeCombo->setCurrentIndex(combo_text_hash[combo_text]);
    }

    QString arg;
    for(int j=0; j<MAX_CURVEFITTING_ARG; j++)
    {
        arg = "Arg" + QString::number(j);
        if (properties.contains(arg)) ArgValueEdit[j]->setText(properties[arg].toString());
    }
    captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> CurveFittingItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["CurveFittingType"] = QVariant(CurveFittingTypeCombo->currentText());
    for(int j=0; j<MAX_CURVEFITTING_ARG; j++)
        properties["Arg"+QString::number(j)]= QVariant( ArgValueEdit[j]->text() );

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> CurveFittingItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
void CurveFittingItemDialog::FitModelChange(const QString& s)
{
    int narg = 0;

    if(s == "T2")
    {
        narg = 2;
        ArgLabel[0]->setText("TEs [ms]");
        ArgValueEdit[0]->setText("9.5, 90");
        ArgLabel[1]->setText("Model Order");
        ArgValueEdit[1]->setText("2");
    }
    else if(s == "T1-IR")
    {
        narg = 3;
        ArgLabel[0]->setText("TIs [ms]");
        ArgValueEdit[0]->setText("120,150,200,400,800,1200,1600");
        ArgLabel[1]->setText("TR [ms]");
        ArgValueEdit[1]->setText("1700");
        ArgLabel[2]->setText("Model Order");
        ArgValueEdit[2]->setText("2");
    }
    else if(s == "T1-LookLocker")
    {
        narg = 10;
        ArgLabel[0]->setText("Tau [ms]");
        ArgValueEdit[0]->setText("6.3");
        ArgLabel[1]->setText("Td [ms]");
        ArgValueEdit[1]->setText("2.4");
        ArgLabel[2]->setText("Tr [ms]");
        ArgValueEdit[2]->setText("2578");
        ArgLabel[3]->setText("Etl");
        ArgValueEdit[3]->setText("8.0");
        ArgLabel[4]->setText("FA [deg]");
        ArgValueEdit[4]->setText("7.0");
        ArgLabel[5]->setText("-");
        ArgValueEdit[5]->setText("");
        ArgLabel[6]->setText("CT1_min [ms]");
        ArgValueEdit[6]->setText("0");
        ArgLabel[7]->setText("CT1_max [ms]");
        ArgValueEdit[7]->setText("10000");
        ArgLabel[8]->setText("CPD_min");
        ArgValueEdit[8]->setText("0");
        ArgLabel[9]->setText("CPD_max");
        ArgValueEdit[9]->setText("8000");
    }
    for(int j=0; j<MAX_CURVEFITTING_ARG; j++)
    {
        if(j < narg)
            ArgValueEdit[j]->setEnabled(true);
        else
        {
            ArgLabel[j]->setText("-");
            ArgValueEdit[j]->setEnabled(false);
            ArgValueEdit[j]->setText("");
        }
        if(s == "T1-LookLocker")
        {
            ArgValueEdit[5]->setEnabled(false);
            ArgValueEdit[5]->setText("");
        }
    }
}
//--------------------------------------------------------------------------------------------------
