/* ParReaderItem.cpp
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

#include <ParReaderItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <DiagramWidgets.h>

//--------------------------------------------------------------------------------------------------
ParReaderItem::ParReaderItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("InputfilePath", QVariant(QString("")));
    addProperty("DataList", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
ParReaderItem::ParReaderItem(const ParReaderItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
ParReaderItem::~ParReaderItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* ParReaderItem::copy() const
{
    return new ParReaderItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString ParReaderItem::uniqueKey() const
{
    return "ParReader";
}
//--------------------------------------------------------------------------------------------------
QString ParReaderItem::iconPath() const
{
    return "";//":/icons/oxygen/IR.png";
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::setInputfilePath(const QString& inputfilePath)
{
    setPropertyValue("InputfilePath", QVariant(inputfilePath));
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ParReaderItem::inputfilePath() const
{
    return propertyValue("InputfilePath").toString();
}
//--------------------------------------------------------------------------------------------------
QString ParReaderItem::caption() const
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QString ParReaderItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ParReaderItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QString ParReaderItem::dataList() const
{
    return propertyValue("DataList").toString();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::setDataList(const QString& dataList)
{
    setPropertyValue("DataList", QVariant(dataList));
}

//--------------------------------------------------------------------------------------------------
QBrush ParReaderItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF ParReaderItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ParReaderItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool ParReaderItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::render(DiagramPainter* painter)
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
void ParReaderItem::writeXmlAttributes(QXmlStreamWriter& xmlReader, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlReader, items);

    xmlReader.writeAttribute("InputfilePath", inputfilePath());
    xmlReader.writeAttribute("DataList", dataList());

    xmlReader.writeAttribute("caption", caption());
    xmlReader.writeAttribute("font", fontToString(font()));
    xmlReader.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
    QXmlStreamAttributes attributes = xmlReader.attributes();
    DiagramRoundItem::readXmlAttributes(xmlReader, items);
    if (attributes.hasAttribute("InputfilePath"))
        setInputfilePath(attributes.value("InputfilePath").toString());
    if (attributes.hasAttribute("DataList"))
        setDataList(attributes.value("DataList").toString());
    if (attributes.hasAttribute("caption"))
        setCaption(attributes.value("caption").toString());
    if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ParReaderItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new ParReaderItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant ParReaderItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void ParReaderItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal ParReaderItem::orientedTextAngle() const
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
ParReaderItemDialog::ParReaderItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	QSizeF diagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, diagramSize);
    captionEdit = new QTextEdit();
    inputfilePathEdit = new QLineEdit();
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

    dataListWdiget = new QListWidget();
    QGroupBox* ParReaderGroup = new QGroupBox("List");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("", dataListWdiget);
    loadListButton = new QPushButton(tr("&Load List"));
    loadListButton->setIconSize(QSize(16, 16));
    loadListButton->setToolTip("Load the list from file");
    loadListButton->setFixedSize(68, 26);
    connect(loadListButton, SIGNAL(clicked()), this, SLOT(loadListFromFile()));
    pLayout->addWidget(loadListButton);
    ParReaderGroup->setLayout(pLayout);
    pLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    pLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    QGroupBox* parGroup = new QGroupBox("Parameter");
    QFormLayout* ppLayout = new QFormLayout();
    ppLayout->addRow("Input File Path: ", inputfilePathEdit);
    ppLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    ppLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ppLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    ppLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(20);
    parGroup->setLayout(ppLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(ParReaderGroup);
    addWidget(parGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();

    setWindowTitle("ParReader  Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
ParReaderItemDialog::~ParReaderItemDialog() { }
//--------------------------------------------------------------------------------------------------
int ParReaderItemDialog::loadListFromFile()
{
   QMessageBox msgBox;
   dataListWdiget->clear();

    QString filename = QFileDialog::getOpenFileName(
        this,
        tr("Load List"),
        QDir::currentPath(),
        tr("Document files (*.txt);;All files (*.*)") );
    if( filename.isNull() )
    {
        return -1;
    }

    QString s("");
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString line;
    int count=0;
    while( !file.atEnd() )
    {
        line = file.readLine();
        QListWidgetItem *item = new QListWidgetItem;
        item->setText((line.simplified()));
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        dataListWdiget->addItem(item);
        count++;
    }
    file.close(); // when your done.
    return 0;
}
//--------------------------------------------------------------------------------------------------
void ParReaderItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());
    if (properties.contains("InputfilePath")) inputfilePathEdit->setText(properties["InputfilePath"].toString());
	if (properties.contains("Caption")) captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font")) fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
    if (properties.contains("DataList"))
    {
        QString text, s1;
        text = properties["DataList"].toString();
        dataListWdiget->clear();
        QRegExp rx("\\{*\\}");    // primitive floating point matching
        int count = 0;
        int pos = 0, newpos = 0;
        pos = 0;
        while ((newpos = rx.indexIn(text, pos)) != -1)
        {
            ++count;
            s1 = text.mid(pos+1, newpos-pos-1);
            pos = newpos + 1;//+= rx.matchedLength();
            QListWidgetItem *item = new QListWidgetItem;
            item->setText(s1);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            dataListWdiget->addItem(item);
        }
    }
    captionEdit->selectAll();
    inputfilePathEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ParReaderItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    properties["InputfilePath"] = QVariant(inputfilePathEdit->text());
   // properties["InputImageType"] = QVariant(InputImageCombo->currentText());
    int N = dataListWdiget->count();
    //msgBox.setText(" SelectItemDialog::properties() is "+N ); msgBox.exec();
    QString s("");
    for(int ii=0; ii< N; ii++)
    {
        s = s + "{" + dataListWdiget->item(ii)->text() + "}";
    }
    properties["DataList"] = QVariant(s);

	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> ParReaderItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void ParReaderItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void ParReaderItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
