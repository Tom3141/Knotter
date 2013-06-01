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

#include "script_graph.hpp"

Script_Graph::Script_Graph(const Graph &graph, QObject *parent) :
    QObject(parent)
{
    from_graph(graph);
}

void Script_Graph::from_graph(const Graph &graph)
{

    foreach ( Node* n, graph.nodes() )
    {
        if ( !node_map.contains(n) )
            add_node(n);
    }

    foreach ( Edge* e, graph.edges() )
    {
        if ( !edge_map.contains(e) )
            add_edge(e);
    }


}


QObject* Script_Graph::add_node(Script_Point p)
{
    Script_Node* n = add_node(new Node(p));
    emit node_added(n);
    return n;
}

QObject *Script_Graph::add_node(double x, double y)
{
    return add_node(Script_Point(x,y));
}


Script_Node *Script_Graph::add_node(Node *n)
{

    Script_Node* sn =  new Script_Node(n,this);
    node_map[n] = sn;
    QObject::connect(sn,SIGNAL(moved(Script_Point)), SLOT(emit_node_moved(Script_Point)));
    QObject::connect(n,SIGNAL(destroyed()), SLOT(node_removed()));
    return sn;
}

Script_Edge* Script_Graph::add_edge(Edge *e)
{

    Script_Edge* se =  new Script_Edge(node_map[e->vertex1()],
                                       node_map[e->vertex2()],this);
    edge_map[e] = se;
    QObject::connect(e,SIGNAL(destroyed()), SLOT(edge_removed()));
    return se;
}

QObject *Script_Graph::connect(Script_Node *n1, Script_Node *n2)
{
    Script_Edge * e = new Script_Edge(n1,n2,this);
    emit edge_added(e);
    return e;
}

Script_Node *Script_Graph::node_at(Script_Point p)
{
    foreach(Script_Node *n, nodes())
    {
        if ( qFuzzyCompare(p,n->pos()) )
            return n;
    }

    return nullptr;
}

QList<Script_Node *> Script_Graph::nodes()
{
    return findChildren<Script_Node*>();
}

QList<Script_Edge *> Script_Graph::edges()
{
    return findChildren<Script_Edge*>();
}

Script_Edge *Script_Graph::script_edge(Edge *e) const
{
    return edge_map[e];
}

QString Script_Graph::toString() const
{
    return "[graph]";
}


void Script_Graph::emit_node_moved(Script_Point pos)
{
    Script_Node *n = qobject_cast<Script_Node*>(sender());
    if ( n )
        emit node_moved(n,pos);
}

void Script_Graph::node_removed()
{
    Node *n = qobject_cast<Node*>(sender());
    if ( n )
    {
        delete node_map[n];
        node_map.remove(n);
    }
}

void Script_Graph::edge_removed()
{
    Edge *e = qobject_cast<Edge*>(sender());
    if ( e )
    {
        delete edge_map[e];
        edge_map.remove(e);
    }
}

QObjectList Script_Graph::nodes_object()
{
    QObjectList l;
    foreach(Script_Node *n, nodes())
        l << n;
    return l;
}

QObjectList Script_Graph::edges_object()
{
    QObjectList l;
    foreach(Script_Edge *e, edges())
        l << e;
    return l;
}
