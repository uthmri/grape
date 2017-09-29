/* DiagramTextItem.cpp
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

#include <DiagramTextItem.h>
#include <Diagram.h>
#include <DiagramItemPoint.h>

DiagramTextItem::DiagramTextItem() : DiagramItem()
{
	QFont font("Arial");
	font.setPointSizeF(100.0);
	Qt::Alignment alignment = Qt::AlignHCenter | Qt::AlignVCenter;
	QBrush brush(QColor(0, 0, 0));

	addProperty("Caption", QVariant(QString()));
	addProperty("Font", QVariant(font));
	addProperty("Alignment", QVariant((quint32)alignment));
	addProperty("Text Brush", QVariant(brush));

	setFlags(CanMove | CanRotate | CanFlip | MatchUnitsWithParent);
	setPlaceType(PlaceMouseUp);

	addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Control, 0));
}
//--------------------------------------------------------------------------------------------------
DiagramTextItem::DiagramTextItem(const DiagramTextItem& item) : DiagramItem(item) { }
//--------------------------------------------------------------------------------------------------
DiagramTextItem::~DiagramTextItem() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItem* DiagramTextItem::copy() const
{
	return new DiagramTextItem(*this);
}
//--------------------------------------------------------------------------------------------------
QString DiagramTextItem::uniqueKey() const
{
	return "text";
}
//--------------------------------------------------------------------------------------------------
QString DiagramTextItem::iconPath() const
{
	return ":/icons/oxygen/draw-text.png";
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::setCaption(const QString& caption)
{
	setPropertyValue("Caption", QVariant(caption));
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::setFont(const QFont& font)
{
	setPropertyValue("Font", QVariant(font));
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::setAlignment(Qt::Alignment alignment)
{
	setPropertyValue("Alignment", QVariant((quint32)alignment));
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::setBrush(const QBrush& brush)
{
	setPropertyValue("Text Brush", QVariant(brush));
}
//--------------------------------------------------------------------------------------------------
QString DiagramTextItem::caption() const
{
	return propertyValue("Caption").toString();
}
//--------------------------------------------------------------------------------------------------
QFont DiagramTextItem::font() const
{
	return propertyValue("Font").value<QFont>();
}
//--------------------------------------------------------------------------------------------------
Qt::Alignment DiagramTextItem::alignment() const
{
	return (Qt::Alignment)propertyValue("Alignment").toUInt();
}
//--------------------------------------------------------------------------------------------------
QBrush DiagramTextItem::brush() const
{
	return propertyValue("Text Brush").value<QBrush>();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::markDirty()
{
	mBoundingRect = QRectF();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QRectF DiagramTextItem::boundingRect() const
{
	return mBoundingRect;
}
//--------------------------------------------------------------------------------------------------
QPainterPath DiagramTextItem::shape() const
{
	QPainterPath mainPath;

	// Main path
	mainPath.addRect(mBoundingRect);

	// Points path
	mainPath.setFillRule(Qt::WindingFill);
	if (isSelected())
	{
		QList<DiagramItemPoint*> lPoints = points();
		QList<DiagramItemPoint*>::ConstIterator pointIter;

		for(pointIter = lPoints.begin(); pointIter != lPoints.end(); pointIter++)
			mainPath.addRect((*pointIter)->boundingRect().translated((*pointIter)->position()));
	}

	return mainPath;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramTextItem::centerPosition() const
{
	return QPointF(0.0, 0.0);
}
//--------------------------------------------------------------------------------------------------
bool DiagramTextItem::isSuperfluous() const
{
	return (caption() == "");
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::render(DiagramPainter* painter)
{
	QFont lFont = font();
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);
	qreal deviceFactor = 1.0;
	QBrush lBrush = brush();

	if (flags() & UsePalette)
	{
		QColor color;

		if (isSelected()) lBrush = painter->outputBrush(DiagramPainter::Item1);
		else lBrush = painter->outputBrush(DiagramPainter::Item0);

		color = lBrush.color();
		color.setAlpha(brush().color().alpha());
		lBrush.setColor(color);
	}

	if (painter->paintEngine()->paintDevice())
		deviceFactor = 96.0 / painter->paintEngine()->paintDevice()->logicalDpiX();
	lFont.setPointSizeF(lFont.pointSizeF() * 0.72 / scaleFactor);       // Scale to workspace
	lFont.setPointSizeF(lFont.pointSizeF() * deviceFactor);             // Scale to device

	if (!mBoundingRect.isValid()) updateLabel(lFont, painter->device());

	painter->setFont(lFont);
	painter->setForItem(QPen(lBrush, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin), lBrush);
	painter->rotate(orientedTextAngle());
	painter->scale(scaleFactor, scaleFactor);
	//painter->drawText(mTextRect, Qt::AlignCenter, caption());
	painter->drawText(mTextRect, alignment(), caption());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items)
{
	DiagramItem::writeXmlAttributes(xmlWriter, items);

	xmlWriter.writeAttribute("caption", caption());
	xmlWriter.writeAttribute("font", fontToString(font()));
	xmlWriter.writeAttribute("brush", brushToString(brush()));
	xmlWriter.writeAttribute("alignment", QString::number((quint32)alignment()));
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items)
{
	QXmlStreamAttributes attributes = xmlReader.attributes();

	DiagramItem::readXmlAttributes(xmlReader, items);

	if (attributes.hasAttribute("caption"))
		setCaption(attributes.value("caption").toString());
	if (attributes.hasAttribute("font"))
		setFont(fontFromString(attributes.value("font").toString()));
	if (attributes.hasAttribute("brush"))
		setBrush(brushFromString(attributes.value("brush").toString()));
	if (attributes.hasAttribute("alignment"))
		setAlignment((Qt::Alignment)attributes.value("alignment").toString().toUInt());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
DiagramItemPropertiesDialog* DiagramTextItem::propertiesDialog(QWidget* parent) const
{
	DiagramTextItemDialog::Flags dialogFlags = DiagramTextItemDialog::ShowPosition;
	if (flags() & UsePalette) dialogFlags |= DiagramTextItemDialog::ItemUsesPalette;

	if (parent == NULL) parent = diagram();

	return new DiagramTextItemDialog(parent, dialogFlags);
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::rotateEvent(const QPointF& diagramPos)
{
	DiagramItem::rotateEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::rotateBackEvent(const QPointF& diagramPos)
{
	DiagramItem::rotateBackEvent(diagramPos);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::flipEvent(const QPointF& diagramPos)
{
	DiagramItem::flipEvent(diagramPos);
	Qt::Alignment align = alignment();

	if (align & Qt::AlignLeft)
	{
		align &= ~(Qt::AlignLeft);
		align |= Qt::AlignRight;
	}
	else if (align & Qt::AlignRight)
	{
		align &= ~(Qt::AlignRight);
		align |= Qt::AlignLeft;
	}

	setAlignment(align);
	markDirty();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
QVariant DiagramTextItem::aboutToChangeEvent(Reason reason, const QVariant& value)
{
	if (reason == UnitsChange)
	{
		DiagramUnits newUnits = (DiagramUnits)value.value<quint16>();
		qreal scaleFactor = unitsScale(units(), newUnits);
		QFont lFont = font();

		lFont.setPointSizeF(lFont.pointSizeF() * scaleFactor);
		setFont(lFont);
	}

	return value;
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::changedEvent(Reason reason, const QVariant& value)
{
	Q_UNUSED(value);

	if (reason == UnitsChange || reason == PropertyChange) markDirty();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
bool DiagramTextItem::createEvent(Diagram* diagram)
{
	bool createItem = false;
	QList<QPointF> pointPositions;
	for(int i = 0; i < numberOfPoints(); i++)
		pointPositions.append(mapToDiagram(point(i)->position()));

	DiagramTextItemDialog::Flags dialogFlags = DiagramTextItemDialog::NoFlags;
	if (flags() & UsePalette) dialogFlags |= DiagramTextItemDialog::ItemUsesPalette;

	DiagramItemPropertiesDialog* dialog = new DiagramTextItemDialog(diagram, dialogFlags);
	dialog->setProperties(properties());
	dialog->setPosition(position());
	dialog->setPointPositions(pointPositions);

	createItem = (dialog->exec() == QDialog::Accepted);
	if (createItem)
	{
		setProperties(dialog->properties());
		setPosition(dialog->position());

		pointPositions = dialog->pointPositions();
		for(int i = 0; i < numberOfPoints() && i < pointPositions.size(); i++)
			point(i)->setPosition(mapFromDiagram(pointPositions[i]));
	}

	delete dialog;
	return createItem;
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItem::updateLabel(const QFont& font, QPaintDevice* device)
{
	Qt::Alignment align = alignment();
	qreal scaleFactor = 1.0 / unitsScale(units(), UnitsMils);

	// Update mTextRect
	mTextRect = QRectF(QPointF(0.0, 0.0), textSize(caption(), font, device));

	if (align & Qt::AlignLeft) mTextRect.translate(0.0, 0.0);
	else if (align & Qt::AlignRight) mTextRect.translate(-mTextRect.width(), 0.0);
	else mTextRect.translate(-mTextRect.width() / 2, 0.0);

	if (align & Qt::AlignBottom) mTextRect.translate(0.0, -mTextRect.height());
	else if (align & Qt::AlignTop) mTextRect.translate(0.0, 0.0);
	else mTextRect.translate(0.0, -mTextRect.height() / 2);

	// Update mBoundingRect
	mBoundingRect = rotateRect(QRectF(
		QPointF(mTextRect.left() * scaleFactor, mTextRect.top() * scaleFactor),
		QPointF(mTextRect.right() * scaleFactor, mTextRect.bottom() * scaleFactor)),
		rotationAngle());
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
qreal DiagramTextItem::orientedTextAngle() const
{
	//bool rotated = (rotationAngle() == 90.0 || rotationAngle() == 270.0);
	qreal textAngle = rotationAngle();

	if (isFlipped())
	{
		if (textAngle == 90) textAngle = 270;
		else if (textAngle == 270) textAngle = 90;
	}

	return textAngle;
}
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
//==================================================================================================
//--------------------------------------------------------------------------------------------------
DiagramTextItemDialog::DiagramTextItemDialog(QWidget* parent, Flags flags) :
	DiagramItemPropertiesDialog(parent)
{
	DiagramFontToolBar::Capabilities fontToolBarCapabilities;

	mFlags = flags;
	mDiagramSize = diagramSizeFromParent(parent);

	fontToolBarCapabilities = (mFlags & ItemUsesPalette) ?
		DiagramFontToolBar::ShowFont | DiagramFontToolBar::ShowAlignment : DiagramFontToolBar::ShowAll;

	positionLayout = new DiagramPositionLayout(mDiagramSize);
	fontToolBar = new DiagramFontToolBar(fontToolBarCapabilities, mDiagramSize);
	captionEdit = new QTextEdit();

	QGroupBox* labelGroup = new QGroupBox("Label");
	QFormLayout* fLayout = new QFormLayout();
	fLayout->addRow("Position: ", positionLayout);
	fLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
	fLayout->setLabelAlignment(Qt::AlignLeft | Qt::AlignTop);
	fLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	fLayout->itemAt(0, QFormLayout::LabelRole)->widget()->setMinimumWidth(80);
	labelGroup->setLayout(fLayout);

	QGroupBox* textGroup = new QGroupBox("Text");
	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->addWidget(fontToolBar);
	vLayout->addWidget(captionEdit, 100);
	textGroup->setLayout(vLayout);

    addWidget(labelGroup);
    addWidget(textGroup);
    addWidget(new QWidget(), 100);
    addButtonBox();
    layout()->setSpacing(0);

	captionEdit->setFocus();

	if ((mFlags & ShowPosition) == 0) labelGroup->setVisible(false);

	setWindowTitle("Text Properties");
	resize(280, 10);
}
//--------------------------------------------------------------------------------------------------
DiagramTextItemDialog::~DiagramTextItemDialog() { }
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItemDialog::setProperties(const QHash<QString, QVariant>& properties)
{
	if (properties.contains("Caption"))
		captionEdit->setText(properties["Caption"].toString());
	if (properties.contains("Font"))
		fontToolBar->setFont(properties["Font"].value<QFont>());
	if (properties.contains("Alignment"))
		fontToolBar->setAlignment((Qt::Alignment)properties["Alignment"].toUInt());
	if (properties.contains("Text Brush"))
		fontToolBar->setBrush(properties["Text Brush"].value<QBrush>());
	captionEdit->selectAll();
}
//--------------------------------------------------------------------------------------------------
void DiagramTextItemDialog::setPosition(const QPointF& position)
{
	positionLayout->setPosition(position);
}
//--------------------------------------------------------------------------------------------------
QHash<QString, QVariant> DiagramTextItemDialog::properties() const
{
	QHash<QString, QVariant> properties;

	properties["Caption"] = QVariant(captionEdit->document()->toPlainText());
	properties["Font"] = QVariant(fontToolBar->font());
	properties["Alignment"] = QVariant(fontToolBar->alignment());
	properties["Text Brush"] = QVariant(fontToolBar->brush());

	return properties;
}
//--------------------------------------------------------------------------------------------------
QPointF DiagramTextItemDialog::position() const
{
	return positionLayout->position();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItemDialog::accept()
{
	if (!captionEdit->document()->toPlainText().isEmpty()) DiagramItemPropertiesDialog::accept();
}
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void DiagramTextItemDialog::hideEvent(QHideEvent* event)
{
	QDialog::hideEvent(event);
	captionEdit->setFocus();
}
//--------------------------------------------------------------------------------------------------
