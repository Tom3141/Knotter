/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "node.hpp"
#include <QPainter>
#include "edge.hpp"
#include <algorithm>

Node::Node(QPointF position)
    : CustomItem(true)
{
    setPos(position);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setZValue(1);
}

Node::~Node()
{}


QRectF Node::boundingRect() const
{
    return QRectF ( -(radius+1), -(radius+1), 2*(radius+1), 2*(radius+1) );
}

QRectF Node::actualRect() const
{
    return QRectF ( -radius, -radius, 2*radius, 2*radius );
}

void Node::paint(QPainter *painter,
                const QStyleOptionGraphicsItem *,
                QWidget *)
{
    painter->setPen(Qt::black);

    if ( isSelected() )
    {
        painter->setBrush(Qt::white);
        painter->drawRect(boundingRect());
    }

    if ( !is_visible() && !highlight)
        return;


    if ( highlight && is_visible() )
    {
        painter->setBrush(Qt::yellow);
        painter->drawEllipse(boundingRect());
    }
    else
    {
        painter->setBrush(Qt::red);
        painter->drawEllipse(actualRect());
    }
}

Edge *Node::get_link(Node *other) const
{
    foreach ( Edge* e, edges )
    {
        if ( e->is_vertex(other) )
            return e;
    }
    return NULL;
}

void Node::add_link(Edge *edge)
{
    edges.push_back(edge);
}

void Node::remove_link(Edge *which)
{
    edges.erase ( std::find ( edges.begin(), edges.end(), which ) );
}

QList<Node *> Node::linked_to() const
{
    QList<Node *> r;
    foreach ( Edge* e, edges )
        r.push_back(e->other(this));
    return r;
}




TraversalInfo Node::next_edge(Edge *edge, Edge::handle_type handle) const
{

    TraversalInfo ti;
    ti.edge_in = edge;
    ti.handle_in = handle;

    if ( edge->vertex1() == this )
    {
        // RH = TL,  LH = BL
        if ( handle == Edge::TOPLEFT )
            ti.handside = TraversalInfo::RIGHT;
        else if ( handle == Edge::BOTTOMLEFT )
            ti.handside = TraversalInfo::LEFT;
        else
            return TraversalInfo("Wrong handle");
    }
    else if ( edge->vertex2() == this )
    {
        // RH = BR,  LH = TR
        if ( handle == Edge::BOTTOMRIGHT )
            ti.handside = TraversalInfo::RIGHT;
        else if ( handle == Edge::TOPRIGHT )
            ti.handside = TraversalInfo::LEFT;
        else
            return TraversalInfo("Wrong handle");
    }
    else
        return TraversalInfo("Wrong edge");

    ti.angle_in =  QLineF ( pos(), edge->other(this)->pos() ).angle();

    ti.angle_out = ti.angle_in;
    ti.angle_delta = 360;
    ti.edge_out = edge;
    foreach(Edge* i, edges)
    {
        if ( i != edge )
        {
            double angle_out = QLineF ( pos(),i->other(this)->pos() ).angle();
            double delta = ti.angle_in - angle_out;
            if ( delta < 0 )
                delta += 360;
            if ( ti.handside == TraversalInfo::RIGHT )
                delta = 360-delta;
            if ( delta < ti.angle_delta )
            {
                ti.angle_delta = delta;
                ti.edge_out = i;
                ti.angle_out = angle_out;
            }
        }
    }


    if ( ti.edge_out->vertex1() == this )
    {
        // RH -> BL,  LH -> TL
        ti.handle_out = ti.handside == TraversalInfo::RIGHT ? Edge::BOTTOMLEFT : Edge::TOPLEFT;
    }
    else if ( ti.edge_out->vertex2() == this )
    {
        // RH -> TR,  LH -> BR
        ti.handle_out = ti.handside == TraversalInfo::RIGHT ? Edge::TOPRIGHT : Edge::BOTTOMRIGHT;
    }
    ti.success = true;
    return ti;
}



