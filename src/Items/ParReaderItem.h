/* ParReaderItem.h
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

#ifndef ParReaderITEM_H
#define ParReaderITEM_H

#include <DiagramRoundItem.h>

#include <NodeItem.h>


class ParReaderItem : public NodeItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
    ParReaderItem(const NodeInterface &NI);
    ParReaderItem(const ParReaderItem& item);
    ~ParReaderItem();

	DiagramItem* copy() const;
    virtual QString uniqueKey() const;
	QString iconPath() const;

    // Selectors

    void setInputImageType(const QString& arg1Port);
    void setDataList(const QString& path);
    void setInputfilePath(const QString& inputfilePath);
    void setCaption(const QString& caption);
    void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);

  //  QString inputImageType() const;

   QString inputfilePath() const;
    QString caption() const;
    QString caption1() ;
    QFont font() const;
	QBrush textBrush() const;
    QString dataList() const;
	void markDirty();

	// Description
	QRectF boundingRect() const;
	QPainterPath shape() const;
	bool isSuperfluous() const;

	// Render
	void render(DiagramPainter* painter);

	// Misc
    void writeXmlAttributes(QXmlStreamWriter& ParReader, const QList<DiagramItem*>& items);
    void readXmlAttributes(QXmlStreamReader& ParReader, const QList<DiagramItem*>& items);

	DiagramItemPropertiesDialog* propertiesDialog(QWidget* parent = NULL) const;

protected:
	void rotateEvent(const QPointF& diagramPos);
	void rotateBackEvent(const QPointF& diagramPos);
	void flipEvent(const QPointF& diagramPos);
	void resizeEvent(DiagramItemPoint* itemPoint, const QPointF& parentPos);

	QVariant aboutToChangeEvent(Reason reason, const QVariant& value);
	void changedEvent(Reason reason, const QVariant& value);

	void updateLabel(const QFont& font, QPaintDevice* device);
    void typeChanged(int index);
	qreal orientedTextAngle() const;
};

//==================================================================================================
class ParReaderItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;
    QLineEdit* inputfilePathEdit;
    QPushButton* loadListButton;
   // int ParReaderTypeIndex;

  //  QComboBox* InputImageCombo;
    QListWidget* dataListWdiget;
    QString mPreviousType;
	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
    enum Capability { NoCapabilities = 0x00, ShowValue = 0x01, ShowType = 0x02,ShowAll  = 0x04};
    Q_DECLARE_FLAGS(Capabilities, Capability)

    ParReaderItemDialog(QWidget* parent = NULL);
    ~ParReaderItemDialog();
	void setProperties(const QHash<QString, QVariant>& properties);
	void setPointPositions(const QList<QPointF>& pointPositions);
	QHash<QString, QVariant> properties() const;
	QList<QPointF> pointPositions() const;

public slots:
	void accept();
    int loadListFromFile();

private:
	void hideEvent(QHideEvent* event);
};

#endif
