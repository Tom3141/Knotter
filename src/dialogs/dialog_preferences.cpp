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

#include "dialog_preferences.hpp"
#include "resource_manager.hpp"

Dialog_Preferences::Dialog_Preferences(QMainWindow *parent) :
    QDialog(parent)
{
    setupUi(this);
    toolbar_editor->setTargetWindow(parent);

    init_combos();


    combo_language->addItems(Resource_Manager::available_languages());
    combo_language->setCurrentIndex ( combo_language->findText(Resource_Manager::current_lang_name()) );

    connect(Resource_Manager::pointer,SIGNAL(language_changed()),this,SLOT(retranslate()));

    check_fluid_refresh->setChecked(Resource_Manager::settings.fluid_refresh());
    check_cache_image->setChecked(Resource_Manager::settings.graph_cache());

    spin_recent_files->setValue(Resource_Manager::settings.max_recent_files());
    check_save_geometry->setChecked(Resource_Manager::settings.save_ui());

    check_save_grid->setChecked(Resource_Manager::settings.save_grid());
    check_save_toolbars->setChecked(Resource_Manager::settings.save_toolbars());
}

void Dialog_Preferences::init_combos()
{
    combo_icon_size->clear();
    combo_icon_size->addItem(tr("Small (16x16)"),16);
    combo_icon_size->addItem(tr("Medium (22x22)"),22);
    combo_icon_size->addItem(tr("Large (48x48)"),48);
    combo_icon_size->addItem(tr("Huge (64x64)"),64);

    combo_icon_size->setCurrentIndex(combo_icon_size->findData(
                                Resource_Manager::settings.icon_size()));

    combo_icon_style->clear();
    combo_icon_style->addItem(tr("Icon Only"),Qt::ToolButtonIconOnly);
    combo_icon_style->addItem(tr("Text Only"),Qt::ToolButtonTextOnly);
    combo_icon_style->addItem(tr("Text Beside Icon"),Qt::ToolButtonTextBesideIcon);
    combo_icon_style->addItem(tr("Text Under Icon"),Qt::ToolButtonTextUnderIcon);
    combo_icon_style->addItem(tr("Follow System Style"),Qt::ToolButtonFollowStyle);

    combo_icon_style->setCurrentIndex(combo_icon_style->findData(
                                Resource_Manager::settings.button_style()));
}

void Dialog_Preferences::set_preferences()
{
    toolbar_editor->apply();
    Resource_Manager::settings.icon_size(combo_icon_size->itemData(
                                    combo_icon_size->currentIndex()).toInt());
    Resource_Manager::settings.button_style( Qt::ToolButtonStyle (
        combo_icon_style->itemData(combo_icon_style->currentIndex()).toInt()) );

    Resource_Manager::change_lang_name(combo_language->currentText());

    Resource_Manager::settings.set_fluid_refresh(check_fluid_refresh->isChecked());
    Resource_Manager::settings.set_graph_cache(check_cache_image->isChecked());

    Resource_Manager::settings.set_max_recent_files(spin_recent_files->value());
    Resource_Manager::settings.set_save_ui(check_save_geometry->isChecked());

    Resource_Manager::settings.set_save_grid(check_save_grid->isChecked());

    Resource_Manager::settings.set_save_toolbars(check_save_toolbars->isChecked());

}

void Dialog_Preferences::retranslate()
{
    retranslateUi(this);

    init_combos();
}



void Dialog_Preferences::on_button_clear_recent_clicked()
{
    Resource_Manager::settings.clear_recent_files();
}

void Dialog_Preferences::on_button_clear_settings_clicked()
{
    Resource_Manager::settings.clear_config();
    group_save->setEnabled(false);
}
