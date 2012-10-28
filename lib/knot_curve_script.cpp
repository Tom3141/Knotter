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
#include "knot_curve_script.hpp"
#include <QScriptEngine>
#include "basic_knot_graph.hpp"
#include "script_wrappers.hpp"

#include <QDebug>


void knot_curve_script::draw_joint(path_builder &pathb, const TraversalInfo &ti, styleinfo style)
{
    script_line start_handle = ti.in.edge->handle_point(ti.in.handle,style.handle_length,style.crossing_distance);
    script_line finish_handle = ti.out.edge->handle_point(ti.out.handle,style.handle_length,style.crossing_distance);
    script_point cusp_point = get_cusp_point(start_handle, finish_handle,ti,style.cusp_distance);
    script_point node_point = ti.middle->pos();
    QScriptEngine engine;

    initialize_engine(engine);

    engine.globalObject().setProperty("start_handle",engine.toScriptValue(start_handle));
    engine.globalObject().setProperty("finish_handle",engine.toScriptValue(finish_handle));
    engine.globalObject().setProperty("cusp_point",engine.toScriptValue(cusp_point));
    engine.globalObject().setProperty("node_point",engine.toScriptValue(node_point));
    engine.globalObject().setProperty("angle",ti.angle_delta);
    engine.globalObject().setProperty("handle_length",style.handle_length);
    engine.globalObject().setProperty("cusp_angle",style.cusp_angle);
    engine.globalObject().setProperty("cusp_distance",style.cusp_distance);


    script_path_builder path(&pathb);
    engine.globalObject().setProperty("path",engine.newQObject(&path));

    engine.evaluate(
    "if ( angle > cusp_angle ) { "
        "handle = new line(start_handle.p1,finish_handle.p1);"
        "handle.translate(cusp_point);"
        "handle.translate(opposite(start_handle.p1));"
        "handle.length = handle_length/2;"
        "h2 = new point(handle.p2);"
        "handle.length = -handle_length/2;"
        "h1 = new point(handle.p2);"
        "path.add_cubic ( start_handle.p1, start_handle.p2, h1, cusp_point );"
        "path.add_cubic ( finish_handle.p1, finish_handle.p2, h2, cusp_point );"
    "} else {"
        "path.add_cubic(start_handle.p1,start_handle.p2,finish_handle.p2,finish_handle.p1);"
    "}"
    );
}
