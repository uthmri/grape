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

#include <SelectItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>
#include <QListWidget>
#include <qfile.h>

//--------------------------------------------------------------------------------------------------
SelectItem::SelectItem(const NodeInterface &NI) : NodeItem(NI)
{
    addProperty("DataList", QVariant(QString("")));
}
//--------------------------------------------------------------------------------------------------
SelectItem::SelectItem(const SelectItem& item) : NodeItem(item) { }
//--------------------------------------------------------------------------------------------------
SelectItem::~SelectItem() { }
//--------------------------------------------------------------------------------------------------
DiagramItem* SelectItem::copy() const
{
    return new SelectItem(this->getNi());
}
//--------------------------------------------------------------------------------------------------
QString SelectItem::uniqueKey() const
{
    return "Select";
}
//--------------------------------------------------------------------------------------------------
QString SelectItem::iconPath() const
{
    return "";//":icons/oxygen/select.png";
}
//--------------------------------------------------------------------------------------------------
void SelectItem::setCaption(const QString& caption)
{
    setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void SelectItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void SelectItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString SelectItem::caption() const
{
    return propertyValue("Caption").toString();
}

QString SelectItem::caption1()
{
    return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont SelectItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush SelectItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
void SelectItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
QRectF SelectItem::boundingRect() const
{
    return DiagramRoundItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath SelectItem::shape() const
{
    QPainterPath shape = DiagramRoundItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);
	return shape;
}
//--------------------------------------------------------------------------------------------------
bool SelectItem::isSuperfluous() const
{
    return (DiagramRoundItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
QString SelectItem::dataList() const
{
    return propertyValue("DataList").toString();
}
//--------------------------------------------------------------------------------------------------
void SelectItem::setDataList(const QString& dataList)
{
    setPropertyValue("DataList", QVariant(dataList));
}
//--------------------------------------------------------------------------------------------------
void SelectItem::render(DiagramPainter* painter)
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
void SelectItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
    DiagramRoundItem::writeXmlAttributes(xmlWriter, items);

    xmlWriter.writeAttribute("DataList", dataList());

    xmlWriter.writeAttribute("caption", caption());
    xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void SelectItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

    DiagramRoundItem::readXmlAttributes(xmlReader, items);

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
DiagramItemPropertiesDialog* SelectItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
    return new SelectItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
void SelectItem::rotateEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SelectItem::rotateBackEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SelectItem::flipEvent(const QPointF& diagramPos)
{
    DiagramRoundItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void SelectItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
    NodeItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
QVariant SelectItem::aboutToChangeEvent(Reason reason, const QVariant& value)
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
void SelectItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
void SelectItem::updateLabel(const QFont& font, QPaintDevice* device)
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
qreal SelectItem::orientedTextAngle() const
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
SelectItemDialog::SelectItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
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
    dataListWdiget = new QListWidget();
    QGroupBox* selectGroup = new QGroupBox("List");
    QFormLayout* pLayout = new QFormLayout();
    pLayout->addRow("", dataListWdiget);
    QVBoxLayout* buttonLayout = new QVBoxLayout();
    loadListButton = new QPushButton(tr("&Load List"));
    loadListButton->setIconSize(QSize(16, 16));
    loadListButton->setToolTip("Load the list from file");
    loadListButton->setFixedSize(68, 26);
    connect(loadListButton, SIGNAL(clicked()), this, SLOT(loadListFromFile()));
    pLayout->addWidget(loadListButton);
    selectGroup->setLayout(pLayout);
    addWidget(positionGroup);
    addWidget(textGroup);
    addWidget(selectGroup);
    addWidget(penGroup);
    addWidget(brushGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
	captionEdit->setFocus();
    setWindowTitle("Select Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
SelectItemDialog::~SelectItemDialog() { }
//--------------------------------------------------------------------------------------------------
int SelectItemDialog::loadListFromFile()
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

        QListWidgetItem *List_item = new QListWidgetItem;
        List_item->setText((line.simplified()));
        List_item->setFlags(List_item->flags() | Qt::ItemIsEditable);
        dataListWdiget->addItem(List_item);
        count++;
    }
    file.close(); // when your done.
    return 0;
}
//--------------------------------------------------------------------------------------------------
void SelectItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
   QMessageBox msgBox;
	if (properties.contains("Pen")) penGroup->setPen(properties["Pen"].value<QPen>());
	if (properties.contains("Brush")) brushGroup->setBrush(properties["Brush"].value<QBrush>());

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

}
//--------------------------------------------------------------------------------------------------
void SelectItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	if (pointPositions.size() >= 2)
	{
		topLeftLayout->setPosition(pointPositions[0]);
		bottomRightLayout->setPosition(pointPositions[1]);
	}
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> SelectItemDialog::properties() const
{
    QMessageBox msgBox;
	QHash<QString, QVariant> properties;

	properties["Pen"] = QVariant(penGroup->pen());
	properties["Brush"] = QVariant(brushGroup->brush());
	properties["Caption"] = QVariant(captionEdit->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Text Brush"] = QVariant(fontToolBar->brush());
    int N = dataListWdiget->count();
    QString s("");
    for(int ii=0; ii< N; ii++)
    {
        s = s + "{" + dataListWdiget->item(ii)->text() + "}";
    }
    properties["DataList"] = QVariant(s);
	return properties;
}
//--------------------------------------------------------------------------------------------------
QList<QPointF> SelectItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	pointPositions.append(topLeftLayout->position());
	pointPositions.append(bottomRightLayout->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
void SelectItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
void SelectItemDialog::hideEvent(QHideEvent* event)
{

	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
