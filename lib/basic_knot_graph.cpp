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
#include "basic_knot_graph.hpp"
#include <algorithm>

basic_knot_graph::basic_knot_graph()
    : default_style ( knot_curve_styler::idof("pointed"), 225, 32, 10, 24)
{
}


void basic_knot_graph::add_node(Node* which)
{
    if ( which )
        nodes.push_back(which);
}

void basic_knot_graph::add_edge(Edge *which)
{
    if ( which )
        edges.push_back ( which );
}

void basic_knot_graph::remove_node(Node *which)
{
    nodes.remove(which);
}

void basic_knot_graph::remove_edge(Edge *which)
{
    edges.remove(which);
}

void basic_knot_graph::clear()
{
    nodes.clear();
    edges.clear();
}



void basic_knot_graph::set_style_info(styleinfo new_style)
{
    default_style = new_style;
}

styleinfo basic_knot_graph::get_style_info() const
{
    return default_style;
}

void basic_knot_graph::build_knotline(path_builder &path_b)
{

    knot_curve_style* pcs = knot_curve_styler::style(default_style.curve_style);

    foreach(Edge*e,edges)
    {
        e->reset();
        QList<QLineF> connected = e->connected(default_style.handle_length,
                                                default_style.crossing_distance);
        foreach ( QLineF l, connected )
        {
            path_b.add_line(l.p1(),l.p2());
        }
    }

    while ( !edges.empty() )
    {
        Edge* e = edges.front();
        Edge::handle_type handle = e->not_traversed();
        if ( handle == Edge::NOHANDLE )
        {
            traversed_edges.push_back(e);
            edges.pop_front();
        }
        else
        {
            if ( ! e->traversed(handle) )
            {
                e->traverse(handle);

                Node* n = e->vertex1();
                if ( handle == Edge::TOPRIGHT || handle == Edge::BOTTOMRIGHT )
                    n = e->vertex2();


                TraversalInfo ti = n->next_edge(e,handle);

                if ( n->has_custom_style() )
                {
                    const styleinfo& csi = n->get_custom_style();
                    knot_curve_style* customcs = knot_curve_styler::style(csi.curve_style);
                    customcs->draw_joint(path_b,n,ti,csi);
                }
                else
                {
                    pcs->draw_joint(path_b,n,ti,default_style);
                }

                ti.edge_out->traverse(ti.handle_out);
            }
        }
    }


    edges = traversed_edges;
    traversed_edges.clear();
}
