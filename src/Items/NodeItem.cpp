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
* along with GRAPE.  If not, see <http://www.gnu.org/licenses/>*/


#include "NodeItem.h"
#include <DiagramItemPoint.h>

//--------------------------------------------------------------------------------------------------
NodeItem::NodeItem() : DiagramRoundItem()
{
}
//--------------------------------------------------------------------------------------------------
NodeItem::NodeItem(const NodeInterface &NI) : DiagramRoundItem()
{
    setNi(NI);
    updateDiagramItemInterface();
}
//--------------------------------------------------------------------------------------------------
NodeItem::NodeItem(const NodeItem& item) : DiagramRoundItem(item)
{
    setNi(item.getNi());
    updateDiagramItemInterface();
}
//--------------------------------------------------------------------------------------------------
NodeInterface NodeItem::getNi() const
{
    return Ni;
}
//--------------------------------------------------------------------------------------------------
void NodeItem::setNi(const NodeInterface &value)
{
    Ni = value;
}
//--------------------------------------------------------------------------------------------------
void NodeItem::updateDiagramItemInterface()
{
    const qreal PI = 3.14159265358;

    QFont font("Arial Black");
    font.setPointSizeF(100.0);
    addProperty("Caption", QVariant(getNi().cap));
    addProperty("Font", QVariant(font));
    addProperty("Text Brush", QBrush(QColor(0, 0, 0)));
    addProperty("Pen", QPen(QColor(0, 0, 0), 12.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    setFlags(CanMove | CanRotate | CanFlip | CanResize | MatchUnitsWithParent);
    setPlaceType(PlaceMouseUp);
    double deg2rad = PI / 180.0;
    for(int j=0; j<12; j++)
        ItemAngle[j] = 0.0;
    clearPoints();
    for(int j=0; j<12; j++)
        addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Control, 0));
    // corner points
    point(0)->setPosition(-400.0f, -200.0f);
    point(1)->setPosition(400.0f, 200.0f);
    point(2)->setPosition(400.0f, -200.0f);
    point(3)->setPosition(-400.0f, 200.0f);
    // mid-edge points
    point(4)->setPosition(0.0f, -200.0f);
    point(5)->setPosition(400.0f, 0.0f);
    point(6)->setPosition(0.0f, 200.0f);
    point(7)->setPosition(-400.0f, 0.0f);

    for(int i=0; i<4; i++)  // 4 corners points
        point(i)->setFlags(DiagramItemPoint::Control);

    for(int i=4; i<12; i++) // 4 points i=at the center of the 4 edges
    {
        point(i)->setFlags(0);
        point(i)->setPosition(0.0f, 0.0f);
    }

    double theta_i, theta_f, theta_c;
    double delta_theta, x, y;
    double a=400, b=200;

    int nInp, nOutp, i;
    nInp = getNi().NumInputs;
    nOutp = getNi().NumOutputs;

    // we now have 4 empty slots (8-11); need to create new points if we have more than 4 connectiopns to the node
    for(int j=0; j<(nInp+nOutp-4); j++)
        addPoint(new DiagramItemPoint(QPointF(0.0, 0.0), DiagramItemPoint::Connection, 0));

    double theta_i_global = (5) * deg2rad - PI;     // initial angle
    double theta_f_global = (175) * deg2rad  - PI;  // finial angle
    if( nInp > 0 )
    {
        delta_theta = (theta_f_global - theta_i_global) / nInp;

        theta_i = theta_i_global;

        for(i=0; i<nInp; i++)
        {
            theta_f = theta_i + delta_theta;
            theta_c = (theta_i + theta_f) / 2.0;
            ItemAngle[8+i] = theta_c;
            x = a * qCos(theta_c);
            y = b * qSin(theta_c);
            point(8+i)->setPosition(x, y);
            point(8+i)->setFlags(DiagramItemPoint::Control | DiagramItemPoint::Connection | DiagramItemPoint::Input);
            theta_i = theta_f;
        }
    }
    theta_i_global = (175) * deg2rad;
    theta_f_global = (5) * deg2rad;

    if( nOutp > 0 )
    {
        delta_theta = (theta_f_global - theta_i_global) / nOutp;

        theta_i = theta_i_global;

        for(i=0; i<nOutp; i++)
        {
            theta_f = theta_i + delta_theta;
            theta_c = (theta_i + theta_f) / 2.0;
            ItemAngle[8+nInp+i] = theta_c;
            x = a * qCos(theta_c);
            y = b * qSin(theta_c);
            point(8+nInp+i)->setPosition(x, y);
            point(8+nInp+i)->setFlags(DiagramItemPoint::Control | DiagramItemPoint::Connection | DiagramItemPoint::Output);
            theta_i = theta_f;
        }
    }
}
//--------------------------------------------------------------------------------------------------
void NodeItem::render(DiagramPainter *painter)
{
    DiagramRoundItem::render(painter);
    QRectF bounds = rectFromPoints(point(0)->position(), point(1)->position());
    QPen lPen = pen();
    QBrush lBrush = brush();

    if (flags() & UsePalette)
    {
        QColor color;
        if (isSelected()) lBrush = painter->outputBrush(DiagramPainter::Item1);
        else lBrush = painter->outputBrush(DiagramPainter::Item0);
        lPen.setBrush(lBrush);

        color = lBrush.color();
        color.setAlpha(brush().color().alpha());
        lBrush.setColor(color);
    }

#ifdef WIN32
    if (painter->paintEngine()->type() == QPaintEngine::Pdf && lBrush.color().alpha() == 0)
    {
        // Hack to get transparency output to PDF to work properly on Windows
        painter->setForItem(lPen, Qt::transparent);
        painter->drawArc(bounds, 0, 5760);
    }
    else
    {
        painter->setForItem(lPen, lBrush);
        painter->drawEllipse(bounds);
    }
#else
    painter->setForItem(lPen, lBrush);
    painter->drawEllipse(bounds);
#endif
}
//--------------------------------------------------------------------------------------------------
void NodeItem::resizeEvent(DiagramItemPoint *point, const QPointF &parentPos)
{
    DiagramBoxResizeItem::resizeEvent(point, parentPos);
    adjustEllipseControlPoints(point);
}
//--------------------------------------------------------------------------------------------------
void NodeItem::adjustEllipseControlPoints(DiagramItemPoint* activePoint)
{
    const float PI = 3.14159265358;
    DiagramItemPoint* startPoint = point(0);
    DiagramItemPoint* endPoint = point(1);
    DiagramBoxResizeItem::adjustEllipseControlPoints(activePoint);
    if (startPoint && endPoint)
    {
        // Adjust points at 45 degree offsets
        QRectF rect = QRectF(startPoint->position(), endPoint->position());
        float radiusX = rect.width() / 2;
        float radiusY = rect.height() / 2;

        DiagramItemPoint* pt = NULL;
        float angle;
        //updateDiagramItemInterface();
        for(int i = 8; i < ((8+getNi().NumInputs+getNi().NumOutputs)); i++)
        {
            angle = ItemAngle[i];
            pt = point(i);
            if (pt)
            {
                pt->setPosition(rect.center() + QPointF(radiusX * qCos(angle), radiusY * qSin(angle)));
            }
        }
    }
}
//--------------------------------------------------------------------------------------------------



