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

#include "dock_knot_display.hpp"
#include "color_delegate.hpp"

#include "color_selector.hpp"

Dock_Knot_Display::Dock_Knot_Display(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
    setWidget(central_widget);
    /*list_colors->setModel(&color_list_model);
    color_list_model.insert_new();
    list_colors->setIndexWidget(color_list_model.index(0,0),new Color_Selector);
    //list_colors->setItemDelegate(new Color_Delegate);*/

    connect(list_colors,SIGNAL(colorsChanged(QList<QColor>)),
            SIGNAL(colors_changed(QList<QColor>)));


    connect(spin_width,SIGNAL(valueChanged(double)), SIGNAL(width_changed(double)));

}

void Dock_Knot_Display::set_colors(const QList<QColor> &c)
{
    list_colors->setColors(c);
}

void Dock_Knot_Display::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void Dock_Knot_Display::on_list_colors_removed(int)
{
    if ( list_colors->count() == 0 )
        list_colors->append();
}
