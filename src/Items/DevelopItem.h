/* DevelopItem.h
 *
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



#ifndef DevelopITEM_H
#define DevelopITEM_H

#include <NodeItem.h>


class DevelopItem : public NodeItem
{
protected:
	QRectF mBoundingRect;
	QRectF mTextRect;

public:
    DevelopItem(const NodeInterface &NI);
    DevelopItem(const DevelopItem& item);
    ~DevelopItem();

	DiagramItem* copy() const;
    virtual QString uniqueKey() const;
	QString iconPath() const;

    // Selectors
    void setCommand(const QString& command);
    void setArg1Port(const QString& dataType1);
    void setArg2Port(const QString& dataType2);
    void setArg3Port(const QString& dataType3);
    void setArg4Port(const QString& dataType4);
    void setArg5Port(const QString& dataType5);
    void setArg1(const QString& arg1);
    void setArg2(const QString& arg2);
    void setArg3(const QString& arg3);
    void setArg4(const QString& arg4);
    void setArg5(const QString& arg5);
    void setOutput1Port(const QString& dataPort1);
    void setOutput2Port(const QString& dataPort2);
    void setOutput3Port(const QString& dataPort3);

    void setOutput1(const QString& output1);
    void setOutput2(const QString& output2);
    void setOutput3(const QString& output3);

    void setCaption(const QString& caption);
    void setFont(const QFont& font);
	void setTextBrush(const QBrush& brush);
   QString command() const;
   QString Arg1Port() const;
   QString Arg2Port() const;
   QString Arg3Port() const;
   QString Arg4Port() const;
   QString Arg5Port() const;
   QString arg1() const;
   QString arg2() const;
   QString arg3() const;
   QString arg4() const;
   QString arg5() const;
   QString Output1Port() const;
   QString Output2Port() const;
   QString Output3Port() const;

   QString output1() const;
    QString output2() const;
    QString output3() const;

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
    void typeChanged(int index);
	qreal orientedTextAngle() const;
};

//==================================================================================================
class DevelopItemDialog : public DiagramItemPropertiesDialog
{
	Q_OBJECT

private:
	DiagramFontToolBar* fontToolBar;
	QTextEdit* captionEdit;
    QLineEdit* commandEdit;
    QLineEdit* arg1Edit;
    QLineEdit* arg2Edit;
    QLineEdit* arg3Edit;
    QLineEdit* arg4Edit;
    QLineEdit* arg5Edit;

    QLineEdit* output1Edit;
    QLineEdit* output2Edit;
    QLineEdit* output3Edit;

    int InputTypeIndex;
    int OutputTypeIndex;
    QComboBox* Arg1PortCombo;
    QComboBox* Arg2PortCombo;
    QComboBox* Arg3PortCombo;
    QComboBox* Arg4PortCombo;
    QComboBox* Arg5PortCombo;

    QComboBox* Output1PortCombo;
    QComboBox* Output2PortCombo;
    QComboBox* Output3PortCombo;
    QCheckBox* arrayCheck;
    QString mPreviousPort;
	DiagramPositionLayout* topLeftLayout;
	DiagramPositionLayout* bottomRightLayout;
	DiagramPenGroup* penGroup;
	DiagramBrushGroup* brushGroup;

public:
    enum Capability { NoCapabilities = 0x00, ShowValue = 0x01, ShowType = 0x02,ShowAll  = 0x04};
    Q_DECLARE_FLAGS(Capabilities, Capability)

    DevelopItemDialog(QWidget* parent = NULL);
    ~DevelopItemDialog();
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
