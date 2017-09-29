/* ChartPolygonItem.cpp
 *
 * Copyright (C) 2013 Jason Allen
 *
 * This file is part of the Jade Diagram Editor.
 *
 * Jade is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Jade is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Jade.  If not, see <http://www.gnu.org/licenses/>
 */

#include <ChartPolygonItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

ChartPolygonItem::ChartPolygonItem() : DiagramPolygonItem()
{
	QFont font("Arial");
	font.setPointSizeF(100.0);

	addProperty("Caption", QVariant(QString("Label")));
	addProperty("Font", QVariant(font));
	addProperty("Text Brush", QBrush(QColor(255, 0, 255)));

	setFlags(CanMove | CanRotate | CanFlip | CanResize | CanInsertRemovePoints | MatchUnitsWithParent);
	setPlaceType(PlaceMouseUp);
}
//--------------------------------------------------------------------------------------------------
ChartPolygonItem::ChartPolygonItem(const ChartPolygonItem& item) : DiagramPolygonItem(item) { }
//--------------------------------------------------------------------------------------------------
ChartPolygonItem::~ChartPolygonItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* ChartPolygonItem::copy() const
{
	return new ChartPolygonItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString ChartPolygonItem::uniqueKey() const
{
	return "ChartPolygon";
}
//--------------------------------------------------------------------------------------------------
QString ChartPolygonItem::iconPath() const
{
	return ":/icons/custom/items/chartpolygon.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::setCaption(const QString& caption)
{
	setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::setTextBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString ChartPolygonItem::caption() const
{
	return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont ChartPolygonItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
QBrush ChartPolygonItem::textBrush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF ChartPolygonItem::boundingRect() const
{
	return DiagramPolygonItem::boundingRect().united(mBoundingRect);
}
//--------------------------------------------------------------------------------------------------
QPainterPath ChartPolygonItem::shape() const
{
	QPainterPath shape = DiagramPolygonItem::shape();

	if (brush().color().alpha() == 0)
		shape.addRect(mBoundingRect);

	return shape;
}
//--------------------------------------------------------------------------------------------------
bool ChartPolygonItem::isSuperfluous() const
{
	return (DiagramPolygonItem::isSuperfluous() && caption() == "");
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::render(DiagramPainter* painter)
{
	QFont lFont = font();
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);
	qreal deviceFactor = 1.0;

	DiagramPolygonItem::render(painter);

	if (painter->paintEngine()->paintDevice())
		deviceFactor = 96.0 / painter->paintEngine()->paintDevice()->logicalDpiX();

	lFont.setPointSizeF(lFont.pointSizeF() * 0.72 / scaleFactor);       // Scale to workspace
	lFont.setPointSizeF(lFont.pointSizeF() * deviceFactor);             // Scale to device

	if (!mBoundingRect.isValid()) updateLabel(lFont, painter->device());

	painter->setFont(lFont);
	painter->setForItem(QPen(textBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), textBrush());
	painter->translate(centerPoint());
	painter->rotate(orientedTextAngle());
	painter->scale(scaleFactor, scaleFactor);
	painter->drawText(mTextRect, Qt::AlignCenter, caption());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramPolygonItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("caption", caption());
	xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("textBrush", brushToString(textBrush()));
}
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramPolygonItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("caption"))
		setCaption(attributes.value("caption").toString());
	if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("textBrush"))
		setTextBrush(brushFromString(attributes.value("textBrush").toString()));
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* ChartPolygonItem::propertiesDialog(QWidget* parent) const
{
	if (parent == NULL) parent = diagram();
	return new ChartPolygonItemDialog(parent);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::rotateEvent(const QPointF& diagramPos)
{
	DiagramPolygonItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::rotateBackEvent(const QPointF& diagramPos)
{
	DiagramPolygonItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::flipEvent(const QPointF& diagramPos)
{
	DiagramPolygonItem::flipEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos)
{
	DiagramPolygonItem::resizeEvent(itemPoint, parentPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant ChartPolygonItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	QVariant result = DiagramPolygonItem::aboutToChangeEvent(reason, value);

	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.toInt();
		qreal scaleFactor = unitsScale(units(), newUnits);
		QFont lFont = font();

		lFont.setPointSizeF(lFont.pointSizeF() * scaleFactor);
		setFont(lFont);
	}
	else if (reason == AddNewItemToDiagram) markDirty();

	return result;
}
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItem::updateLabel(const QFont& font, QPaintDevice* device)
{
	QSizeF textSizeF = textSize(caption(), font, device);
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);

	// Update mTextRect
	mTextRect = QRectF(QPointF(-textSizeF.width() / 2, -textSizeF.height() / 2), textSizeF);

	// Update mBoundingRect
	mBoundingRect = rotateRect(QRectF(
		QPointF(mTextRect.left() * scaleFactor, mTextRect.top() * scaleFactor),
		QPointF(mTextRect.right() * scaleFactor, mTextRect.bottom() * scaleFactor)),
		rotationAngle()).translated(centerPoint());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
qreal ChartPolygonItem::orientedTextAngle() const
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
QPointF ChartPolygonItem::centerPoint() const
{
	QPointF centerPoint;
	QList<DiagramItemPoint*> lPoints = points();

	for(QList<DiagramItemPoint*>::Iterator pointIter = lPoints.begin(); pointIter != lPoints.end(); pointIter++)
		centerPoint += (*pointIter)->position();

	if (!lPoints.isEmpty()) centerPoint /= lPoints.size();

	return centerPoint;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
ChartPolygonItemDialog::ChartPolygonItemDialog(QWidget* parent) : DiagramItemPropertiesDialog(parent)
{
	mDiagramSize = diagramSizeFromParent(parent);

	fontToolBar = new DiagramFontToolBar(
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowBrush, mDiagramSize);
	captionEdit = new QTextEdit();

	penGroup = new DiagramPenGroup(DiagramPenGroup::ShowAll, mDiagramSize);
	brushGroup = new DiagramBrushGroup(DiagramBrushGroup::ShowColor);

	QGroupBox* textGroup = new QGroupBox("Text");
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(fontToolBar);
	vLayout->addWidget(captionEdit, 100);
	textGroup->setLayout(vLayout);

	pointsFrame = new QFrame();
	pointFormLayout = new QFormLayout();
	pointsFrame->setLayout(pointFormLayout);

	QScrollArea* scrollWidget = new QScrollArea();
	scrollWidget->setWidget(pointsFrame);
	scrollWidget->setWidgetResizable(true);

	QFrame* generalFrame = new QFrame();
	vLayout = new QVBoxLayout();
	vLayout->addWidget(textGroup);
	vLayout->addWidget(penGroup);
	vLayout->addWidget(brushGroup);
	vLayout->addWidget(new QWidget(), 100);
	generalFrame->setLayout(vLayout);

	QFrame* pointFrame = new QFrame();
	vLayout = new QVBoxLayout();
	vLayout->addWidget(scrollWidget, 100);
	pointFrame->setLayout(vLayout);

	QTabWidget* tabWidget = new QTabWidget();
	tabWidget->addTab(generalFrame, "General");
	tabWidget->addTab(pointFrame, "Points");

    addWidget(tabWidget);
    addButtonBox();

	captionEdit->setFocus();

	setWindowTitle("Chart Polygon Properties");
	resize(320, 10);
}
//--------------------------------------------------------------------------------------------------
ChartPolygonItemDialog::~ChartPolygonItemDialog() { }
//--------------------------------------------------------------------------------------------------
void ChartPolygonItemDialog::setProperties(const QHash<QString, QVariant>& properties)
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
void ChartPolygonItemDialog::setPointPositions(const QList<QPointF>& pointPositions)
{
	DiagramPositionLayout* positionLayout;

	pointPositionLayouts.clear();
	delete pointFormLayout;
	pointFormLayout = new QFormLayout();

	for(qint32 i = 0; i < pointPositions.size(); i++)
	{
		positionLayout = new DiagramPositionLayout(mDiagramSize);
		positionLayout->setPosition(pointPositions[i]);
		pointPositionLayouts.append(positionLayout);

		if (i == 0) pointFormLayout->addRow("First Point:", positionLayout);
		else if (i == pointPositions.size() - 1) pointFormLayout->addRow("Last Point:", positionLayout);
		else pointFormLayout->addRow("", positionLayout);
	}

	pointFormLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	pointFormLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	pointFormLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	pointFormLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	pointsFrame->setLayout(pointFormLayout);
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> ChartPolygonItemDialog::properties() const
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
QList<QPointF> ChartPolygonItemDialog::pointPositions() const
{
	QList<QPointF> pointPositions;

	for(int i = 0; i < pointPositionLayouts.size(); i++)
		pointPositions.append(pointPositionLayouts[i]->position());

	return pointPositions;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void ChartPolygonItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
