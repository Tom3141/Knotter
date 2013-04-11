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
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "ui_main_window.h"
#include <QDoubleSpinBox>

class Main_Window : public QMainWindow, private Ui::Main_Window
{
    Q_OBJECT

private:
    QDoubleSpinBox* zoomer; ///< Zoom on statusbar
public:
    explicit Main_Window(QWidget *parent = 0);

    /// Change all the strings to their translated version
    void retranslate();

    ~Main_Window();

private:
    /// Initialize menus
    void init_menus();
    /// Initialize toolbars
    void init_toolbars();
    /// Load saved configuration
    void load_config();

private slots:
    void set_icon_size(int);
    void set_tool_button_style(Qt::ToolButtonStyle);

    void on_action_Preferences_triggered();
};

#endif // MAIN_WINDOW_HPP
