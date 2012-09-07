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
#include "export_dialog.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include "resource_loader.hpp"
#include "translator.hpp"

Export_Dialog::Export_Dialog(QWidget *parent) :
    QDialog(parent), canvas ( 0 )
{
    setupUi(this);
    setWindowIcon(load::icon("document-export"));
    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}

void Export_Dialog::set_knot_view(KnotView *kn)
{
    canvas = kn;
}

void Export_Dialog::on_export_svg_button_clicked()
{
    QString exname = QFileDialog::getSaveFileName(this,tr("Export Knot as SVG"),filename,
                "SVG Images (*);;XML files (*.xml);;All files (*)" );

    if ( exname.isNull() )
        return;

    QFile quf(exname);
    if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(exname));
        return;
    }

    filename = exname;


    canvas->graph().export_svg( quf, only_shape_check->isChecked() );

    quf.close();
}

void Export_Dialog::on_export_raster_button_clicked()
{


    QStringList filters = QStringList()
        << tr("PNG Images (*.png)")         // 0
        << tr("Jpeg Images (*.jpg *.jpeg)") // 1
        << tr("Bitmap (*.bmp)")             // 2
        << tr("All files (*)")              // 3
        ;
    static const int png = 0;

    QFileDialog export_dialog(this,tr("Export Knot as Image"),filename);
    export_dialog.setAcceptMode ( QFileDialog::AcceptSave );
    export_dialog.setNameFilters(filters);

    if ( !export_dialog.exec() )
        return;

    QString exname = export_dialog.selectedFiles()[0];


    /*QString exname = QFileDialog::getSaveFileName(this,tr("Export Knot as SVG"),filename,
                "SVG Images (*);;XML files (*.xml);;All files (*)" );*/


    QFile quf(exname);
    if ( ! quf.open(QIODevice::WriteOnly) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(exname));
        return;
    }

    filename = exname;

    int name_filter = filters.indexOf(export_dialog.selectedFilter());


    // pixmap
    QColor back = Qt::white;
    if ( name_filter == png )
        back = Qt::transparent;

    canvas->graph().export_raster(quf,false,back,antialias_check->isChecked(),
            get_size(),100-quality_slider->value());

    quf.close();
}

QSize Export_Dialog::get_size() const
{
    return QSize ( width_spin->value(), height_spin->value() );
}

QRectF Export_Dialog::get_area()
{
    if ( area_everything->isChecked() )
        return canvas->scene()->itemsBoundingRect();
    else
        return canvas->sceneRect();
}

void Export_Dialog::on_quality_slider_valueChanged(int value)
{
    quality_label->setText(QString("%1%").arg(value));
}

void Export_Dialog::reset_size()
{

    height_spin->blockSignals(true);
    width_spin->blockSignals(true);

    width_spin->setValue(get_area().width());
    height_spin->setValue(get_area().height());

    height_spin->blockSignals(false);
    width_spin->blockSignals(false);

    ratio = double ( height_spin->value() ) / width_spin->value();

}

void Export_Dialog::on_width_spin_valueChanged(int arg1)
{
    if ( ratio_check->isChecked() )
    {
        height_spin->blockSignals(true);
        height_spin->setValue(ratio*arg1);
        height_spin->blockSignals(false);
    }
}

void Export_Dialog::on_height_spin_valueChanged(int arg1)
{
    if ( ratio_check->isChecked() )
    {
        width_spin->blockSignals(true);
        width_spin->setValue(arg1/ratio);
        width_spin->blockSignals(false);
    }
}

void Export_Dialog::on_ratio_check_toggled(bool)
{
    ratio = double ( height_spin->value() ) / width_spin->value();
}

void Export_Dialog::retranslate()
{
    retranslateUi(this);
}
