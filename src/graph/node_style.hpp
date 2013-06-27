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

#ifndef NODE_STYLE_HPP
#define NODE_STYLE_HPP

#include <QObject>
#include "path_builder.hpp"
#include "node_cusp_shape.hpp"

class Edge_Type;

class Knot_Style
{

public:

    enum Enabled_Styles_Enum
    {
        NOTHING           = 0x00,
        CUSP_SHAPE        = 0x01,
        CUSP_ANGLE        = 0x02,
        HANDLE_LENGTH     = 0x04,
        CROSSING_DISTANCE = 0x08,
        CUSP_DISTANCE     = 0x10,
        EDGE_SLIDE        = 0x20,
        EDGE_TYPE         = 0x40,
        NODE_EVERYTHING   = CUSP_SHAPE|CUSP_ANGLE|HANDLE_LENGTH|CUSP_DISTANCE,
        EDGE_EVERYTHING   = HANDLE_LENGTH|CROSSING_DISTANCE|EDGE_SLIDE|EDGE_TYPE,
        EVERYTHING        = 0xFF
    };
    Q_DECLARE_FLAGS(Enabled_Styles, Enabled_Styles_Enum)

public:
    /// Which styles to override
    Enabled_Styles enabled_style;
    /// Minimum angle required to enable the cusp
    double cusp_angle;
    /// Length of the handles (line from start point to control point in SVG curves )
    double handle_length;
    /// Length of the gap that highlights the thread passing under another one
    double crossing_distance;
    /// Distance from the node to the cusp point
    double cusp_distance;
    /// Value in [0,1] that determines the offset of the crossing
    double edge_slide;
    /// Polymorphic object that renders the cusp
    Cusp_Shape* cusp_shape;
    /// Polymorphic object that renders the crossing
    Edge_Type* edge_type;

public:
    Knot_Style (
               double cusp_angle = 225,
               double handle_length = 24,
               double crossing_distance = 10,
               double cusp_distance = 32,
               double edge_slide = 0.5,
               Cusp_Shape* cusp_shape = nullptr,
               Edge_Type* edge_type = nullptr,
               Enabled_Styles enabled_style = NOTHING )
        :   enabled_style ( enabled_style ),
            cusp_angle(cusp_angle),
            handle_length(handle_length),
            crossing_distance(crossing_distance),
            cusp_distance ( cusp_distance ),
            edge_slide(edge_slide),
            cusp_shape ( cusp_shape ),
            edge_type(edge_type)
    {}

    /// Set disabled style to the values in other
    Knot_Style default_to(const Knot_Style& other) const;

    void build(const Traversal_Info& ti,Path_Builder&path, const Knot_Style& default_style) const
    {
        Knot_Style def = default_to(default_style);
        if ( def.cusp_shape )
            def.cusp_shape->draw_joint(path,ti,def);
    }


};

Q_DECLARE_METATYPE(Knot_Style::Enabled_Styles)

#endif // NODE_STYLE_HPP
