/* NodeItem.cpp
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


#ifndef NODEITEM_H
#define NODEITEM_H

#include <DiagramRoundItem.h>
#include "Nodes/nodeinterface.h"

class NodeItem : public DiagramRoundItem
{
private:
//    float ItemAngle[36];
    std::map<int,float> ItemAngle;

public:
    NodeInterface Ni;
public:
    NodeItem();
    NodeItem(const NodeInterface &NI);
    NodeItem(const NodeItem& item);
    NodeInterface getNi() const;
    void setNi(const NodeInterface &value);
    void updateDiagramItemInterface();

    virtual void render(DiagramPainter* painter);
    void adjustEllipseControlPoints(DiagramItemPoint *activePoint);
protected:
    virtual void resizeEvent(DiagramItemPoint* point, const QPointF& parentPos);
};

#endif // NODEITEM_H
