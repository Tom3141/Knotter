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

#ifndef NODE_CUSP_SHAPE_HPP
#define NODE_CUSP_SHAPE_HPP
#include <QString>
#include "path_builder.hpp"

class Traversal_Info;
class Node_Style;

class Node_Cusp_Shape
{
public:
    /// (Translated) Human-readable name
    virtual QString name() const = 0;

    /// Machine-readable name, must be unique and constant for every derived class
    virtual QString machine_name() const = 0;


    virtual void draw_joint ( Path_Builder& path,
                                const Traversal_Info& ti,
                                const Node_Style& style ) const = 0;
protected:
    /**
        \brief get line pointing to the cusp endpoint
        \param start        starning handle line
        \param finish       ending handle line
        \param node         node
        \param ti           traversal info
        \param def_dist     default distance from node (only if delta ~ 360)
    */
    QPointF cusp_point ( QLineF start,
                         QLineF finish,
                         const Traversal_Info& ti,
                         double def_dist ) const;
};


class Cusp_Rounded : public Node_Cusp_Shape
{
    QString name() const override { return QObject::tr("Rounded"); }

    QString machine_name() const override { return "rounded"; }

    void draw_joint ( Path_Builder& path,
                        const Traversal_Info& ti,
                        const Node_Style& style ) const override;
};

#endif // NODE_CUSP_SHAPE_HPP
