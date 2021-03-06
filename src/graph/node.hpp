/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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
#ifndef NODE_HPP
#define NODE_HPP

#include <QPointF>
#include "graph_item.hpp"
#include "node_style.hpp"
#include <QPainter>
#include "c++.hpp"

class Edge;

class Node : public Graph_Item
{
    Q_OBJECT

public:
    /// Radius used to display the node
    static int radius;
    /// Color used to display the node (resting)
    static QColor color_resting;
    /// Color used to display the node (highlighted)
    static QColor color_highlighted;
    /// Color used to display the node (selected outline)
    static QColor color_selected;

private:
    QList<Edge*> m_edges;

    Node_Style m_style;

public:
    Node(QPointF pos );

    Node_Style& style() { return m_style; }

    void set_style(Node_Style st) { m_style = st; }

    /**
     *  Add edge to node
     *  \param e Edge to be added
     */
    void add_edge(Edge* e);
    /**
     *  Remove edge from node
     *  \param e Edge to be removed
     *  \pre e is in the edge list
     */
    void remove_edge(Edge*e);
    /**
     *  Whether exists an edge from this node to the given node
     */
    bool has_edge_to(const Node*n) const;


    /**
     *  Get the edge from this node to the given node
     */
    Edge* edge_to(const Node*n) const;

    //double distance_squared(QPointF to) const;

    QList<Edge*> edges() const { return m_edges; }

    int type() const override { return UserType + 0x01; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* =0, QWidget* =0) override;
    QRectF boundingRect() const override;


    /**
     *  \brief Moves the node
     *
     *  Use this instead of setPos
     */
    void move(QPointF p);

    static int external_radius() { return radius+1; }

signals:
    void moved(QPointF);

};

#endif // NODE_HPP
