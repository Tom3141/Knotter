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

#include "node_mover.hpp"
#include "knot_view.hpp"
#include "commands.hpp"


Node_Mover::Node_Mover(QList<Node *> nodes, QPointF pivot)
    : nodes(nodes), pivot(pivot), start_pos(pivot)
{
    offset.reserve(nodes.size());
    foreach(Node* n,nodes)
    {
        offset.push_back(n->pos() - pivot);
    }
}

void Node_Mover::move(QPointF delta)
{
    pivot += delta;
    foreach(Node* n,nodes)
    {
        n->setPos(n->pos()+delta);
    }
}

void Node_Mover::deploy(Knot_View *view)
{
    if ( nodes.size() > 1 )
    {
        view->undo_stack_pointer()->beginMacro(QObject::tr("Move Nodes"));
        for ( int i = 0; i < nodes.size(); i++ )
        {
            Node* n = nodes[i];
            view->undo_stack_pointer()->push(
                    new Move_Node(n,offset[i]+start_pos,n->pos(),view));
        }
        view->undo_stack_pointer()->endMacro();
    }
    else if ( nodes.size() == 1 )
    {
        Node* n = nodes[0];
        view->undo_stack_pointer()->push(
                new Move_Node(n,offset[0]+start_pos,n->pos(),view));
    }

    nodes.clear();
    offset.clear();
}
