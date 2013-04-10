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
#include "edge.hpp"

Edge::Edge(Node *v1, Node *v2, QObject *parent) :
    QObject(parent), v1(v1), v2(v2)
{
    attach();
}

void Edge::detach()
{
    v1->remove_edge(this);
    v2->remove_edge(this);
}

void Edge::attach()
{
    v1->add_edge(this);
    v2->add_edge(this);
}
