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

#include "transform_handle.hpp"
#include "resource_manager.hpp"
#include "node.hpp"


QSvgRenderer Transform_Handle::scale_rest;
QSvgRenderer Transform_Handle::scale_active;
QSvgRenderer Transform_Handle::rotate_rest;
QSvgRenderer Transform_Handle::rotate_active;
QSvgRenderer Transform_Handle::side_scale_rest;
QSvgRenderer Transform_Handle::side_scale_active;
QSvgRenderer Transform_Handle::skew_rest;
QSvgRenderer Transform_Handle::skew_active;
double Transform_Handle::m_image_size = 24;
bool Transform_Handle::images_initialized = false;

Transform_Handle::Transform_Handle(Mode mode, int image_angle)
    : m_mode(mode), m_image_angle(image_angle), m_angle(0)
{
    if ( !images_initialized )
    {
        images_initialized = true;

        scale_rest.load(resource_manager().program.data("img/handle_scale_rest.svg"));
        scale_active.load(resource_manager().program.data("img/handle_scale_active.svg"));
        rotate_rest.load(resource_manager().program.data("img/handle_rotate_rest.svg"));
        rotate_active.load(resource_manager().program.data("img/handle_rotate_active.svg"));
        side_scale_rest.load(resource_manager().program.data("img/handle_scaleside_rest.svg"));
        side_scale_active.load(resource_manager().program.data("img/handle_scaleside_active.svg"));
        skew_rest.load(resource_manager().program.data("img/handle_scaleside_rest.svg"));
        skew_active.load(resource_manager().program.data("img/handle_scaleside_active.svg"));
    }
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

QRectF Transform_Handle::boundingRect() const
{
    const double sz = m_image_size;
    return QRectF(QPointF(-sz,-sz),QSizeF(sz*2,sz*2));
}

void Transform_Handle::set_mode(Transform_Handle::Mode mode)
{
    m_mode = mode;
}

void Transform_Handle::set_image_angle(int angle)
{
    m_image_angle = angle;
}

void Transform_Handle::set_angle(double angle)
{
    m_angle = angle;
}

void Transform_Handle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QSvgRenderer* rend;
    switch ( m_mode )
    {
        case ROTATE:
            rend = highlighted ? &rotate_active : &rotate_rest;
            break;
        case SCALE: default:
            rend = highlighted ? &scale_active : &scale_rest;
            break;
        case SKEW:
            painter->rotate(90);
            rend = highlighted ? &skew_active : &skew_rest;
            break;
        case SIDE_SCALE:
            rend = highlighted ? &side_scale_active : &side_scale_rest;
            break;
    }

    painter->rotate(-m_image_angle-m_angle);
    painter->translate(-m_image_size/2,
                       -m_image_size/2);
    rend->render(painter,QRectF(-m_image_size/2,-m_image_size/2,m_image_size,m_image_size));

}
