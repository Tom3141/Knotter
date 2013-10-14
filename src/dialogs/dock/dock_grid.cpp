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

#include "dock_grid.hpp"


#include "dock_grid.hpp"
#include "resource_manager.hpp"

Dock_Grid::Dock_Grid(QWidget *parent) :
    QDockWidget(parent), target(nullptr)
{
    setupUi(this);
    connect(button_move,SIGNAL(clicked()),SIGNAL(move_grid()));
    connect(spin_size,SIGNAL(valueChanged(int)),
            &Resource_Manager::settings, SLOT(set_grid_size(int)));

}

void Dock_Grid::set_grid(Snapping_Grid *target_grid)
{

    target = target_grid;

    if ( target )
    {
        spin_size->setValue(target->size());
        connect(spin_size,SIGNAL(valueChanged(int)),target,SLOT(set_size(int)));


        combo_shape->setCurrentIndex(target->shape());

        check_enable->setChecked(target->is_enabled());
        connect(check_enable,SIGNAL(clicked(bool)),target,SLOT(enable(bool)));
        connect(target,SIGNAL(enabled(bool)),check_enable,SLOT(setChecked(bool)));

        spin_x->blockSignals(true);
        spin_x->setValue(target->origin().x());
        spin_x->blockSignals(false);
        spin_y->blockSignals(true);
        spin_y->setValue(target->origin().y());
        spin_y->blockSignals(false);

        connect(target,SIGNAL(moved(QPointF)),SLOT(grid_moved(QPointF)));


        setEnabled(true);
    }
    else
    {
        check_enable->setChecked(false);
        setEnabled(false);
    }
}

void Dock_Grid::unset_grid(Snapping_Grid *grid)
{
    if ( target == grid )
    {
        grid->disconnect(this);
        disconnect(grid);
        check_enable->disconnect(grid);
        grid->disconnect(check_enable);
        spin_size->disconnect(grid);
        set_grid( nullptr );
    }
}

void Dock_Grid::changeEvent(QEvent *e)
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

void Dock_Grid::position_spin_changed()
{
    if ( target )
    {
        target->set_origin(QPointF(spin_x->value(),spin_y->value()));
    }
}

void Dock_Grid::grid_moved(QPointF p)
{
    spin_x->setValue(p.x());
    spin_y->setValue(p.y());
}

void Dock_Grid::on_button_reset_clicked()
{
    spin_x->setValue(0);
    spin_y->setValue(0);
}

void Dock_Grid::on_combo_shape_currentIndexChanged(int index)
{
    if ( target )
    {
        target->set_shape(Snapping_Grid::Grid_Shape(index));
        Resource_Manager::settings.set_grid_shape(target->shape());
    }
}

void Dock_Grid::on_check_enable_toggled(bool arg1)
{
    if ( target )
    {
        Resource_Manager::settings.set_grid_enabled(arg1);
    }
}
