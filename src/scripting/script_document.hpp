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

#ifndef SCRIPT_DOCUMENT_HPP
#define SCRIPT_DOCUMENT_HPP

#include <QObject>
#include "script_graph.hpp"
#include "knot_view.hpp"

/**
 * @brief Wrapper to Knot_View
 */
class Script_Document : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString filename READ filename)
    Q_PROPERTY(QObject* graph READ graph)

    Knot_View*   wrapped;
    Script_Graph m_graph;


public:
    explicit Script_Document(Knot_View* wrapped, QObject *parent = 0);

    QString filename() const;
    Script_Graph* graph();

    Q_INVOKABLE QString toString() const;

public slots:
    void add_node(Script_Node* n);
    void add_edge(Script_Edge* e);
    void move_node(Script_Node* n, Script_Point p);
    
};

Q_DECLARE_METATYPE(Script_Document*)

#endif // SCRIPT_DOCUMENT_HPP
