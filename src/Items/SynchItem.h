/* SynchItem.h
 *
* Copyright (C) 2016 UTHealth MRI Research
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

#ifndef SynchItem_H
#define SynchItem_H

#include <NodeItem.h>

class SynchItem : public NodeItem
{
protected:
    QRectF mBoundingRect;
    QRectF mTextRect;

public:
    SynchItem(const NodeInterface &NI);
    SynchItem(const SynchItem& item);
    ~SynchItem();

    DiagramItem* copy() const;
    virtual QString uniqueKey() const;
    QString iconPath() const;

    // Selectors
    //void setStartValue(const QString& path);
   // void setStep(const QString& path);
   // void setEndValue(const QString& path);
    void setCaption(const QString& caption);
    void setFont(const QFont& font);
    void setTextBrush(const QBrush& brush);
    QString startValue() const;
    QString step() const;
    QString endValue() const;
    QString caption() const;
    QString caption1() ;
    QFont font() const;
    QBrush textBrush() const;

    void markDirty();

    // Description
    QRectF boundingRect() const;
    QPainterPath shape() const;
    bool isSuperfluous() const;

    // Render
    void render(DiagramPainter* painter);

    // Misc
    void writeXmlAttributes(QXmlStreamWriter& xmlWriter, const QList<DiagramItem*>& items);
    void readXmlAttributes(QXmlStreamReader& xmlReader, const QList<DiagramItem*>& items);

    DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
    void rotateEvent(const QPointF& diagramPos);
    void rotateBackEvent(const QPointF& diagramPos);
    void flipEvent(const QPointF& diagramPos);
    void resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos);

    QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
    void changedEvent(Reason reason, const QVariant& value);

    void updateLabel(const QFont& font, QPaintDevice* device);

    qreal orientedTextAngle() const;
};

//==================================================================================================
class SynchItemDialog : public DiagramItemPropertiesDialog
{
    Q_OBJECT

private:
    DiagramFontToolBar* fontToolBar;
    QTextEdit* captionEdit;
//    QTextEdit* inputPathEdit;
   // QLineEdit* startValueEdit;
   // QLineEdit* stepEdit;
   // QLineEdit* endValueEdit;

    DiagramPositionLayout* topLeftLayout;
    DiagramPositionLayout* bottomRightLayout;
    DiagramPenGroup* penGroup;
    DiagramBrushGroup* brushGroup;

public:
    SynchItemDialog(QWidget* parent = NULL);
    ~SynchItemDialog();

    void setProperties(const QHash<QString, QVariant>& properties);
    void setPointPositions(const QList<QPointF>& pointPositions);
    QHash<QString, QVariant> properties() const;
    QList<QPointF> pointPositions() const;

public slots:
    void accept();

private:
    void hideEvent(QHideEvent* event);
};

#endif
