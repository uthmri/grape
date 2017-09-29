/* DiagramTextItem.h
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

#ifndef DIAGRAMTEXTITEM_H
#define DIAGRAMTEXTITEM_H

#include <DiagramItem.h>
#include <DiagramWidgets.h>

class DiagramTextItem : public DiagramItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
	DiagramTextItem();
	DiagramTextItem(const DiagramTextItem& item);
	~DiagramTextItem();

	virtual DiagramItem* copy() const;
	virtual QString uniqueKey() const;
	virtual QString iconPath() const;

	// Selectors
	void setCaption(const QString& caption);
	void setFont(const QFont& font);
	void setAlignment(Qt::Alignment alignment);
	void setBrush(const QBrush& brush);
	QString caption() const;
	QFont font() const;
	Qt::Alignment alignment() const;
	QBrush brush() const;

	void markDirty();

	// Description
	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;
	virtual QPointF centerPosition() const;
	virtual bool isSuperfluous() const;

	// Render
	virtual void render(DiagramPainter* painter);

	// Miscellaneous
	virtual void writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	virtual void readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);

	virtual DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
	virtual void rotateEvent(const QPointF& diagramPos);
	virtual void rotateBackEvent(const QPointF& diagramPos);
	virtual void flipEvent(const QPointF& diagramPos);

	virtual QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
	virtual void changedEvent(Reason reason, const QVariant& value);

	virtual bool createEvent(Diagram* diagram);

	void updateLabel(const QFont& font, QPaintDevice* device);

	qreal orientedTextAngle() const;
};

//==================================================================================================

class DiagramTextItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

public:
	enum Flag { NoFlags = 0, ItemUsesPalette = 0x01, ShowPosition = 0x02 };
	Q_DECLARE_FLAGS(Flags, Flag)

private:
	Flags mFlags;
	QSizeF mDiagramSize;

	DiagramPositionLayout* positionLayout;

	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;

public:
	DiagramTextItemDialog(QWidget* parent = NULL, Flags flags = ShowPosition);
	~DiagramTextItemDialog();

	void setProperties(const QHash<QString, QVariant>& properties);
	void setPosition(const QPointF& position);
	QHash<QString, QVariant> properties() const;
	QPointF position() const;

public slots:
	void accept();

private:
	void hideEvent(QHideEvent* event);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramTextItemDialog::Flags)

#endif
