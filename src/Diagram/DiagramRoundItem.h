/* DiagramRoundItem.h
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

#ifndef DIAGRAMROUNDITEM_H
#define DIAGRAMROUNDITEM_H

#include <DiagramBaseItems.h>

class DiagramRoundItem : public DiagramBoxResizeItem
{
public:
    DiagramRoundItem();
    DiagramRoundItem(const DiagramRoundItem& item);
    virtual ~DiagramRoundItem();

	virtual DiagramItem* copy() const;
	virtual QString uniqueKey() const;
	virtual QString iconPath() const;

	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;

	virtual void render(DiagramPainter* painter);

	virtual void writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
	virtual void readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);

	virtual DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
	virtual void resizeEvent(DiagramItemPoint* point, const QPointF& parentPos);
};

//==================================================================================================

class DiagramRoundItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

public:
	enum Flag { NoFlags = 0, ItemUsesPalette = 0x01 };
	Q_DECLARE_FLAGS(Flags, Flag)

private:
	Flags mFlags;

	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
    DiagramRoundItemDialog(QWidget* parent = NULL, Flags flags = NoFlags);
    ~DiagramRoundItemDialog();

	void setProperties(const QHash<QString, QVariant>& properties);
	void setPointPositions(const QList<QPointF>& pointPositions);
	QHash<QString, QVariant> properties() const;
	QList<QPointF> pointPositions() const;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(DiagramRoundItemDialog::Flags)

#endif
