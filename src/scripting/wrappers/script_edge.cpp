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

#include "script_edge.hpp"
#include "script_graph.hpp"
#include "resource_manager.hpp"

Script_Edge::Script_Edge(Edge *wrapped, Script_Graph *graph):
    QObject(graph), wrapped(wrapped), graph(graph)
{
}

bool Script_Edge::is_vertex(const QObject *node) const
{
    return  vertex1() == node || vertex2() == node ;
}

QObject *Script_Edge::other(QObject *node)
{
    return node == vertex1() ? vertex2() : vertex1();
}

Script_Node *Script_Edge::vertex1() const
{
    return graph->script_node(wrapped->vertex1());
}

Script_Node *Script_Edge::vertex2() const
{
    return graph->script_node(wrapped->vertex2());
}

Script_Line Script_Edge::line()
{
    return wrapped->to_line();
}

Script_Point Script_Edge::midpoint()
{
    return wrapped->midpoint();
}

QString Script_Edge::toString() const
{
    return "[edge]";
}

QString Script_Edge::type()
{
    return wrapped->style()->machine_name();
}

void Script_Edge::set_type(QString type_name)
{
    Edge_Type* t = Resource_Manager::edge_style_from_machine_name(type_name);
    emit type_changed(t);
    wrapped->set_style(t);
}
