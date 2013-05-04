/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

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

#ifndef EDGE_STYLE_HPP
#define EDGE_STYLE_HPP

#include <QPainter>
#include "c++.hpp"
#include "node_style.hpp"
#include "path_builder.hpp"
#include "edge.hpp"

class Edge_Style
{
public:
    Edge_Style();
    virtual ~Edge_Style(){}

    void paint_regular(QPainter*painter, const Edge& edge);
    void paint_highlighted(QPainter*painter, const Edge& edge);

    virtual void paint(QPainter*painter, const Edge& edge);

    /**
     *  \brief Perform any rendering to path and return the next handle
    */
    virtual Edge::Handle traverse(Edge* edge, Edge::Handle handle,Path_Builder& path,
                          const Node_Style& default_style ) const = 0;
    /**
     *  \brief Get handle geometry
     *
     *  p1 is the point where the path line should pass, p2 the control point
     */
    virtual QLineF handle(const Edge *edge, Edge::Handle handle,
                          const Node_Style &default_style) const;

    /// (Translated) Human-readable name, used in the UI
    virtual QString name() const = 0;


    /// Machine-readable (unique) name, used as identifier
    virtual QString machine_name() const = 0;

};

class Edge_Normal : public Edge_Style
{

    /**
     *  \brief Perform any rendering to path and return the next handle
    */
    virtual Edge::Handle traverse(Edge* edge, Edge::Handle handle,Path_Builder& path,
                          const Node_Style& default_style ) const override;
    QString name() const override;
    QString machine_name() const override;
};


class Edge_Inverted : public Edge_Style
{
    void paint(QPainter*painter, const Edge& edge) override;
};

class Edge_Wall : public Edge_Style
{
    void paint(QPainter*painter, const Edge& edge) override;
};


class Edge_Hole : public Edge_Style
{
    void paint(QPainter*painter, const Edge& edge) override;
};




#endif // EDGE_STYLE_HPP
