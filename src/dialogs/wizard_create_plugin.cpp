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

#include "wizard_create_plugin.hpp"
#include "resource_manager.hpp"
#include <QFileInfo>
#include "plugin.hpp"
#include "QFileDialog"
#include "QMessageBox"
#include "json_stuff.hpp"

Wizard_Create_Plugin::Wizard_Create_Plugin(QWidget *parent) :
    QWizard(parent)
{
    setupUi(this);

    combo_type->addItem(tr("Script"),Plugin::Script);
    combo_type->addItem(tr("Cusp"),Plugin::Cusp);
    combo_type->addItem(tr("Crossing"),Plugin::Crossing);


    int row = table_data->rowCount();
    table_data->insertRow(row);
    table_data->setItem(row,0,new QTableWidgetItem("requires"));
    table_data->setItem(row,1,new QTableWidgetItem(Resource_Manager::trimmed_program_version()));
    table_data->insertRow(row+1);
    table_data->setItem(row+1,0,new QTableWidgetItem("version"));
    table_data->setItem(row+1,1,new QTableWidgetItem("1"));

}

void Wizard_Create_Plugin::changeEvent(QEvent *e)
{
    QWizard::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

bool Wizard_Create_Plugin::validateCurrentPage()
{
    if ( currentPage() == page_info )
    {
        if ( plugin_id().isEmpty() )
        {
            QMessageBox::warning(this,tr("Error"),tr("Name is required"));
            return false;
        }
    }
    else if ( currentPage() == page_directory )
    {
        QFileInfo fi (text_directory->text());
        QFileInfo parent ( fi.dir().absolutePath() );
        if ( ( fi.exists() && ( !fi.isDir() || !fi.isWritable() ) ) ||
             !parent.exists()  || !parent.isWritable() )
        {
            QMessageBox::warning(this,tr("Error"),
                tr("%1 is not a writable directory.")
                    .arg(text_directory->text()));
            return false;
        }
    }

    return true;
}

QString Wizard_Create_Plugin::plugin_id()
{
    QString id = text_name->text().toLower();
    id.replace(QRegExp("-| "),"_");
    id.remove(QRegExp("[^_.a-z0-9]"));
    return id;
}


void Wizard_Create_Plugin::accept()
{
    QFileInfo directory_info (text_directory->text());
    if ( !directory_info.exists() )
    {
        directory_info.dir().mkdir(directory_info.fileName());
    }
    QDir directory(text_directory->text());

    QFile json_file(directory.absoluteFilePath("plugin_"+plugin_id()+".json"));
    if ( !json_file.open(QFile::WriteOnly|QFile::Text) )
    {
        QMessageBox::critical(this,tr("Error"),tr("Error while creating plugin file."));
        return;
    }


    QVariantMap data;
    for ( int i = 0; i < table_data->rowCount(); i++ )
        data[table_data->item(i,0)->text()] = table_data->item(i,1)->text();
    data["name"] = text_name->text();
    data["description"] = text_description->toPlainText();
    data["type"] = combo_type->currentText().toLower();
    data["script"] = plugin_id()+".js";

    json_write_file(json_file,data);


    QWizard::accept();


    QFile source_file(directory.absoluteFilePath(plugin_id()+".js"));
    if ( !source_file.open(QFile::WriteOnly|QFile::Text) )
    {
        QMessageBox::critical(this,tr("Error"),tr("Error while creating source file."));
        return;
    }

    source_file.write(text_source->toPlainText().toUtf8());
}

void Wizard_Create_Plugin::on_pushButton_clicked()
{
    int row = table_data->rowCount();
    table_data->insertRow(row);
}

void Wizard_Create_Plugin::on_pushButton_2_clicked()
{
    table_data->removeRow(table_data->currentRow());
}

void Wizard_Create_Plugin::on_button_directory_clicked()
{

    QString dir = QFileDialog::getExistingDirectory(this,tr("Select Directory"),
                                                    text_directory->text());
    if ( !dir.isEmpty() )
        text_directory->setText(dir);
}

void Wizard_Create_Plugin::on_text_name_editingFinished()
{

    foreach(QString s, Resource_Manager::data_directories_unckecked("plugins"))
    {
        if ( QFileInfo(s).isWritable() )
        {
            text_directory->setText(s+"/"+plugin_id());
            break;
        }
    }
}

void Wizard_Create_Plugin::on_text_directory_editingFinished()
{
    QString text = text_directory->text();
    if ( text.endsWith('/') )
        text_directory->setText(text.left(text.size()-1));
}
