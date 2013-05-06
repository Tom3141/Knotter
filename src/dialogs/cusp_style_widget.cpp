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

#include "cusp_style_widget.hpp"
#include "resource_manager.hpp"


Cusp_Style_Widget::Cusp_Style_Widget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    label_tooltip();

    foreach(Cusp_Shape* cs, Resource_Manager::cusp_shapes())
        combo_cusp_shape->addItem(cs->icon(),cs->name(),
                                  QVariant::fromValue(cs) );

    connect(spin_handle_length,SIGNAL(valueChanged(double)),
            SIGNAL(handle_length_changed(double)));
    connect(spin_crossing_gap,SIGNAL(valueChanged(double)),
            SIGNAL(crossing_distance_changed(double)));
    connect(spin_cusp_angle,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_angle_changed(double)));
    connect(spin_cusp_distance,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_distance_changed(double)));

}

void Cusp_Style_Widget::set_style(const Node_Style &st)
{
    if ( st.enabled_style & Node_Style::CROSSING_DISTANCE )
    {
        spin_crossing_gap->setValue(st.crossing_distance);
        check_crossing_gap->setChecked(true);
    }
    else
        check_crossing_gap->setChecked(false);

    if ( st.enabled_style & Node_Style::CUSP_SHAPE )
    {
        for(int i = 0; i < combo_cusp_shape->count(); i++ )
        {
            Cusp_Shape* cs = cusp_shape(i);
            if ( st.cusp_shape && cs->name() == st.cusp_shape->name() )
            {
                combo_cusp_shape->setCurrentIndex(i);
                break;
            }
        }
        check_cusp_shape->setChecked(true);
    }
    else
        check_cusp_shape->setChecked(false);

    if ( st.enabled_style & Node_Style::CUSP_ANGLE )
    {
        spin_cusp_angle->setValue(st.cusp_angle);
        check_cusp_angle->setChecked(true);
    }
    else
        check_cusp_angle->setChecked(false);

    if ( st.enabled_style & Node_Style::CUSP_DISTANCE )
    {
        spin_cusp_distance->setValue(st.cusp_distance);
        check_cusp_distance->setChecked(true);
    }
    else
        check_cusp_distance->setChecked(false);

    if ( st.enabled_style & Node_Style::HANDLE_LENGTH )
    {
        spin_handle_length->setValue(st.handle_length);
        check_handle_length->setChecked(true);
    }
    else
        check_handle_length->setChecked(false);
}

Node_Style Cusp_Style_Widget::get_style() const
{
    return Node_Style(
                spin_cusp_angle->value(),
                spin_handle_length->value(),
                spin_crossing_gap->value(),
                spin_cusp_distance->value(),
                cusp_shape(),
                enabled_styles()
                );
}

Node_Style::Enabled_Styles Cusp_Style_Widget::enabled_styles() const
{
    Node_Style::Enabled_Styles es = Node_Style::NOTHING;
    if ( check_crossing_gap->isChecked() )
        es |= Node_Style::CROSSING_DISTANCE;
    if ( check_cusp_angle->isChecked() )
        es |= Node_Style::CUSP_ANGLE;
    if ( check_cusp_distance->isChecked() )
        es |= Node_Style::CUSP_DISTANCE;
    if ( check_cusp_shape->isChecked() )
        es |= Node_Style::CUSP_SHAPE;
    if ( check_handle_length->isChecked() )
        es |= Node_Style::HANDLE_LENGTH;
    return es;
}

Cusp_Shape *Cusp_Style_Widget::cusp_shape() const
{
    return cusp_shape(combo_cusp_shape->currentIndex());
}

void Cusp_Style_Widget::hide_checkboxes()
{
    foreach(QCheckBox* cb, findChildren<QCheckBox*>())
    {
        cb->setChecked(true);
        cb->hide();
    }
}

Cusp_Shape *Cusp_Style_Widget::cusp_shape(int index) const
{
    return combo_cusp_shape->itemData(index).value<Cusp_Shape*>();
}

void Cusp_Style_Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
        {
            retranslateUi(this);
            label_tooltip();

            // reload cusp shapes
            int current_index = combo_cusp_shape->currentIndex();
            Cusp_Shape* current_shape = cusp_shape();
            blockSignals(true);
            combo_cusp_shape->clear();

            for( int i = 0; i < Resource_Manager::cusp_shapes().size(); i++ )
            {
                Cusp_Shape* cs = Resource_Manager::cusp_shapes()[i];
                if ( cs == current_shape )
                    current_index = i;
                combo_cusp_shape->addItem(cs->icon(),cs->name(),
                                          QVariant::fromValue(cs) );
            }

            if ( current_index > combo_cusp_shape->count() )
                current_index = 0;

            combo_cusp_shape->setCurrentIndex(current_index);

            blockSignals(false);

            Cusp_Shape* new_shape = cusp_shape(current_index);

            if ( new_shape != current_shape )
                emit cusp_shape_changed(new_shape);
        }
            break;
        default:
            break;
    }
}

void Cusp_Style_Widget::on_combo_cusp_shape_activated(int index)
{
    emit cusp_shape_changed(cusp_shape(index));
}

void Cusp_Style_Widget::checkbox_toggled()
{
    emit enabled_styles_changed(enabled_styles());
}

void Cusp_Style_Widget::label_tooltip()
{
    foreach(QLabel* l, findChildren<QLabel*>())
    {
        QWidget *bud = l->buddy();
        if ( bud )
            l->setToolTip(bud->toolTip());
    }
}
