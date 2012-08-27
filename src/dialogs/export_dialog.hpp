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
#ifndef EXPORT_DIALOG_HPP
#define EXPORT_DIALOG_HPP

#include "ui_export_dialog.h"
#include "knotview.hpp"

class Export_Dialog : public QDialog, private Ui::Export_Dialog
{
        Q_OBJECT

    protected:
        QString     filename;
        KnotView*   canvas;
        double      ratio; ///< height/width

        QSize get_size() const;
        QRectF get_area();
    public:
        explicit Export_Dialog( QWidget *parent = 0);

        void set_knot_view ( KnotView* kn );

    public slots:
        void reset_size();


    private slots:
        void on_export_svg_button_clicked();
        void on_export_raster_button_clicked();
        void on_quality_slider_valueChanged(int value);
        void on_width_spin_valueChanged(int arg1);
        void on_height_spin_valueChanged(int arg1);
        void on_ratio_check_toggled(bool checked);
        void retranslate();
};

#endif // EXPORT_DIALOG_HPP
