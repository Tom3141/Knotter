/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#include "image_exporter.hpp"

#include <QSvgGenerator>


void export_svg(QIODevice &file, const Graph& graph, bool draw_graph, bool draw_bg_image, const Background_Image &bg_img)
{
    if ( !file.isWritable() && !file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }

    QRectF fibr = graph.full_image_bounding_rect();
    QSvgGenerator gen;
    gen.setOutputDevice(&file);
    gen.setTitle("");
    gen.setDescription("");
    gen.setViewBox(QRect(QPoint(0,0),fibr.size().toSize()));

    QPainter painter;
    painter.begin(&gen);
    painter.translate(-fibr.topLeft());

    if ( draw_bg_image )
        bg_img.render(&painter);

    if ( draw_graph )
        graph.paint_graph(&painter);

    graph.const_paint(&painter);

    painter.end();
}


void export_raster(QIODevice &file, const Graph& graph, QColor background,
                   bool antialias, QSize img_size, int quality, bool draw_graph,
                   bool draw_bg_image, const Background_Image& bg_img,
                   const char* format )
{

    if ( !file.isWritable() && !file.open(QIODevice::WriteOnly))
    {
        return;
    }

    if ( img_size.width() == 0 )
        img_size.setWidth(1);
    if ( img_size.height() == 0 )
        img_size.setHeight(1);

    QRectF fibr = graph.full_image_bounding_rect();
    QSizeF actual_size = fibr.size();
    double scale_x = img_size.width() / actual_size.width();
    double scale_y = img_size.height() / actual_size.height();
    QPointF offset = -fibr.topLeft();


    QPixmap *pix;

    if ( antialias )
    {
        pix = new QPixmap(img_size*2);
        scale_x *= 2;
        scale_y *= 2;
    }
    else
        pix = new QPixmap(img_size);

    pix->fill(background);

    QPainter painter;
    painter.begin(pix);
    painter.translate(offset.x()*scale_x,offset.y()*scale_y);
    painter.scale(scale_x,scale_y);

    if ( draw_bg_image )
        bg_img.render(&painter);

    if ( draw_graph )
        graph.paint_graph(&painter);
    graph.const_paint(&painter);

    painter.end();

    if ( antialias )
        pix->scaled(img_size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)
                .save(&file,format,quality);
    else
        pix->save(&file,format,quality);

    delete pix;
}
