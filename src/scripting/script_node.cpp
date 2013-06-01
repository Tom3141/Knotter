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

#include "script_node.hpp"

Script_Node::Script_Node(Script_Point p, QObject *parent) :
    QObject(parent), m_pos(p), m_wrapped_node(nullptr)
{
}

Script_Node::Script_Node(Node *n, QObject *parent):
    QObject(parent), m_pos(n->pos()), m_wrapped_node(n)
{
    connect(m_wrapped_node,SIGNAL(destroyed()),SLOT(clean_wrappen_node()));
}

Script_Node::Script_Node(const Script_Node &o)
    : QObject(o.parent()), m_pos(o.m_pos), m_edges(o.m_edges)
{
}

void Script_Node::set_pos(Script_Point p)
{
    m_pos = p;
    emit moved(p);
}

void Script_Node::set_x(double x)
{
    m_pos.setX(x);
    emit moved(m_pos);
}

void Script_Node::set_y(double y)
{
    m_pos.setY(y);
    emit moved(m_pos);
}

Node *Script_Node::generate_wrapped_node()
{
    if ( m_wrapped_node )
        return m_wrapped_node;

    m_wrapped_node = new Node(m_pos);
    connect(m_wrapped_node,SIGNAL(destroyed()),SLOT(clean_wrappen_node()));

    return m_wrapped_node;
}

QString Script_Node::toString() const
{
    return "[node "+m_pos.toString()+"]";
}


void Script_Node::clean_wrappen_node()
{
    m_wrapped_node = nullptr;
}
